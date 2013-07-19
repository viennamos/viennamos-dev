#ifndef VIENNAFVM_PDE_ASSEMBLER_HPP
#define VIENNAFVM_PDE_ASSEMBLER_HPP

/* =======================================================================
   Copyright (c) 2011, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
           ViennaFVM - The Vienna Finite Volume Method Library
                             -----------------

   authors:    Karl Rupp                             rupp@iue.tuwien.ac.at
               Josef Weinbub                      weinbub@iue.tuwien.ac.at

               (add your name here)

   license:    To be discussed, see file LICENSE in the ViennaFVM base directory
======================================================================= */


// *** local includes
//
#include "viennafvm/integral_form.hpp"
#include "viennafvm/extract_integrals.hpp"
#include "viennafvm/linear_pde_system.hpp"
#include "viennafvm/mapping.hpp"
#include "viennafvm/util.hpp"
#include "viennafvm/flux.hpp"
#include "viennafvm/ncell_quantity.hpp"

#include "viennagrid/forwards.hpp"
#include "viennagrid/algorithm/voronoi.hpp"
#include "viennagrid/algorithm/volume.hpp"
#include "viennagrid/algorithm/centroid.hpp"
#include "viennagrid/domain/coboundary_iteration.hpp"

#include "viennamath/manipulation/eval.hpp"
#include "viennamath/manipulation/diff.hpp"

#include "viennadata/api.hpp"

//#define VIENNAFVMDEBUG

namespace viennafvm
{
  template <typename CellType, typename FacetType, typename CellValueAccessorType, typename FacetValueAccessorType, typename FacetDistanceAccessorType>
  void compute_gradients_for_cell(CellType const & inner_cell, FacetType const & facet, CellType const & outer_cell,
                                  CellValueAccessorType const cell_value_accessor, FacetValueAccessorType facet_value_accessor, FacetDistanceAccessorType const facet_distance_accessor)
  {
    double value_inner = cell_value_accessor(inner_cell);
    double value_outer = cell_value_accessor(outer_cell);
    double distance    = facet_distance_accessor(facet);

    facet_value_accessor(facet) = (value_outer - value_inner) / distance;
  }




  class linear_assembler
  {
    public:

      /** @brief  Assembles the full PDE system into the same matrix */
      template <typename LinPdeSysT,
                typename SegmentT,
                typename StorageType,
                typename MatrixT,
                typename VectorT>
      void operator()(LinPdeSysT const & pde_system,
                      SegmentT   const & segment,
                      StorageType      & storage,
                      MatrixT          & system_matrix,
                      VectorT          & load_vector)
      {
        typedef viennamath::equation                          equ_type;
        typedef viennamath::expr                              expr_type;
        typedef typename expr_type::interface_type            interface_type;
        typedef typename expr_type::numeric_type              numeric_type;

        typedef typename viennagrid::result_of::cell_tag<SegmentT>::type CellTag;

        std::size_t map_index = viennafvm::create_mapping(pde_system, segment, storage);

        system_matrix.clear();
        system_matrix.resize(map_index, map_index, false);
        load_vector.clear();
        load_vector.resize(map_index);


        for (std::size_t pde_index = 0; pde_index < pde_system.size(); ++pde_index)
        {
#ifdef VIENNAFVM_DEBUG
          std::cout << std::endl;
          std::cout << "//" << std::endl;
          std::cout << "//   Equation " << pde_index << std::endl;
          std::cout << "//" << std::endl;
#endif
          assemble(pde_system, pde_index,
                   segment, storage,
                   system_matrix, load_vector);

        } // for pde_index
      } // functor


