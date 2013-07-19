#ifndef VIENNAMATH_COMPILETIME_OPERATIONS_CT_CT_HPP
#define VIENNAMATH_COMPILETIME_OPERATIONS_CT_CT_HPP

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
#include "viennamath/compiletime/ct_variable.hpp"
#include "viennamath/compiletime/ct_binary_expr.hpp"

/** @file viennamath/compiletime/operations/ct_ct.hpp
    @brief Metafunctions for the generic operator overloads of compiletime expressions are defined here
*/

namespace viennamath
{

  namespace result_of
  {
    /** @brief Metafunction for adding two compiletime expressions */
    template <typename LHS, typename RHS>
    struct add<LHS, RHS, true, true>
    {
      typedef ct_binary_expr<LHS,
                             op_plus<default_numeric_type>,
                             RHS >              type;

      static type instance(LHS const & /*lhs*/, RHS const & /*rhs*/) { return type(); }
    };

    /** @brief Metafunction for subtracting two compiletime expressions */
    template <typename LHS, typename RHS>
    struct subtract<LHS, RHS, true, true>
    {
      typedef ct_binary_expr<LHS,
                             op_minus<default_numeric_type>,
                             RHS >              type;

      static type instance(LHS const & /*lhs*/, RHS const & /*rhs*/) { return type(); }
    };

    /** @brief Metafunction for multiplying two compiletime expressions */
    template <typename LHS, typename RHS>
    struct mult<LHS, RHS, true, true>
    {
      typedef ct_binary_expr<LHS,
                             op_mult<default_numeric_type>,
                             RHS >              type;

      static type instance(LHS const & /*lhs*/, RHS const & /*rhs*/) { return type(); }
    };

    /** @brief Metafunction for dividing two compiletime expressions */
    template <typename LHS, typename RHS>
    struct div<LHS, RHS, true, true>
    {
      typedef ct_binary_expr<LHS,
                             op_div<default_numeric_type>,
                             RHS >              type;

      static type instance(LHS const & /*lhs*/, RHS const & /*rhs*/) { return type(); }
    };

  }




}

#endif
