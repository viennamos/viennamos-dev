#ifndef VIENNAFVM_COMMON_HPP
#define VIENNAFVM_COMMON_HPP

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

#include <iostream>
#include <fstream>
#include <vector>

#include <math.h>

#include "viennagrid/domain/domain.hpp"
#include "viennagrid/point.hpp"

#include "viennamath/runtime/function_symbol.hpp"

#include "viennafvm/forwards.h"

namespace viennafvm
{

  template <typename DomainSegmentType, typename AccessorType>
  void disable_quantity(DomainSegmentType const & domseg,
                        AccessorType accessor)
  {
    typedef typename viennagrid::result_of::cell_tag<DomainSegmentType>::type CellTag;

    typedef typename viennagrid::result_of::element<DomainSegmentType, CellTag>::type               CellType;
    typedef typename viennagrid::result_of::const_element_range<DomainSegmentType, CellTag>::type  CellContainer;
    typedef typename viennagrid::result_of::iterator<CellContainer>::type                       CellIterator;

    CellContainer cells = viennagrid::elements(domseg);
    for (CellIterator cit  = cells.begin();
                      cit != cells.end();
                    ++cit)
    {
      //set flag:
      accessor(*cit) = true;
    }
  }


  template <typename DomainSegmentType, typename A, typename B, typename KeyType>
  void disable_quantity(DomainSegmentType const & domseg,
                        viennadata::storage<A,B> & storage,
                        KeyType const & key)
  {
    typedef typename viennagrid::result_of::cell<DomainSegmentType>::type CellType;
    disable_quantity(domseg, viennadata::make_accessor<KeyType, bool, CellType>(storage, key));
  }

  template <typename DomainSegmentType, typename A, typename B, typename InterfaceType>
  void disable_quantity(DomainSegmentType const & domseg,
                        viennadata::storage<A,B> & storage,
                        viennamath::rt_function_symbol<InterfaceType> const & func_symbol)
  {
    disable_quantity( domseg, storage, viennafvm::disable_quantity_key(func_symbol.id()) );
  }

} //namespace viennashe

#endif
