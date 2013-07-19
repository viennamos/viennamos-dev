#ifndef VIENNAMATH_COMPILETIME_OPERATIONS_CT_CONSTANT_HPP
#define VIENNAMATH_COMPILETIME_OPERATIONS_CT_CONSTANT_HPP

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
#include "viennamath/compiletime/ct_constant.hpp"

/** @file viennamath/compiletime/operations/ct_constant.hpp
    @brief Specialized operator overloads involoving compiletime constants are defined here.
*/

namespace viennamath
{

  ////////////////// Operations //////////////////////////////

  //operator+

  /** @brief Special overload:  ct_c1 + (c2 + X) -> [ct_c1 + c2] + X, where ct_c1 is a compiletime constant, c2 is a runtime constant, and X is any other expression */
  template <long value, typename ScalarType, typename RHS>
  ct_binary_expr<rt_constant< typename promote_traits<long, op_plus<default_numeric_type>, ScalarType>::result_type >,
                 op_plus<default_numeric_type>,
                 RHS >
  operator+(ct_constant<value> const & /*lhs*/,
            ct_binary_expr<rt_constant<ScalarType>, op_plus<default_numeric_type>, RHS> const & other)
  {
    typedef rt_constant< typename promote_traits<long, op_plus<default_numeric_type>, ScalarType>::result_type >   result_constant;
    return ct_binary_expr<result_constant,
                          op_plus<default_numeric_type>,
                          RHS >(result_constant(value + other.lhs()), other.rhs());
  }

  /** @brief Special overload:  ct_c1 + (ct_c2 + X) -> [ct_c1 + ct_c2] + X, where ct_c1 and ct_c2 are compiletime constants and X is any other expression */
  template <long value, long value2, typename RHS>
  ct_binary_expr<ct_constant<value + value2>,
                 op_plus<default_numeric_type>,
                 RHS >
  operator+(ct_constant<value> const & /*lhs*/,
            ct_binary_expr<ct_constant<value2>, op_plus<default_numeric_type>, RHS> const & other)
  {
    return ct_binary_expr<ct_constant<value + value2>,
                          op_plus<default_numeric_type>,
                          RHS >(ct_constant<value + value2>(), other.rhs());
  }

  /** @brief Special overload:  ct_c1 + (c2 - X)    -> [ct_c1 + c2]    - X, where ct_c1 is a compiletime constant, c2 is a runtime constant, and X is any other expression */
  template <long value, typename ScalarType, typename RHS>
  ct_binary_expr<rt_constant< typename promote_traits<long, op_plus<default_numeric_type>, ScalarType>::result_type >,
                 op_minus<default_numeric_type>,
                 RHS >
  operator+(ct_constant<value> const & /*lhs*/,
            ct_binary_expr<rt_constant<ScalarType>, op_minus<default_numeric_type>, RHS> const & other)
  {
    typedef rt_constant< typename promote_traits<long, op_plus<default_numeric_type>, ScalarType>::result_type >   result_constant;
    return ct_binary_expr<result_constant,
                          op_minus<default_numeric_type>,
                          RHS >(result_constant(value + other.lhs()), other.rhs());
  }

  /** @brief Special overload:  ct_c1 + (ct_c2 - X) -> [ct_c1 + ct_c2] - X, where ct_c1 and ct_c2 are compiletime constants and X is any other expression */
  template <long value, long value2, typename RHS>
  ct_binary_expr<ct_constant<value + value2>,
                 op_minus<default_numeric_type>,
                 RHS >
  operator+(ct_constant<value> const & /*lhs*/,
            ct_binary_expr<ct_constant<value2>, op_minus<default_numeric_type>, RHS> const & other)
  {
    return ct_binary_expr<ct_constant<value + value2>,
                          op_minus<default_numeric_type>,
                          RHS >(ct_constant<value + value2>(), other.rhs());
  }

  /** @brief Special overload: Condense addition of two compiletime constants into one constant */
  template <long value, long other_value>
  ct_constant<value + other_value>
  operator+(ct_constant<value> const & /*lhs*/,
            ct_constant<other_value> const & /*other*/)
  {
    return ct_constant<value + other_value>();
  }


