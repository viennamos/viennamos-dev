#ifndef VIENNAFVM_VIENNACL_SUPPORT_HPP
#define VIENNAFVM_VIENNACL_SUPPORT_HPP

/* =======================================================================
   Copyright (c) 2011, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
           ViennaFVM - The Vienna Finite Volume Method Library
                             -----------------

   authors:    Karl Rupp                          rupp@iue.tuwien.ac.at
               Josef Weinbub                   weinbub@iue.tuwien.ac.at
               (add your name here)

   license:    see file LICENSE in the ViennaFVM base directory
======================================================================= */

#include "viennacl/ocl/device.hpp"
#include "viennacl/ocl/platform.hpp"

namespace viennafvm {

void print_current_device(std::ostream& stream = std::cout)
{
  stream << "  -----------------------------------------" << std::endl;
  stream << "  Current OpenCL Device:" << std::endl;
  viennacl::ocl::device current_device = viennacl::ocl::current_device();
  stream << "  Name:             " << current_device.name() << std::endl;
  stream << "  Compute Units:    " << current_device.max_compute_units() << std::endl;
  stream << "  Workgroup Size:   " << current_device.max_workgroup_size() << std::endl;
  stream << "  Global Memory:    " << current_device.global_memory()/(1024*1024) << " MB" << std::endl;
  stream << "  Local Memory:     " << current_device.local_memory()/1024 << " KB" << std::endl;
  stream << "  Max-alloc Memory: " << current_device.max_allocable_memory()/(1024*1024) << " MB" << std::endl;
  stream << "  Double Support:   " << current_device.double_support() << std::endl;
  stream << "  Driver Version:   " << current_device.driver_version() << std::endl;
  stream << "  -----------------------------------------" << std::endl;
}

void print_platform_devices(std::ostream& stream = std::cout)
{
   //
   //  retrieve the devices
   //
   typedef std::vector< viennacl::ocl::platform > platforms_type;
   platforms_type platforms = viennacl::ocl::get_platforms();
   
   bool is_first_element = true;
   for (platforms_type::iterator platform_iter  = platforms.begin();
                                 platform_iter != platforms.end();
                               ++platform_iter)
   {
    typedef std::vector<viennacl::ocl::device> devices_type;
    devices_type devices = platform_iter->devices(CL_DEVICE_TYPE_ALL);

    //
    // print some platform info
    //
    stream << "# =========================================" << std::endl;
    stream << "#         Platform Information             " << std::endl;
    stream << "# =========================================" << std::endl;
    
    stream << "#" << std::endl;
    stream << "# Vendor and version: " << platform_iter->info() << std::endl;
    stream << "#" << std::endl;
    
    if (is_first_element)
    {
      stream << "# ViennaCL uses this OpenCL platform by default." << std::endl;
      is_first_element = false;
    }
    
    
    //
    //  traverse the devices and print the information
    //
    stream << "# " << std::endl;
    stream << "# Available Devices: " << std::endl;
    stream << "# " << std::endl;
    for(devices_type::iterator iter = devices.begin(); iter != devices.end(); iter++)
    {
        stream << std::endl;

        if(*iter == viennacl::ocl::current_device())
          stream << "  (Current) -------------------------------" << std::endl;
        else
          stream << "  -----------------------------------------" << std::endl;
        stream << "  No.:              " << std::distance(devices.begin(), iter) << std::endl;
        stream << "  Name:             " << iter->name() << std::endl;
        stream << "  Compute Units:    " << iter->max_compute_units() << std::endl;
        stream << "  Workgroup Size:   " << iter->max_workgroup_size() << std::endl;
        stream << "  Global Memory:    " << iter->global_memory()/(1024*1024) << " MB" << std::endl;
        stream << "  Local Memory:     " << iter->local_memory()/1024 << " KB" << std::endl;
        stream << "  Max-alloc Memory: " << iter->max_allocable_memory()/(1024*1024) << " MB" << std::endl;
        stream << "  Double Support:   " << iter->double_support() << std::endl;
        stream << "  Driver Version:   " << iter->driver_version() << std::endl;
        stream << "  -----------------------------------------" << std::endl;
    }
    stream << std::endl;
    stream << "###########################################" << std::endl;
    stream << std::endl;
   }
   

}

} // viennafvm

#endif
