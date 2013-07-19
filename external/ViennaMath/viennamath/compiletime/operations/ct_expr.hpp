#ifndef VIENNAMATH_COMPILETIME_OPERATIONS_CT_EXPR_HPP
#define VIENNAMATH_COMPILETIME_OPERATIONS_CT_EXPR_HPP

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




#include "viennamath/forwards.h"
//#include "viennamath/runtime/constant.hpp"
//#include "viennamath/runtime/variable.hpp"
#include "viennamath/compiletime/ct_binary_expr.hpp"
//#include "viennamath/runtime/binary_operators.hpp"

/** @file viennamath/compiletime/operations/ct_expr.hpp
    @brief Special operator overloads containing a binary expression are collected here.
*/

namespace viennamath
{
  /////////////////////////////////// operator+ ///////////////////////////////////

      /** @brief Special overload: (ct_c1 + X) + ct_c2 becomes [ct_c1 + ct_c2] + X, where ct_c1 and ct_c2 are compiletime constants and X is any other expression */
      template <typename ScalarType, long value_1, typename RHS, long value_2>
      ct_binary_expr<ct_constant< value_1 + value_2 >,
                     op_plus<ScalarType>,
                     RHS >
      operator+(ct_binary_expr<ct_constant<value_1>, op_plus<ScalarType>, RHS> const & lhs,
                ct_constant<value_2> const & /*other*/)
      {
        return ct_binary_expr<ct_constant< value_1 + value_2 >,
                              op_plus<ScalarType>,
                              RHS >(ct_constant< value_1 + value_2 >(),
                                    lhs.rhs());
      }


      /** @brief Special overload: (ct_c1 - X) + ct_c2 becomes [ct_c1 + ct_c2] - X, where ct_c1 and ct_c2 are compiletime constants and X is any other expression */
      template <typename ScalarType, long value_1, typename RHS, long value_2>
      ct_binary_expr<ct_constant< value_1 + value_2 >,
              op_minus<ScalarType>,
              RHS >
      operator+(ct_binary_expr<ct_constant<value_1>, op_minus<ScalarType>, RHS> const & lhs,
                ct_constant<value_2> const & /*other*/)
      {
        return ct_binary_expr<ct_constant< value_1 + value_2 >,
                              op_minus<ScalarType>,
                              RHS >(ct_constant< value_1 + value_2 >(),
                                    lhs.rhs());
      }

      /** @brief Special overload: (c1 - X) + ct_c2 -> [c1 + ct_c2] - X, where ct_c1 and ct_c2 are compiletime constants and X is any other expression */
      template <typename ScalarType, typename OtherScalarType, typename RHS, long value>
      ct_binary_expr< rt_constant<typename promote_traits<OtherScalarType, op_plus<ScalarType>, long>::result_type >,
                      op_minus<ScalarType>,
                      RHS >
      operator+(ct_binary_expr<rt_constant<OtherScalarType>, op_minus<ScalarType>, RHS> const & lhs,
                ct_constant<value> const & other)
      {
        typedef rt_constant< typename promote_traits<OtherScalarType, op_plus<ScalarType>, long>::result_type >    result_constant;
        return ct_binary_expr<result_constant,
                              op_minus<ScalarType>,
                              RHS >(result_constant(static_cast<OtherScalarType>(lhs.lhs()) + value),
                                    lhs.rhs());
      }


  /////////////////////////////////// operator- ///////////////////////////////////

      /** @brief Special overload:  (A + B) - A  -> B, where A is an variable and X can be anything */
      template <typename ScalarType, id_type id, typename RHS>
      RHS
      operator-(ct_binary_expr<ct_variable<id>, op_plus<ScalarType>, RHS> const & lhs,
                ct_variable<id> const & /*other*/)
      {
        return lhs.rhs();
      }


      /** @brief Special overload:  (ct_c1 + X) - ct_c2 -> [ct_c1 - ct_c2] + X, where ct_c1 and ct_c2 are compiletime constants and X is any other expression */
      template <long value_1, typename RHS, long value_2, typename T>
      ct_binary_expr< ct_constant< value_1 - value_2 >,
                      op_plus<T>,
                      RHS >
      operator-(ct_binary_expr<ct_constant<value_1>, op_plus<T>, RHS> const & lhs,
                ct_constant<value_2> const & /*other*/)
      {
        return ct_binary_expr<ct_constant< value_1 - value_2 >,
                              op_plus<T>,
                              RHS >(ct_constant< value_1 - value_2 >(),
                                    lhs.rhs());
      }

      /** @brief Special overload:  (c1 + X) - ct_c2 -> [c1 - ct_c2] + X, where ct_c1 and ct_c2 are compiletime constants and X is any other expression */
      template <typename OtherScalarType, typename RHS, long value, typename T>
      ct_binary_expr< rt_constant<typename promote_traits<OtherScalarType, op_plus<T>, long>::result_type >,
                      op_plus<T>,
                      RHS >
      operator-(ct_binary_expr<rt_constant<OtherScalarType>, op_plus<T>, RHS> const & lhs,
                ct_constant<value> const & other)
      {
        typedef rt_constant< typename promote_traits<OtherScalarType, op_plus<T>, long>::result_type >    result_constant;
        return ct_binary_expr<result_constant,
                              op_plus<T>,
                              RHS >(result_constant(static_cast<OtherScalarType>(lhs.lhs()) - value),
                                    lhs.rhs());
      }

      /** @brief Special overload:  (ct_c1 - X) - ct_c2 -> [ct_c1 - ct_c2] - X, where ct_c1 and ct_c2 are compiletime constants and X is any other expression */
      template <long value_1, typename RHS, long value_2, typename T>
      ct_binary_expr< ct_constant< value_1 - value_2 >,
               op_minus<T>,
               RHS >
      operator-(ct_binary_expr<ct_constant<value_1>, op_minus<T>, RHS> const & lhs,
                ct_constant<value_2> const & /*other*/)
      {
        return ct_binary_expr<ct_constant< value_1 - value_2 >,
                              op_minus<T>,
                              RHS >(ct_constant< value_1 - value_2 >(),
                                    lhs.rhs());
      }

      /** @brief Special overload:  (c1 - X) - ct_c2 -> [c1 - ct_c2] - X, where ct_c1 and ct_c2 are compiletime constants and X is any other expression */
      template <typename OtherScalarType, typename RHS, long value, typename T>
      ct_binary_expr< rt_constant<typename promote_traits<OtherScalarType, op_plus<T>, long>::result_type >,
                      op_minus<T>,
                      RHS >
      operator-(ct_binary_expr<rt_constant<OtherScalarType>, op_minus<T>, RHS> const & lhs,
                ct_constant<value> const & /*other*/)
      {
        typedef rt_constant< typename promote_traits<OtherScalarType, op_plus<T>, long>::result_type >    result_constant;
        return ct_binary_expr<result_constant,
                              op_minus<T>,
                               RHS >(result_constant(static_cast<OtherScalarType>(lhs.lhs()) - value),
                                     lhs.rhs());
      }



  /////////////////////////////////// operator* ///////////////////////////////////



  /////////////////////////////////// operator/ ///////////////////////////////////

      /** @brief Special overload: division of the same thing leads to '1' */
      template <typename LHS, typename OP, typename RHS>
      ct_constant<1>
      operator/(ct_binary_expr<LHS, OP, RHS> const & /*lhs*/,
                ct_binary_expr<LHS, OP, RHS> const & /*other*/)
      {
        return ct_constant<1>();
      }

}

#endif
