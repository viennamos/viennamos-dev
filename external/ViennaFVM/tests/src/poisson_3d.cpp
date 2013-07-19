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

#define VIENNAFVM_DEBUG

// ViennaFVM includes:
#include "viennafvm/forwards.h"
//#include "viennafvm/poisson_assembler.hpp"
#include "viennafvm/linear_assembler.hpp"
#include "viennafvm/io/vtk_writer.hpp"
#include "viennafvm/ncell_quantity.hpp"

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
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/operation.hpp>
#include <boost/numeric/ublas/operation_sparse.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/lu.hpp>


//ViennaCL includes:
#ifndef VIENNACL_HAVE_UBLAS
 #define VIENNACL_HAVE_UBLAS
#endif

#ifdef USE_OPENCL
  #include "viennacl/matrix.hpp"
  #include "viennacl/vector.hpp"
#endif
#include "viennacl/linalg/cg.hpp"
#include "viennacl/linalg/norm_2.hpp"
#include "viennacl/linalg/prod.hpp"


//
// Solve system of linear equations:
//
template <typename MatrixType, typename VectorType>
VectorType solve(MatrixType const & system_matrix,
                 VectorType const & load_vector)
{
  typedef typename VectorType::value_type        numeric_type;
  VectorType result(load_vector.size());

  std::cout << "* solve(): Solving linear system" << std::endl;

#ifdef USE_OPENCL
  viennacl::matrix<viennafem::numeric_type> vcl_matrix(load_vector.size(), load_vector.size());
  viennacl::vector<viennafem::numeric_type> vcl_rhs(load_vector.size());
  viennacl::vector<viennafem::numeric_type> vcl_result(load_vector.size());

  viennacl::copy(system_matrix, vcl_matrix);
  viennacl::copy(load_vector, vcl_rhs);

  vcl_result = viennacl::linalg::solve(vcl_matrix, vcl_rhs, viennacl::linalg::cg_tag());

  viennacl::copy(vcl_result, result);
#else
  result = viennacl::linalg::solve(system_matrix, load_vector, viennacl::linalg::cg_tag());
  std::cout << "* solve(): Residual: " << norm_2(prod(system_matrix, result) - load_vector) << std::endl;
#endif

  //std::cout << load_vector << std::endl;

  //print solution:
  //std::cout << "Solution: ";
  //for (size_t i=0; i<ublas_result.size(); ++i)
  //  std::cout << ublas_result(i) << " ";
  //std::cout << std::endl;
  //std::cout << std::endl;

  return result;
}



int main()
{
  typedef double   numeric_type;

  typedef viennagrid::tetrahedral_3d_domain   DomainType;
  typedef viennagrid::result_of::segmentation<DomainType>::type SegmentationType;

  typedef viennagrid::result_of::cell_tag<DomainType>::type CellTag;

  typedef viennagrid::result_of::element<DomainType, CellTag>::type        CellType;

  typedef viennadata::storage<> StorageType;


  typedef viennagrid::result_of::element_range<DomainType, CellTag>::type  CellContainer;
  typedef viennagrid::result_of::iterator<CellContainer>::type                CellIterator;
  typedef viennagrid::result_of::vertex_range<CellType>::type               VertexOnCellContainer;
  typedef viennagrid::result_of::iterator<VertexOnCellContainer>::type        VertexOnCellIterator;

  typedef viennamath::function_symbol   FunctionSymbol;
  typedef viennamath::equation          Equation;


  typedef boost::numeric::ublas::compressed_matrix<numeric_type>  MatrixType;
  typedef boost::numeric::ublas::vector<numeric_type>             VectorType;

  typedef viennafvm::boundary_key      BoundaryKey;

  //
  // Create a domain from file
  //
  DomainType domain;
  SegmentationType segmentation(domain);
  StorageType storage;

  try
  {
    viennagrid::io::netgen_reader my_netgen_reader;
    my_netgen_reader(domain, segmentation, "../data/cube3072.mesh");
  }
  catch (...)
  {
    std::cerr << "File-Reader failed. Aborting program..." << std::endl;
    return EXIT_FAILURE;
  }

  // Specify Poisson equation:
  FunctionSymbol u(0, viennamath::unknown_tag<>());   //an unknown function used for PDE specification
  Equation poisson_eq = viennamath::make_equation( viennamath::laplace(u), -1);  // \Delta u = -1

  MatrixType system_matrix;
  VectorType load_vector;

  //
  // Setting boundary information on domain (this should come from device specification)
  //
  //setting some boundary flags:

  viennagrid::result_of::default_point_accessor<DomainType>::type point_accessor = viennagrid::default_point_accessor(domain);

  viennadata::result_of::accessor<StorageType, BoundaryKey, bool, CellType>::type boundary_accessor =
      viennadata::accessor<BoundaryKey, bool, CellType>(storage, BoundaryKey( u.id() ));

  CellContainer cells = viennagrid::elements(domain);
  for (CellIterator cit  = cells.begin();
                    cit != cells.end();
                  ++cit)
  {
    bool cell_on_boundary = false;

    VertexOnCellContainer vertices = viennagrid::elements(*cit);
    for (VertexOnCellIterator vit  = vertices.begin();
                              vit != vertices.end();
                            ++vit)
    {
      //boundary for first equation: Homogeneous Dirichlet everywhere
      if (point_accessor(*vit)[0] == 0.0 || point_accessor(*vit)[0] == 1.0
        || point_accessor(*vit)[2] == 0.0 || point_accessor(*vit)[2] == 1.0 )
      {
        cell_on_boundary = true;
        break;
      }
    }
    boundary_accessor(*cit) = cell_on_boundary;
  }



  //
  // Create PDE assembler functor
  //
  viennafvm::linear_assembler fvm_assembler;


  //
  // Assemble system
  //
  fvm_assembler(viennafvm::make_linear_pde_system(poisson_eq, u),  // PDE with associated unknown
                domain,
                storage,
                system_matrix,
                load_vector
               );


  //std::cout << system_matrix << std::endl;
  //std::cout << load_vector << std::endl;

  //std::cout << poisson_config_1.load_vector() << std::endl;

  VectorType pde_result = solve(system_matrix, load_vector);

  //
  // Writing solution back to domain (discussion about proper way of returning a solution required...)
  //
  viennafvm::io::write_solution_to_VTK_file(pde_result, "poisson_3d", domain, segmentation, storage, 0);

  std::cout << "*****************************************" << std::endl;
  std::cout << "* Poisson solver finished successfully! *" << std::endl;
  std::cout << "*****************************************" << std::endl;
  return EXIT_SUCCESS;
}
