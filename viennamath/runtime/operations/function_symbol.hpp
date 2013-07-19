#ifndef VIENNAMATH_FUNCTION_SYMBOL_OPERATIONS_HPP
#define VIENNAMATH_FUNCTION_SYMBOL_OPERATIONS_HPP

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
#include "viennamath/runtime/binary_operators.hpp"

/** @file viennamath/runtime/operations/function_symbol.hpp
    @brief Defines special (non-standard) operator overloads for runtime function symbols.
*/

namespace viennamath
{


    ////////////////// Operations //////////////////////////////

    ////// operator+

    ////// operator-

    ////// operator*
    /** @brief Special overload for the multiplication of a function symbol with a ViennaMath runtime constant */
    template <typename NumericT, typename InterfaceType>
    rt_binary_expr<InterfaceType> operator*(rt_constant<NumericT, InterfaceType> const & lhs, rt_function_symbol<InterfaceType> const & rhs)
    {
      return rt_binary_expr<InterfaceType>(lhs.clone(), new op_binary<op_mult<typename InterfaceType::numeric_type>, InterfaceType>(), rhs.clone());
    }

    /** @brief Special overload for the multiplication of a function symbol with a ViennaMath runtime expression wrapper */
    template <typename InterfaceType>
    rt_binary_expr<InterfaceType> operator*(rt_expr<InterfaceType> const & lhs, rt_function_symbol<InterfaceType> const & rhs)
    {
      return rt_binary_expr<InterfaceType>(lhs.get()->clone(), new op_binary<op_mult<typename InterfaceType::numeric_type>, InterfaceType>(), rhs.clone());
    }

    ////// operator/


}

#endif
