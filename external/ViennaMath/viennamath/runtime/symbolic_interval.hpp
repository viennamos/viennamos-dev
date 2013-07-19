#ifndef VIENNAMATH_RUNTIME_SYMBOLIC_INTERVAL_HPP
#define VIENNAMATH_RUNTIME_SYMBOLIC_INTERVAL_HPP

/* =======================================================================
   Copyright (c) 2012, Institute for Microelectronics,
                       Institute for Analysis and Scientific Computing,
                       TU Wien.
                             -----------------
               ViennaMath - Symbolic and Numerical Math in C++
                             -----------------

   Author:     Karl Rupp                          rupp@iue.tuwien.ac.at

   License:    MIT (X11), see file LICENSE in the ViennaMath base directory
======================================================================= */





#include <ostream>
#include "viennamath/forwards.h"

/** @file  symbolic_interval.hpp
    @brief Defines a class representing an interval. This is only a placeholder class, the actual interval is substituted later.
*/

namespace viennamath
{

  /** @brief A symbolic runtime interval */
  template <typename InterfaceType>
  class rt_symbolic_interval
  {

    public:
      typedef rt_expr<InterfaceType>                  value_type;
      typedef InterfaceType                           interface_type;
      typedef typename InterfaceType::numeric_type    numeric_type;

      rt_symbolic_interval(id_type interval_id = 0) : id_(interval_id) {}

      /** @brief Returns the ID of the interval (used for dispatching purposes). */
      id_type id() const { return id_; }

    private:
      id_type id_;
  };

  /** @brief Convenience overload for obtaining a string representation of a symbolic interval */
  template <typename InterfaceType>
  std::ostream& operator<<(std::ostream & stream, rt_symbolic_interval<InterfaceType> const & e)
  {
    stream << "symbolic_interval[" << e.id() << "]";
    return stream;
  }




  //////// helper functions for creating equations (uniform interface for compile time and run time: ///////////




}

#endif
