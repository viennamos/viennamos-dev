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


/** @brief Structure the device by assigning 'roles', such as 'Oxide' to a segment.
    Also, assign a doping to the semiconductor regions */
template<typename Domain, typename Segmentation, typename Storage>
void prepare(viennamini::device<Domain, Segmentation, Storage>& device)
{
  const int source          = 0;
  const int channel         = 1;
  const int drain           = 2;
  const int oxide           = 3;
  const int gate_contact    = 4;
  const int body            = 5;
  const int body_contact    = 6;
  const int source_contact  = 7;
  const int drain_contact   = 8;


  // Segment 0: Source
  device.assign_name          (source, "source");
  device.assign_material      (source, "Si");
  device.assign_semiconductor (source, 1.E24, 1.E8);   // ND, NA

  // Segment 1: Channel
  device.assign_name          (channel, "channel");
  device.assign_material      (channel, "Si");
  device.assign_semiconductor (channel, 1.E17, 1.E15);

  // Segment 2: Drain
  device.assign_name          (drain, "drain");
  device.assign_material      (drain, "Si");
  device.assign_semiconductor (drain, 1.E24, 1.E8);

  // Segment 3: Oxide
  device.assign_name          (oxide, "oxide");
  device.assign_material      (oxide, "HfO2");
  device.assign_oxide         (oxide);

  // Segment 4: Gate Contact
  device.assign_name          (gate_contact, "gate_contact");
  device.assign_material      (gate_contact, "Cu");
  device.assign_contact       (gate_contact);

  // Segment 5: Body
  device.assign_name          (body, "body");
  device.assign_material      (body, "Si");
  device.assign_semiconductor (body, 1.E17, 1.E15);

  // Segment 6: Body Contact
  device.assign_name          (body_contact, "body_contact");
  device.assign_material      (body_contact, "Cu");
  device.assign_contact       (body_contact);

  // Segment 7: Source Contact
  device.assign_name          (source_contact, "source_contact");
  device.assign_material      (source_contact, "Cu");
  device.assign_contact       (source_contact);

  // Segment 8: Drain Contact
  device.assign_name          (drain_contact, "drain_contact");
  device.assign_material      (drain_contact, "Cu");
  device.assign_contact       (drain_contact);

}

/** @brief Assign actual values to the dirichlet contacts */
void prepare_boundary_conditions(viennamini::config& config)
{
  const int gate_contact    = 4;
  const int body_contact    = 6;
  const int source_contact  = 7;
  const int drain_contact   = 8;

  // Segment 4: Gate Contact
  config.assign_contact(gate_contact, 0.3, 0.0);  // segment id, contact potential, workfunction

  // Segment 6: Body Contact
  config.assign_contact(body_contact, 0.0, 0.0);

  // Segment 7: Source Contact
  config.assign_contact(source_contact, 0.0, 0.0);

  // Segment 8: Drain Contact
  config.assign_contact(drain_contact, 0.3, 0.0);
}

/** @brief Scales the entire simulation domain (device) by the provided factor. This is accomplished by multiplying all point coordinates with this factor. */
template <typename DomainType>
void scale_domain(DomainType & domain, double factor)
{
  typedef typename viennagrid::result_of::element<DomainType, viennagrid::vertex_tag>::type         VertexType;
  typedef typename viennagrid::result_of::element_range<DomainType, viennagrid::vertex_tag>::type   VertexContainer;
  typedef typename viennagrid::result_of::iterator<VertexContainer>::type                           VertexIterator;

  VertexContainer vertices = viennagrid::elements<VertexType>(domain);
  for ( VertexIterator vit = vertices.begin();
        vit != vertices.end();
        ++vit )
  {
    viennagrid::point(domain, *vit) *= factor;
  }
}

int main(int argc, char* argv[])
{
    typedef double                                                       numeric_type;
    typedef viennagrid::domain_t< viennagrid::config::tetrahedral_3d >    domain_type;
    typedef viennagrid::result_of::segmentation<domain_type>::type       segmentation_type;
    typedef segmentation_type::segment_type                              segment_type;
    typedef viennadata::storage<>                                        storage_type;

    //
    // Create a domain from file
    //
    domain_type         domain;
    segmentation_type   segments(domain);
    storage_type        storage;

    try
    {
    viennagrid::io::netgen_reader my_reader;
    my_reader(domain, segments, "../examples/data/half-trigate-2.mesh");
    }
    catch (...)
    {
    std::cerr << "File-Reader failed. Aborting program..." << std::endl;
    return EXIT_FAILURE;
    }

    //
    // scale to nanometer
    //
    scale_domain(domain, 1e-9);

    //
    // Prepare material library
    //
    typedef vmat::Library<vmat::tag::pugixml>::type  material_library_type;
    material_library_type matlib;
    matlib.load("../external/ViennaMaterials/database/materials.xml");

    //
    // Create a device and a config object
    //
    typedef viennamini::device<domain_type, segmentation_type, storage_type>   device_type;
    device_type device(domain, segments, storage);
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
    config.damping()                            = 0.3;
    config.linear_breaktol()                    = 1.0E-13;
    config.linear_iterations()                  = 500;
    config.nonlinear_iterations()               = 100;
    config.nonlinear_breaktol()                 = 1.0E-3;
    config.initial_guess_smoothing_iterations() = 6;

    //
    // Create a simulator object
    //
    typedef viennamini::simulator<device_type, material_library_type>     simulator_type;
    simulator_type simulator(device, matlib, config);

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

