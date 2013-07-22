/* =======================================================================
   Copyright (c) 2011, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
           ViennaFVM - The Vienna Finite Volume Method Library
                             -----------------

   authors:    Karl Rupp                          rupp@iue.tuwien.ac.at
               (add your name here)

   license:    To be discussed, see file LICENSE in the ViennaFVM base directory
======================================================================= */

//#define VIENNAFVM_DEBUG

// Define NDEBUG to get any reasonable performance with ublas:
#define NDEBUG

#define VIENNAMINI_DEBUG

// include necessary system headers
#include <iostream>

// ViennaMini main include:
#include "viennamini/simulator.hpp"

// Vienna Includes
#include "viennamaterials/library.hpp"
#include "viennamaterials/kernels/pugixml.hpp"

#include "viennagrid/io/vtk_writer.hpp"
#include "viennagrid/algorithm/scale.hpp"

const int source          = 0;
const int channel         = 1;
const int drain           = 2;
const int oxide           = 3;
const int gate_contact    = 4;
const int body            = 5;
const int body_contact    = 6;
const int source_contact  = 7;
const int drain_contact   = 8;

/** @brief Structure the device by assigning 'roles', such as 'Oxide' to a segment.
    Also, assign a doping to the semiconductor regions */
template<typename DomainT, typename SegmentationT, typename StorageT>
void prepare(viennamini::device<DomainT, SegmentationT, StorageT>& device)
{
  // Source
  device.assign_name          (source, "source");
  device.assign_material      (source, "Si");
  device.assign_semiconductor (source, 1e24, 1.E8);   // ND, NA

  // Channel
  device.assign_name          (channel, "channel");
  device.assign_material      (channel, "Si");
  device.assign_semiconductor (channel, 1.E18, 1.E14);

  // Drain
  device.assign_name          (drain, "drain");
  device.assign_material      (drain, "Si");
  device.assign_semiconductor (drain, 1.E24, 1.E8);

  // Oxide
  device.assign_name          (oxide, "oxide");
  device.assign_material      (oxide, "HfO2");
  device.assign_oxide         (oxide);

  // Gate Contact
  device.assign_name          (gate_contact, "gate_contact");
  device.assign_material      (gate_contact, "Cu");
  device.assign_contact       (gate_contact);

  // Body
  device.assign_name          (body, "body");
  device.assign_material      (body, "Si");
  device.assign_semiconductor (body, 1.E18, 1.E14);

  // Body Contact
  device.assign_name          (body_contact, "body_contact");
  device.assign_material      (body_contact, "Cu");
  device.assign_contact       (body_contact);

  // Source Contact
  device.assign_name          (source_contact, "source_contact");
  device.assign_material      (source_contact, "Cu");
  device.assign_contact       (source_contact);

  // Drain Contact
  device.assign_name          (drain_contact, "drain_contact");
  device.assign_material      (drain_contact, "Cu");
  device.assign_contact       (drain_contact);

}

/** @brief Assign actual values to the dirichlet contacts */
void prepare_boundary_conditions(viennamini::config& config)
{
  // Gate Contact
  config.assign_contact(gate_contact, 0.2, 0.4);  // segment id, contact potential, workfunction

  // Body Contact
  config.assign_contact(body_contact, 0.0, 0.0);

  // Source Contact
  config.assign_contact(source_contact, 0.0, 0.0);

  // Drain Contact
  config.assign_contact(drain_contact, 0.2, 0.0);
}

int main(int argc, char* argv[])
{
  if(argc != 2)
  {
      std::cerr << "Missing parameters - Usage: " << argv[0] << " path/to/trigate.mesh" << std::endl;
      return -1;
  }
#ifdef VIENNACL_WITH_OPENMP
  viennafvm::print_platform_devices();
#endif

  typedef double                                                       NumericType;
  typedef viennagrid::domain_t< viennagrid::config::tetrahedral_3d >   DomainType;
  typedef viennagrid::result_of::segmentation<DomainType>::type        SegmentationType;
  typedef SegmentationType::segment_type                               SegmentType;
  typedef viennadata::storage<>                                        StorageType;

  //
  // Create a domain from file
  //
  DomainType         domain;
  SegmentationType   segments(domain);
  StorageType        storage;

  try
  {
  viennagrid::io::netgen_reader my_reader;
  my_reader(domain, segments, argv[1]);
  }
  catch (...)
  {
  std::cerr << "File-Reader failed. Aborting program..." << std::endl;
  return EXIT_FAILURE;
  }

  //
  // scale to nanometer
  //
  viennagrid::scale(domain, 1e-9);

  //
  // Prepare material library
  //
  typedef vmat::Library<vmat::tag::pugixml>::type  MaterialLibraryType;
  MaterialLibraryType matlib;
  matlib.load("../external/ViennaMaterials/database/materials.xml");

  //
  // Create a device and a config object
  //
  typedef viennamini::device<DomainType, SegmentationType, StorageType>   DeviceType;
  DeviceType device(domain, segments, storage);
  viennamini::config config;

  //
  // Prepare device, i.e., assign doping and segment roles,
  // e.g., oxide, semiconductor, contact
  //
  prepare(device);

  //
  // Assign contact values
  //
  prepare_boundary_conditions(config);

  //
  // Set simulation parameters
  //
  config.temperature()                        = 300;
  config.damping()                            = 1.0;
  config.linear_breaktol()                    = 1.0E-13;
  config.linear_iterations()                  = 1000;
  config.nonlinear_iterations()               = 100;
  config.nonlinear_breaktol()                 = 1.0E-2;
  config.initial_guess_smoothing_iterations() = 5;

  //
  // Create a simulator object
  //
  typedef viennamini::simulator<DeviceType, MaterialLibraryType>     SimulatorType;
  SimulatorType simulator(device, matlib, config);

  //
  // Run the simulation
  //
  simulator();

  // Write results to vtk files
  simulator.write_result("trigate");


  std::cout << "********************************************" << std::endl;
  std::cout << "* MOSFET simulation finished successfully! *" << std::endl;
  std::cout << "********************************************" << std::endl;
  return EXIT_SUCCESS;
}

