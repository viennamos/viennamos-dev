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
#include "viennagrid/algorithm/scale.hpp"
#include "viennagrid/algorithm/quantity_transfer.hpp"

#include "viennafvm/quantity.hpp"
#include "viennamini/utils/average.hpp"
#include "viennamini/utils/filter.hpp"

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
    return "Classical Device Simulator";
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
 //   emit materialsAvailable(material_manager->getLibrary());
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

//      if((quan == pot_quan_vertex) || (quan == pot_quan_cell))
//        multiview->setCurrentLogScale(false);
//      else
//        multiview->setCurrentLogScale(true);

      // order the renderer to overlay the mesh with a quantitiy distribution
      render->color_quantity(quan.name, quan.name, quan.cell_level);
      render->update();
  }
}

/**
 * @brief Function returns the size of the sequence of a given quantity
 * this function is currently out of order, need to revisit the approach of handling
 * a sequence of quantities .. this is also coupled with the VCR function of ViennaMOS
 */
std::size_t ViennaMiniModule::quantity_sequence_size(std::string quankey)
{
//    if(device_id == Device22u::ID())
//    {
//        Device22u* device = database->value<Device22u*>(Device22u::ID());
//        typedef Device22u::Quantities    QuantitiesT;
//        QuantitiesT& quantities = device->quantities();
//        QuantitiesT::iterator it = quantities.find(quankey);
//        return (it->data_sequence).size();
//    }
//    else if(device_id == Device33u::ID())
//    {
//        Device33u* device = database->value<Device33u*>(Device33u::ID());
//        typedef Device33u::Quantities    QuantitiesT;
//        QuantitiesT& quantities = device->quantities();
//        QuantitiesT::iterator it = quantities.find(quankey);
//        return (it->data_sequence).size();
//    }
//        else return 0;
}

/**
 * @brief If there are input-dependenices check them before returning true
 */
bool ViennaMiniModule::is_ready()
{
  if(database->has_key("vmini_device"))
    return true;
  else return false;
}

/**
 * @brief Give the module the opportunity to check whether there is new data available
 */
