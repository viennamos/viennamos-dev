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



#include "copy.hpp"
#include "device.h"
#include "offload_to_worker.hpp"
#include "keys_quantities.hpp"
#include "keys_units.hpp"

#include "viennagrid/io/netgen_reader.hpp"
#include "viennagrid/algorithm/geometric_transform.hpp"
#include "viennagrid/algorithm/quantity_transfer.hpp"

#include "viennafvm/quantity.hpp"
#include "viennamini/utils/average.hpp"
#include "viennamini/utils/filter.hpp"
#include "viennamini/utils/convert.hpp"

#include "viennaminimodule.h"

#include <QDebug>

/**
 * @brief The module's c'tor registers the module's UI widget and registers
 * output quantities
 */
ViennaMiniModule::ViennaMiniModule() : ModuleInterface(this)
{
    // setup a new UI widget and register it with this module
    //
    widget    = new ViennaMiniForm();
    register_module_widget(widget); // takes ownership of the widget - no deleting required

    vmini_device_db_key_ = "vmini_device";

    // change the name of the 'run' button
    //
    module_control_widget->getButtonRun()->setText("Run Simulation");
}

ViennaMiniModule::~ViennaMiniModule()
{
}

QString ViennaMiniModule::name()
{
    return "ViennaMini";
}

QString ViennaMiniModule::description()
{
    return "Classical Device Simulator for solving Laplace and Drift-Diffusion Problems";
}

QString ViennaMiniModule::version()
{
    return "1.0";
}

// function is called after the framework forwarded
// the base data to the module, e.g., material manager
// so in this case we are lettings our UI know, that the
// material data is available
//

/**
 * @brief Function is after the framework forwarded
 * the base data to the module, e.g., material manager
 * so in this case we are lettings our UI know, that the
 * material data is available
 */
void ViennaMiniModule::preprocess()
{
}

/**
 * @brief Function orders the render3D view to visualize a
 * quantity distribution on top of a mesh
 */
void ViennaMiniModule::render(Quantity& quan, int step)
{
  // retrieve the 'current' render object from the framework
  Render3D* render = multiview->getCurrentRender3D();

  // if current is actually a render view, and not a, for instance, chart view
  if(render)
  {
      // make sure that the 'domain' is up-to-date
      render->update_render_domain();

      if(quan.name == viennamini::id::potential())
        multiview->setCurrentLogScale(false);
      else
        multiview->setCurrentLogScale(true);

      // order the renderer to overlay the mesh with a quantitiy distribution
      // the first string parameter has to be the step-encoded key,
      // consisting of the quantity name and the simulation index postfix
      render->color_quantity(quan.name+viennamini::convert<std::string>(step), quan.name, quan.cell_level);
      render->update();
  }

  Chart2D* chart = multiview->getCurrentChart2D();

  // if current is actually a render view, and not a, for instance, chart view
  if(chart)
  {
      chart->showAllPlots();
  }
}

/**
 * @brief Function returns the size of the sequence of a given quantity
 * this function is currently out of order, need to revisit the approach of handling
 * a sequence of quantities .. this is also coupled with the VCR function of ViennaMOS
 */
std::size_t ViennaMiniModule::quantity_sequence_size(std::string quankey)
{
  // for each quantity we have the size-1 of the problem descrption
  // -1 because we only transfered results and not the initial guess,
  // which is on the 0 index of the prbolem description set.
//  if(vmini_simulator_->device_handle()->is_triangular2d())
//  {
//    return vmini_simulator_->device_handle()->get_problem_description_triangular_2d_set().size()-1;
//  }
//  else
//  if(vmini_simulator_->device_handle()->is_tetrahedral3d())
//  {
//    return vmini_simulator_->device_handle()->get_problem_description_tetrahedral_3d_set().size()-1;
//  }
}

/**
 * @brief If there are input-dependenices check them before returning true
 *
 * This method is called whenever a module in the active module set finished execution.
 * As a module might have generated data required by this module, we can check
 * here whether the correct data is available in the database.
 */
bool ViennaMiniModule::is_ready()
{
  if(database->has_key(vmini_device_db_key_))
    return true;
  else return false;
}

/**
 * @brief Give the module the opportunity to check whether there is new data available
 *
 * This method is called by the framework whenever a module in the active module set
 * finished execution and, triggered by that, this module reported its readiness via
 * returning 'true' in the 'is_ready()' method.
 *
 * application case: another module, such as a device generator, stored a new device
 * on the central database. Here we can access this device and build up the GUI,
 * which is most likely depending on the new device structure
 */
void ViennaMiniModule::update()
{
  if(database->has_key(vmini_device_db_key_))
  {
    viennamini::device_handle vmini_device = database->at<viennamini::device_handle>(vmini_device_db_key_);

//    vmini_simulator_.reset();
//    vmini_simulator_ = viennamini::simulator_handle(new viennamini::simulator);
//    vmini_simulator_->set_device_handle(vmini_device);
//    widget->process(vmini_simulator_, multiview->getCurrentRender3D());
  }
}

