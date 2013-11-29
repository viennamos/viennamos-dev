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


#include "device_generator_module.h"


/**
 * @brief The module's c'tor registers the module's UI widget and registers
 * output quantities
 */
DeviceGeneratorModule::DeviceGeneratorModule() : ModuleInterface(this)
{
    // setup a new UI widget and register it with this module
    //
    widget    = new DeviceGeneratorForm();
    register_module_widget(widget); // takes ownership of the widget - no deleting required

}

DeviceGeneratorModule::~DeviceGeneratorModule()
{
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

}




