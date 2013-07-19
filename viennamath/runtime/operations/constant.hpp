#ifndef VIENNAMATH_RUNTIME_OPERATIONS_CONSTANT_HPP
#define VIENNAMATH_RUNTIME_OPERATIONS_CONSTANT_HPP

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
#include "viennamath/compiletime/promote_traits.hpp"
#include "viennamath/runtime/binary_operators.hpp"

/** @file viennamath/runtime/operations/constant.hpp
    @brief Defines special (non-standard) operator overloads for runtime constants.
*/

namespace viennamath
{

    ////////////////// Operations //////////////////////////////

    ////// operator+


    /** @brief Special overload for c1 + (c2 + X) -> [c1 + c2] + X, where c1 and c2 are runtime constants and X is any expression: */
    template <typename ScalarType, typename InterfaceType, typename OtherScalarType, typename RHS>
    ct_binary_expr<rt_constant<typename promote_traits<ScalarType, op_plus<typename InterfaceType::numeric_type>, OtherScalarType>::result_type, InterfaceType >,
                   op_plus<typename InterfaceType::numeric_type>,
                   RHS >
    operator+(rt_constant<ScalarType, InterfaceType> const & lhs,
              ct_binary_expr<rt_constant<OtherScalarType, InterfaceType>, op_plus<typename InterfaceType::numeric_type>, RHS> const & other)
    {
      typedef rt_constant<typename promote_traits<ScalarType, op_plus<typename InterfaceType::numeric_type>, OtherScalarType>::result_type, InterfaceType >  ret_constant;
      return ct_binary_expr<ret_constant,
                            op_plus<typename InterfaceType::numeric_type>,
                            RHS
                           >(ret_constant(static_cast<ScalarType>(lhs) + static_cast<OtherScalarType>(other.lhs())),
                             other.rhs());
    }

    /** @brief Special overload for c1 + (ct_c2 + X) -> [c1 + ct_c2] + X, where c1 is a runtime constant, ct_c2 is a compiletime constant, and X is any expression: */
    template <typename ScalarType, typename InterfaceType, long value, typename RHS>
    ct_binary_expr<rt_constant<typename promote_traits<ScalarType, op_plus<typename InterfaceType::numeric_type>, long>::result_type, InterfaceType >,
                   op_plus<typename InterfaceType::numeric_type>,
                   RHS >
    operator+(rt_constant<ScalarType, InterfaceType> const & lhs,
              ct_binary_expr<ct_constant<value>, op_plus<typename InterfaceType::numeric_type>, RHS> const & other)
    {
      typedef rt_constant<typename promote_traits<ScalarType, op_plus<typename InterfaceType::numeric_type>, long>::result_type, InterfaceType >  ret_constant;
      return ct_binary_expr<ret_constant,
                            op_plus<typename InterfaceType::numeric_type>,
                            RHS
                           >(ret_constant(static_cast<ScalarType>(lhs) + value),
                             other.rhs());
    }

    /** @brief Special overload for c1 + (c2 - X) -> [c1 + c2] - X, where c1 and c2 are runtime constants and X is any expression: */
    template <typename ScalarType, typename InterfaceType, typename OtherScalarType, typename RHS>
    ct_binary_expr<rt_constant<typename promote_traits<ScalarType, op_plus<typename InterfaceType::numeric_type>, OtherScalarType>::result_type, InterfaceType >,
                   op_minus<typename InterfaceType::numeric_type>,
                   RHS >
    operator+(rt_constant<ScalarType, InterfaceType> const & lhs,
              ct_binary_expr<rt_constant<OtherScalarType, InterfaceType>, op_minus<typename InterfaceType::numeric_type>, RHS> const & other)
    {
      typedef rt_constant<typename promote_traits<ScalarType, op_plus<typename InterfaceType::numeric_type>, OtherScalarType>::result_type, InterfaceType >  ret_constant;
      return ct_binary_expr<ret_constant,
                            op_minus<typename InterfaceType::numeric_type>,
                            RHS
                           >(ret_constant(static_cast<ScalarType>(lhs) + static_cast<OtherScalarType>(other.lhs())),
                             other.rhs());
    }