      /** @brief  Assembles one PDE out of the PDE system into the matrix */
      template <typename LinPdeSysT,
                typename SegmentT,
                typename StorageType,
                typename MatrixT,
                typename VectorT>
      void operator()(LinPdeSysT const & pde_system,
                      std::size_t        pde_index,
                      SegmentT   const & segment,
                      StorageType      & storage,
                      MatrixT          & system_matrix,
                      VectorT          & load_vector)
      {
        typedef typename SegmentT::config_type                config_type;
        typedef viennamath::equation                          equ_type;
        typedef viennamath::expr                              expr_type;
        typedef typename expr_type::interface_type            interface_type;
        typedef typename expr_type::numeric_type              numeric_type;

        typedef typename viennagrid::result_of::cell_tag<SegmentT>::type CellTag;

        std::size_t map_index = viennafvm::create_mapping(pde_system, pde_index, segment, storage);

        system_matrix.clear();
        system_matrix.resize(map_index, map_index, false);
        load_vector.clear();
        load_vector.resize(map_index);


#ifdef VIENNAFVM_DEBUG
        std::cout << std::endl;
        std::cout << "//" << std::endl;
        std::cout << "//   Equation " << pde_index << std::endl;
        std::cout << "//" << std::endl;
#endif
        assemble(pde_system, pde_index,
                 segment, storage,
                 system_matrix, load_vector);

      } // functor


    private:

