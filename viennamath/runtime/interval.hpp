#ifndef VIENNAMATH_RUNTIME_INTERVAL_HPP
#define VIENNAMATH_RUNTIME_INTERVAL_HPP

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

/** @file   interval.hpp
    @brief  Defines a class for the representation of an integration interval. Also defines some convenience generator functions.
*/

namespace viennamath
{

  /** @brief Represents an interval [a,b] at runtime. Note that the interval boundaries do not need to be constants. */
  template <typename InterfaceType>
  class rt_interval
  {

    public:
      typedef rt_expr<InterfaceType>                  value_type;
      typedef InterfaceType                           interface_type;
      typedef typename InterfaceType::numeric_type    numeric_type;

      explicit rt_interval() : lower_(new rt_constant<numeric_type, InterfaceType>(0)),
                               upper_(new rt_constant<numeric_type, InterfaceType>(1)) {}

      explicit rt_interval(const rt_expr<InterfaceType> & a,
                           const rt_expr<InterfaceType> & b) : lower_(a.get()->clone()),
                                                               upper_(b.get()->clone()) {}

      rt_interval(rt_interval const & other) : lower_(other.lower_.get()->clone()),
                                               upper_(other.upper_.get()->clone()) {}

      /** @brief Returns the lower bound 'a' of the interval. */
      rt_expr<InterfaceType> const & lower() const { return lower_; }
      /** @brief Returns the upper bound 'b' of the interval. */
      rt_expr<InterfaceType> const & upper() const { return upper_; }

      rt_interval const & operator=(rt_interval const & other)
      {
        lower_ = other.lower().get()->clone();
        upper_ = other.upper().get()->clone();
      };

    private:
      rt_expr<InterfaceType> lower_;
      rt_expr<InterfaceType> upper_;
  };

  /** @brief Convenience overload for streaming an interval to an output stream. */
  template <typename InterfaceType>
  std::ostream& operator<<(std::ostream & stream, rt_interval<InterfaceType> const & e)
  {
    stream << "interval(" << e.lower() << ", " << e.upper() << ")";
    return stream;
  }




  //////// helper functions for creating equations (uniform interface for compile time and run time: ///////////


  //
  // run time
  //

  ////// scalar is first argument:
  /** @brief Creates an interval with two numerical limits */
  inline rt_interval<> make_interval(default_numeric_type lhs, default_numeric_type rhs)
  {
    rt_expr<> a = rt_constant<>(lhs);
    rt_expr<> b = rt_constant<>(rhs);
    return rt_interval<>(a, b);
  }

  /** @brief Creates an interval, where the upper bound is given by a ViennaMath constant */
  template <typename NumericT, typename InterfaceType, typename RHSType>
  rt_interval<InterfaceType> make_interval(typename InterfaceType::numeric_type lhs, rt_constant<NumericT, InterfaceType> const & rhs)
  {
    return rt_interval<InterfaceType>(lhs, rhs);
  }

  /** @brief Creates an interval, where the upper bound is given by a ViennaMath runtime variable */
  template <typename InterfaceType>
  rt_interval<InterfaceType> make_interval(typename InterfaceType::numeric_type lhs, rt_variable<InterfaceType> const & rhs)
  {
    return rt_interval<InterfaceType>(lhs, rhs);
  }

  /** @brief Creates an interval, where the upper bound is given by a ViennaMath compiletime variable */
  template <id_type id>
  rt_interval<> make_interval(default_numeric_type lhs, ct_variable<id> const & rhs)
  {
    return rt_interval<>(lhs, rhs);
  }

  /** @brief Creates an interval, where the upper bound is given by a ViennaMath unary expression */
  template <typename InterfaceType>
  rt_interval<InterfaceType> make_interval(typename InterfaceType::numeric_type lhs, rt_unary_expr<InterfaceType> const & rhs)
  {
    return rt_interval<InterfaceType>(lhs, rhs);
  }

  /** @brief Creates an interval, where the upper bound is given by a ViennaMath binary expression */
  template <typename InterfaceType>
  rt_interval<InterfaceType> make_interval(typename InterfaceType::numeric_type lhs, rt_binary_expr<InterfaceType> const & rhs)
  {
    return rt_interval<InterfaceType>(lhs, rhs);
  }

  /** @brief Creates an interval, where the upper bound is given by a ViennaMath expression wrapper */
  template <typename InterfaceType>
  rt_interval<InterfaceType> make_interval(typename InterfaceType::numeric_type lhs, rt_expr<InterfaceType> const & rhs)
  {
    return rt_interval<InterfaceType>(lhs, rhs);
  }