void ViennaMiniModule::update()
{

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
  try
  {
    viennamini::device_handle vmini_device = database->at<viennamini::device_handle>("vmini_device");

    vmini_simulator.reset();
    vmini_simulator = viennamini::simulator_handle(new viennamini::simulator);
    vmini_simulator->set_device_handle(vmini_device);

    vmini_simulator->problem_id() = viennamini::id::laplace();

    vmini_simulator->current_contact_potential(1) = 1.0;
    vmini_simulator->current_contact_potential(5) = 0.0;
    vmini_simulator->run();

    if(vmini_device->is_triangular2d())
    {
      typedef viennamini::mesh_triangular_2d                                          MeshType;
      typedef typename viennagrid::result_of::cell_tag<MeshType>::type                CellTag;
      typedef viennagrid::vertex_tag                                                  VertexTag;
      typedef typename viennagrid::result_of::element<MeshType, CellTag>::type        CellType;
      typedef typename viennagrid::result_of::element<MeshType, VertexTag>::type      VertexType;
      typedef viennafvm::quantity<CellType,   double>   CellQuantityType;
      typedef viennafvm::quantity<VertexType, double>   VertexQuantityType;

      MeshType & mesh = vmini_device->get_segmesh_triangular_2d().mesh;

      CellQuantityType   & potential_cell = vmini_device->get_problem_description_triangular_2d(0).get_quantity(viennamini::id::potential());
      VertexQuantityType   potential_vertex(0, potential_cell.get_name(), viennagrid::vertices(mesh).size());

      viennagrid::quantity_transfer<CellTag, viennagrid::vertex_tag> (
          mesh,
          potential_cell, potential_vertex,
          viennamini::arithmetic_averaging(),
          viennamini::any_filter(), viennamini::any_filter()
      );
      viennamos::copy(vmini_device, potential_vertex, multiview);

      register_quantity(Quantity(potential_vertex.get_name(), "V", "ViennaMini", viennamos::VERTEX, viennamos::SCALAR));


    }

    //    pot_quan_vertex("potential",            "Volt", this->name().toStdString(), VERTEX, SCALAR);
    //    n_quan_vertex("electron_concentration", "1/m^3", this->name().toStdString(), VERTEX, SCALAR);
    //    p_quan_vertex("hole_concentration",     "1/m^3", this->name().toStdString(), VERTEX, SCALAR);
    //    pot_quan_cell("potential",            "Volt", this->name().toStdString(), CELL, SCALAR);
    //    n_quan_cell("electron_concentration", "1/m^3", this->name().toStdString(), CELL, SCALAR);
    //    p_quan_cell("hole_concentration",     "1/m^3", this->name().toStdString(), CELL, SCALAR);

        // register output quantities of this module in the framework
        //
    //    register_quantity(pot_quan_vertex);
    //    register_quantity(n_quan_vertex);
    //    register_quantity(p_quan_vertex);
    //    register_quantity(pot_quan_cell);
    //    register_quantity(n_quan_cell);
    //    register_quantity(p_quan_cell);



  }
  catch(std::exception& e) {
    QMessageBox::critical(0, QString("Error"), QString(e.what()));
    return;
  }


  // viennamos::copy(vmini_device->

//    DeviceParameters& parameters = widget->getParameters();

//    if((device_id == viennamos::Device2u::ID()) && (has<viennamos::Device2u>()))
//    {
//        viennamos::Device2u& device = access<viennamos::Device2u>();
//        ViennaMiniWorker* worker = new ViennaMiniWorker(&device, material_manager->getLibrary(), parameters,
//                                                        pot_quan_vertex, n_quan_vertex, p_quan_vertex,
//                                                        pot_quan_cell, n_quan_cell, p_quan_cell);
//        viennamos::offload(worker, messenger, SIGNAL(finished()), this, SLOT(transferResult()));
//    }
//    else
//    if((device_id == viennamos::Device3u::ID()) && (has<viennamos::Device3u>()))
//    {
//        viennamos::Device3u& device = access<viennamos::Device3u>();
//        ViennaMiniWorker* worker = new ViennaMiniWorker(&device, material_manager->getLibrary(), parameters,
//                                                        pot_quan_vertex, n_quan_vertex, p_quan_vertex,
//                                                        pot_quan_cell, n_quan_cell, p_quan_cell);
//        viennamos::offload(worker, messenger, SIGNAL(finished()), this, SLOT(transferResult()));
//    }
}

/**
 * @brief Function is called after the offloaded worker is finished and takes
 * care of copying the output data to the framework
 * This function is not part of the Module interface
 */
void ViennaMiniModule::transferResult()
{
//  if((device_id == viennamos::Device2u::ID()) && (has<viennamos::Device2u>()))
//  {
//    viennamos::Device2u& device = access<viennamos::Device2u>();
//    viennamos::copy(device, pot_quan_vertex, multiview);
//    viennamos::copy(device, n_quan_vertex,   multiview);
//    viennamos::copy(device, p_quan_vertex,   multiview);
//    viennamos::copy(device, pot_quan_cell,   multiview);
//    viennamos::copy(device, n_quan_cell,     multiview);
//    viennamos::copy(device, p_quan_cell,     multiview);
//  }
//  else
//  if((device_id == viennamos::Device3u::ID()) && (has<viennamos::Device3u>()))
//  {
//    viennamos::Device3u& device = access<viennamos::Device3u>();
//    viennamos::copy(device, pot_quan_vertex, multiview);
//    viennamos::copy(device, n_quan_vertex,   multiview);
//    viennamos::copy(device, p_quan_vertex,   multiview);
//    viennamos::copy(device, pot_quan_cell,   multiview);
//    viennamos::copy(device, n_quan_cell,     multiview);
//    viennamos::copy(device, p_quan_cell,     multiview);
//  }
//  emit finished();
}