      template <typename PDESystemType,
                typename SegmentT,
                typename StorageType,
                typename MatrixT,
                typename VectorT>
      void assemble(PDESystemType const & pde_system,
                    std::size_t           pde_index,
                    SegmentT      const & segment,
                    StorageType & storage,
                    MatrixT             & system_matrix,
                    VectorT             & load_vector)
      {
        typedef typename SegmentT::config_type                config_type;
        typedef viennamath::equation                          equ_type;
        typedef viennamath::expr                              expr_type;
        typedef typename expr_type::interface_type            interface_type;
        typedef typename expr_type::numeric_type              numeric_type;

        typedef typename viennagrid::result_of::cell_tag<SegmentT>::type CellTag;
        typedef typename viennagrid::result_of::facet_tag<CellTag>::type FacetTag;

        typedef typename viennagrid::result_of::element<SegmentT, FacetTag>::type                FacetType;
        typedef typename viennagrid::result_of::element<SegmentT, CellTag  >::type                CellType;

        typedef typename viennagrid::result_of::const_element_range<SegmentT, CellTag>::type    CellContainer;
        typedef typename viennagrid::result_of::iterator<CellContainer>::type                      CellIterator;

        typedef typename viennagrid::result_of::const_element_range<CellType, FacetTag>::type  FacetOnCellContainer;
        typedef typename viennagrid::result_of::iterator<FacetOnCellContainer>::type               FacetOnCellIterator;

        viennamath::equation          const & pde         = pde_system.pde(pde_index);
        viennamath::function_symbol   const & u           = pde_system.unknown(pde_index)[0];
        viennafvm::linear_pde_options const & pde_options = pde_system.option(pde_index);

        viennafvm::mapping_key   map_key(u.id());
        viennafvm::boundary_key  bnd_key(u.id());


#ifdef VIENNAFVM_DEBUG
        std::cout << " - Strong form: " << pde << std::endl;
#endif

        equ_type integral_form = viennafvm::make_integral_form( pde );

#ifdef VIENNAFVM_DEBUG
        std::cout << " - Integral form: " << integral_form << std::endl;
#endif

        //
        // Preprocess symbolic representation:
        //

        //Note: Assuming that LHS holds all matrix terms, while RHS holds all load vector terms
        expr_type  partial_omega_integrand = extract_surface_integrand<FacetType>(storage, integral_form.lhs(), u);
        expr_type   matrix_omega_integrand = extract_volume_integrand<CellType>(storage, integral_form.lhs(), u);
        expr_type      rhs_omega_integrand = extract_volume_integrand<CellType>(storage, integral_form.rhs(), u);
        expr_type  stabilization_integrand = prepare_for_evaluation<CellType>(storage, pde_options.damping_term(), u);

#ifdef VIENNAFVM_DEBUG
        std::cout << " - Surface integrand for matrix: " << partial_omega_integrand << std::endl;
        std::cout << " - Volume integrand for matrix:  " <<  matrix_omega_integrand << std::endl;
        std::cout << " - Stabilization for matrix:     " << stabilization_integrand << std::endl;
        std::cout << " - Volume integrand for rhs:     " <<     rhs_omega_integrand << std::endl;
#endif

        viennafvm::flux_handler<StorageType, CellType, FacetType, interface_type>  flux(storage, partial_omega_integrand, u);

        expr_type substituted_matrix_omega_integrand  = viennamath::diff(matrix_omega_integrand, u);

        std::vector<double> p(3); //dummy vector for evaluation

        //
        // Preprocess domain
        //
        setup(segment, storage);


        typename viennadata::result_of::accessor<StorageType, viennafvm::mapping_key, long, CellType>::type cell_mapping_accessor =
            viennadata::make_accessor(storage, map_key);

        typename viennadata::result_of::accessor<StorageType, viennafvm::facet_area_key, double, FacetType>::type facet_area_accessor =
            viennadata::make_accessor(storage, viennafvm::facet_area_key());

        typename viennadata::result_of::accessor<StorageType, viennafvm::boundary_key, double, CellType>::type boundary_accessor =
            viennadata::make_accessor(storage, bnd_key);

        typename viennadata::result_of::accessor<StorageType, viennafvm::current_iterate_key, double, CellType>::type current_iterate_accessor =
            viennadata::make_accessor(storage, viennafvm::current_iterate_key(u.id()));

        //
        // Actual assembly:
        //
        CellContainer cells = viennagrid::elements(segment);
        for (CellIterator cit = cells.begin(); cit != cells.end(); ++cit)
        {
          long row_index = cell_mapping_accessor(*cit);

          if (row_index < 0)
            continue;

          // update ncell quantities in expressions for current cell:
          viennamath::rt_traversal_wrapper<interface_type> cell_updater( new detail::ncell_updater<CellType, interface_type>(*cit) );

          substituted_matrix_omega_integrand.get()->recursive_traversal(cell_updater);
          stabilization_integrand.get()->recursive_traversal(cell_updater);
          rhs_omega_integrand.get()->recursive_traversal(cell_updater);



          typename viennadata::result_of::accessor<StorageType, facet_distance_key, double, FacetType>::type facet_distance_accessor =
              viennadata::make_accessor(storage, facet_distance_key());

          //
          // Boundary integral terms:
          //
          FacetOnCellContainer facets_on_cell = viennagrid::elements(*cit);
          for (FacetOnCellIterator focit  = facets_on_cell.begin();
                                   focit != facets_on_cell.end();
                                 ++focit)
          {
            CellType const * other_cell = util::other_cell_of_facet(*focit, *cit, segment);

            if (other_cell)
            {
              long col_index = cell_mapping_accessor(*other_cell);
              double effective_facet_area = facet_area_accessor(*focit);

              for (std::size_t i=0; i<pde_system.size(); ++i)
                compute_gradients_for_cell(*cit, *focit, *other_cell,
                                           viennadata::make_accessor<current_iterate_key, double, CellType>(storage, current_iterate_key(pde_system.unknown(i)[0].id())),
                                           viennadata::make_accessor<current_iterate_key, double, FacetType>(storage, current_iterate_key(pde_system.unknown(i)[0].id())),
                                           facet_distance_accessor);

              if (col_index == viennafvm::DIRICHLET_BOUNDARY)
              {
                double boundary_value = boundary_accessor(*other_cell);
                double current_value  = current_iterate_accessor(*cit);

                // updates are homogeneous, hence no direct contribution to RHS here. Might change later when boundary values are slowly increased.
                load_vector(row_index)              -= flux.out(*cit, *focit, *other_cell, facet_distance_accessor) * effective_facet_area * (boundary_value - current_value);
                system_matrix(row_index, row_index) -= flux.in(*cit, *focit, *other_cell, facet_distance_accessor) * effective_facet_area;

                load_vector(row_index) -= flux.out(*cit, *focit, *other_cell, facet_distance_accessor) * effective_facet_area * current_value;
                load_vector(row_index) += flux.in(*cit, *focit, *other_cell, facet_distance_accessor) * effective_facet_area * current_iterate_accessor(*cit);
              }
              else if (col_index >= 0)
              {
                system_matrix(row_index, col_index) += flux.out(*cit, *focit, *other_cell, facet_distance_accessor) * effective_facet_area;
                system_matrix(row_index, row_index) -= flux.in(*cit, *focit, *other_cell, facet_distance_accessor) * effective_facet_area;

                load_vector(row_index) -= flux.out(*cit, *focit, *other_cell, facet_distance_accessor) * effective_facet_area * current_iterate_accessor(*other_cell);
                load_vector(row_index) += flux.in(*cit, *focit, *other_cell, facet_distance_accessor) * effective_facet_area * current_iterate_accessor(*cit);
              }
              // else: nothing to do because other cell is not considered for this quantity

            }
          }

          //
          // Volume terms
          //
          double cell_volume      = viennagrid::volume(*cit);

          // Matrix (including residual contributions)
          system_matrix(row_index, row_index) += viennamath::eval(substituted_matrix_omega_integrand, p) * cell_volume;
          load_vector(row_index) -= viennamath::eval(substituted_matrix_omega_integrand, p) * cell_volume * current_iterate_accessor(*cit);

          system_matrix(row_index, row_index) += viennamath::eval(stabilization_integrand, p) * cell_volume;

          // RHS
          load_vector(row_index) += viennamath::eval(rhs_omega_integrand, p) * cell_volume;
          //std::cout << "Writing " << viennamath::eval(omega_integrand, p) << " * " << cell_volume << " to rhs at " << row_index << std::endl;

        } // for cells

      } // assemble