  //constant:
  /** @brief Creates an interval, where the lower bound is given by a ViennaMath runtime constant */
  template <typename NumericT, typename InterfaceType, typename RHSType>
  rt_interval<InterfaceType> make_interval(rt_constant<NumericT, InterfaceType> const & lhs, RHSType const & rhs)
  {
    return rt_interval<InterfaceType>(lhs, rhs);
  }

  //variable:
  /** @brief Creates an interval, where the lower bound is given by a ViennaMath runtime variable */
  template <typename InterfaceType, typename RHSType>
  rt_interval<InterfaceType> make_interval(rt_variable<InterfaceType> const & lhs, RHSType const & rhs)
  {
    return rt_interval<InterfaceType>(lhs, rhs);
  }

  /** @brief Creates an interval, where the lower bound is given by a ViennaMath compiletime variable */
  template <id_type id, typename RHSType>
  rt_interval<> make_interval(ct_variable<id> const & lhs, RHSType const & rhs)
  {
    rt_variable<> temp(id);
    return rt_interval<>(temp, rhs);
  }

  //function_symbol:
  /** @brief Creates an interval, where the lower bound is given by a ViennaMath runtime function symbol */
  template <typename InterfaceType, typename RHSType>
  rt_interval<InterfaceType> make_interval(rt_function_symbol<InterfaceType> const & lhs, RHSType const & rhs)
  {
    return rt_interval<InterfaceType>(lhs, rhs);
  }

  //unary:
  /** @brief Creates an interval, where the lower bound is given by a ViennaMath runtime unary expression */
  template <typename InterfaceType, typename RHSType>
  rt_interval<InterfaceType> make_interval(rt_unary_expr<InterfaceType> const & lhs, RHSType const & rhs)
  {
    return rt_interval<InterfaceType>(lhs, rhs);
  }


  //binary
  /** @brief Creates an interval, where the lower bound is given by a ViennaMath runtime binary expression */
  template <typename InterfaceType, typename RHSType>
  rt_interval<InterfaceType> make_interval(rt_binary_expr<InterfaceType> const & lhs, RHSType const & rhs)
  {
    return rt_interval<InterfaceType>(lhs, rhs);
  }

  //expr
  /** @brief Creates an interval, where the lower bound is given by a ViennaMath runtime expression wrapper */
  template <typename InterfaceType, typename RHSType>
  rt_interval<InterfaceType> make_interval(rt_expr<InterfaceType> const & lhs, RHSType const & rhs)
  {
    return rt_interval<InterfaceType>(lhs, rhs);
  }

  //compile time with run time stuff also possible:
  /** @brief Creates an interval, where the lower bound is given by a ViennaMath compiletime binary expression */
  template <typename LHS, typename OP, typename RHS, typename RHSType>
  rt_interval<typename RHSType::interface_type>
  make_interval(ct_binary_expr<LHS, OP, RHS> const & lhs, RHSType const & rhs)
  {
    return rt_interval<typename RHSType::interface_type>(lhs, rhs);
  }

  /** @brief Creates an interval, where the lower bound is given by a ViennaMath compiletime unary expression */
  template <typename LHS, typename OP, typename RHSType>
  rt_interval<typename RHSType::interface_type>
  make_interval(ct_unary_expr<LHS, OP> const & lhs, RHSType const & rhs)
  {
    return rt_interval<typename RHSType::interface_type>(lhs, rhs);
  }

  /** @brief Creates an interval, where the lower bound is given by a ViennaMath compiletime constant */
  template <long val, typename RHSType>
  rt_interval<typename RHSType::interface_type>
  make_interval(ct_constant<val> const & lhs, RHSType const & rhs)
  {
    return rt_interval<typename RHSType::interface_type>(lhs, rhs);
  }

  /** @brief Creates an interval, where the lower bound is given by a ViennaMath compiletime function symbol */
  template <typename TAG, typename RHSType>
  rt_interval<typename RHSType::interface_type>
  make_interval(ct_function_symbol<TAG> const & lhs, RHSType const & rhs)
  {
    return rt_interval<typename RHSType::interface_type>(lhs, rhs);
  }

  /** @brief Creates an interval, where the lower bound is given by a ViennaMath compiletime variable */
  template <id_type id, typename RHSType>
  rt_interval<typename RHSType::interface_type>
  make_interval(ct_variable<id> const & lhs, RHSType const & rhs)
  {
    return rt_interval<typename RHSType::interface_type>(lhs, rhs);
  }

}

#endif
