#ifndef VIENNAMATH_RUNTIME_UNARY_OPERATORS_HPP
#define VIENNAMATH_RUNTIME_UNARY_OPERATORS_HPP

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




#include <string>
#include <vector>
#include <math.h>
#include "viennamath/forwards.h"
#include "viennamath/runtime/op_interface.hpp"
#include "viennamath/compiletime/unary_op_tags.hpp"

/** @file unary_operators.hpp
    @brief Defines the wrapper class for all unary operations.
*/

namespace viennamath
{
  //
  // Various forward-declarations for differentation:
  //

  template <typename InterfaceType, typename NumericT>
  InterfaceType * diff_impl(const InterfaceType * e, op_id<NumericT>, const InterfaceType * diff_var);

  template <typename InterfaceType, typename NumericT>
  InterfaceType * diff_impl(const InterfaceType * e, op_exp<NumericT>, const InterfaceType * diff_var);

  template <typename InterfaceType, typename NumericT>
  InterfaceType * diff_impl(const InterfaceType * e, op_sin<NumericT>, const InterfaceType * diff_var);

  template <typename InterfaceType, typename NumericT>
  InterfaceType * diff_impl(const InterfaceType * e, op_cos<NumericT>, const InterfaceType * diff_var);

  template <typename InterfaceType, typename NumericT>
  InterfaceType * diff_impl(const InterfaceType * e, op_tan<NumericT>,  const InterfaceType * diff_var);

  template <typename InterfaceType, typename NumericT>
  InterfaceType * diff_impl(const InterfaceType * e, op_fabs<NumericT>, const InterfaceType * diff_var);

  template <typename InterfaceType, typename NumericT>
  InterfaceType * diff_impl(const InterfaceType * e, op_sqrt<NumericT>, const InterfaceType * diff_var);

  template <typename InterfaceType, typename NumericT>
  InterfaceType * diff_impl(const InterfaceType * e, op_log<NumericT>, const InterfaceType * diff_var);

  template <typename InterfaceType, typename NumericT>
  InterfaceType * diff_impl(const InterfaceType * e, op_log10<NumericT>, const InterfaceType * diff_var);


  /** @brief Helper function returning true if the two arguments are of the same type. */
  template <typename T>
  bool unary_op_equal(T const & /*lhs*/, T const & /*rhs*/) { return true; }

  //
  // main class for all unary operations:
  //

  /** @brief Main class for all unary operations.
   *
   * @tparam UnaryOperation   Type of the unary operation to be wrapped at runtime.
   * @tparam InterfaceType    The expression runtime interface used for expression manipulation.
   */
  template <typename UnaryOperation, typename InterfaceType>
  class op_unary : public op_interface<InterfaceType>
  {
    public:
      typedef typename InterfaceType::numeric_type    numeric_type;

      op_unary() {}

      op_unary(UnaryOperation const & uo) : unary_op_(uo) {}

      /** @brief Returns the expression tag */
      UnaryOperation const & op() const { return unary_op_; }

      //run time stuff:
      /** @brief Returns a string representation of the operator */
      std::string str() const { return unary_op_.str(); }

      /** @brief Returns a copy of the operator. The caller must ensure the proper deletion of the returned pointer. */
      op_interface<InterfaceType> * clone() const { return new op_unary<UnaryOperation, InterfaceType>(unary_op_); }

      /** @brief Applies the unary operation to the value */
      numeric_type apply(numeric_type value) const { return unary_op_.apply(value); }

      //numeric_type apply(numeric_type lhs, numeric_type rhs) const { return unary_op_.apply(lhs); }

      /** @brief Returns whether this operation is a unary operation (which is the case...) */
      bool is_unary() const { return true; }

      /** @brief Differentiates the unary operation acting on 'e' with respect to the variable passed. */
      InterfaceType * diff(const InterfaceType * e,
                           const InterfaceType * diff_var) const
      {
        return diff_impl(e, unary_op_, diff_var);
      }

      /** @brief Returns a simplified form of the unary expression (collect constants, remove trivial operations, etc.). The caller is responsible for deleting the pointer returned. */
      InterfaceType * simplify(const InterfaceType * lhs,
                               const InterfaceType * rhs) const; // Definition can be found right after the definition if binary_expr.

      /** @brief Returns true if (and only if) the underlying unary expression can be simplified (collect constants, remove trivial operations, etc.) */
      bool can_simplify() const { return unary_op_.can_simplify(); }

      /** @brief Returns true if the passed 'other' operation equals this operation */
      bool equal(const op_interface<InterfaceType> * other) const
      {
        const op_unary<UnaryOperation, InterfaceType> * ptr = dynamic_cast<const op_unary<UnaryOperation, InterfaceType> *>(other);
        if (ptr != NULL)
          return unary_op_equal(ptr->op(), unary_op_); //needed for partial derivatives

        return false;
      }


    private:
      /** @brief Instance of the operation class.
       *
       * We use an UnaryOperation member, because the UnaryOperation tag might have a state -> pure static tag dispatch not enough then */
      UnaryOperation unary_op_;
  };


}

#endif
