#ifndef VIENNAMATH_EXPRESSION_HPP
#define VIENNAMATH_EXPRESSION_HPP

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

/** @file expression.hpp
    @brief This is the main include file of ViennaMath. Only includes other headers and is for convenience only.
*/



#include "viennamath/forwards.h"
#include "viennamath/compiletime/promote_traits.hpp"

#include "viennamath/compiletime/ct_binary_expr.hpp"
#include "viennamath/compiletime/ct_constant.hpp"
#include "viennamath/compiletime/ct_equation.hpp"
#include "viennamath/compiletime/ct_function_symbol.hpp"
#include "viennamath/compiletime/ct_interval.hpp"
#include "viennamath/compiletime/ct_unary_expr.hpp"
#include "viennamath/compiletime/ct_variable.hpp"
#include "viennamath/compiletime/ct_vector.hpp"

#include "viennamath/runtime/variable.hpp"
#include "viennamath/runtime/binary_expr.hpp"
#include "viennamath/runtime/binary_operators.hpp"
#include "viennamath/runtime/unary_expr.hpp"
#include "viennamath/runtime/unary_operators.hpp"
#include "viennamath/runtime/expr.hpp"
#include "viennamath/runtime/equation.hpp"
#include "viennamath/runtime/interval.hpp"
#include "viennamath/runtime/symbolic_interval.hpp"
#include "viennamath/runtime/function_symbol.hpp"
#include "viennamath/runtime/functor_wrapper.hpp"
#include "viennamath/runtime/integral.hpp"
#include "viennamath/runtime/vector_expr.hpp"

#include "viennamath/manipulation/detail/unary_operations.hpp"
#include "viennamath/manipulation/detail/binary_operations.hpp"

#include "viennamath/compiletime/operations/ct_ct.hpp"
#include "viennamath/compiletime/operations/ct_constant.hpp"
#include "viennamath/compiletime/operations/ct_variable.hpp"
#include "viennamath/compiletime/operations/ct_expr.hpp"

#include "viennamath/runtime/operations/rt_rt.hpp"
#include "viennamath/runtime/operations/constant.hpp"
#include "viennamath/runtime/operations/function_symbol.hpp"


#endif
