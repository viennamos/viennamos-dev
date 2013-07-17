#ifndef VIENNAFVM_UTIL_HPP
#define VIENNAFVM_UTIL_HPP

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

#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cmath>

#include "viennadata/api.hpp"
#include "viennafvm/forwards.h"
#include "viennagrid/algorithm/voronoi.hpp"
#include "viennagrid/domain/coboundary_iteration.hpp"



/** @file viennafvm/util.hpp
    @brief Miscellaneous utilities
*/

namespace viennafvm
{

  namespace util
  {

    ///////////////////// outer unit vectors on facet ///////////////////
    template <typename FacetType, typename CellType>
    typename viennagrid::result_of::point<CellType>::type
    unit_outer_normal(FacetType const & facet, CellType const & cell);

    // 1d
    template <typename FacetType, typename WrappedConfigType, typename PointAccessorType>
    typename viennagrid::result_of::point<PointAccessorType>::type
    unit_outer_normal(FacetType const & facet, viennagrid::element_t<viennagrid::line_tag, WrappedConfigType> const & cell, PointAccessorType const point_accessor)
    {
      typedef typename viennagrid::result_of::point<PointAccessorType>::type      PointType;
      typedef typename viennagrid::line_tag CellTag;

      FacetType const * other_facet = &(viennagrid::elements<typename CellTag::facet_tag>(cell)[0]);
      if ( point_accessor(*other_facet)[0] == point_accessor(facet)[0])
        other_facet = &(viennagrid::elements<CellTag::facet_tag>(cell)[1]);

      PointType n;

      n[0] = (point_accessor(facet)[0] < point_accessor(*other_facet)[0]) ? -1: 1;  //left facet has normal -1, right facet has normal +1

      return n;
    }

    // 2d
    namespace detail
    {
      template <typename FacetType, typename CellType, typename PointAccessorType>
      typename viennagrid::result_of::point<PointAccessorType>::type
      unit_outer_normal_2d(FacetType const & facet, CellType const & cell, PointAccessorType const point_accessor)
      {
        typedef typename viennagrid::result_of::point<PointAccessorType>::type      PointType;

        typedef typename viennagrid::result_of::vertex<CellType>::type VertexType;

        //
        // Find a vertex which is not part of the face
        //
        VertexType const * non_facet_vertex = NULL;

        for (std::size_t i=0; i<viennagrid::vertices(cell).size(); ++i)
        {
          non_facet_vertex = &(viennagrid::vertices(cell)[i]);
          bool is_part_of_facet = false;

          for (std::size_t j=0; j<viennagrid::vertices(facet).size(); ++j)
          {
            if (non_facet_vertex == &(viennagrid::vertices(facet)[j]))
            {
              is_part_of_facet = true;
              break;
            }
          }
          if (!is_part_of_facet)
            break;
        }

        //
        // Now compute direction vector
        //
        PointType edge_vec  = point_accessor(viennagrid::vertices(facet)[1]) - point_accessor(viennagrid::vertices(facet)[0]);
        edge_vec /= viennagrid::norm(edge_vec);

        PointType other_vec = point_accessor(*non_facet_vertex) - point_accessor(viennagrid::vertices(facet)[0]);
        other_vec -= viennagrid::inner_prod(edge_vec, other_vec) * edge_vec; //orthogonalize (one step of Gram-Schmidt)
        other_vec /= -1.0 * viennagrid::norm(other_vec); //make it unit length and flip direction

        return other_vec;
      }
    }

    // interface for triangles
    template <typename FacetType, typename WrappedConfigType, typename PointAccessorType>
    typename viennagrid::result_of::point<PointAccessorType>::type
    unit_outer_normal(FacetType const & facet, viennagrid::element_t<viennagrid::triangle_tag, WrappedConfigType> const & cell, PointAccessorType point_accessor)
    {
      return detail::unit_outer_normal_2d(facet, cell, point_accessor);
    }