    /** @brief Special overload for c1 + (ct_c2 - X) -> [c1 + ct_c2] - X, where c1 is a runtime constant, ct_c2 is a compiletime constant, and X is any expression: */
    template <typename ScalarType, typename InterfaceType, long value, typename RHS>
    ct_binary_expr<rt_constant<typename promote_traits<ScalarType, op_plus<typename InterfaceType::numeric_type>, long>::result_type, InterfaceType >,
                   op_minus<typename InterfaceType::numeric_type>,
            RHS >
    operator+(rt_constant<ScalarType, InterfaceType> const & lhs,
              ct_binary_expr<ct_constant<value>, op_minus<typename InterfaceType::numeric_type>, RHS> const & other)
    {
      typedef rt_constant<typename promote_traits<ScalarType, op_plus<typename InterfaceType::numeric_type>, long>::result_type, InterfaceType >  ret_constant;
      return ct_binary_expr<ret_constant,
                            op_minus<typename InterfaceType::numeric_type>,
                            RHS
                           >(ret_constant(static_cast<ScalarType>(lhs) + value),
                             other.rhs());
    }


    /** @brief Special overload for (ct_c1 + X) + c2 -> [ct_c1 + c2] + X, where ct_c1 is a compile constant, c2 is a runtime constant, and X is any expression: */
    template <long value, typename RHS, typename ScalarType, typename T, typename InterfaceType>
    ct_binary_expr< rt_constant<typename promote_traits<long, op_plus<T>, ScalarType>::result_type >,
                    op_plus<T>,
                    RHS >
    operator+(ct_binary_expr<ct_constant<value>, op_plus<T>, RHS> const & lhs,
              rt_constant<ScalarType, InterfaceType> const & other)
    {
      typedef rt_constant< typename promote_traits<long, op_plus<T>, ScalarType>::result_type >    result_constant;
      return ct_binary_expr<result_constant,
                            op_plus<T>,
                            RHS >(result_constant(value + static_cast<ScalarType>(other)),
                                  lhs.rhs());
    }

    /** @brief Special overload for (ct_c1 - X) + c2 -> [ct_c1 + c2] - X, where ct_c1 is a compile constant, c2 is a runtime constant, and X is any expression: */
    template <long value, typename RHS, typename ScalarType, typename InterfaceType>
    ct_binary_expr< rt_constant<default_numeric_type, InterfaceType >,
                    op_minus<default_numeric_type>,
                    RHS >
    operator+(ct_binary_expr<ct_constant<value>, op_minus<default_numeric_type>, RHS> const & lhs,
              rt_constant<ScalarType, InterfaceType> const & other)
    {
      typedef rt_constant<ScalarType, InterfaceType>    result_constant;
      return ct_binary_expr<result_constant,
                            op_minus<default_numeric_type>,
                            RHS >(result_constant(value + other),
                                  lhs.rhs());
    }



    ////// operator-

    /** @brief Special overload for c1 - (c2 + X) -> [c1 - c2] - X, where c1 and c2 are runtime constants and X is any expression: */
    template <typename ScalarType, typename InterfaceType, typename OtherScalarType, typename RHS>
    ct_binary_expr<rt_constant<typename promote_traits<ScalarType, op_minus<typename InterfaceType::numeric_type>, OtherScalarType>::result_type, InterfaceType >,
                   op_minus<typename InterfaceType::numeric_type>,
                   RHS >
    operator-(rt_constant<ScalarType, InterfaceType> const & lhs,
              ct_binary_expr<rt_constant<OtherScalarType, InterfaceType>, op_plus<typename InterfaceType::numeric_type>, RHS> const & other)
    {
      typedef rt_constant<typename promote_traits<ScalarType, op_minus<typename InterfaceType::numeric_type>, OtherScalarType>::result_type, InterfaceType >  ret_constant;
      return ct_binary_expr<ret_constant,
                            op_minus<typename InterfaceType::numeric_type>,
                            RHS
                           >(ret_constant(static_cast<ScalarType>(lhs) - static_cast<OtherScalarType>(other.lhs())),
                             other.rhs());
    }

