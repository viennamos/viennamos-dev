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

// include necessary system headers
#include <iostream>

// ViennaMini main include:
#include "viennamini/simulator.hpp"


template <typename DomainType, typename MaterialLibrary>
void init_quantities(DomainType const & my_domain, MaterialLibrary & matlib, double n_plus, double p_plus)
{
  viennamini::permittivity_key       eps_key;
  viennamini::donator_doping_key     ND_key;
  viennamini::acceptor_doping_key    NA_key;
  viennamini::builtin_potential_key  builtin_key;

  //
  // Init permittivity
  //
  viennafvm::set_quantity_region(eps_key, my_domain, true);               // permittivity is (for simplicity) defined everywhere
  viennafvm::set_quantity_value(eps_key, my_domain,               matlib.getParameterValue("Si", "permittivity")   * 8.854e-12);                // permittivity of silicon
  viennafvm::set_quantity_value(eps_key, my_domain.segments()[2], matlib.getParameterValue("HfO2", "permittivity") * 8.854e-12);  // permittivty of HfO2

  //
  // Initialize doping
  //

  // donator doping
  viennafvm::set_quantity_region(ND_key, my_domain.segments()[4], true);    // source
  viennafvm::set_quantity_region(ND_key, my_domain.segments()[5], true);    // drain
  viennafvm::set_quantity_region(ND_key, my_domain.segments()[6], true);    // body
  viennafvm::set_quantity_region(ND_key, my_domain.segments()[7], true);    // body contact (floating body)

  viennafvm::set_quantity_value(ND_key, my_domain.segments()[4],  n_plus);      // source
  viennafvm::set_quantity_value(ND_key, my_domain.segments()[5],  n_plus);      // drain
  viennafvm::set_quantity_value(ND_key, my_domain.segments()[6],  1e32/p_plus); // body
  viennafvm::set_quantity_value(ND_key, my_domain.segments()[7],  1e32/p_plus); // body contact (floating body)

  // acceptor doping
  viennafvm::set_quantity_region(NA_key, my_domain.segments()[4], true);   // source
  viennafvm::set_quantity_region(NA_key, my_domain.segments()[5], true);   // drain
  viennafvm::set_quantity_region(NA_key, my_domain.segments()[6], true);   // body
  viennafvm::set_quantity_region(NA_key, my_domain.segments()[7], true);   // body contact (floating body)

  viennafvm::set_quantity_value(NA_key, my_domain.segments()[4],  1e32/n_plus); // source
  viennafvm::set_quantity_value(NA_key, my_domain.segments()[5],  1e32/n_plus); // drain
  viennafvm::set_quantity_value(NA_key, my_domain.segments()[6],  p_plus);      // body
  viennafvm::set_quantity_value(NA_key, my_domain.segments()[7],  p_plus);      // body contact (floating body)

  // built-in potential:
  // TODO: This can be derived automatically from the specification of the doping.
  //       Compute this as a preprocessing/setup step for the actual simulation
  viennafvm::set_quantity_region(builtin_key, my_domain, true);   // defined everywhere

  viennafvm::set_quantity_value(builtin_key, my_domain.segments()[0], viennamini::built_in_potential(300, n_plus, 1e32/n_plus)); // gate
  viennafvm::set_quantity_value(builtin_key, my_domain.segments()[1], viennamini::built_in_potential(300, n_plus, 1e32/n_plus)); // source contact
  viennafvm::set_quantity_value(builtin_key, my_domain.segments()[2], viennamini::built_in_potential(300, n_plus, 1e32/n_plus)); // oxide (for simplicity set to same as gate)
  viennafvm::set_quantity_value(builtin_key, my_domain.segments()[3], viennamini::built_in_potential(300, n_plus, 1e32/n_plus)); // drain contact
  viennafvm::set_quantity_value(builtin_key, my_domain.segments()[4], viennamini::built_in_potential(300, n_plus, 1e32/n_plus)); // source
  viennafvm::set_quantity_value(builtin_key, my_domain.segments()[5], viennamini::built_in_potential(300, n_plus, 1e32/n_plus)); // drain
  viennafvm::set_quantity_value(builtin_key, my_domain.segments()[6], viennamini::built_in_potential(300, 1e32/p_plus, p_plus)); // body
  viennafvm::set_quantity_value(builtin_key, my_domain.segments()[7], viennamini::built_in_potential(300, 1e32/p_plus, p_plus)); // body contact (floating body)
}