  //operator-

  /** @brief Special overload:  ct_c1 - (c2 + X) -> [ct_c1 - c2] - X, where ct_c1 is a compiletime constant, c2 is a runtime constant, and X is any other expression */
  template <long value, typename ScalarType, typename RHS>
  ct_binary_expr<rt_constant< typename promote_traits<long, op_minus<default_numeric_type>, ScalarType>::result_type >,
                 op_minus<default_numeric_type>,
                 RHS >
  operator-(ct_constant<value> const & /*lhs*/,
            ct_binary_expr<rt_constant<ScalarType>, op_plus<default_numeric_type>, RHS> const & other)
  {
    typedef rt_constant< typename promote_traits<long, op_minus<default_numeric_type>, ScalarType>::result_type >   result_constant;
    return ct_binary_expr<result_constant,
                          op_minus<default_numeric_type>,
                          RHS >(result_constant(value - other.lhs()), other.rhs());
  }

  /** @brief Special overload:  ct_c1 - (ct_c2 + X) -> [ct_c1 - ct_c2] - X, where ct_c1 and ct_c2 are compiletime constants and X is any other expression */
  template <long value, long value2, typename RHS>
  ct_binary_expr<ct_constant<value - value2>,
                 op_minus<default_numeric_type>,
                 RHS >
  operator-(ct_constant<value> const & /*lhs*/,
            ct_binary_expr<ct_constant<value2>, op_plus<default_numeric_type>, RHS> const & other)
  {
    return ct_binary_expr<ct_constant<value - value2>,
                          op_minus<default_numeric_type>,
                          RHS >(ct_constant<value - value2>(), other.rhs());
  }

  /** @brief Special overload:  ct_c1 - (c2 - X)    -> [ct_c1 - c2] + X, where ct_c1 is a compiletime constant, c2 is a runtime constant, and X is any other expression */
  template <long value, typename ScalarType, typename RHS>
  ct_binary_expr<rt_constant< typename promote_traits<long, op_minus<default_numeric_type>, ScalarType>::result_type >,
                 op_plus<default_numeric_type>,
                 RHS >
  operator-(ct_constant<value> const & /*lhs*/,
            ct_binary_expr<rt_constant<ScalarType>, op_minus<default_numeric_type>, RHS> const & other)
  {
    typedef rt_constant< typename promote_traits<long, op_minus<default_numeric_type>, ScalarType>::result_type >   result_constant;
    return ct_binary_expr<result_constant,
                          op_plus<default_numeric_type>,
                          RHS >(result_constant(value - other.lhs()), other.rhs());
  }

  /** @brief Special overload:  ct_c1 - (ct_c2 - X) -> [ct_c1 - ct_c2] + X, where ct_c1 and ct_c2 are compiletime constants and X is any other expression */
  template <long value, long value2, typename RHS>
  ct_binary_expr<ct_constant<value - value2>,
                 op_plus<default_numeric_type>,
                 RHS >
  operator-(ct_constant<value> const & /*lhs*/,
            ct_binary_expr<ct_constant<value2>, op_minus<default_numeric_type>, RHS> const & other)
  {
    return ct_binary_expr<ct_constant<value - value2>,
                          op_plus<default_numeric_type>,
                          RHS >(ct_constant<value - value2>(), other.rhs());
  }

  /** @brief Special overload: Condense subtraction of two compiletime constants into one constant */
  template <long value, long other_value>
  ct_constant<value - other_value>
  operator-(ct_constant<value> const & /*lhs*/,
            ct_constant<other_value> const & /*other*/)
  {
    return ct_constant<value - other_value>();
  }


  //operator*

  /** @brief Special overload: Condense multiplication of two compiletime constants into one constant */
  template <long value, long other_value>
  ct_constant<value * other_value>
  operator*(ct_constant<value> const & /*lhs*/,
            ct_constant<other_value> const & /*other*/)
  {
    return ct_constant<value * other_value>();
  }


  //operator/

  // (none so far)

}

#endif