    /** @brief Special overload for c1 - (ct_c2 + X) -> [c1 - ct_c2] - X, where c1 is a runtime constant, ct_c2 is a compiletime constant, and X is any expression: */
    template <typename ScalarType, typename InterfaceType, long value, typename RHS>
    ct_binary_expr<rt_constant<typename promote_traits<ScalarType, op_minus<typename InterfaceType::numeric_type>, long>::result_type, InterfaceType >,
                   op_minus<typename InterfaceType::numeric_type>,
                   RHS >
    operator-(rt_constant<ScalarType, InterfaceType> const & lhs,
              ct_binary_expr<ct_constant<value>, op_plus<typename InterfaceType::numeric_type>, RHS> const & other)
    {
      typedef rt_constant<typename promote_traits<ScalarType, op_minus<typename InterfaceType::numeric_type>, long>::result_type, InterfaceType >  ret_constant;
      return ct_binary_expr<ret_constant,
                            op_minus<typename InterfaceType::numeric_type>,
                            RHS
                           >(ret_constant(static_cast<ScalarType>(lhs) - value),
                             other.rhs());
    }

    /** @brief Special overload for c1 - (c2 - X) -> [c1 - c2] + X, where c1 and c2 are runtime constants and X is any expression: */
    template <typename ScalarType, typename InterfaceType, typename OtherScalarType, typename RHS>
    ct_binary_expr<rt_constant<typename promote_traits<ScalarType, op_minus<typename InterfaceType::numeric_type>, OtherScalarType>::result_type, InterfaceType >,
                   op_plus<typename InterfaceType::numeric_type>,
                   RHS >
    operator-(rt_constant<ScalarType, InterfaceType> const & lhs,
              ct_binary_expr<rt_constant<OtherScalarType, InterfaceType>, op_minus<typename InterfaceType::numeric_type>, RHS> const & other)
    {
      typedef rt_constant<typename promote_traits<ScalarType, op_minus<typename InterfaceType::numeric_type>, OtherScalarType>::result_type, InterfaceType >  ret_constant;
      return ct_binary_expr<ret_constant,
                            op_plus<typename InterfaceType::numeric_type>,
                            RHS
                           >(ret_constant(static_cast<ScalarType>(lhs) - static_cast<OtherScalarType>(other.lhs())),
                             other.rhs());
    }

    /** @brief Special overload for c1 - (ct_c2 - X) -> [c1 - ct_c2] + X, where c1 is a runtime constant, ct_c2 is a compiletime constant, and X is any expression: */
    template <typename ScalarType, typename InterfaceType, long value, typename RHS>
    ct_binary_expr<rt_constant<typename promote_traits<ScalarType, op_minus<typename InterfaceType::numeric_type>, long>::result_type, InterfaceType >,
                   op_plus<typename InterfaceType::numeric_type>,
                   RHS >
    operator-(rt_constant<ScalarType, InterfaceType> const & lhs,
              ct_binary_expr<ct_constant<value>, op_minus<typename InterfaceType::numeric_type>, RHS> const & other)
    {
      typedef rt_constant<typename promote_traits<ScalarType, op_minus<typename InterfaceType::numeric_type>, long>::result_type, InterfaceType >  ret_constant;
      return ct_binary_expr<ret_constant,
                            op_plus<typename InterfaceType::numeric_type>,
                            RHS
                           >(ret_constant(static_cast<ScalarType>(lhs) - value),
                             other.rhs());
    }



    /////////// operator*

    /////////// operator/

}

#endif