/** @brief Scales the entire simulation domain (device) by the provided factor. This is accomplished by multiplying all point coordinates with this factor. */
template <typename DomainType>
void scale_domain(DomainType & domain, double factor)
{
  typedef typename viennagrid::result_of::ncell_range<DomainType, 0 > ::type VertexContainer;
  typedef typename viennagrid::result_of::iterator<VertexContainer>::type VertexIterator;

  VertexContainer vertices = viennagrid::ncells < 0 > (domain);
  for ( VertexIterator vit = vertices.begin();
        vit != vertices.end();
        ++vit )
  {
    vit->point() *= factor; // scale
  }
}

int main()
{
  typedef double   numeric_type;

  typedef viennagrid::config::triangular_2d                           ConfigType;
  typedef viennagrid::result_of::domain<ConfigType>::type             DomainType;
  typedef typename ConfigType::cell_tag                     CellTag;

  typedef viennagrid::result_of::ncell<ConfigType, CellTag::dim>::type        CellType;

  //
  // Create a domain from file
  //
  DomainType my_domain;

  try
  {
    viennagrid::io::netgen_reader my_reader;
    my_reader(my_domain, "../examples/data/mosfet.mesh");
  }
  catch (...)
  {
    std::cerr << "File-Reader failed. Aborting program..." << std::endl;
    return EXIT_FAILURE;
  }

  scale_domain(my_domain, 1e-9); // scale to nanometer

  //
  // Prepare material library
  //
  typedef vmat::Library<vmat::tag::pugixml>::type  MaterialLibrary;
  MaterialLibrary matlib;
  matlib.load("../external/ViennaMaterials/database/materials.xml");

  //
  // Set initial values
  //
  double n_plus = 1e24;
  double p_plus = 1e20;

  init_quantities(my_domain, matlib, n_plus, p_plus);

  //
  // Instantiate simulator object:
  //
  viennamini::simulator<MaterialLibrary>  my_simulator;

  //
  // Setting boundary information on domain (see mosfet.in2d for segment indices)
  // TODO: The boundary setters should go into viennamini::simulator in some way, so that the simulator object holds the complete simulation state
  //       A user should call something like viennamini::set_dirichlet_boundary(), which then forwards to ViennaFVM (or possibly ViennaFVM).
  //

  // potential:
  double built_in_pot = viennamini::built_in_potential(300, n_plus, 1e32/n_plus); // should match specification in init_quantities()!
  viennafvm::set_dirichlet_boundary(my_domain.segments()[0], 0.2 + built_in_pot, my_simulator.quantity_potential()); // Gate contact
  viennafvm::set_dirichlet_boundary(my_domain.segments()[1], 0.0 + built_in_pot, my_simulator.quantity_potential()); // Source contact
  viennafvm::set_dirichlet_boundary(my_domain.segments()[3], 0.2 + built_in_pot, my_simulator.quantity_potential()); // Drain contact
  // using floating body, hence commented:
  viennafvm::set_dirichlet_boundary(my_domain.segments()[7], 0.0 + viennamini::built_in_potential(300, 1e32/p_plus, p_plus), my_simulator.quantity_potential()); // Body contact

  // electron density
  viennafvm::set_dirichlet_boundary(my_domain.segments()[1],      n_plus, my_simulator.quantity_electron_density()); // Source contact
  viennafvm::set_dirichlet_boundary(my_domain.segments()[3],      n_plus, my_simulator.quantity_electron_density()); // Drain contact
  viennafvm::set_dirichlet_boundary(my_domain.segments()[7], 1e32/p_plus, my_simulator.quantity_electron_density()); // Body contact

  // hole density
  viennafvm::set_dirichlet_boundary(my_domain.segments()[1], 1e32/n_plus, my_simulator.quantity_hole_density()); // Source contact
  viennafvm::set_dirichlet_boundary(my_domain.segments()[3], 1e32/n_plus, my_simulator.quantity_hole_density()); // Drain contact
  viennafvm::set_dirichlet_boundary(my_domain.segments()[7],      p_plus, my_simulator.quantity_hole_density()); // Body contact


  //
  // Set quantity mask: By default, a quantity is defined on the entire domain.
  //                    Boundary equations are already specified above.
  //                    All that is left is to specify regions where a quantity 'does not make sense'
  //                    Here, we need to disable {n,p} in the gate oxide and the gate
  //
  // ViennaMini-remark: This can be done fully automatic by keeping track of material properties.
  //                    For example, it is clear that the electron and hole concentration is not defined for an insulator.
  //

  viennafvm::disable_quantity(my_domain.segments()[0], my_simulator.quantity_electron_density()); // Gate contact
  viennafvm::disable_quantity(my_domain.segments()[2], my_simulator.quantity_electron_density()); // Gate oxide

  viennafvm::disable_quantity(my_domain.segments()[0], my_simulator.quantity_hole_density()); // Gate contact
  viennafvm::disable_quantity(my_domain.segments()[2], my_simulator.quantity_hole_density()); // Gate oxide

  //
  // Initial conditions (required for nonlinear problems)
  //
  viennafvm::set_initial_guess(my_domain, my_simulator.quantity_potential(), viennamini::builtin_potential_key());
//  viennafvm::smooth_initial_guess(my_domain, my_simulator.quantity_potential(), viennafvm::arithmetic_mean_smoother());
//  viennafvm::smooth_initial_guess(my_domain, my_simulator.quantity_potential(), viennafvm::arithmetic_mean_smoother());

  viennafvm::set_initial_guess(my_domain, my_simulator.quantity_electron_density(), viennamini::donator_doping_key());
//  viennafvm::smooth_initial_guess(my_domain, my_simulator.quantity_electron_density(), viennafvm::geometric_mean_smoother());
//  viennafvm::smooth_initial_guess(my_domain, my_simulator.quantity_electron_density(), viennafvm::geometric_mean_smoother());

  viennafvm::set_initial_guess(my_domain, my_simulator.quantity_hole_density(), viennamini::acceptor_doping_key());
//  viennafvm::smooth_initial_guess(my_domain, my_simulator.quantity_hole_density(), viennafvm::geometric_mean_smoother());
//  viennafvm::smooth_initial_guess(my_domain, my_simulator.quantity_hole_density(), viennafvm::geometric_mean_smoother());


  viennagrid::io::vtk_writer<DomainType> my_vtk_writer;
  viennagrid::io::add_scalar_data_on_cells<viennamini::donator_doping_key,   double, DomainType>(my_vtk_writer, viennamini::donator_doping_key(),   "donators");
  viennagrid::io::add_scalar_data_on_cells<viennamini::acceptor_doping_key,  double, DomainType>(my_vtk_writer, viennamini::acceptor_doping_key(),  "acceptors");
  viennagrid::io::add_scalar_data_on_cells<viennamini::builtin_potential_key,double, DomainType>(my_vtk_writer, viennamini::builtin_potential_key(),"pot_bi");    
  my_vtk_writer(my_domain, "mosfet_setup");

  //
  // Run simulator
  //
  viennamini::Config config;  
  my_simulator(my_domain, config);


  //
  // Writing all solution variables back to domain.
  //
  std::vector<long> result_ids(3); //TODO: Better way to make potential, electron_density and hole_density accessible
  result_ids[0] = my_simulator.quantity_potential().id();
  result_ids[1] = my_simulator.quantity_electron_density().id();
  result_ids[2] = my_simulator.quantity_hole_density().id();

  //
  // TODO:
  //
  viennafvm::io::write_solution_to_VTK_file(my_simulator.result(), "mosfet_result", my_domain, result_ids);

  std::cout << "********************************************" << std::endl;
  std::cout << "* MOSFET simulation finished successfully! *" << std::endl;
  std::cout << "********************************************" << std::endl;
  return EXIT_SUCCESS;
}

