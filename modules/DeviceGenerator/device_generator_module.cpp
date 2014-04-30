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

// Framework includes
//
#include "copy.hpp"

// Module includes
//
#include "device_generator_module.h"

// ViennaMini includes
//
#include "viennamini/utils/is_zero.hpp"

//#include "templates/capacitor1d.hpp"
//#include "templates/capacitor2d.hpp"
//#include "templates/capacitor3d.hpp"
//#include "templates/diode_np2d.hpp"

// ViennaMesh includes
//
#include "viennamesh/viennamesh.hpp"

/**
 * @brief The module's c'tor registers the module's UI widget and registers
 * output quantities
 */
DeviceGeneratorModule::DeviceGeneratorModule() :
  ModuleInterface(this)
{
    // setup a new UI widget and register it with this module
    //
    widget    = new DeviceGeneratorForm;
    register_module_widget(widget); // takes ownership of the widget - no deleting required
    csg_generator_ = new CSGGenerator;

    QObject::connect(widget, SIGNAL(meshFileEntered(QString const&)), this, SLOT(loadMeshFile(QString const&)));
    QObject::connect(widget, SIGNAL(csggeneratorTriggered()), this, SLOT(raiseCSGEditor()));
    QObject::connect(widget, SIGNAL(scaleDevice(double)), this, SLOT(scaleDevice(double)));
    QObject::connect(csg_generator_, SIGNAL(newCSGAvailable(QString)), this, SLOT(generateCSGDevice(QString)));

    // change the name of the 'run' button
    //
    module_control_widget->getButtonRun()->setText("Set Device");
}

DeviceGeneratorModule::~DeviceGeneratorModule()
{
  delete widget;
  delete csg_generator_;
}

QString DeviceGeneratorModule::name()
{
    return "Device Generator";
}

QString DeviceGeneratorModule::description()
{
    return "Generates semiconductor devices for device simulation";
}

QString DeviceGeneratorModule::version()
{
    return "1.0";
}

/**
 * @brief Function is after the framework forwarded
 * the base data to the module, e.g., material manager
 * so in this case we are lettings our UI know, that the
 * material data is available
 */
void DeviceGeneratorModule::preprocess()
{

}

/**
 * @brief Function orders the render3D view to visualize a
 * quantity distribution on top of a mesh
 */
void DeviceGeneratorModule::render(Quantity& quan, int step)
{

}

/**
 * @brief Function returns the size of the sequence of a given quantity
 * this function is currently out of order, need to revisit the approach of handling
 * a sequence of quantities .. this is also coupled with the VCR function of ViennaMOS
 */
std::size_t DeviceGeneratorModule::quantity_sequence_size(std::string quankey)
{

}

/**
 * @brief If there are input-dependenices check them before returning true
 */
bool DeviceGeneratorModule::is_ready()
{
    return true;
}

/**
 * @brief Give the module the opportunity to check whether there is new data available
 */
void DeviceGeneratorModule::update()
{

}

/**
 * @brief Reset the module to it's initial state
 */
void DeviceGeneratorModule::reset()
{

}

/**
 * @brief Function is executed when the 'Apply' button is pressed
 */
void DeviceGeneratorModule::execute()
{
  if(vmini_device_.get())
  {
    std::string vmini_device_db_key = "vmini_device";
    if(database->has_key(vmini_device_db_key)) database->erase<viennamini::device_handle>(vmini_device_db_key);
    database->insert(vmini_device_db_key, vmini_device_);
  }
  else
  {
    QMessageBox::critical(0, QString(this->name()+" Error"), "No device loaded!");
    return;
  }
}

/**
 * @brief Function reads an input mesh into the framework's central device database
 * This function is not part of the Module interface*
 */
