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

// ViennaFVM includes:
#include "viennafvm/forwards.h"
#include "viennafvm/linear_assembler.hpp"
#include "viennafvm/io/vtk_writer.hpp"
#include "viennafvm/boundary.hpp"
#include "viennafvm/pde_solver.hpp"
#include "viennafvm/initial_guess.hpp"
#include "viennafvm/linear_solvers/viennacl.hpp"

// ViennaGrid includes:
#include "viennagrid/domain/domain.hpp"
#include "viennagrid/config/default_configs.hpp"
#include "viennagrid/io/netgen_reader.hpp"
#include "viennagrid/io/vtk_writer.hpp"
#include "viennagrid/algorithm/voronoi.hpp"

// ViennaData includes:
#include "viennadata/api.hpp"

// ViennaMath includes:
#include "viennamath/expression.hpp"

#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/operation.hpp>
#include <boost/numeric/ublas/operation_sparse.hpp>

//
// Defining a bunch of accessor keys for physical quantities.
// These should be part of a semiconductor application based on ViennaFVM, not of ViennaFVM itself
// (which is just a generic finite volume solver and agnostic with respect to the actual physics).
//

struct permittivity_key
{
  // Operator< is required for compatibility with std::map
  bool operator<(permittivity_key const & /*other*/) const { return false; }
};

struct builtin_potential_key
{
  // Operator< is required for compatibility with std::map
  bool operator<(builtin_potential_key const & /*other*/) const { return false; }
};

// N_D
struct donator_doping_key
{
  // Operator< is required for compatibility with std::map
  bool operator<(donator_doping_key const & /*other*/) const { return false; }
};

// N_A
struct acceptor_doping_key
{
  // Operator< is required for compatibility with std::map
  bool operator<(acceptor_doping_key const & /*other*/) const { return false; }
};





double built_in_potential(double /*temperature*/, double doping_n, double doping_p)
{
  const double net_doping = doping_n - doping_p;
  const double x = std::abs(net_doping) / (2.0 * 1e16);

  double bpot = 0.026 * std::log(x + std::sqrt( 1.0 + x*x ) );
                              // V_T * arsinh( net_doping/(2 n_i))

  if ( net_doping < 0) //above formula does not yet consider the doping polarity
    bpot *= -1.0;

  return bpot;
}

template <typename SegmentationType, typename StorageType>
void init_quantities(SegmentationType const & segmentation, StorageType & storage, double n_plus, double p_plus)
{
  //
  // Init permittivity
  //
  viennafvm::set_quantity_region(segmentation.domain(), storage, permittivity_key(), true);               // permittivity is (for simplicity) defined everywhere
  viennafvm::set_quantity_value(segmentation.domain(),  storage, permittivity_key(), 11.7 * 8.854e-12);   // permittivity of silicon
  viennafvm::set_quantity_value(segmentation(3),        storage, permittivity_key(), 15.6 * 8.854e-12);   // permittivty of HfO2

  //
  // Initialize doping
  //

  // donator doping
  viennafvm::set_quantity_region(segmentation(5), storage, donator_doping_key(), true);    // source
  viennafvm::set_quantity_region(segmentation(6), storage, donator_doping_key(), true);    // drain
  viennafvm::set_quantity_region(segmentation(7), storage, donator_doping_key(), true);    // body
  viennafvm::set_quantity_region(segmentation(8), storage, donator_doping_key(), true);    // body contact (floating body)

  viennafvm::set_quantity_value(segmentation(5), storage, donator_doping_key(), n_plus);      // source
  viennafvm::set_quantity_value(segmentation(6), storage, donator_doping_key(), n_plus);      // drain
  viennafvm::set_quantity_value(segmentation(7), storage, donator_doping_key(), 1e32/p_plus); // body
  viennafvm::set_quantity_value(segmentation(8), storage, donator_doping_key(), 1e32/p_plus); // body contact (floating body)

  // acceptor doping
  viennafvm::set_quantity_region(segmentation(5), storage, acceptor_doping_key(), true);   // source
  viennafvm::set_quantity_region(segmentation(6), storage, acceptor_doping_key(), true);   // drain
  viennafvm::set_quantity_region(segmentation(7), storage, acceptor_doping_key(), true);   // body
  viennafvm::set_quantity_region(segmentation(8), storage, acceptor_doping_key(), true);   // body contact (floating body)

  viennafvm::set_quantity_value(segmentation(5), storage, acceptor_doping_key(), 1e32/n_plus); // source
  viennafvm::set_quantity_value(segmentation(6), storage, acceptor_doping_key(), 1e32/n_plus); // drain
  viennafvm::set_quantity_value(segmentation(7), storage, acceptor_doping_key(), p_plus);      // body
  viennafvm::set_quantity_value(segmentation(8), storage, acceptor_doping_key(), p_plus);      // body contact (floating body)

  // built-in potential:
  viennafvm::set_quantity_region(segmentation.domain(), storage, builtin_potential_key(), true);   // defined everywhere

  viennafvm::set_quantity_value(segmentation(1), storage, builtin_potential_key(), built_in_potential(300, n_plus, 1e32/n_plus)); // gate
  viennafvm::set_quantity_value(segmentation(2), storage, builtin_potential_key(), built_in_potential(300, n_plus, 1e32/n_plus)); // source contact
  viennafvm::set_quantity_value(segmentation(3), storage, builtin_potential_key(), built_in_potential(300, n_plus, 1e32/n_plus)); // oxide (for simplicity set to same as gate)
  viennafvm::set_quantity_value(segmentation(4), storage, builtin_potential_key(), built_in_potential(300, n_plus, 1e32/n_plus)); // drain contact
  viennafvm::set_quantity_value(segmentation(5), storage, builtin_potential_key(), built_in_potential(300, n_plus, 1e32/n_plus)); // source
  viennafvm::set_quantity_value(segmentation(6), storage, builtin_potential_key(), built_in_potential(300, n_plus, 1e32/n_plus)); // drain
  viennafvm::set_quantity_value(segmentation(7), storage, builtin_potential_key(), built_in_potential(300, 1e32/p_plus, p_plus)); // body
  viennafvm::set_quantity_value(segmentation(8), storage, builtin_potential_key(), built_in_potential(300, 1e32/p_plus, p_plus)); // body contact (floating body)
}