/**
 * @brief Reset the module to it's initial state
 */
void ViennaMiniModule::reset()
{

}

/**
 * @brief Function is executed when the 'Run' button is pressed
 */
void ViennaMiniModule::execute()
{
//  try
//  {
//    vmini_simulator_->run();

//    // ---------------------------------------------------------
//    //
//    // now transfer the data from ViennaMini to the renderer
//    //
//    // ---------------------------------------------------------

//    // reset the quantity set, so no previous simulation results spoil the current results
//    //
//    clear_quantities();

//    // and register new quantities, which will be provided in the next lines
//    //
//    register_quantity(Quantity("Potential", "V", this->name().toStdString(), viennamos::VERTEX, viennamos::SCALAR));
//    if(vmini_simulator_->problem_id() == viennamini::id::poisson_drift_diffusion_np())
//    {
//      register_quantity(Quantity("Electron Density", "1/m^3", this->name().toStdString(), viennamos::VERTEX, viennamos::SCALAR));
//      register_quantity(Quantity("Hole Density", "1/m^3", this->name().toStdString(), viennamos::VERTEX, viennamos::SCALAR));
//    }

//    // do the actual data transfer for the 2d case
//    //
//    if(vmini_simulator_->device_handle()->is_triangular2d())
//    {
//      typedef viennamini::mesh_triangular_2d                                          MeshType;
//      typedef typename viennagrid::result_of::cell_tag<MeshType>::type                CellTag;
//      typedef viennagrid::vertex_tag                                                  VertexTag;
//      typedef typename viennagrid::result_of::element<MeshType, CellTag>::type        CellType;
//      typedef typename viennagrid::result_of::element<MeshType, VertexTag>::type      VertexType;
//      typedef viennafvm::quantity<CellType,   double>   CellQuantityType;
//      typedef viennafvm::quantity<VertexType, double>   VertexQuantityType;

//      MeshType & mesh = vmini_simulator_->device_handle()->get_segmesh_triangular_2d().mesh;

//      viennamini::problem_description_triangular_2d_set& problem_description = vmini_simulator_->device_handle()->get_problem_description_triangular_2d_set();

//      // simulation result indices are 1-based, 0 contains the initial guesses
//      for(int si = 1; si < problem_description.size(); si++)
//      {
//        // get the potential result of this simulation run ..
//        CellQuantityType   & potential_cell = problem_description[si].get_quantity(viennamini::id::potential());

//        // .. and prepare a corresponding vertex quantity. encode the simulation run into the name
//        // note that si must be zero based again ... TODO ..
//        std::string encoded_quantity_run = potential_cell.get_name()+viennamini::convert<std::string>()(si-1);
//        VertexQuantityType   potential_vertex(0, encoded_quantity_run, viennagrid::vertices(mesh).size());

//        // now, transfer the cell quantity to the corresponding vertex quantity
//        viennagrid::quantity_transfer<CellTag, viennagrid::vertex_tag> (
//            mesh,
//            potential_cell, potential_vertex,
//            viennamini::arithmetic_averaging(),
//            viennamini::any_filter(), viennamini::any_filter()
//        );

//        // and copy it to the renderer
//        viennamos::copy(vmini_simulator_->device_handle(), potential_vertex, multiview);

//        // if DD is simulated, add the electron and hole distributions as well
//        //
//        if(vmini_simulator_->problem_id() == viennamini::id::poisson_drift_diffusion_np())
//        {
//          // electrons
//          //
//          CellQuantityType   & electrons_cell = problem_description[si].get_quantity(viennamini::id::electron_density());
//          encoded_quantity_run = electrons_cell.get_name()+viennamini::convert<std::string>()(si-1);
//          VertexQuantityType   electrons_vertex(0, encoded_quantity_run, viennagrid::vertices(mesh).size());

//          viennagrid::quantity_transfer<CellTag, viennagrid::vertex_tag> (
//              mesh,
//              electrons_cell, electrons_vertex,
//              viennamini::arithmetic_averaging(),
//              viennamini::any_filter(), viennamini::any_filter()
//          );

//          viennamos::copy(vmini_simulator_->device_handle(), electrons_vertex, multiview);

//          // holes
//          //
//          CellQuantityType   & holes_cell = problem_description[si].get_quantity(viennamini::id::hole_density());
//          encoded_quantity_run = holes_cell.get_name()+viennamini::convert<std::string>()(si-1);
//          VertexQuantityType   holes_vertex(0, encoded_quantity_run, viennagrid::vertices(mesh).size());

//          viennagrid::quantity_transfer<CellTag, viennagrid::vertex_tag> (
//              mesh,
//              holes_cell, holes_vertex,
//              viennamini::arithmetic_averaging(),
//              viennamini::any_filter(), viennamini::any_filter()
//          );

//          viennamos::copy(vmini_simulator_->device_handle(), holes_vertex, multiview);
//        }
//      }
//    }
//    else
//    if(vmini_simulator_->device_handle()->is_tetrahedral3d())
//    {
//        typedef viennamini::mesh_tetrahedral_3d                                         MeshType;
//        typedef typename viennagrid::result_of::cell_tag<MeshType>::type                CellTag;
//        typedef viennagrid::vertex_tag                                                  VertexTag;
//        typedef typename viennagrid::result_of::element<MeshType, CellTag>::type        CellType;
//        typedef typename viennagrid::result_of::element<MeshType, VertexTag>::type      VertexType;
//        typedef viennafvm::quantity<CellType,   double>   CellQuantityType;
//        typedef viennafvm::quantity<VertexType, double>   VertexQuantityType;

//        MeshType & mesh = vmini_simulator_->device_handle()->get_segmesh_tetrahedral_3d().mesh;

//        viennamini::problem_description_tetrahedral_3d_set& problem_description = vmini_simulator_->device_handle()->get_problem_description_tetrahedral_3d_set();

//        // simulation result indices are 1-based, 0 contains the initial guesses
//        for(int si = 1; si < problem_description.size(); si++)
//        {
//          // get the potential result of this simulation run ..
//          CellQuantityType   & potential_cell = problem_description[si].get_quantity(viennamini::id::potential());

//          // .. and prepare a corresponding vertex quantity. encode the simulation run into the name
//          // note that si must be zero based again ... TODO ..
//          std::string encoded_quantity_run = potential_cell.get_name()+viennamini::convert<std::string>()(si-1);
//          VertexQuantityType   potential_vertex(0, encoded_quantity_run, viennagrid::vertices(mesh).size());

//          // now, transfer the cell quantity to the corresponding vertex quantity
//          viennagrid::quantity_transfer<CellTag, viennagrid::vertex_tag> (
//              mesh,
//              potential_cell, potential_vertex,
//              viennamini::arithmetic_averaging(),
//              viennamini::any_filter(), viennamini::any_filter()
//          );

//          // and copy it to the renderer
//          viennamos::copy(vmini_simulator_->device_handle(), potential_vertex, multiview);

//          // if DD is simulated, add the electron and hole distributions as well
//          //
//          if(vmini_simulator_->problem_id() == viennamini::id::poisson_drift_diffusion_np())
//          {
//            // electrons
//            //
//            CellQuantityType   & electrons_cell = problem_description[si].get_quantity(viennamini::id::electron_density());
//            encoded_quantity_run = electrons_cell.get_name()+viennamini::convert<std::string>()(si-1);
//            VertexQuantityType   electrons_vertex(0, encoded_quantity_run, viennagrid::vertices(mesh).size());

//            viennagrid::quantity_transfer<CellTag, viennagrid::vertex_tag> (
//                mesh,
//                electrons_cell, electrons_vertex,
//                viennamini::arithmetic_averaging(),
//                viennamini::any_filter(), viennamini::any_filter()
//            );

//            viennamos::copy(vmini_simulator_->device_handle(), electrons_vertex, multiview);

//            // holes
//            //
//            CellQuantityType   & holes_cell = problem_description[si].get_quantity(viennamini::id::hole_density());
//            encoded_quantity_run = holes_cell.get_name()+viennamini::convert<std::string>()(si-1);
//            VertexQuantityType   holes_vertex(0, encoded_quantity_run, viennagrid::vertices(mesh).size());

//            viennagrid::quantity_transfer<CellTag, viennagrid::vertex_tag> (
//                mesh,
//                holes_cell, holes_vertex,
//                viennamini::arithmetic_averaging(),
//                viennamini::any_filter(), viennamini::any_filter()
//            );

//            viennamos::copy(vmini_simulator_->device_handle(), holes_vertex, multiview);
//          }
//        }
//    }


//    // ---------------------------------------------------------
//    //
//    // now transfer the CSV data to the chart renderer
//    //
//    // ---------------------------------------------------------

//    viennamini::data_table& vmini_table = vmini_simulator_->data_table();
//    vtkSmartPointer<vtkTable> vtk_table = vtkSmartPointer<vtkTable>::New();

//    for(std::size_t ci = 0; ci < vmini_table.column_size(); ci++)
//    {
//        vtkSmartPointer<vtkDoubleArray> vtk_col = vtkSmartPointer<vtkDoubleArray>::New();
//        std::string col_name = vmini_table.get_column_name(ci);
//        viennamini::data_table::column_type & col = vmini_table.get_column(col_name);
//        vtk_col->SetName(col_name.c_str());
//        vtk_col->SetArray(&vmini_table.get_column(col_name)[0], vmini_table.get_column(col_name).size(), 1);
//        vtk_table->AddColumn(vtk_col);
//    }

//    multiview->resetTables();
//    multiview->addTable("ViennaMini", vtk_table);
//    multiview->update();
//  }
//  catch(std::exception& e) {
//    QMessageBox::critical(0, QString(this->name()+" Error"), QString(e.what()));
//    return;
//  }
}
