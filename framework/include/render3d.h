#ifndef RENDER3D_H
#define RENDER3D_H

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

#include <vector>

#include <QVTKWidget.h>
#include <QObject>
#include <QString>
#include <QMessageBox>
#include <QDebug>
#include <QPaintEvent>


#include <vtkPolyDataAlgorithm.h>
#include <vtkPolyDataMapper.h>
#include <vtkDataSetMapper.h>
#include <vtkOpenGLPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleImage.h>
#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkCamera.h>
#include <vtkCellArray.h>
#include <vtkUnstructuredGrid.h>
#include <vtkAxisActor.h>
#include <vtkTransform.h>
#include <vtkLookupTable.h>
#include <vtkLogLookupTable.h>
#include <vtkDataSetMapper.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkFloatArray.h>
#include <vtkDoubleArray.h>
#include <vtkTextProperty.h>
#include <vtkScalarBarWidget.h>
#include <vtkScalarBarRepresentation.h>
#include <vtkInformation.h>
#include <vtkInteractorObserver.h>
#include <vtkCubeAxesActor.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkMultiBlockDataSet.h>
#include <vtkCompositeDataGeometryFilter.h>

// include ParaView's pimped VTK implementations
#include "external/Paraview/vtkPVScalarBarActor.h"
#include "external/Paraview/vtkPVAxesWidget.h"
#include "external/Paraview/vtkPVAxesActor.h"

#include "common.hpp"
//#include "quantity_set.hpp"

#include <boost/array.hpp>

class Render3D : public QVTKWidget
{
    Q_OBJECT

public:
    typedef boost::array<double, 3>     RGB;

    typedef vtkPVScalarBarActor ScalarBarActor;
    typedef vtkPVAxesActor      AxesActor;
    typedef vtkPVAxesWidget     AxesWidget;

    typedef vtkSmartPointer<vtkDataSetMapper>   Mapper;
    typedef std::vector<Mapper>                 Mappers;
    typedef vtkSmartPointer<vtkActor>           Actor;
    typedef std::vector<Actor>                  Actors;

    explicit Render3D(vtkSmartPointer<vtkMultiBlockDataSet> domain, QWidget *parent = 0);
    ~Render3D();
    void  reset();
    void  reset_grid();
    void  reset_renderer();
    void  update_render_domain();
    void  print_current_statistics();
    void  update_render();
    void  color_current_quantity();
    void  color_quantity(std::string const& key, std::string const& display_name, int cell_lvl = VERTEX);
    void  color_quantity_vertex();
    void  color_quantity_cell();
    void  color_segments();
    void  toggle_color_legend();
    void  toggle_axes();
    void  toggle_coordinates();
    void  toggle_2d3d();
    void  build_coordinates();
    void  set_background_color(double* rgb);
    void  set_solid_color(double* rgb);
    void  set_edge_color(double* rgb);
    void  set_log(bool state);
    void  update();
    void  reset_view();
    void  color_solid();
    void  switch_to_surface_with_edges_representation();
    void  switch_to_surface_representation();
    void  switch_to_point_representation();
    void  switch_to_wireframe_representation();
    void  set_segment_color(std::size_t segment_index, double* rgb);
    void  set_segment_opacity(std::size_t segment_index, float opacity);
    int   segment_size();
    void  extract_segment_colors(std::vector<RGB>& segment_rgbs);

    vtkSmartPointer<ScalarBarActor>&   get_colorbar()   { return colorbar; }
    vtkSmartPointer<vtkCubeAxesActor>& get_cubeAxes()   { return cubeAxisActor; }

signals:
    void grid_updated();

    // ---------------------------------------------------------------------
private:

    template<typename LookUpTable>
    void make_lookup_from_blue_to_red(vtkSmartPointer<LookUpTable>& lut)
    {
      lut->SetHueRange(0.667, 0.0);
      lut->SetSaturationRange(1.0, 1.0);
      lut->SetValueRange(1.0, 1.0);
    }

    template<typename LookUpTable>
    void make_lookup_from_red_to_blue(vtkSmartPointer<LookUpTable>& lut)
    {
      lut->SetHueRange(0.0, 0.667);
      lut->SetSaturationRange(1.0, 1.0);
      lut->SetValueRange(1.0, 1.0);
    }

    template<typename LookUpTable>
    void make_lookup_greyscale(vtkSmartPointer<LookUpTable>& lut)
    {
      lut->SetHueRange(0.0, 0.0);
      lut->SetSaturationRange(0.0, 0.0);
      lut->SetValueRange(0.2, 1.0);
    }

    template<typename LookUpTable>
    void update_lookup_table_range(vtkSmartPointer<LookUpTable>& lut, double min, double max)
    {
        lut->SetRange(min, max);
        make_lookup_from_blue_to_red(lut);
        lut->SetNumberOfColors(256);
        lut->Build();
    }

    template<typename LookUpTable>
    void update_segment_index_lookup_table_range(vtkSmartPointer<LookUpTable>& lut, std::size_t segment_size)
    {
      lut->SetRange(0, segment_size);
      make_lookup_from_blue_to_red(lut);
      lut->SetNumberOfColors(segment_size);
      lut->Build();
    }

    void resetCameraParameters();

    vtkSmartPointer<vtkMultiBlockDataSet>                local_domain;
    vtkSmartPointer<vtkMultiBlockDataSet>                central_domain;
    Mappers                                              mappers;
    Actors                                               actors;
    vtkSmartPointer<vtkRenderer>                         renderer;
    vtkSmartPointer<vtkRenderWindow>                     renderWindow;
    vtkSmartPointer<vtkRenderWindowInteractor>           interactor;
    vtkSmartPointer<vtkInteractorStyleTrackballCamera>   style3D;
    vtkSmartPointer<vtkInteractorStyleImage>             style2D;
    vtkSmartPointer<vtkLookupTable>                      lutLinear;
    vtkSmartPointer<vtkLogLookupTable>                   lutLog;
    vtkSmartPointer<vtkLookupTable>                      lutSegmentIndices;
    vtkSmartPointer<ScalarBarActor>                      colorbar;
    vtkSmartPointer<vtkScalarBarWidget>                  scalarWidget;
    vtkSmartPointer<AxesWidget>                          axesWidget;
    vtkSmartPointer<vtkCubeAxesActor>                    cubeAxisActor;
    vtkSmartPointer<vtkCompositeDataGeometryFilter>      local_domain_geom;

    double initial_cameraFocalPoint[3];
    double initial_cameraPosition[3];
    double initial_cameraViewUp[3];

    double last_solid_color[3];

    bool use_log;
    int  current_quantity_cell_lvl;
    std::string current_quantity_key;

    int state;

    static const int UNSET      = -1;
    static const int SOLID      = 0;
    static const int QUANTITY   = 1;
    static const int SEGMENT    = 2;
};

#endif // RENDER3D_H
