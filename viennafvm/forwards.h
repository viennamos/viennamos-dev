#ifndef VIENNAFVM_FORWARDS_H
#define VIENNAFVM_FORWARDS_H

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

#include <ostream>
#include "viennadata/api.hpp"
#include "viennamath/forwards.h"

#define NUM_PI 3.1415926535897932384626433832795

namespace viennafvm
{

  /** @brief The default floating point type to be used in ViennaFVM.
   *
   *  Feel free to change this typedef to a high-precision type if required.
   *  Keep in mind that only float and double types can be used for GPU acceleration.
   */
  typedef double             numeric_type;

  enum
  {
    cell_volume = 0,
    cell_boundary = 1
  };


  // define keys and configure ViennaData to use a type-based dispatch:
  class boundary_key
  {
    public:
      boundary_key(long id) : id_(id) {}

      bool operator<(boundary_key const & other) const { return id_ < other.id_; }

    private:
      long id_;
  };

  class mapping_key
  {
    public:
      mapping_key(long id) : id_(id) {}

      bool operator<(mapping_key const & other) const { return id_ < other.id_; }

    private:
      long id_;
  };

  class current_iterate_key
  {
    public:
      current_iterate_key(long id) : id_(id) {}

      bool operator<(current_iterate_key const & other) const { return id_ < other.id_; }

    private:
      long id_;
  };

  class disable_quantity_key
  {
    public:
      disable_quantity_key(long id) : id_(id) {}

      bool operator<(disable_quantity_key const & other) const { return id_ < other.id_; }

    private:
      long id_;
  };

  //box-integration related:
  struct facet_distance_key
  {
      bool operator<(facet_distance_key const &) const { return false; }
  };

  struct facet_area_key
  {
      bool operator<(facet_area_key const &) const { return false; }
  };

  template <typename CellType, typename InterfaceType = viennamath::default_interface_type>
  class ncell_quantity;


  // some constants:
  enum
  {
    QUANTITY_DISABLED  = -2,
    DIRICHLET_BOUNDARY = -1
  };
}


namespace viennamath
{
  namespace traits
  {
    template <typename InterfaceType, typename CellType>
    InterfaceType * clone(viennafvm::ncell_quantity<CellType, InterfaceType> const & c);
  }
}


#endif
