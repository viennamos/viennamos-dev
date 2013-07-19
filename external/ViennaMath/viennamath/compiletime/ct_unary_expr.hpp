#ifndef VIENNAMATH_COMPILETIME_CT_UNARY_EXPR_HPP
#define VIENNAMATH_COMPILETIME_CT_UNARY_EXPR_HPP

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
#include "viennamath/compiletime/unary_op_tags.hpp"

/** @file   viennamath/compiletime/ct_unary_expr.hpp
    @brief  Defines a compiletime unary expression (represents the expression f(e), where f is function and 'e' is an expression)
*/

namespace viennamath
{

  //A compile time expression
  /** @brief Defines a unary expression f(e), where f is function and 'e' is an expression
   *
   * @tparam LHS     The expression on which the unary operation acts
   * @tparam OP      A tag identifying the operation
   */
  template <typename LHS,
            typename OP>
  class ct_unary_expr
  {
      typedef typename expression_traits<LHS>::const_reference_type    internal_lhs_type;
    public:
      typedef typename OP::numeric_type            numeric_type;

      typedef LHS    lhs_type;
      typedef OP     op_type;

      explicit ct_unary_expr() : lhs_(LHS()) {}

      explicit ct_unary_expr(internal_lhs_type lhs) : lhs_(lhs){}

      /** @brief Returns the operation the unary operation is acting on */
      internal_lhs_type lhs() const { return lhs_; }

      template <typename VectorType>
      numeric_type operator()(VectorType const & v) const
      {
        //std::cout << "ct_expr::operator()" << std::endl;
        return OP::apply(static_cast<numeric_type>(lhs_(v)), static_cast<numeric_type>(rhs_(v)));
      }

    private:
      internal_lhs_type lhs_;
  };


  //stream operator for output:
  /** @brief Convenience overload for printing a compiletime unary expression to an output stream */
  template <typename LHS, typename OP>
  std::ostream& operator<<(std::ostream & stream, ct_unary_expr<LHS, OP> const & other)
  {
    stream << "[" << other.lhs() << OP().str() << "]";
    return stream;
  }


}

#endif
