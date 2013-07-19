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

#define VIENNAFVM_DEBUG

// include necessary system headers
#include <iostream>

// ViennaFVM includes:
#include "viennafvm/forwards.h"
#include "viennafvm/linear_assembler.hpp"
#include "viennafvm/io/vtk_writer.hpp"
#include "viennafvm/boundary.hpp"
#include "viennafvm/linear_solve.hpp"
#include "viennafvm/pde_solver.hpp"


// ViennaGrid includes:
#include "viennagrid/domain/domain.hpp"
#include <viennagrid/config/default_configs.hpp>
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


int main()
{
  typedef double   numeric_type;

  typedef viennagrid::triangular_2d_domain   DomainType;
  typedef viennagrid::result_of::segmentation<DomainType>::type SegmentationType;

  typedef viennagrid::result_of::cell_tag<DomainType>::type CellTag;

  typedef viennagrid::result_of::element<DomainType, CellTag>::type        CellType;

  typedef viennamath::function_symbol   FunctionSymbol;
  typedef viennamath::equation          Equation;

  typedef viennadata::storage<> StorageType;


  

  typedef viennagrid::result_of::element_range<DomainType, CellTag>::type     CellContainer;
  typedef viennagrid::result_of::iterator<CellContainer>::type                CellIterator;
  typedef viennagrid::result_of::vertex_range<CellType>::type                 VertexOnCellContainer;
  typedef viennagrid::result_of::iterator<VertexOnCellContainer>::type        VertexOnCellIterator;

  typedef boost::numeric::ublas::compressed_matrix<numeric_type>  MatrixType;
  typedef boost::numeric::ublas::vector<numeric_type>             VectorType;

  typedef viennamath::function_symbol   FunctionSymbol;
  typedef viennamath::equation          Equation;

  //
  // Create a domain from file
  //
  DomainType domain;
  SegmentationType segmentation(domain);
  StorageType storage;

  try
  {
    viennagrid::io::netgen_reader my_reader;
    my_reader(domain, segmentation, "../examples/data/square128.mesh");
  }
  catch (...)
  {
    std::cerr << "File-Reader failed. Aborting program..." << std::endl;
    return EXIT_FAILURE;
  }

  //
  // Specify two PDEs:
  //
  FunctionSymbol u(0, viennamath::unknown_tag<>());   //an unknown function used for PDE specification
  FunctionSymbol v(1, viennamath::unknown_tag<>());   //an unknown function used for PDE specification
  Equation poisson_equ_1 = viennamath::make_equation( viennamath::laplace(u), -1);
  Equation poisson_equ_2 = viennamath::make_equation( viennamath::laplace(v), 0);

  MatrixType system_matrix_1, system_matrix_2;
  VectorType load_vector_1, load_vector_2;

  //
  // Setting boundary information on domain (this should come from device specification)
  //

  viennagrid::result_of::default_point_accessor<DomainType>::type point_accessor = viennagrid::default_point_accessor(domain);

  viennadata::result_of::accessor<StorageType, viennafvm::boundary_key, bool, CellType>::type boundary_u_accessor =
      viennadata::make_accessor(storage, viennafvm::boundary_key( u.id() ));
      
  viennadata::result_of::accessor<StorageType, viennafvm::boundary_key, double, CellType>::type boundary_u_value_accessor =
      viennadata::make_accessor(storage, viennafvm::boundary_key( u.id() ));

      
  viennadata::result_of::accessor<StorageType, viennafvm::boundary_key, bool, CellType>::type boundary_v_accessor =
      viennadata::make_accessor(storage, viennafvm::boundary_key( v.id() ));

  viennadata::result_of::accessor<StorageType, viennafvm::boundary_key, double, CellType>::type boundary_v_value_accessor =
      viennadata::make_accessor(storage, viennafvm::boundary_key( v.id() ));

      
  //setting some boundary flags:
  CellContainer cells = viennagrid::elements(domain);
  for (CellIterator cit  = cells.begin();
                    cit != cells.end();
                  ++cit)
  {
    VertexOnCellContainer vertices = viennagrid::elements(*cit);
    for (VertexOnCellIterator vit  = vertices.begin();
                              vit != vertices.end();
                            ++vit)
    {
      //boundary for first equation: Homogeneous Dirichlet everywhere
      if ( point_accessor(*vit)[0] == 0.0 || point_accessor(*vit)[0] == 1.0
           || point_accessor(*vit)[1] == 0.0 || point_accessor(*vit)[1] == 1.0 )
      {
        //simulation with ID 0 uses homogeneous boundary data
        boundary_u_accessor(*cit) = true;
        boundary_u_value_accessor(*cit) = 0.0;
      }

      //boundary for second equation (ID 1): 0 at left boundary, 1 at right boundary
      if ( point_accessor(*vit)[0] == 0.0)
      {
        boundary_v_accessor(*cit) = true;
        boundary_v_value_accessor(*cit) = 0.0;
      }

      else if ( point_accessor(*vit)[0] == 1.0)
      {
        boundary_v_accessor(*cit) = true;
        boundary_v_value_accessor(*cit) = 1.0;
      }
    }
  }

  //
  // Create PDE solver instance
  //
  viennafvm::pde_solver<> pde_solver;

  //
  // Pass system to solver:
  //
  pde_solver(viennafvm::make_linear_pde_system(poisson_equ_1, u),  // PDE with associated unknown
             domain,
             storage);

  viennafvm::io::write_solution_to_VTK_file(pde_solver.result(), "poisson_2d_1", domain, segmentation, storage, 0);

  pde_solver(viennafvm::make_linear_pde_system(poisson_equ_2, v, viennafvm::make_linear_pde_options(1, 1)),  // PDE with associated unknown
             domain,
             storage);

  //
  // Writing solution back to domain (discussion about proper way of returning a solution required...)
  //
  viennafvm::io::write_solution_to_VTK_file(pde_solver.result(), "poisson_2d_2", domain, segmentation, storage, 1);

  std::cout << "*****************************************" << std::endl;
  std::cout << "* Poisson solver finished successfully! *" << std::endl;
  std::cout << "*****************************************" << std::endl;
  return EXIT_SUCCESS;
}

