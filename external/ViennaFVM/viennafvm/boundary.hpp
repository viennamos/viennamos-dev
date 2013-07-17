#ifndef VIENNAFVM_BOUNDARY_HPP
#define VIENNAFVM_BOUNDARY_HPP

/* =========================================================================
   Copyright (c) 2012, Institute for Microelectronics,
                       Institute for Analysis and Scientific Computing,
                       TU Wien.
                             -----------------
               ViennaFVM - The Vienna Finite Volume Method Library
                             -----------------

   Author:     Karl Rupp                          rupp@iue.tuwien.ac.at
               (add your name here)

   License:    MIT (X11), see file LICENSE in the ViennaFEM base directory
============================================================================ */

#include "viennafvm/forwards.h"

#include "viennamath/forwards.h"
#include "viennamath/manipulation/substitute.hpp"
#include "viennamath/expression.hpp"
#include "viennadata/api.hpp"

#include "viennagrid/forwards.hpp"
#include "viennagrid/domain/segmentation.hpp"

/** @file  boundary.hpp
    @brief Provide convenience routines for setting boundary conditions
*/

namespace viennafvm
{

  template <typename DomainSegmentType, typename BoundaryAccessorType, typename BoundaryValueAccessorType>
  void set_dirichlet_boundary(DomainSegmentType const & seg,
                              BoundaryAccessorType boundary_accessor,
                              BoundaryValueAccessorType boundary_value_accessor,
                              numeric_type const & value)
  {
    typedef typename viennagrid::result_of::cell_tag<DomainSegmentType>::type CellTag;

    typedef typename viennagrid::result_of::element<DomainSegmentType, CellTag>::type               CellType;
    typedef typename viennagrid::result_of::const_element_range<DomainSegmentType, CellTag>::type  CellContainer;
    typedef typename viennagrid::result_of::iterator<CellContainer>::type                       CellIterator;

    CellContainer cells = viennagrid::elements(seg);
    for (CellIterator cit  = cells.begin();
                      cit != cells.end();
                    ++cit)
    {
      //set flag:
      boundary_accessor(*cit) = true;

      //set data:
      boundary_value_accessor(*cit) = value;
    }
  }


  template <typename DomainSegmentType, typename A, typename B, typename KeyType>
  void set_dirichlet_boundary(DomainSegmentType const & domseg,
                              viennadata::storage<A,B> & storage,
                              KeyType const & key,
                              numeric_type const & value)
  {
    typedef typename viennagrid::result_of::cell< DomainSegmentType >::type CellType;
    set_dirichlet_boundary(domseg,
                           viennadata::accessor<KeyType, bool, CellType>(storage, key),
                           viennadata::accessor<KeyType, numeric_type, CellType>(storage, key),
                           value);
  }


  template <typename DomainSegmentType, typename A, typename B, typename InterfaceType>
  void set_dirichlet_boundary(DomainSegmentType const & domseg,
                              viennadata::storage<A,B> & storage,
                              viennamath::rt_function_symbol<InterfaceType> const & func_symbol,
                              numeric_type const & value)
  {
    set_dirichlet_boundary( domseg, storage, viennafvm::boundary_key(func_symbol.id()), value );
  }


}
#endif
