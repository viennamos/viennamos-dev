#ifndef VIENNAMATH_MANIPULATION_DROP_DEPENDENT_TERMS_HPP
#define VIENNAMATH_MANIPULATION_DROP_DEPENDENT_TERMS_HPP

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
#include "viennamath/manipulation/expand.hpp"
#include "viennamath/manipulation/substitute.hpp"
#include "viennamath/manipulation/simplify.hpp"

/** @file    drop_dependent_terms.hpp
    @brief   Provides routines for dropping all terms with a dependency on a certain variable or expression from an expression. Only compiletime support at the moment.
*/

namespace viennamath
{

  ///////////////// compile time ////////////////////////////

  namespace result_of
  {

    // Reuse existing functionality: Replace FactorType by ct_constant<0>, then optimize:

    /** @brief Interface metafunction for removing dependent terms from an expression
     *
     * @tparam FactorType              The variable/expression for which all dependent terms should be thrown away
     * @tparam ExpressionType          The expression from which the factor should be removed
     */
    template <typename FactorType,
              typename ExpressionType>
    struct drop_dependent_terms
    {
      typedef typename viennamath::result_of::expand<ExpressionType>::type    expanded_expression; //[KR]: Is it actually necessary to expand here?

      typedef typename viennamath::result_of::substitute<FactorType, ct_constant<0>, expanded_expression>::type    substituted_expression;

      typedef typename viennamath::result_of::simplify<substituted_expression>::type    type;
    };
  }

  //interface function:
  /** @brief Main user function for dropping all terms from an expression which depend on a certain variable or expression.
   *
   * Technically, this is accomplished by replacing the respective factor by zero
   *
   * @param f     The factor which should be set to zero
   * @param e     The full expression from which the dependent terms should be dropped
   */
  template <typename FactorType,
            typename ExpressionType>
  typename result_of::drop_dependent_terms<FactorType, ExpressionType>::type
  drop_dependent_terms(FactorType const & /*f*/, ExpressionType const & /*e*/)
  {
    return typename result_of::drop_dependent_terms<FactorType, ExpressionType>::type();
  }

  //////////////// run time ///////////////////////////

  // Not yet available
}

#endif