void DeviceGeneratorModule::loadMeshFile(QString const& filename)
{
  // completly reset the simulator object, as a new device
  // forces us to reset everything and begin from scratch
  //
  if(vmini_device_.get()) vmini_device_.reset();
  vmini_device_ = viennamini::device_handle(new viennamini::device); // TODO pass a stream object into the c'tor

  std::vector<int> segment_ids;

  QString type = widget->getMeshType();

  if(type == viennamos::key::line1d)
  {
    try
    {
      vmini_device_->read(filename.toStdString(), viennamini::line_1d());
      viennamos::copy(vmini_device_, multiview);
    }
    catch(std::exception& e) {
      QMessageBox::critical(0, QString(this->name()+" Error"), QString(e.what()));
      return;
    }
  }
  else
  if(type == viennamos::key::triangular2d)
  {
    try
    {
      vmini_device_->read(filename.toStdString(), viennamini::triangular_2d());
      viennamos::copy(vmini_device_, multiview);
    }
    catch(std::exception& e) {
      QMessageBox::critical(0, QString(this->name()+" Error"), QString(e.what()));
      return;
    }
  }
  else
  if(type == viennamos::key::tetrahedral3d)
  {
    try
    {
      vmini_device_->read(filename.toStdString(), viennamini::tetrahedral_3d());
      viennamos::copy(vmini_device_, multiview);
    }
    catch(std::exception& e) {
      QMessageBox::critical(0, QString(this->name()+" Error"), QString(e.what()));
      return;
    }
  }
  else
  {
    QMessageBox::critical(0, QString(this->name()+" Error"), QString("Mesh dimension/type not supported!"));
    return;
  }


  // link the material database with the device ..
  //
  vmini_device_->set_material_database(material_manager->getLibrary());

  // which it requires to finalize the device, e.g., assign material-specific permittivities
  //
  vmini_device_->update();

  // forward the device handle, i.e., a smart pointer, to the
  // widget. also setup the GUI elements accordingly
  //
  widget->process(vmini_device_, multiview->getCurrentRender3D());

  // show the loaded device in the current render window
  //
  multiview->show_current_grid_segments();

  multiview->resetAllViews();
}

void DeviceGeneratorModule::scaleDevice(double factor)
{
  // Skip the scaling procedure, if a factor of 1.0 has been entered
  if(viennamini::is_zero(factor-1.0)) return;
  if(!vmini_device_.get())            return;

  try
  {
    vmini_device_->scale(factor);
    viennamos::copy(vmini_device_, multiview);
    multiview->show_current_grid_segments();
    multiview->resetAllViews();
  }
  catch(std::exception& e) {
    QMessageBox::critical(0, QString(this->name()+" Error"), QString(e.what()));
    return;
  }
}

void DeviceGeneratorModule::raiseCSGEditor()
{

  csg_generator_->show();
}

void DeviceGeneratorModule::generateCSGDevice(QString const& csg_string)
{
  // completly reset the simulator object, as a new device
  // forces us to reset everything and begin from scratch
  //
  if(vmini_device_.get()) vmini_device_.reset();
  vmini_device_ = viennamini::device_handle(new viennamini::device); // TODO pass a stream object into the c'tor
  vmini_device_->make_tetrahedral3d();

//  qDebug() << "about to mesh the following csg string: \"" << csg_string << "\"";

  // deactivate viennamesh output
  //
//  viennamesh::logger().set_log_level<viennamesh::info_tag>(0);
//  viennamesh::logger().set_log_level<viennamesh::stack_tag>(0);

  viennamesh::algorithm_handle mesher( new viennamesh::netgen::csg_mesh_generator() );
//  mesher->set_input( "default", csg_string.toStdString() );
//  mesher->set_input( "delaunay", true );                    // use delaunay meshing
////  mesher->set_input( "cell_size", 1.0 );                    // set the cell size
//  mesher->set_input( "grading", 0.3 );                      // set the element grading, 0...1 (0 => uniform mesh; 1 => aggressive local grading)
//  mesher->set_input( "optimization_steps", 3 );             // set the number of optimization steps for 3-D mesh optimization
////  mesher->set_input( "optimize_string", "cmdmustm" );
//  mesher->reference_output( "default", vmini_device_->get_segmesh_tetrahedral_3d() );
//  mesher->run();

//  vmini_device_->update_problem_description();

//  // the CSG has been meshed, now transfer to ViennaMOS
//  //
//  viennamos::copy(vmini_device_, multiview);

//  // link the material database with the device ..
//  //
//  vmini_device_->set_material_library(material_manager->getLibrary());

//  // which it requires to finalize the device, e.g., assign material-specific permittivities
//  //
//  vmini_device_->update();

//  // forward the device handle, i.e., a smart pointer, to the
//  // widget. also setup the GUI elements accordingly
//  //
//  widget->process(vmini_device_, multiview->getCurrentRender3D());

//  // show the loaded device in the current render window
//  //
//  multiview->show_current_grid_segments();

//  multiview->resetAllViews();
}
