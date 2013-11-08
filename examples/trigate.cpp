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

// include necessary system headers
#include <iostream>

// ViennaMini main include:
#include "viennamini/simulator.hpp"

// Vienna Includes
#include "viennamaterials/library.hpp"
#include "viennamaterials/kernels/pugixml.hpp"

const int source          = 1;
const int channel         = 2;
const int drain           = 3;
const int oxide           = 4;
const int gate_contact    = 5;
const int body            = 6;
const int body_contact    = 7;
const int source_contact  = 8;
const int drain_contact   = 9;

/** @brief Structure the device by assigning 'roles', such as 'Oxide' to a segment.
    Also, assign a doping to the semiconductor regions */
template<typename MeshT, typename SegmentationT, typename StorageT>
void prepare(viennamini::device<MeshT, SegmentationT, StorageT>& device)
{
  // Source
  device.assign_name          (source, "source");
  device.assign_material      (source, "Si");
  device.assign_semiconductor (source, 1e24, 1.E8);   // n+

  // Channel
  device.assign_name          (channel, "channel");
  device.assign_material      (channel, "Si");
  device.assign_semiconductor (channel, 1.E12, 1.E20);  // p-substrate

  // Drain
  device.assign_name          (drain, "drain");
  device.assign_material      (drain, "Si");
  device.assign_semiconductor (drain, 1.E24, 1.E8);     // n+

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
  device.assign_semiconductor (body, 1.E12, 1.E20);  // p-substrate

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
  config.assign_contact(gate_contact, 0.4, 0.4);  // segment id, contact potential, workfunction

  // Body Contact
  config.assign_contact(body_contact, 0.0, 0.0);

  // Source Contact
  config.assign_contact(source_contact, 0.0, 0.0);

  // Drain Contact
  config.assign_contact(drain_contact, 0.4, 0.0);
}

int main(int argc, char* argv[])
{
  //
  // Create a domain from file
  //
  viennamini::MeshTetrahedral3DType           mesh;
  viennamini::SegmentationTetrahedral3DType   segments(mesh);
  viennamini::StorageType                     storage;

  try
  {
    viennagrid::io::netgen_reader my_reader;
    my_reader(mesh, segments, "../external/ViennaDeviceCollection/half-trigate3d/half-trigate3d.mesh");
  }
  catch (...)
  {
    std::cerr << "File-Reader failed. Aborting program..." << std::endl;
    return EXIT_FAILURE;
  }

  //
  // scale to nanometer
  //
  viennagrid::scale(mesh, 1e-9);

  //
  // Prepare material library
  //
  viennamini::MatLibPugixmlType matlib;
  matlib.load("../external/ViennaMaterials/database/materials.xml");

  //
  // Create a device and a config object
  //
  viennamini::DeviceTetrahedral3DType device(mesh, segments, storage);
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
  viennamini::SimulatorTetrahedral3DType sim(device, matlib, config);

  //
  // Run the simulation
  //
  sim();

  // Write results to vtk files
  sim.write_result("trigate");


  std::cout << "********************************************" << std::endl;
  std::cout << "* TRIGATE simulation finished successfully! *" << std::endl;
  std::cout << "********************************************" << std::endl;
  return EXIT_SUCCESS;
}

