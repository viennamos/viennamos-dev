#ifndef VIENNAMATH_RUNTIME_OPERATIONS_RT_RT_HPP
#define VIENNAMATH_RUNTIME_OPERATIONS_RT_RT_HPP

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
#include "viennamath/runtime/binary_expr.hpp"

/** @file viennamath/runtime/operations/rt_rt.hpp
    @brief Defines the helper facilities for operator overloads for all ViennaMath expression types.
*/

namespace viennamath
{

  /** @brief Any generic free functions for unifying interfaces are defined here. */
  namespace traits
  {
    template <typename InterfaceType, typename NumericT>
    InterfaceType * clone(rt_constant<NumericT, InterfaceType> const & c) { return c.clone(); }

    template <typename InterfaceType>
    InterfaceType * clone(default_numeric_type d) { return new rt_constant<default_numeric_type, InterfaceType>(d); }

    template <typename InterfaceType, long value>
    InterfaceType * clone(ct_constant<value> const & /*c*/) { return new rt_constant<default_numeric_type, InterfaceType>(value); }

    template <typename InterfaceType>
    InterfaceType * clone(rt_variable<InterfaceType> const & v) { return v.clone(); }

    template <typename InterfaceType, id_type id>
    InterfaceType * clone(ct_variable<id> const & /*v*/) { return new rt_variable<InterfaceType>(id); }

    template <typename InterfaceType>
    InterfaceType * clone(rt_function_symbol<InterfaceType> const & fs) { return fs.clone(); }

    template <typename InterfaceType>
    InterfaceType * clone(rt_unary_expr<InterfaceType> const & e) { return e.clone(); }

    template <typename InterfaceType>
    InterfaceType * clone(rt_binary_expr<InterfaceType> const & e) { return e.clone(); }

    template <typename InterfaceType>
    InterfaceType * clone(rt_expr<InterfaceType> const & e) { return e.get()->clone(); }

    template <typename InterfaceType, typename LHS, typename OP, typename RHS>
    InterfaceType * clone(ct_binary_expr<LHS, OP, RHS> const & e) { return new rt_binary_expr<InterfaceType>(e); }

    template <typename InterfaceType, typename LHS, typename OP>
    InterfaceType * clone(ct_unary_expr<LHS, OP> const & e) { return new rt_unary_expr<InterfaceType>(e); }

  }


  namespace result_of
  {
    /** @brief Returns the type of adding two expressions together. Default case: both types are runtime expressions. */
    template <typename LHS, typename RHS, bool b1, bool b2>
    struct add
    {
      typedef typename interface<LHS, RHS>::type     iface_type;
      typedef typename iface_type::numeric_type      numeric_type;
      typedef rt_binary_expr<iface_type>             type;

      static type instance(LHS const & lhs, RHS const & rhs) { return type(traits::clone<iface_type>(lhs), new op_binary<op_plus<numeric_type>, iface_type>(), traits::clone<iface_type>(rhs)); }
    };

    /** @brief Returns the type when subtracting two expressions. Default case: both types are runtime expressions. */
    template <typename LHS, typename RHS, bool b1, bool b2>
    struct subtract
    {
      typedef typename interface<LHS, RHS>::type     iface_type;
      typedef typename iface_type::numeric_type      numeric_type;
      typedef rt_binary_expr<iface_type>             type;

      static type instance(LHS const & lhs, RHS const & rhs) { return type(traits::clone<iface_type>(lhs), new op_binary<op_minus<numeric_type>, iface_type>(), traits::clone<iface_type>(rhs)); }
    };

    /** @brief Returns the type when multiplying two expressions. Default case: both types are runtime expressions. */
    template <typename LHS, typename RHS, bool b1, bool b2>
    struct mult
    {
      typedef typename interface<LHS, RHS>::type     iface_type;
      typedef typename iface_type::numeric_type      numeric_type;
      typedef rt_binary_expr<iface_type>             type;

      static type instance(LHS const & lhs, RHS const & rhs) { return type(traits::clone<iface_type>(lhs), new op_binary<op_mult<numeric_type>, iface_type>(), traits::clone<iface_type>(rhs)); }
    };

    /** @brief Returns the type when dividing two expressions. Default case: both types are runtime expressions. */
    template <typename LHS, typename RHS, bool b1, bool b2>
    struct div
    {
      typedef typename interface<LHS, RHS>::type     iface_type;
      typedef typename iface_type::numeric_type      numeric_type;
      typedef rt_binary_expr<iface_type>             type;

      static type instance(LHS const & lhs, RHS const & rhs) { return type(traits::clone<iface_type>(lhs), new op_binary<op_div<numeric_type>, iface_type>(), traits::clone<iface_type>(rhs)); }
    };

  }




}

#endif
