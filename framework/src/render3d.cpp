/*
 *
 * Copyright (c) 2013, Institute for Microelectronics, TU Wien.
 *
 * This file is part of ViennaMOS     http://viennamos.sourceforge.net/
 *
 * Contact: Josef Weinbub             weinbub@iue.tuwien.ac.at
 *
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "render3d.h"

#include <QRegion>
#include <QStyleOption>
#include <QPainter>
#include <QWindowsStyle>

#include "vtkPointData.h"



Render3D::Render3D(vtkSmartPointer<vtkMultiBlockDataSet> domain, QWidget *parent)
    : QVTKWidget(parent)
{
  local_domain = vtkSmartPointer<vtkMultiBlockDataSet>::New();
  central_domain = domain;
  local_domain_geom = vtkSmartPointer<vtkCompositeDataGeometryFilter>::New();

  this->resize(400, 400);

  use_log = false; // by default, use linear scaling
  state = SOLID; // by default, we color the mesh solid

//  mapper = Mapper::New();
//  actor = vtkActor::New();
//  actor->SetMapper(mapper);

  // Create renderer
  renderer = vtkSmartPointer<vtkRenderer>::New();
  //renderer->AddActor(actor);

  // backup initial camera parameters for later resets ..
  renderer->GetActiveCamera()->GetFocalPoint(initial_cameraFocalPoint);
  renderer->GetActiveCamera()->GetPosition(initial_cameraPosition);
  renderer->GetActiveCamera()->GetViewUp(initial_cameraViewUp);

  // QVTKWidget has already created a render window and interactor
  renderWindow = this->GetRenderWindow();
  renderWindow->AddRenderer(renderer);
  interactor = renderWindow->GetInteractor();
  style3D = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
  style2D = vtkSmartPointer<vtkInteractorStyleImage>::New();
  interactor->SetInteractorStyle(style3D); // by default, we use the 3d style ..

  lutLinear = vtkSmartPointer<vtkLookupTable>::New();
  lutLog    = vtkSmartPointer<vtkLogLookupTable>::New();
  lutSegmentIndices = vtkSmartPointer<vtkLookupTable>::New();
  colorbar = vtkSmartPointer<ScalarBarActor>::New();
  scalarWidget =vtkSmartPointer<vtkScalarBarWidget>::New();

  axesWidget = vtkSmartPointer<AxesWidget>::New();
  axesWidget->SetInteractor(interactor);
  axesWidget->SetParentRenderer(renderer);


  cubeAxisActor = vtkSmartPointer<vtkCubeAxesActor>::New();
  cubeAxisActor->SetVisibility(0);
  //cubeAxisActor->SetTickLocationToInside();
  cubeAxisActor->SetTickLocationToOutside();
  //cubeAxisActor->SetTickLocationToBoth();
#if VTK_MAJOR_VERSION > 5 || (VTK_MAJOR_VERSION == 5 && VTK_MINOR_VERSION > 8)
  cubeAxisActor->SetEnableDistanceLOD(0);
  cubeAxisActor->SetEnableViewAngleLOD(0);
#endif
  cubeAxisActor->SetCamera(renderer->GetActiveCamera());

  this->reset();



#if VTK_MAJOR_VERSION <= 5

    // the 'mapper' can not handle multiblock data, thus we have to take a detour
    // via the vtkCompositeDataGeometryFilter which kindof appends all blocks to one polydata object
    // which in turn can be handled by the mapper
    //
    local_domain_geom->SetInputConnection(local_domain->GetProducerPort());

    //mapper->SetInputConnection(local_domain_geom->GetOutputPort());
#else
    std::cout << "Render3D::update_render_domain() - needs love for VTK versions >5"
   // mapper->SetInputData(local_domain);
#endif
}

Render3D::~Render3D()
{
}

void Render3D::reset_renderer()
{
  renderer->RemoveAllViewProps();
  renderer->AddActor(cubeAxisActor);
  renderer->AddActor2D(colorbar);
}

void Render3D::reset()
{
  colorbar->SetVisibility(0); // initially, hide it ..
  // setting the title first! (even an empty one ..) is important for the subsequent parameter settings,
  // as the scalarwidget produces otherwise artefacts in the render window
  // when it's enabled.
  colorbar->SetTitle( "" );
  colorbar->GetTitleTextProperty()->SetFontSize(9);
  colorbar->GetTitleTextProperty()->SetFontFamily(VTK_ARIAL);
  colorbar->GetTitleTextProperty()->SetColor(1.0, 1.0, 1.0);
  colorbar->GetTitleTextProperty()->ShadowOff();
  colorbar->GetLabelTextProperty()->SetFontSize(9);
  colorbar->GetLabelTextProperty()->SetFontFamily(VTK_ARIAL);
  colorbar->GetLabelTextProperty()->SetColor(1.0, 1.0, 1.0);
  colorbar->GetLabelTextProperty()->ShadowOff();
  //colorbar->SetNumberOfLabels(5);
  //colorbar->SetAspectRatio(20.0);
  colorbar->SetAutomaticLabelFormat(1);
  colorbar->SetLookupTable(lutLinear);


  //actor->GetProperty()->SetColor(0.0, 0.0, 1.0);
  renderer->SetBackground(0.328125, 0.347656, 0.425781); // paraview default

  lutLinear->SetRange(0.0, 0.0);
  lutLinear->Build();
  lutLog->SetRange(1.0, 1.0);
  lutLog->Build();
  lutSegmentIndices->SetRange(0.0, 0.0);
  lutSegmentIndices->Build();

  // wrapping the bar-actor into the bar widget allows the bar to be
  // interactable around
  //
  scalarWidget->SetScalarBarActor(colorbar);
  scalarWidget->SetInteractor(interactor);
  // set the initial position and size of the scalarbar
  // [jw] I think that this is the position of the widget's bounding box's bottom-left point ..
  scalarWidget->GetScalarBarRepresentation()->SetPosition(0.85, 0.3);
  // this is width/height relative to setposition -> so you span the bounding box of the widget
  scalarWidget->GetScalarBarRepresentation()->SetPosition2(0.10, 0.4);

  axesWidget->SetInteractive(1);
  axesWidget->SetViewport(0.0, 0.0, 0.2, 0.2);
  axesWidget->SetEnabled(1);

  reset_renderer();
}

void Render3D::reset_grid()
{
    reset_renderer();
    mappers.clear();
    actors.clear();
    local_domain->PrepareForNewData();
}

void Render3D::update_render_domain()
{
    this->reset_grid();

    // do not actually copy data, but only copy pointers
    // --> don't waste memory ... a render view is a view,
    // thus it should work on the central datastructure!
    //
    local_domain->ShallowCopy(central_domain);

    for(unsigned int si = 0; si < local_domain->GetNumberOfBlocks(); si++)
    {
        vtkPointSet* generic_grid = vtkPointSet::SafeDownCast(local_domain->GetBlock(si));

        mappers.push_back(Mapper::New());
    #if VTK_MAJOR_VERSION <= 5
        mappers.back()->SetInputConnection(generic_grid->GetProducerPort());
    #else
        mapper->SetInputData(local_domain);
    #endif
        actors.push_back(Actor::New());
        actors.back()->SetMapper(mappers.back());
        actors.back()->GetProperty()->SetColor(0.0, 0.0, 1.0); // default color blue
        renderer->AddActor(actors.back());

        // color the segments by assigning the segment ID as cell quantity
        //
        vtkIntArray* segment_array = vtkIntArray::New();
        segment_array->SetName(key::segment_index.toStdString().c_str());
        segment_array->SetNumberOfValues(generic_grid->GetNumberOfCells());
        for(int ci = 0; ci < generic_grid->GetNumberOfCells(); ci++)
            segment_array->SetValue(ci, si);
        generic_grid->GetCellData()->AddArray(segment_array);

        // we are casting here to vtkPointSet instead of using vtkUnstructured/vtkStructured grids
        // as the render3d should work with both, we use the superclass of both unstruct/structured grids here
        //
        for(int i = 0; i < generic_grid->GetPointData()->GetNumberOfArrays(); i++)
            generic_grid->GetPointData()->AddArray(vtkPointSet::SafeDownCast(central_domain->GetBlock(si))->GetPointData()->GetArray(i));

        for(int i = 0; i < generic_grid->GetCellData()->GetNumberOfArrays(); i++)
            generic_grid->GetCellData()->AddArray(vtkPointSet::SafeDownCast(central_domain->GetBlock(si))->GetCellData()->GetArray(i));


    }

    local_domain->Update();
    local_domain_geom->Update();

    this->update_segment_index_lookup_table_range(lutSegmentIndices, this->segment_size());


    emit grid_updated();


//#if VTK_MAJOR_VERSION <= 5

//    // the 'mapper' can not handle multiblock data, thus we have to take a detour
//    // via the vtkCompositeDataGeometryFilter which kindof appends all blocks to one polydata object
//    // which in turn can be handled by the mapper
//    //
//    local_domain_geom->SetInputConnection(local_domain->GetProducerPort());
//    local_domain_geom->Update();

//    mapper->SetInputConnection(local_domain_geom->GetOutputPort());
//    mapper->Update();
//#else
//    std::cout << "Render3D::update_render_domain() - needs love for VTK versions >5"
//    mapper->SetInputData(local_domain);
//#endif

    build_coordinates(); // prepare the coordinate axis ..

    //reset_view(); // as the mesh changed, we want to 'reset' the camera ..
    //refresh();





}

void Render3D::print_current_statistics()
{
    for(unsigned int si = 0; si < local_domain->GetNumberOfBlocks(); si++)
    {
        std::cout << "block " << si << std::endl;
        std::cout << "  point size            " << vtkPointSet::SafeDownCast(local_domain->GetBlock(si))->GetNumberOfPoints() << std::endl;
        std::cout << "  cell size             " << vtkPointSet::SafeDownCast(local_domain->GetBlock(si))->GetNumberOfCells() << std::endl;
        std::cout << "  point quantities size " << vtkPointSet::SafeDownCast(local_domain->GetBlock(si))->GetPointData()->GetNumberOfArrays() << std::endl;
        std::cout << "  cell quantities size  " << vtkPointSet::SafeDownCast(local_domain->GetBlock(si))->GetCellData()->GetNumberOfArrays() << std::endl;
    }
}

void Render3D::update_render()
{
    if(state == SOLID)     color_solid();
    else
    if(state == SEGMENT)   color_segments();
    else
    if(state == QUANTITY)  color_current_quantity();
    this->update();
}

void Render3D::color_current_quantity()
{
    color_quantity(current_quantity_key, colorbar->GetTitle(), current_quantity_cell_lvl);
}

void Render3D::color_quantity(std::string const& key, std::string const& display_name, int cell_lvl)
{
//  qDebug() << "key " << QString::fromStdString(key) << " display " << QString::fromStdString(display_name)
//           << " celllvl " << cell_lvl;

    // store the fact that the current quantity is a vertex quantity
    current_quantity_cell_lvl = cell_lvl;

    // store the quantity access key for the vtkgrid datastructure
    current_quantity_key = key;

    colorbar->SetTitle(display_name.c_str());

    // make sure the visualizer, as of now, does not do a solid coloring ..
    state = QUANTITY;

    if(cell_lvl == VERTEX) color_quantity_vertex();
    else
    if(cell_lvl == CELL)   color_quantity_cell();
    else {
        QMessageBox::critical(0, QString("Error"), QString("Only vertex or cell quantity's are supported for rendering!"));
    }
}


void Render3D::color_quantity_vertex()
{
    double domain_quantity_range[2];

    for(unsigned int si = 0; si < local_domain->GetNumberOfBlocks(); si++)
    {
        vtkPointSet* generic_segment = vtkPointSet::SafeDownCast(local_domain->GetBlock(si));

        if(generic_segment->GetPointData()->HasArray(current_quantity_key.c_str()))
        {
            generic_segment->GetPointData()->SetActiveScalars(current_quantity_key.c_str());

            double* range = generic_segment->GetPointData()->GetScalars()->GetRange();

            if(si == 0)
            {
                domain_quantity_range[0] = range[0];
                domain_quantity_range[1] = range[1];
            }
            else
            {
                domain_quantity_range[0] = std::min(domain_quantity_range[0], range[0]);
                domain_quantity_range[1] = std::max(domain_quantity_range[1], range[1]);
            }
        }
        else
        {
            QMessageBox::critical(0, QString("Error"), QString("Quantity is not available on the domain"));
            return;
        }
    }

    if(use_log)
    {
        if(domain_quantity_range[0] > 0) // the minimum of the values should be greater 0 for log ..
            update_lookup_table_range(lutLog, domain_quantity_range[0], domain_quantity_range[1]);
        else if(domain_quantity_range[0] == 0.0)
            update_lookup_table_range(lutLog, 1.0, domain_quantity_range[1]);
        else
            QMessageBox::warning(0, QString("Warning"), QString("Values are <= 0.0"));
        colorbar->SetLookupTable(lutLog);
    }
    else
    {
        update_lookup_table_range(lutLinear, domain_quantity_range[0], domain_quantity_range[1]);
        colorbar->SetLookupTable(lutLinear);
    }

    for(Mappers::iterator iter = mappers.begin(); iter != mappers.end(); iter++)
    {
        Mapper map = *iter;
        map->ScalarVisibilityOn();
        map->SetScalarModeToUsePointData();
        map->SetColorModeToMapScalars();
        map->SelectColorArray(current_quantity_key.c_str());
        map->UseLookupTableScalarRangeOff();

        if(use_log && (domain_quantity_range[0] == 0.0))
            map->SetScalarRange(1.0, domain_quantity_range[1]);
        else
            map->SetScalarRange(domain_quantity_range);

        if(use_log) map->SetLookupTable(lutLog);
        else        map->SetLookupTable(lutLinear);
    }
}

void Render3D::color_quantity_cell()
{
    double domain_quantity_range[2];

    for(unsigned int si = 0; si < local_domain->GetNumberOfBlocks(); si++)
    {
        vtkPointSet* generic_segment = vtkPointSet::SafeDownCast(local_domain->GetBlock(si));

        if(generic_segment->GetCellData()->HasArray(current_quantity_key.c_str()))
        {
            generic_segment->GetCellData()->SetActiveScalars(current_quantity_key.c_str());

            double* range = generic_segment->GetCellData()->GetScalars()->GetRange();
            if(si == 0)
            {
                domain_quantity_range[0] = range[0];
                domain_quantity_range[1] = range[1];
            }
            else
            {
                domain_quantity_range[0] = std::min(domain_quantity_range[0], range[0]);
                domain_quantity_range[1] = std::max(domain_quantity_range[1], range[1]);
            }
        }
        else
        {
            QMessageBox::critical(0, QString("Error"), QString("Quantity is not available on the domain"));
            return;
        }
    }

    if(use_log)
    {
        if(domain_quantity_range[0] > 0) // the minimum of the values should be greater 0 for log ..
            update_lookup_table_range(lutLog, domain_quantity_range[0], domain_quantity_range[1]);
        else if(domain_quantity_range[0] == 0.0)
            update_lookup_table_range(lutLog, 1.0, domain_quantity_range[1]);
        else
            QMessageBox::warning(0, QString("Warning"), QString("Values are <= 0.0"));
        colorbar->SetLookupTable(lutLog);
    }
    else
    {
        update_lookup_table_range(lutLinear, domain_quantity_range[0], domain_quantity_range[1]);
        colorbar->SetLookupTable(lutLinear);
    }

    for(Mappers::iterator iter = mappers.begin(); iter != mappers.end(); iter++)
    {
        Mapper map = *iter;
        map->ScalarVisibilityOn();
        map->SetScalarModeToUseCellData();
        map->SetColorModeToMapScalars();
        map->SelectColorArray(current_quantity_key.c_str());
        map->UseLookupTableScalarRangeOff();

        if(use_log && (domain_quantity_range[0] == 0.0))
            map->SetScalarRange(1.0, domain_quantity_range[1]);
        else
            map->SetScalarRange(domain_quantity_range);

        if(use_log) map->SetLookupTable(lutLog);
        else        map->SetLookupTable(lutLinear);
    }
}

void Render3D::color_solid()
{
    for(Mappers::iterator iter = mappers.begin(); iter != mappers.end(); iter++)
        (*iter)->ScalarVisibilityOff();

    this->set_solid_color(last_solid_color);

    state = SOLID;
}

void Render3D::color_segments()
{
  for(Mappers::iterator iter = mappers.begin(); iter != mappers.end(); iter++)
      (*iter)->ScalarVisibilityOff();

  for(int i = 0; i < this->segment_size(); i++)
  {
      double rgb[3];
      lutSegmentIndices->GetColor(double(i), rgb);
      this->set_segment_color(i, rgb);
  }
  state = SEGMENT;
}

void Render3D::toggle_color_legend()
{
    if(colorbar->GetVisibility())
    {
        scalarWidget->EnabledOff();
        colorbar->SetVisibility(0);

    }
    else
    {
        colorbar->SetVisibility(1);
        scalarWidget->EnabledOn();
    }
    update();
}

void Render3D::toggle_axes()
{
  if(axesWidget->GetAxesActor()->GetVisibility())
  {
    axesWidget->GetAxesActor()->SetVisibility(0);
    axesWidget->SetEnabled( 0 );
  }
  else
  {
    axesWidget->GetAxesActor()->SetVisibility(1);
    axesWidget->SetEnabled( 1 );
  }
    update();
}

void Render3D::toggle_coordinates()
{
    if(cubeAxisActor->GetVisibility())
    {
        cubeAxisActor->SetVisibility(0);
    }
    else
    {
        cubeAxisActor->SetVisibility(1);
        this->build_coordinates();
    }
    update();
}

void Render3D::toggle_2d3d()
{
    // in case of vtk > 5.8, use the improved 2d version of the coordinate axis
    //
#if VTK_MAJOR_VERSION > 5 || (VTK_MAJOR_VERSION == 5 && VTK_MINOR_VERSION > 8)
    if(cubeAxisActor->GetUse2DMode() == 0)
        cubeAxisActor->SetUse2DMode(1);
    else cubeAxisActor->SetUse2DMode(0);
#endif

    // toggle the interaction with the renderwindow to be 2d or 3d
    if(interactor->GetInteractorStyle() == static_cast<vtkInteractorObserver*>(style3D))
    {
        // to ensure that the 2d object is correctly shown,
        // reset the view prior to restricting the interaction to 2D
        this->reset_view();
        interactor->SetInteractorStyle(style2D);
    }
    else interactor->SetInteractorStyle(style3D);

    // show the changes
    //
    update();
}

void Render3D::build_coordinates()
{
    cubeAxisActor->SetCamera(renderer->GetActiveCamera());

    for(unsigned int si = 0; si < local_domain->GetNumberOfBlocks(); si++)
    {
        // bounds: xMin, xMax, yMin, yMax, zMin, zMax
        //double* bounds = vtkPointSet::SafeDownCast(local_domain->GetBlock(si))->GetBounds();
        double* bounds = vtkDataSet::SafeDownCast(local_domain_geom->GetOutput())->GetBounds();

        // test if it's the default vtkGrid, which uses a BB which doesn't make sense
        // -> so no coor axes are visualized
        // than use a more reasonable boundsarray
        // so that the user actually 'sees' the axes
        if((bounds[0] == 1.0) && (bounds[1] == -1.0) && (bounds[2] == 1.0) &&
           (bounds[3] == -1.0) && (bounds[4] == 1.0) && (bounds[5] == -1.0))
        {
            cubeAxisActor->SetBounds(0.0, 1.0, 0.0, 1.0, 0.0, 1.0);
            // in this case, do a manual 'reset view', as there isnt a mesh loaded anyway ..
            this->reset_view();
        }
        else cubeAxisActor->SetBounds(bounds);
    }
}

void Render3D::set_background_color(double* rgb)
{
    renderer->SetBackground(rgb[0], rgb[1], rgb[2]);
}

void Render3D::set_solid_color(double* rgb)
{
  last_solid_color[0] = rgb[0];
  last_solid_color[1] = rgb[1];
  last_solid_color[2] = rgb[2];

  for(int i = 0; i < this->segment_size(); i++)
  {
    this->set_segment_color(i, rgb);
  }

//    for(Actors::iterator iter = actors.begin(); iter != actors.end(); iter++ )
//        (*iter)->GetProperty()->SetColor(rgb[0], rgb[1], rgb[2]);
}

void Render3D::set_edge_color(double* rgb)
{
    for(Actors::iterator iter = actors.begin(); iter != actors.end(); iter++ )
        (*iter)->GetProperty()->SetEdgeColor(rgb[0], rgb[1], rgb[2]);
}

void Render3D::set_log(bool state)
{
    use_log = state;
}

void Render3D::update()
{
    for(Mappers::iterator iter = mappers.begin(); iter != mappers.end(); iter++)
        (*iter)->Update();
    renderer->Render();
    renderer->GetRenderWindow()->Render();

}

void Render3D::reset_view()
{
    resetCameraParameters();
    renderer->ResetCamera();
}

void Render3D::switch_to_surface_with_edges_representation()
{
    for(Actors::iterator iter = actors.begin(); iter != actors.end(); iter++ )
    {
        (*iter)->GetProperty()->EdgeVisibilityOn();
        (*iter)->GetProperty()->SetRepresentationToSurface();
    }

    for(Mappers::iterator iter = mappers.begin(); iter != mappers.end(); iter++)
        if((*iter)->GetInput())  (*iter)->GetInput()->Update();
    update();
}

void Render3D::switch_to_surface_representation()
{
    for(Actors::iterator iter = actors.begin(); iter != actors.end(); iter++ )
    {
        (*iter)->GetProperty()->EdgeVisibilityOff();
        (*iter)->GetProperty()->SetRepresentationToSurface();
    }

    for(Mappers::iterator iter = mappers.begin(); iter != mappers.end(); iter++)
        if((*iter)->GetInput())  (*iter)->GetInput()->Update();
    update();
}

void Render3D::switch_to_point_representation()
{
    for(Actors::iterator iter = actors.begin(); iter != actors.end(); iter++ )
        (*iter)->GetProperty()->SetRepresentationToPoints();

    for(Mappers::iterator iter = mappers.begin(); iter != mappers.end(); iter++)
        if((*iter)->GetInput())  (*iter)->GetInput()->Update();

    update();
}

void Render3D::switch_to_wireframe_representation()
{
    for(Actors::iterator iter = actors.begin(); iter != actors.end(); iter++ )
        (*iter)->GetProperty()->SetRepresentationToWireframe();

    for(Mappers::iterator iter = mappers.begin(); iter != mappers.end(); iter++)
        if((*iter)->GetInput())  (*iter)->GetInput()->Update();
    update();
}

void Render3D::resetCameraParameters()
{
    renderer->GetActiveCamera()->SetFocalPoint(initial_cameraFocalPoint);
    renderer->GetActiveCamera()->SetViewUp(initial_cameraViewUp);
    renderer->GetActiveCamera()->SetPosition(initial_cameraPosition);
}

void Render3D::set_segment_color(std::size_t segment_index, double* rgb)
{
    if(segment_index < actors.size())
        actors[segment_index]->GetProperty()->SetColor(rgb[0], rgb[1], rgb[2]);
}

void Render3D::set_segment_opacity(std::size_t segment_index, float opacity)
{
    if(segment_index < actors.size())
        actors[segment_index]->GetProperty()->SetOpacity(opacity);
}

int Render3D::segment_size()
{
    return local_domain->GetNumberOfBlocks();
}

void Render3D::extract_segment_colors(std::vector<RGB>& segment_rgbs)
{
  this->update_segment_index_lookup_table_range(lutSegmentIndices, this->segment_size());

  segment_rgbs.clear();
  segment_rgbs.resize(this->segment_size());
  for(int i = 0; i < this->segment_size(); i++)
  {
      RGB     rgb;
      lutSegmentIndices->GetColor(double(i), rgb.c_array());
      segment_rgbs[i] = rgb;
  }
}