    // interface for quadrilaterals
    template <typename FacetType, typename WrappedConfigType, typename PointAccessorType>
    typename viennagrid::result_of::point<PointAccessorType>::type
    unit_outer_normal(FacetType const & facet, viennagrid::element_t<viennagrid::quadrilateral_tag, WrappedConfigType> const & cell, PointAccessorType point_accessor)
    {
      return detail::unit_outer_normal_2d(facet, cell, point_accessor);
    }


    // 3d
    namespace detail
    {
      template <typename FacetType, typename CellType, typename PointAccessorType>
      typename viennagrid::result_of::point<PointAccessorType>::type
      unit_outer_normal_3d(FacetType const & facet, CellType const & cell, PointAccessorType point_accessor)
      {
        typedef typename viennagrid::result_of::point<PointAccessorType>::type      PointType;

        typedef typename viennagrid::result_of::vertex<CellType>::type VertexType;

        //
        // Find a vertex which is not part of the face
        //
        VertexType const * non_facet_vertex = NULL;

        for (std::size_t i=0; i<viennagrid::vertices(cell).size(); ++i)
        {
          non_facet_vertex = &(viennagrid::vertices(cell)[i]);
          bool is_part_of_facet = false;

          for (std::size_t j=0; j<viennagrid::vertices(facet).size(); ++j)
          {
            if (non_facet_vertex == &(viennagrid::vertices(facet)[j]))
            {
              is_part_of_facet = true;
              break;
            }
          }
          if (!is_part_of_facet)
            break;
        }

        //
        // Now compute direction vector via cross-prod
        //
        PointType edge_vec1  = point_accessor(viennagrid::vertices(facet)[1]) - point_accessor(viennagrid::vertices(facet)[0]);
        PointType edge_vec2  = point_accessor(viennagrid::vertices(facet)[2]) - point_accessor(viennagrid::vertices(facet)[0]);
        PointType normal_vec = viennagrid::cross_prod(edge_vec1, edge_vec2);
        normal_vec /= viennagrid::norm(normal_vec);

        // ensure normal vector is pointing outwards:
        PointType other_vec = point_accessor(*non_facet_vertex) - point_accessor(viennagrid::vertices(facet)[0]);
        if (viennagrid::inner_prod(normal_vec, other_vec) > 0)
          normal_vec *= -1.0;

        return normal_vec;
      }
    }

    // interface for tetrahedra
    template <typename FacetType, typename WrappedConfigType, typename PointAccessorType>
    typename viennagrid::result_of::point<PointAccessorType>::type
    unit_outer_normal(FacetType const & facet, viennagrid::element_t<viennagrid::tetrahedron_tag, WrappedConfigType> const & cell, PointAccessorType point_accessor)
    {
      return detail::unit_outer_normal_3d(facet, cell, point_accessor);
    }

    // interface for hexahedra
    template <typename FacetType, typename WrappedConfigType, typename PointAccessorType>
    typename viennagrid::result_of::point<PointAccessorType>::type
    unit_outer_normal(FacetType const & facet, viennagrid::element_t<viennagrid::hexahedron_tag, WrappedConfigType> const & cell, PointAccessorType point_accessor)
    {
      return detail::unit_outer_normal_3d(facet, cell, point_accessor);
    }





    //
    // Other cell of facet
    //


    template <typename FacetType, typename CellType, typename DomainType>
    CellType const * other_cell_of_facet(FacetType const & facet, CellType const & cell, DomainType const & domain)
    {
      typedef typename CellType::tag      CellTag;

      typedef typename viennagrid::result_of::const_coboundary_range<DomainType, FacetType, CellTag>::type CellOnFacetRange;
      typedef typename viennagrid::result_of::iterator<CellOnFacetRange>::type                  CellOnFacetIterator;

      CellOnFacetRange    cells = viennagrid::coboundary_elements<FacetType, CellTag>(domain, viennagrid::handle(domain, facet) );
      CellOnFacetIterator cofit = cells.begin();

      if (&(*cofit) == &cell) // we know the first cell pointed to by the iterator already, so we pick the 'other'
        ++cofit;

      if (cofit != cells.end())
        return &(*cofit);

      return NULL;  // facet is part of one cell only, so there is no 'other' cell
    }

  } //namespace util

} //namespace viennafvm
#endif
