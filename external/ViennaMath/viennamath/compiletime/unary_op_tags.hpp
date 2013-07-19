#ifndef VIENNAMATH_COMPILETIME_UNARY_OP_TAGS_HPP
#define VIENNAMATH_COMPILETIME_UNARY_OP_TAGS_HPP

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

/** @file  unary_op_tags.hpp
    @brief Defines the tags for unary operations.
*/

namespace viennamath
{
  //
  // identity
  //
  /** @brief Identity mapping: x |--> x
   *
   * @tparam NumericT     The underlying numeric (floating point) type
   */
  template <typename NumericT>
  struct op_id
  {
    static std::string str() { return "id"; }
    NumericT apply(NumericT value) const { return value; }
    bool can_simplify() const { return true; }
  };

  //
  // exponential
  //
  /** @brief Exponential mapping: x |--> exp(x)
   *
   * @tparam NumericT     The underlying numeric (floating point) type
   */
  template <typename NumericT>
  struct op_exp
  {
    static std::string str() { return "exp"; }
    NumericT apply(NumericT value) const { return ::exp(value); }
    bool can_simplify() const { return true; }
  };

  //
  // sinus
  //
  /** @brief Sine function: x |--> sin(x)
   *
   * @tparam NumericT     The underlying numeric (floating point) type
   */
  template <typename NumericT>
  struct op_sin
  {
    static std::string str() { return "sin"; }
    NumericT apply(NumericT value) const { return ::sin(value); }
    bool can_simplify() const { return true; }
  };

  //
  // cosinus
  //
  /** @brief Cosine function: x |--> cos(x)
   *
   * @tparam NumericT     The underlying numeric (floating point) type
   */
  template <typename NumericT>
  struct op_cos
  {
    static std::string str() { return "cos"; }
    NumericT apply(NumericT value) const { return ::cos(value); }
    bool can_simplify() const { return true; }
  };


  //
  // tangens
  //
  /** @brief Tangent function: x |--> tan(x)
   *
   * @tparam NumericT     The underlying numeric (floating point) type
   */
  template <typename NumericT>
  struct op_tan
  {
    static std::string str() { return "tan"; }
    NumericT apply(NumericT value) const { return ::tan(value); }
    bool can_simplify() const { return true; }
  };

  //
  // absolute value
  //
  /** @brief Modulus: x |--> |x|
   *
   * @tparam NumericT     The underlying numeric (floating point) type
   */
  template <typename NumericT>
  struct op_fabs
  {
    static std::string str() { return "fabs"; }
    NumericT apply(NumericT value) const { return ::fabs(value); }
    bool can_simplify() const { return true; }
  };

  //
  // square root
  //
  /** @brief Square root: x |--> \\sqrt{x}
   *
   * @tparam NumericT     The underlying numeric (floating point) type
   */
  template <typename NumericT>
  struct op_sqrt
  {
    static std::string str() { return "sqrt"; }
    NumericT apply(NumericT value) const { return ::sqrt(value); }
    bool can_simplify() const { return true; }
  };


  //
  // natural logarithm (aka ln())
  //
  /** @brief Natural logarithm function: x |--> ln(x)
   *
   * @tparam NumericT     The underlying numeric (floating point) type
   */
  template <typename NumericT>
  struct op_log //natural logarithm
  {
    static std::string str() { return "log"; }
    NumericT apply(NumericT value) const { return ::log(value); }
    bool can_simplify() const { return true; }
  };

  //
  // logarithm, base 10
  //
  /** @brief Logarithm with base 10: x |--> \\log_{10}(x)
   *
   * @tparam NumericT     The underlying numeric (floating point) type
   */
  template <typename NumericT>
  struct op_log10
  {
    static std::string str() { return "log10"; }
    NumericT apply(NumericT value) const { return ::log10(value); }
    bool can_simplify() const { return true; }
  };

  ////////////     formal stuff //////////////

  //
  // gradient:
  //
  /** @brief Gradient operator: f |--> grad(x)
   *
   * @tparam NumericT     The underlying numeric (floating point) type
   */
  template <typename NumericT>
  struct op_gradient
  {
    static std::string str() { return "grad"; }

    NumericT apply(NumericT /*value*/) const
    {
      throw "Cannot evaluate formal gradient. Use transformations first.";
      return 0;
    }
    bool can_simplify() const { return false; }
  };


  //
  // divergence
  //
  /** @brief Divergence operator: f |--> div(f)
   *
   * @tparam NumericT     The underlying numeric (floating point) type
   */
  template <typename NumericT>
  struct op_divergence
  {
    static std::string str() { return "div"; }

    NumericT apply(NumericT /*value*/) const
    {
      throw "Cannot evaluate formal divergence. Use transformations first.";
      return 0;
    }
    bool can_simplify() const { return false; }
  };


  //
  // symbolic partial derivative with respect to variable<id>:
  //
  /** @brief Divergence operator: f |--> d f / (d x_i)
   *
   * @tparam NumericT     The underlying numeric (floating point) type
   */
  template <typename NumericT>
  class op_partial_deriv
  {
    public:
      op_partial_deriv() : id_(0) {}
      op_partial_deriv(id_type i) : id_(i) {}

      std::string str() const
      {
        std::stringstream ss;
        ss << "partial_deriv<" << id_ << ">";
        return ss.str();
      }

      NumericT apply(NumericT /*value*/) const
      {
        throw "Cannot evaluate formal partial derivative. Use transformations first.";
        return 0;
      }

      bool can_simplify() const { return false; }

      id_type id() const { return id_; }

    private:
      id_type id_;
  };

  //needed for comparison of unary expressions:
  /** @brief Compares two partial derivatives for equality with respect to the differentiation variable */
  template <typename T>
  bool unary_op_equal(op_partial_deriv<T> const & lhs, op_partial_deriv<T> const & rhs) { return lhs.id() == rhs.id(); }



  //
  // integral:
  //
  /** @brief Integration operator: f |--> \\int_I f d x
   *
   * @tparam IntervalType           The integration interval
   * @tparam integration_variable   ID of the integration variable
   */
  template <typename IntervalType, id_type integration_variable>
  class op_ct_integral
  {
      typedef typename IntervalType::numeric_type    NumericT;

    public:
      typedef IntervalType         interval_type;

      static std::string str() { return "symb_int"; }

      NumericT apply(NumericT value) const
      {
        throw "Cannot evaluate symbolic_integration. Use transformations first.";
        return value;
      }

      bool can_simplify() const { return false; }

    private:
      //IntervalType const & interval;

  };

}

#endif

