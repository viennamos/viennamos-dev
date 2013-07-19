#ifndef VIENNAMATH_RUNTIME_BINARY_OPERATORS_HPP
#define VIENNAMATH_RUNTIME_BINARY_OPERATORS_HPP

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
#include <iostream>
#include <math.h>
#include "viennamath/forwards.h"
#include "viennamath/runtime/op_interface.hpp"
#include "viennamath/compiletime/binary_op_tags.hpp"

/** @file binary_operators.hpp
    @brief Defines the binary operation class wrapping all binary operation tags at runtime.
*/

namespace viennamath
{

  //differentiation: (defined in manipulation/detail/binary_operations.hpp)
  template <typename InterfaceType, typename NumericT>
  InterfaceType * diff_impl(const InterfaceType * lhs, op_plus<NumericT>, const InterfaceType * rhs, const InterfaceType * diff_var);

  template <typename InterfaceType, typename NumericT>
  InterfaceType * diff_impl(const InterfaceType * lhs, op_minus<NumericT>, const InterfaceType * rhs, const InterfaceType * diff_var);

  template <typename InterfaceType, typename NumericT>
  InterfaceType * diff_impl(const InterfaceType * lhs, op_mult<NumericT>, const InterfaceType * rhs, const InterfaceType * diff_var);

  template <typename InterfaceType, typename NumericT>
  InterfaceType * diff_impl(const InterfaceType * lhs, op_div<NumericT>, const InterfaceType * rhs, const InterfaceType * diff_var);


  //optimization: (defined in manipulation/detail/binary_operations.hpp)
  template <typename InterfaceType, typename NumericT>
  InterfaceType * simplify_impl(const InterfaceType * lhs, op_plus<NumericT>, const InterfaceType * rhs);

  template <typename InterfaceType, typename NumericT>
  InterfaceType * simplify_impl(const InterfaceType * lhs, op_minus<NumericT>, const InterfaceType * rhs);

  template <typename InterfaceType, typename NumericT>
  InterfaceType * simplify_impl(const InterfaceType * lhs, op_mult<NumericT>, const InterfaceType * rhs);

  template <typename InterfaceType, typename NumericT>
  InterfaceType * simplify_impl(const InterfaceType * lhs, op_div<NumericT>, const InterfaceType * rhs);


  template <typename InterfaceType, typename NumericT>
  bool can_simplify_impl(const InterfaceType * lhs, op_plus<NumericT>, const InterfaceType * rhs);

  template <typename InterfaceType, typename NumericT>
  bool can_simplify_impl(const InterfaceType * lhs, op_minus<NumericT>, const InterfaceType * rhs);

  template <typename InterfaceType, typename NumericT>
  bool can_simplify_impl(const InterfaceType * lhs, op_mult<NumericT>, const InterfaceType * rhs);

  template <typename InterfaceType, typename NumericT>
  bool can_simplify_impl(const InterfaceType * lhs, op_div<NumericT>, const InterfaceType * rhs);


  //////////////////////////////////////// binary operations ////////////////////////////////////////

  /** @brief This is the common wrapper class for binary operations.
   *
   * @tparam BinaryOperation    A tag encoding the binary operation.
   * @tparam InterfaceType    The expression runtime interface used for expression manipulation.
   */
  template <typename BinaryOperation, typename InterfaceType>
  class op_binary : public op_interface<InterfaceType>
  {
    public:
      typedef typename InterfaceType::numeric_type         numeric_type;

      //run time stuff:

      /** @brief Returns a textual representation of the binary operation */
      std::string str() const { return BinaryOperation::str(); }

      /** @brief Returns a copy of the operator. The caller must ensure the proper deletion of the returned pointer. */
      op_interface<InterfaceType> * clone() const { return new op_binary<BinaryOperation, InterfaceType>(); }

      /** @brief Applies the binary operation to the two values */
      numeric_type apply(numeric_type lhs, numeric_type rhs) const { return BinaryOperation::apply(lhs, rhs); }

      /** @brief Returns whether this operation is a unary operation (which is not the case...) */
      bool is_unary() const { return false; }

      /** @brief Differentiates the unary operation acting on 'lhs' and 'rhs' with respect to the variable passed. */
      InterfaceType * diff(const InterfaceType * lhs,
                           const InterfaceType * rhs,
                           const InterfaceType * diff_var) const
      {
        return diff_impl(lhs, BinaryOperation(), rhs, diff_var);
      }

      /** @brief Returns a simplified form of the unary expression (collect constants, remove trivial operations, etc.). The caller is responsible for deleting the pointer returned. */
      InterfaceType * simplify(const InterfaceType * lhs,
                               const InterfaceType * rhs) const
      {
        return simplify_impl(lhs, BinaryOperation(), rhs);
      }

      /** @brief Returns true if (and only if) the underlying unary expression can be simplified (collect constants, remove trivial operations, etc.) */
      bool can_simplify() const { std::cerr << "Warning in op_binary::optimizable(): Call without action" << std::endl; return false; }

      bool can_simplify(const InterfaceType * lhs,
                        const InterfaceType * rhs) const
      {
        return can_simplify_impl(lhs, BinaryOperation(), rhs);
      }

      /** @brief Returns true if the passed 'other' operation equals this operation */
      bool equal(const op_interface<InterfaceType> * other) const
      {
        return (dynamic_cast<const op_binary<BinaryOperation, InterfaceType> *>(other) != NULL);
      }

  };




}

#endif