      template <typename SegmentT, typename StorageType>
      void setup(SegmentT const & segment, StorageType & storage)
      {
        typedef typename SegmentT::config_type                config_type;
        typedef viennamath::equation                          equ_type;
        typedef viennamath::expr                              expr_type;
        typedef typename expr_type::numeric_type              numeric_type;

        typedef typename viennagrid::result_of::cell_tag<SegmentT>::type CellTag;
        typedef typename viennagrid::result_of::facet_tag<CellTag>::type FacetTag;

        typedef typename viennagrid::result_of::point<SegmentT>::type                  PointType;
        typedef typename viennagrid::result_of::element<SegmentT, FacetTag>::type  FacetType;
        typedef typename viennagrid::result_of::element<SegmentT, CellTag  >::type  CellType;

        typedef typename viennagrid::result_of::const_element_range<SegmentT, FacetTag>::type  FacetContainer;
        typedef typename viennagrid::result_of::iterator<FacetContainer>::type                     FacetIterator;

        typedef typename viennagrid::result_of::const_coboundary_range<SegmentT, FacetType, CellTag>::type CellOnFacetRange;
//         typedef typename viennagrid::result_of::const_element_range<FacetType, CellTag>::type   CellOnFacetRange;
        typedef typename viennagrid::result_of::iterator<CellOnFacetRange>::type                   CellOnFacetIterator;



        typename viennadata::result_of::accessor<StorageType, viennafvm::facet_area_key, double, FacetType>::type facet_area_accessor =
            viennadata::make_accessor(storage, viennafvm::facet_area_key());

        typename viennadata::result_of::accessor<StorageType, viennafvm::facet_distance_key, double, FacetType>::type facet_distance_accessor =
            viennadata::make_accessor(storage, viennafvm::facet_distance_key());


        FacetContainer facets = viennagrid::elements(segment);
        for (FacetIterator fit  = facets.begin();
                           fit != facets.end();
                         ++fit)
        {

          CellOnFacetRange    cells = viennagrid::coboundary_elements<FacetType, CellTag>(segment, fit.handle());

          if (cells.size() == 2)
          {
            CellOnFacetIterator cofit    = cells.begin();

            PointType centroid_1         = viennagrid::centroid(*cofit); ++cofit;
            PointType centroid_2         = viennagrid::centroid(*cofit);
            PointType center_connection  = centroid_1 - centroid_2;
            PointType outer_normal       = util::unit_outer_normal(*fit, *cofit, viennagrid::default_point_accessor(segment)); //note: consistent orientation of center_connection and outer_normal is important here!

            double center_connection_len = viennagrid::norm(center_connection);
            double effective_facet_ratio = viennagrid::inner_prod(center_connection, outer_normal) / center_connection_len;  // inner product of unit vectors
            double effective_facet_area  = viennagrid::volume(*fit) * effective_facet_ratio;

            facet_area_accessor(*fit) = effective_facet_area;
            facet_distance_accessor(*fit) = center_connection_len;
          }
        }
    }
  };

  template <typename InterfaceType, typename SegmentT, typename MatrixT, typename VectorT>
  void assemble_pde( viennafvm::linear_pde_system<InterfaceType>  & pde_system,
                     SegmentT & segment,
                     MatrixT  & matrix,
                     VectorT  & rhs
                    )
  {
     viennafvm::linear_assembler()(pde_system, segment, matrix, rhs);
  }
}

#endif







