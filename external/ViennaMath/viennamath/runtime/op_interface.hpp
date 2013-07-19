#ifndef VIENNAMATH_RUNTIME_OP_INTERFACE_HPP
#define VIENNAMATH_RUNTIME_OP_INTERFACE_HPP

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




#include <vector>
#include <exception>
#include "viennamath/forwards.h"
#include "viennamath/exception.hpp"

/** @file op_interface.hpp
    @brief Defines a common abstract interface for all operations.
*/

namespace viennamath
{


  /** @brief The abstract runtime interface for all operations (unary and binary).
   *
   * @tparam InterfaceType    The expression runtime interface. Usually rt_expression_interface, but extensions are possible.
   */
  template <typename InterfaceType>
  class op_interface
  {
    public:
      typedef typename InterfaceType::numeric_type       numeric_type;

      virtual ~op_interface() {}

      /** @brief The operator returns a pointer to a copy of itself */
      virtual op_interface * clone() const = 0;

      /** @brief Returns an identification string describing the operator, e.g. exp, sin, cos, ... */
      virtual std::string    str() const = 0;

      /** @brief Applys the operator to the provided value. Unary operators must overwrite this function. */
      virtual numeric_type   apply(numeric_type /*value*/) const { throw no_rhs_provided_exception(); }

      /** @brief Applys the operator to the left hand side 'lhs' and the right hand side 'rhs'. Binary operators must overwrite this function. */
      virtual numeric_type   apply(numeric_type /*lhs*/,
                                   numeric_type /*rhs*/) const { throw rhs_provided_for_unary_operation_exception(); }

      /** @brief A boolean flag that specifies whether the operator is an unary operator, i.e. taking one argument only */
      virtual bool                   is_unary() const { return true; }

      //unary diff:
      /** @brief Interface for differentation of a unary expression */
      virtual InterfaceType * diff(const InterfaceType * /*e*/,
                                   const InterfaceType * /*diff_var*/) const { throw no_rhs_provided_exception(); }

      //binary diff:
      /** @brief Interface for differentation of a binary expression */
      virtual InterfaceType * diff(const InterfaceType * /*lhs*/,
                                   const InterfaceType * /*rhs*/,
                                   const InterfaceType * /*diff_var*/) const { throw rhs_provided_for_unary_operation_exception(); }

      //optimization for binary operators:
      /** @brief Interface for the simplification of a binary expression by passing the two operands */
      virtual InterfaceType * simplify(const InterfaceType * /*lhs*/,
                                       const InterfaceType * /*rhs*/) const = 0; //{ throw ex_rhs_provided_for_unary_operator(); }

      //optimization for unary operators:

      /** @brief Returns true if the unary expression can be simplified */
      virtual bool can_simplify() const = 0;
      /** @brief Returns true if the binary expression can be simplified */
      virtual bool can_simplify(const InterfaceType * /*lhs*/,
                                const InterfaceType * /*rhs*/) const { return false; }

      /** @brief Returns true if 'other' is the same operation as the respective object. */
      virtual bool equal(const op_interface * /*other*/) const = 0;
  };




}

#endif