/** @brief Scales the entire simulation domain (device) by the provided factor. This is accomplished by multiplying all point coordinates with this factor. */
template <typename DomainType>
void scale_domain(DomainType & domain, double factor)
{
  typedef typename viennagrid::result_of::vertex_range<DomainType> ::type VertexContainer;
  typedef typename viennagrid::result_of::iterator<VertexContainer>::type VertexIterator;

  typename viennagrid::result_of::default_point_accessor<DomainType>::type point_accessor = viennagrid::default_point_accessor(domain);

  VertexContainer vertices = viennagrid::elements(domain);
  for ( VertexIterator vit = vertices.begin();
        vit != vertices.end();
        ++vit )
  {
    point_accessor(*vit) *= factor; // scale
  }
}

int main()
{
  typedef double   numeric_type;

  typedef viennagrid::triangular_2d_domain                         DomainType;
  typedef viennagrid::result_of::segmentation<DomainType>::type    SegmentationType;

  typedef viennagrid::result_of::cell_tag<DomainType>::type            CellTag;
  typedef viennagrid::result_of::element<DomainType, CellTag>::type    CellType;

  typedef viennamath::function_symbol   FunctionSymbol;
  typedef viennamath::equation          Equation;

  typedef viennadata::storage<>         StorageType;

  //
  // Create a domain from file
  //
  DomainType domain;
  SegmentationType segmentation(domain);
  StorageType storage;

  try
  {
    viennagrid::io::netgen_reader my_reader;
    my_reader(domain, segmentation, "../examples/data/mosfet.mesh");
  }
  catch (...)
  {
    std::cerr << "File-Reader failed. Aborting program..." << std::endl;
    return EXIT_FAILURE;
  }

  scale_domain(domain, 1e-9); // scale to nanometer

  //
  // Set initial values
  //
  double n_plus = 1e24;
  double p_plus = 1e20;

  init_quantities(segmentation, storage, n_plus, p_plus);

  //
  // Setting boundary information on domain (see mosfet.in2d for segment indices)
  //
  FunctionSymbol psi(0);   // potential, using id=0
  FunctionSymbol n(1);     // electron concentration, using id=1
  FunctionSymbol p(2);     // hole concentration, using id=2

  // potential:
  double built_in_pot = built_in_potential(300, n_plus, 1e32/n_plus); // should match specification in init_quantities()!
  viennafvm::set_dirichlet_boundary(segmentation(1), storage, psi, 0.2 + built_in_pot ); // Gate contact
  viennafvm::set_dirichlet_boundary(segmentation(2), storage, psi, 0.0 + built_in_pot ); // Source contact
  viennafvm::set_dirichlet_boundary(segmentation(4), storage, psi, 0.2 + built_in_pot ); // Drain contact
  // using floating body, hence commented:
  viennafvm::set_dirichlet_boundary(segmentation(8), storage, psi, 0.0 + built_in_potential(300, 1e32/p_plus, p_plus)); // Body contact

  // electron density
  viennafvm::set_dirichlet_boundary(segmentation(2), storage, n, n_plus); // Source contact
  viennafvm::set_dirichlet_boundary(segmentation(4), storage, n, n_plus); // Drain contact
  viennafvm::set_dirichlet_boundary(segmentation(8), storage, n, 1e32/p_plus); // Body contact

  // hole density
  viennafvm::set_dirichlet_boundary(segmentation(2), storage, p, 1e32/n_plus); // Source contact
  viennafvm::set_dirichlet_boundary(segmentation(4), storage, p, 1e32/n_plus); // Drain contact
  viennafvm::set_dirichlet_boundary(segmentation(8), storage, p, p_plus); // Body contact


  //
  // Set quantity mask: By default, a quantity is defined on the entire domain.
  //                    Boundary equations are already specified above.
  //                    All that is left is to specify regions where a quantity 'does not make sense'
  //                    Here, we need to disable {n,p} in the gate oxide and the gate
  //
  viennafvm::disable_quantity(segmentation(1), storage, n); // Gate contact
  viennafvm::disable_quantity(segmentation(3), storage, n); // Gate oxide

  viennafvm::disable_quantity(segmentation(1), storage, p); // Gate contact
  viennafvm::disable_quantity(segmentation(3), storage, p); // Gate oxide

  //
  // Initial conditions (required for nonlinear problems)
  //
  viennafvm::set_initial_guess(domain, storage, psi, builtin_potential_key());
  //viennafvm::smooth_initial_guess(domain, psi, viennafvm::arithmetic_mean_smoother());
  //viennafvm::smooth_initial_guess(domain, psi, viennafvm::arithmetic_mean_smoother());

  viennafvm::set_initial_guess(domain, storage, n, donator_doping_key());
  //viennafvm::smooth_initial_guess(domain, n, viennafvm::geometric_mean_smoother());
  //viennafvm::smooth_initial_guess(domain, n, viennafvm::geometric_mean_smoother());

  viennafvm::set_initial_guess(domain, storage, p, acceptor_doping_key());
  //viennafvm::smooth_initial_guess(domain, p, viennafvm::geometric_mean_smoother());
  //viennafvm::smooth_initial_guess(domain, p, viennafvm::geometric_mean_smoother());



  //
  // Specify PDEs:
  //

  viennafvm::ncell_quantity<CellType, viennamath::expr::interface_type>  permittivity;       permittivity.wrap_constant( storage, permittivity_key() );
  viennafvm::ncell_quantity<CellType, viennamath::expr::interface_type>  donator_doping;   donator_doping.wrap_constant( storage, donator_doping_key() );
  viennafvm::ncell_quantity<CellType, viennamath::expr::interface_type>  acceptor_doping; acceptor_doping.wrap_constant( storage, acceptor_doping_key() );

  double q  = 1.6e-19;
  double kB = 1.38e-23; // Boltzmann constant
  double mu = 1;        // mobility (constant is fine for the moment)
  double T  = 300;
  double VT = kB * T / q;
  double D  = mu * VT;  //diffusion constant

  // here is all the fun: specify DD system
  Equation poisson_eq = viennamath::make_equation( viennamath::div(permittivity * viennamath::grad(psi)),                     /* = */ q * ((n - donator_doping) - (p - acceptor_doping)));
  Equation cont_eq_n  = viennamath::make_equation( viennamath::div(D * viennamath::grad(n) - mu * viennamath::grad(psi) * n), /* = */ 0);
  Equation cont_eq_p  = viennamath::make_equation( viennamath::div(D * viennamath::grad(p) + mu * viennamath::grad(psi) * p), /* = */ 0);

  // Specify the PDE system:
  viennafvm::linear_pde_system<> pde_system;
  pde_system.add_pde(poisson_eq, psi); // equation and associated quantity
  pde_system.add_pde(cont_eq_n, n);    // equation and associated quantity
  pde_system.add_pde(cont_eq_p, p);    // equation and associated quantity

  pde_system.option(0).damping_term( (n + p) * (-q / VT) );
  pde_system.option(1).geometric_update(true);
  pde_system.option(2).geometric_update(true);

  pde_system.is_linear(false); // temporary solution up until automatic nonlinearity detection is running

  //
  // Setup Linear Solver
  //
  viennafvm::linsolv::viennacl  linear_solver;

  //
  // Create PDE solver instance and run the solver:
  //
  viennafvm::pde_solver<> pde_solver;

  pde_solver(pde_system, domain, storage, linear_solver);   // weird math happening in here ;-)


  //
  // Writing all solution variables back to domain
  //
  std::vector<long> result_ids(pde_system.size());
  for (std::size_t i=0; i<pde_system.size(); ++i)
    result_ids[i] = pde_system.unknown(i)[0].id();

  viennafvm::io::write_solution_to_VTK_file(pde_solver.result(), "mosfet", domain, segmentation, storage, result_ids);

  std::cout << "********************************************" << std::endl;
  std::cout << "* MOSFET simulation finished successfully! *" << std::endl;
  std::cout << "********************************************" << std::endl;
  return EXIT_SUCCESS;
}

