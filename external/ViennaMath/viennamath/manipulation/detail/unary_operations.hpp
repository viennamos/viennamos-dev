#ifndef VIENNAMATH_UNARY_OPERATORS_MANIPULATION_HPP
#define VIENNAMATH_UNARY_OPERATORS_MANIPULATION_HPP

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
#include "viennamath/exception.hpp"
#include "viennamath/compiletime/unary_op_tags.hpp"
#include "viennamath/runtime/unary_expr.hpp"
#include "viennamath/runtime/binary_expr.hpp"
#include "viennamath/runtime/integral.hpp"

/** @file  viennamath/manipulation/detail/unary_operations.hpp
    @brief Contains the handling of unary operations (overloads for exp(), etc.)
*/

namespace viennamath
{

  //
  // identity
  //
  /** @brief Differentation of a unary expression containing identity */
  template <typename InterfaceType, typename NumericT>
  InterfaceType * diff_impl(const InterfaceType * e, op_id<NumericT>, const InterfaceType * diff_var)
  {
    return e->diff(diff_var);
  }

  //
  // exponential
  //
  /** @brief Differentation of exp(e), where e is an expression */
  template <typename InterfaceType, typename NumericT>
  InterfaceType * diff_impl(const InterfaceType * e, op_exp<NumericT>, const InterfaceType * diff_var)
  {
    return new rt_binary_expr<InterfaceType>( new rt_unary_expr<InterfaceType>(e->clone(), new op_unary<op_exp<NumericT>, InterfaceType>()),
                                           new op_binary<op_mult<typename InterfaceType::numeric_type>, InterfaceType>(),
                                           e->diff(diff_var));
  }

  /** @brief Overload of the exponential function for a ViennaMath runtime constant */
  template <typename NumericT, typename InterfaceType>
  rt_unary_expr<InterfaceType> exp(rt_constant<NumericT, InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.clone(), new op_unary<op_exp<typename InterfaceType::numeric_type>, InterfaceType>());
  }

  /** @brief Overload of the exponential function for a ViennaMath runtime variable */
  template <typename InterfaceType>
  rt_unary_expr<InterfaceType> exp(rt_variable<InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.clone(), new op_unary<op_exp<typename InterfaceType::numeric_type>, InterfaceType>());
  }

  /** @brief Overload of the exponential function for a ViennaMath compiletime variable */
  template <id_type id>
  rt_unary_expr<> exp(ct_variable<id> const & /*other*/)
  {
    return rt_unary_expr<>(new rt_variable<>(id), new op_unary<op_exp<default_numeric_type> >());
  }

  /** @brief Overload of the exponential function for a ViennaMath runtime binary expression */
  template <typename InterfaceType>
  rt_unary_expr<InterfaceType> exp(rt_binary_expr<InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.clone(), new op_unary<op_exp<typename InterfaceType::numeric_type>, InterfaceType>());
  }

  /** @brief Overload of the exponential function for a ViennaMath compiletime binary expression */
  template <typename LHS, typename OP, typename RHS>
  rt_unary_expr<> exp(ct_binary_expr<LHS, OP, RHS> const & other)
  {
    return rt_unary_expr<>(new rt_binary_expr<>(other),
                        new op_unary<op_exp<default_numeric_type> >());
  }

  /** @brief Overload of the exponential function for a ViennaMath compiletime unary expression */
  template <typename LHS, typename OP>
  rt_unary_expr<> exp(ct_unary_expr<LHS, OP> const & other)
  {
    return rt_unary_expr<>(new rt_unary_expr<>(other),
                           new op_unary<op_exp<default_numeric_type> >());
  }

  /** @brief Overload of the exponential function for a ViennaMath runtime expression wrapper */
  template <typename InterfaceType>
  rt_unary_expr<InterfaceType> exp(rt_expr<InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.get()->clone(), new op_unary<op_exp<typename InterfaceType::numeric_type>, InterfaceType>());
  }


  //
  // sinus
  //

  /** @brief Differentation of sin(e), where e is an expression */
  template <typename InterfaceType, typename NumericT>
  InterfaceType * diff_impl(const InterfaceType * e, op_sin<NumericT>, const InterfaceType * diff_var)
  {
    return new rt_binary_expr<InterfaceType>( new rt_unary_expr<InterfaceType>(e->clone(), new op_unary<op_cos<NumericT>, InterfaceType>()),
                                           new op_binary<op_mult<typename InterfaceType::numeric_type>, InterfaceType>(),
                                           e->diff(diff_var) );
  }

  /** @brief Overload of the sine function for a ViennaMath runtime constant */
  template <typename NumericT, typename InterfaceType>
  rt_unary_expr<InterfaceType> sin(rt_constant<NumericT, InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.clone(), new op_unary<op_sin<typename InterfaceType::numeric_type>, InterfaceType>());
  }

  /** @brief Overload of the sine function for a ViennaMath runtime variable */
  template <typename InterfaceType>
  rt_unary_expr<InterfaceType> sin(rt_variable<InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.clone(), new op_unary<op_sin<typename InterfaceType::numeric_type>, InterfaceType>());
  }

  /** @brief Overload of the sine function for a ViennaMath compiletime variable */
  template <id_type id>
  rt_unary_expr<> sin(ct_variable<id> const & /*other*/)
  {
    return rt_unary_expr<>(new rt_variable<>(id), new op_unary<op_sin<default_numeric_type> >());
  }

  /** @brief Overload of the sine function for a ViennaMath compiletime binary expression */
  template <typename LHS, typename OP, typename RHS>
  rt_unary_expr<> sin(ct_binary_expr<LHS, OP, RHS> const & other)
  {
    return rt_unary_expr<>(new rt_binary_expr<>(other),
                           new op_unary<op_sin<default_numeric_type> >());
  }

  /** @brief Overload of the sine function for a ViennaMath compiletime unary expression */
  template <typename LHS, typename OP>
  rt_unary_expr<> sin(ct_unary_expr<LHS, OP> const & other)
  {
    return rt_unary_expr<>(new rt_unary_expr<>(other),
                           new op_unary<op_sin<default_numeric_type> >());
  }


  /** @brief Overload of the sine function for a ViennaMath runtime unary expression */
  template <typename InterfaceType>
  rt_unary_expr<InterfaceType> sin(rt_unary_expr<InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.clone(), new op_unary<op_sin<typename InterfaceType::numeric_type>, InterfaceType>());
  }

  /** @brief Overload of the sine function for a ViennaMath runtime binary expression */
  template <typename InterfaceType>
  rt_unary_expr<InterfaceType> sin(rt_binary_expr<InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.clone(), new op_unary<op_sin<typename InterfaceType::numeric_type>, InterfaceType>());
  }

  /** @brief Overload of the sine function for a ViennaMath runtime expression wrapper */
  template <typename InterfaceType>
  rt_unary_expr<InterfaceType> sin(rt_expr<InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.get()->clone(), new op_unary<op_sin<typename InterfaceType::numeric_type>, InterfaceType>());
  }



  //
  // cosinus
  //
  /** @brief Differentation of cos(e), where e is an expression */
  template <typename InterfaceType, typename NumericT>
  InterfaceType * diff_impl(const InterfaceType * e, op_cos<NumericT>, const InterfaceType * diff_var)
  {
    return new rt_binary_expr<InterfaceType>( new rt_unary_expr<InterfaceType>(e->clone(), new op_unary<op_sin<NumericT>, InterfaceType>()),
                                           new op_binary<op_mult<typename InterfaceType::numeric_type>, InterfaceType>(),
                                           new rt_binary_expr<InterfaceType>( new rt_constant<NumericT, InterfaceType>(-1),
                                                                              new op_binary<op_mult<typename InterfaceType::numeric_type>, InterfaceType>(),
                                                                              e->diff(diff_var) )
                                          );
  }

  /** @brief Overload of the cosine function for a ViennaMath runtime constant */
  template <typename NumericT, typename InterfaceType>
  rt_unary_expr<InterfaceType> cos(rt_constant<NumericT, InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.clone(), new op_unary<op_cos<typename InterfaceType::numeric_type>, InterfaceType>());
  }

  /** @brief Overload of the cosine function for a ViennaMath runtime variable */
  template <typename InterfaceType>
  rt_unary_expr<InterfaceType> cos(rt_variable<InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.clone(), new op_unary<op_cos<typename InterfaceType::numeric_type>, InterfaceType>());
  }

  /** @brief Overload of the cosine function for a ViennaMath compiletime variable */
  template <id_type id>
  rt_unary_expr<> cos(ct_variable<id> const & /*other*/)
  {
    return rt_unary_expr<>(new rt_variable<>(id), new op_unary<op_cos<default_numeric_type> >());
  }

  /** @brief Overload of the cosine function for a ViennaMath compiletime binary expression */
  template <typename LHS, typename OP, typename RHS>
  rt_unary_expr<> cos(ct_binary_expr<LHS, OP, RHS> const & other)
  {
    return rt_unary_expr<>(new rt_binary_expr<>(other),
                        new op_unary<op_cos<default_numeric_type> >());
  }

  /** @brief Overload of the cosine function for a ViennaMath compiletime unary expression */
  template <typename LHS, typename OP>
  rt_unary_expr<> cos(ct_unary_expr<LHS, OP> const & other)
  {
    return rt_unary_expr<>(new rt_unary_expr<>(other),
                           new op_unary<op_cos<default_numeric_type> >());
  }


  /** @brief Overload of the cosine function for a ViennaMath runtime unary expression */
  template <typename InterfaceType>
  rt_unary_expr<InterfaceType> cos(rt_unary_expr<InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.clone(), new op_unary<op_cos<typename InterfaceType::numeric_type>, InterfaceType>());
  }

  /** @brief Overload of the cosine function for a ViennaMath runtime binary expression */
  template <typename InterfaceType>
  rt_unary_expr<InterfaceType> cos(rt_binary_expr<InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.clone(), new op_unary<op_cos<typename InterfaceType::numeric_type>, InterfaceType>());
  }

  /** @brief Overload of the cosine function for a ViennaMath runtime expression wrapper */
  template <typename InterfaceType>
  rt_unary_expr<InterfaceType> cos(rt_expr<InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.get()->clone(), new op_unary<op_cos<typename InterfaceType::numeric_type>, InterfaceType>());
  }

  //
  // tangens
  //
  /** @brief Differentation of tan(e), where e is an expression */
  template <typename InterfaceType, typename NumericT>
  InterfaceType * diff_impl(const InterfaceType * e, op_tan<NumericT>, const InterfaceType * diff_var)
  {
    // 1/cos^2(e):
    return new rt_binary_expr<InterfaceType>( e->diff(diff_var),
                                              new op_binary<op_div<typename InterfaceType::numeric_type>, InterfaceType>(),
                                              new rt_binary_expr<InterfaceType>( new rt_unary_expr<InterfaceType>(e->clone(),
                                                                                                                  new op_unary<op_cos<NumericT>, InterfaceType>()),
                                                                                 new op_binary<op_mult<typename InterfaceType::numeric_type>, InterfaceType>(),
                                                                                 new rt_unary_expr<InterfaceType>(e->clone(),
                                                                                                                  new op_unary<op_cos<NumericT>, InterfaceType>()) )
                                            );
  }

  /** @brief Overload of the tangent function for a ViennaMath runtime constant */
  template <typename NumericT, typename InterfaceType>
  rt_unary_expr<InterfaceType> tan(rt_constant<NumericT, InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.clone(), new op_unary<op_tan<typename InterfaceType::numeric_type>, InterfaceType>());
  }

  /** @brief Overload of the tangent function for a ViennaMath runtime variable */
  template <typename InterfaceType>
  rt_unary_expr<InterfaceType> tan(rt_variable<InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.clone(), new op_unary<op_tan<typename InterfaceType::numeric_type>, InterfaceType>());
  }

  /** @brief Overload of the tangent function for a ViennaMath compiletime variable */
  template <id_type id>
  rt_unary_expr<> tan(ct_variable<id> const & /*other*/)
  {
    return rt_unary_expr<>(new rt_variable<>(id), new op_unary<op_tan<default_numeric_type> >());
  }

  /** @brief Overload of the tangent function for a ViennaMath compiletime binary expression */
  template <typename LHS, typename OP, typename RHS>
  rt_unary_expr<> tan(ct_binary_expr<LHS, OP, RHS> const & other)
  {
    return rt_unary_expr<>(new rt_binary_expr<>(other),
                           new op_unary<op_tan<default_numeric_type> >());
  }

  /** @brief Overload of the tangent function for a ViennaMath compiletime unary expression */
  template <typename LHS, typename OP>
  rt_unary_expr<> tan(ct_unary_expr<LHS, OP> const & other)
  {
    return rt_unary_expr<>(new rt_unary_expr<>(other),
                           new op_unary<op_tan<default_numeric_type> >());
  }

  /** @brief Overload of the tangent function for a ViennaMath runtime unary expression */
  template <typename InterfaceType>
  rt_unary_expr<InterfaceType> tan(rt_unary_expr<InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.clone(), new op_unary<op_tan<typename InterfaceType::numeric_type>, InterfaceType>());
  }

  /** @brief Overload of the tangent function for a ViennaMath runtime binary expression */
  template <typename InterfaceType>
  rt_unary_expr<InterfaceType> tan(rt_binary_expr<InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.clone(), new op_unary<op_tan<typename InterfaceType::numeric_type>, InterfaceType>());
  }

  /** @brief Overload of the tangent function for a ViennaMath runtime expression wrapper */
  template <typename InterfaceType>
  rt_unary_expr<InterfaceType> tan(rt_expr<InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.get()->clone(), new op_unary<op_tan<typename InterfaceType::numeric_type>, InterfaceType>());
  }


  //
  // absolute value
  //
  /** @brief Implementation of the differentation of |e|, where e is an expression. Since the modulus is not differentiable everywhere, an exception is thrown. */
  template <typename InterfaceType, typename NumericT>
  InterfaceType * diff_impl(const InterfaceType * /*e*/, op_fabs<NumericT>, const InterfaceType * /*diff_var*/)
  {
    throw expression_not_differentiable_exception("modulus operation not differentiable!");
    return NULL;  //TODO: Think about returning a piecewise function here?
  }

  /** @brief Overload of the modulus function for a ViennaMath runtime constant */
  template <typename NumericT, typename InterfaceType>
  rt_unary_expr<InterfaceType> fabs(rt_constant<NumericT, InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.clone(), new op_unary<op_fabs<typename InterfaceType::numeric_type>, InterfaceType>());
  }

  /** @brief Overload of the modulus function for a ViennaMath runtime variable */
  template <typename InterfaceType>
  rt_unary_expr<InterfaceType> fabs(rt_variable<InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.clone(), new op_unary<op_fabs<typename InterfaceType::numeric_type>, InterfaceType>());
  }

  /** @brief Overload of the modulus function for a ViennaMath compiletime variable */
  template <id_type id>
  rt_unary_expr<> fabs(ct_variable<id> const & other)
  {
    return rt_unary_expr<>(new rt_variable<>(id), new op_unary<op_fabs<default_numeric_type> >());
  }

  /** @brief Overload of the modulus function for a ViennaMath compiletime binary expression */
  template <typename LHS, typename OP, typename RHS>
  rt_unary_expr<> fabs(ct_binary_expr<LHS, OP, RHS> const & other)
  {
    return rt_unary_expr<>(new rt_binary_expr<>(other),
                           new op_unary<op_fabs<default_numeric_type> >());
  }

  /** @brief Overload of the modulus function for a ViennaMath compiletime unary expression */
  template <typename LHS, typename OP>
  rt_unary_expr<> fabs(ct_unary_expr<LHS, OP> const & other)
  {
    return rt_unary_expr<>(new rt_unary_expr<>(other),
                           new op_unary<op_fabs<default_numeric_type> >());
  }


  /** @brief Overload of the modulus function for a ViennaMath runtime unary expression */
  template <typename InterfaceType>
  rt_unary_expr<InterfaceType> fabs(rt_unary_expr<InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.clone(), new op_unary<op_fabs<typename InterfaceType::numeric_type>, InterfaceType>());
  }

  /** @brief Overload of the modulus function for a ViennaMath runtime binary expression */
  template <typename InterfaceType>
  rt_unary_expr<InterfaceType> fabs(rt_binary_expr<InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.clone(), new op_unary<op_fabs<typename InterfaceType::numeric_type>, InterfaceType>());
  }

  /** @brief Overload of the modulus function for a ViennaMath runtime expression wrapper */
  template <typename InterfaceType>
  rt_unary_expr<InterfaceType> fabs(rt_expr<InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.get()->clone(), new op_unary<op_fabs<typename InterfaceType::numeric_type>, InterfaceType>());
  }


  //
  // square root
  //
  /** @brief Implementation of the differentation of \\sqrt{e}, where e is an expression. */
  template <typename InterfaceType, typename NumericT>
  InterfaceType * diff_impl(const InterfaceType * e, op_sqrt<NumericT>, const InterfaceType * diff_var)
  {
      return new rt_binary_expr<InterfaceType>( e->diff(diff_var),
                                             new op_binary<op_div<typename InterfaceType::numeric_type>, InterfaceType>(),
                                             new rt_binary_expr<InterfaceType>( new rt_constant<NumericT>(2),
                                                                             new op_binary<op_mult<typename InterfaceType::numeric_type>, InterfaceType>(),
                                                                             new rt_unary_expr<InterfaceType>(e->clone(),
                                                                                                           new op_unary<op_sqrt<NumericT>, InterfaceType>())
                                                                           )
                                            );
  }

  /** @brief Overload of the square-root function for a ViennaMath runtime constant */
  template <typename NumericT, typename InterfaceType>
  rt_unary_expr<InterfaceType> sqrt(rt_constant<NumericT, InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.clone(), new op_unary<op_sqrt<typename InterfaceType::numeric_type>, InterfaceType>());
  }

  /** @brief Overload of the square-root function for a ViennaMath runtime variable */
  template <typename InterfaceType>
  rt_unary_expr<InterfaceType> sqrt(rt_variable<InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.clone(), new op_unary<op_sqrt<typename InterfaceType::numeric_type>, InterfaceType>());
  }

  /** @brief Overload of the square-root function for a ViennaMath compiletime variable */
  template <id_type id>
  rt_unary_expr<> sqrt(ct_variable<id> const & /*other*/)
  {
    return rt_unary_expr<>(new rt_variable<>(id), new op_unary<op_sqrt<default_numeric_type> >());
  }

  /** @brief Overload of the square-root function for a ViennaMath compiletime binary expression */
  template <typename LHS, typename OP, typename RHS>
  rt_unary_expr<> sqrt(ct_binary_expr<LHS, OP, RHS> const & other)
  {
    return rt_unary_expr<>(new rt_binary_expr<>(other),
                           new op_unary<op_sqrt<default_numeric_type> >());
  }

  /** @brief Overload of the square-root function for a ViennaMath compiletime unary expression */
  template <typename LHS, typename OP>
  rt_unary_expr<> sqrt(ct_unary_expr<LHS, OP> const & other)
  {
    return rt_unary_expr<>(new rt_unary_expr<>(other),
                           new op_unary<op_sqrt<default_numeric_type> >());
  }


  /** @brief Overload of the square-root function for a ViennaMath runtime unary expression */
  template <typename InterfaceType>
  rt_unary_expr<InterfaceType> sqrt(rt_unary_expr<InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.clone(), new op_unary<op_sqrt<typename InterfaceType::numeric_type>, InterfaceType>());
  }

  /** @brief Overload of the square-root function for a ViennaMath runtime binary expression */
  template <typename InterfaceType>
  rt_unary_expr<InterfaceType> sqrt(rt_binary_expr<InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.clone(), new op_unary<op_sqrt<typename InterfaceType::numeric_type>, InterfaceType>());
  }

  /** @brief Overload of the square-root function for a ViennaMath runtime expression wrapper */
  template <typename InterfaceType>
  rt_unary_expr<InterfaceType> sqrt(rt_expr<InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.get()->clone(), new op_unary<op_sqrt<typename InterfaceType::numeric_type>, InterfaceType>());
  }

  //
  // natural logarithm (aka ln())
  //
  /** @brief Implementation of the differentation of ln(e), where e is an expression. */
  template <typename InterfaceType, typename NumericT>
  InterfaceType * diff_impl(const InterfaceType * e, op_log<NumericT>, const InterfaceType * diff_var)
  {
    return new rt_binary_expr<InterfaceType>( e->diff(diff_var),
                                           new op_binary<op_div<typename InterfaceType::numeric_type>, InterfaceType>(),
                                           new rt_unary_expr<InterfaceType>(e->clone())
                                         );
  }

  /** @brief Overload of the natural logarithm function for a ViennaMath runtime constant */
  template <typename NumericT, typename InterfaceType>
  rt_unary_expr<InterfaceType> log(rt_constant<NumericT, InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.clone(), new op_unary<op_log<typename InterfaceType::numeric_type>, InterfaceType>());
  }

  /** @brief Overload of the natural logarithm function for a ViennaMath runtime variable */
  template <typename InterfaceType>
  rt_unary_expr<InterfaceType> log(rt_variable<InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.clone(), new op_unary<op_log<typename InterfaceType::numeric_type>, InterfaceType>());
  }

  /** @brief Overload of the natural logarithm function for a ViennaMath compiletime variable */
  template <id_type id>
  rt_unary_expr<> log(ct_variable<id> const & /*other*/)
  {
    return rt_unary_expr<>(new rt_variable<>(id), new op_unary<op_log<default_numeric_type> >());
  }

  /** @brief Overload of the natural logarithm function for a ViennaMath compiletime binary expression */
  template <typename LHS, typename OP, typename RHS>
  rt_unary_expr<> log(ct_binary_expr<LHS, OP, RHS> const & other)
  {
    return rt_unary_expr<>(new rt_binary_expr<>(other),
                           new op_unary<op_log<default_numeric_type> >());
  }

  /** @brief Overload of the natural logarithm function for a ViennaMath compiletime unary expression */
  template <typename LHS, typename OP>
  rt_unary_expr<> log(ct_unary_expr<LHS, OP> const & other)
  {
    return rt_unary_expr<>(new rt_unary_expr<>(other),
                           new op_unary<op_log<default_numeric_type> >());
  }

  /** @brief Overload of the natural logarithm function for a ViennaMath runtime unary expression */
  template <typename InterfaceType>
  rt_unary_expr<InterfaceType> log(rt_unary_expr<InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.clone(), new op_unary<op_log<typename InterfaceType::numeric_type>, InterfaceType>());
  }

  /** @brief Overload of the natural logarithm function for a ViennaMath runtime binary expression */
  template <typename InterfaceType>
  rt_unary_expr<InterfaceType> log(rt_binary_expr<InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.clone(), new op_unary<op_log<typename InterfaceType::numeric_type>, InterfaceType>());
  }

  /** @brief Overload of the natural logarithm function for a ViennaMath runtime expression wrapper */
  template <typename InterfaceType>
  rt_unary_expr<InterfaceType> log(rt_expr<InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.get()->clone(), new op_unary<op_log<typename InterfaceType::numeric_type>, InterfaceType>());
  }


  //
  // logarithm, base 10
  //

  //
  /** @brief Implementation of the differentation of log10(e), where e is an expression.
   *
   * Uses the rule (log10(f))' = f' / (ln(f) * ln(10))
   */
  template <typename InterfaceType, typename NumericT>
  InterfaceType * diff_impl(const InterfaceType * e, op_log10<NumericT>, const InterfaceType * diff_var)
  {
    return new rt_binary_expr<InterfaceType>( e->diff(diff_var),
                                           new op_binary<op_div<typename InterfaceType::numeric_type>, InterfaceType>(),
                                           new rt_binary_expr<InterfaceType>( new rt_constant<NumericT, InterfaceType>( ::log(10.0) ),
                                                                           new op_binary<op_mult<typename InterfaceType::numeric_type>, InterfaceType>(),
                                                                           new rt_unary_expr<InterfaceType>(e->clone())
                                                                         )
                                          );
  }

  /** @brief Overload of the logarithm with base 10 for a ViennaMath runtime constant */
  template <typename NumericT, typename InterfaceType>
  rt_unary_expr<InterfaceType> log10(rt_constant<NumericT, InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.clone(), new op_unary<op_log10<typename InterfaceType::numeric_type>, InterfaceType>());
  }

  /** @brief Overload of the logarithm with base 10 for a ViennaMath runtime variable */
  template <typename InterfaceType>
  rt_unary_expr<InterfaceType> log10(rt_variable<InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.clone(), new op_unary<op_log10<typename InterfaceType::numeric_type>, InterfaceType>());
  }

  /** @brief Overload of the logarithm with base 10 for a ViennaMath compiletime variable */
  template <id_type id>
  rt_unary_expr<> log10(ct_variable<id> const & /*other*/)
  {
    return rt_unary_expr<>(new rt_variable<>(id), new op_unary<op_log10<default_numeric_type> >());
  }

  /** @brief Overload of the logarithm with base 10 for a ViennaMath compiletime binary expression */
  template <typename LHS, typename OP, typename RHS>
  rt_unary_expr<> log10(ct_binary_expr<LHS, OP, RHS> const & other)
  {
    return rt_unary_expr<>(new rt_binary_expr<>(other),
                           new op_unary<op_log10<default_numeric_type> >());
  }

  /** @brief Overload of the logarithm with base 10 for a ViennaMath compiletime unary expression */
  template <typename LHS, typename OP>
  rt_unary_expr<> log10(ct_unary_expr<LHS, OP> const & other)
  {
    return rt_unary_expr<>(new rt_unary_expr<>(other),
                           new op_unary<op_log10<default_numeric_type> >());
  }

  /** @brief Overload of the logarithm with base 10 for a ViennaMath runtime unary expression */
  template <typename InterfaceType>
  rt_unary_expr<InterfaceType> log10(rt_unary_expr<InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.clone(), new op_unary<op_log10<typename InterfaceType::numeric_type>, InterfaceType>());
  }

  /** @brief Overload of the logarithm with base 10 for a ViennaMath runtime binary expression */
  template <typename InterfaceType>
  rt_unary_expr<InterfaceType> log10(rt_binary_expr<InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.clone(), new op_unary<op_log10<typename InterfaceType::numeric_type>, InterfaceType>());
  }

  /** @brief Overload of the logarithm with base 10 for a ViennaMath runtime expression wrapper */
  template <typename InterfaceType>
  rt_unary_expr<InterfaceType> log10(rt_expr<InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.get()->clone(), new op_unary<op_log10<typename InterfaceType::numeric_type>, InterfaceType>());
  }


  ///////////////////  formal (symbolic) stuff ////////////////////////////////

  //
  // gradient
  //
  /** @brief Implementation of the differentation of a gradient. Only valid for Gateau derivative */
  template <typename InterfaceType, typename NumericT>
  InterfaceType * diff_impl(const InterfaceType * e, op_gradient<NumericT>, const InterfaceType * diff_var)
  {
    typedef rt_function_symbol<InterfaceType>  FunctionSymbolType;
    const FunctionSymbolType * ptr = dynamic_cast< const FunctionSymbolType *>(diff_var);
    if (ptr != NULL)
    {
      InterfaceType * c0 = new rt_constant<NumericT, InterfaceType>(0);
      InterfaceType * new_expr = e->diff(diff_var);

      if (new_expr->deep_equal(c0)) // grad(0) is directly compressed to 0
      {
        delete new_expr;
        return c0;
      }

      // return grad(...)
      delete c0;
      return new rt_unary_expr<InterfaceType>(e->diff(diff_var), new op_unary<op_gradient<NumericT>, InterfaceType>());
    }

    throw expression_not_differentiable_exception("Cannot differentiate gradient operator!");
    return NULL;
  }

  /** @brief Overload of the gradient function for a ViennaMath runtime function symbol */
  template <typename InterfaceType>
  rt_unary_expr<InterfaceType> grad(rt_function_symbol<InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.clone(), new op_unary<op_gradient<typename InterfaceType::numeric_type>, InterfaceType>());
  }

  /** @brief Overload of the gradient function for a ViennaMath runtime unary expression */
  template <typename InterfaceType>
  rt_unary_expr<InterfaceType> grad(rt_unary_expr<InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.clone(), new op_unary<op_gradient<typename InterfaceType::numeric_type>, InterfaceType>());
  }


  //
  // divergence
  //
  /** @brief Implementation of the differentation of the divergence operator. This is currently not supported, thus an exception is thrown. */
  template <typename InterfaceType, typename NumericT>
  InterfaceType * diff_impl(const InterfaceType * /*e*/, op_divergence<NumericT>, const InterfaceType * /*diff_var*/)
  {
    throw expression_not_differentiable_exception("Cannot differentiate divergence operator!");
    return NULL;
  }

  /** @brief Overload of the divergence function for a ViennaMath runtime expression wrapper */
  template <typename InterfaceType>
  rt_unary_expr<InterfaceType> div(rt_expr<InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.get()->clone(), new op_unary<op_divergence<typename InterfaceType::numeric_type>, InterfaceType>());
  }

  /** @brief Overload of the divergence function for a ViennaMath runtime unary expression */
  template <typename InterfaceType>
  rt_unary_expr<InterfaceType> div(rt_unary_expr<InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.clone(), new op_unary<op_divergence<typename InterfaceType::numeric_type>, InterfaceType>());
  }

  /** @brief Overload of the divergence function for a ViennaMath runtime binary expression */
  template <typename InterfaceType>
  rt_unary_expr<InterfaceType> div(rt_binary_expr<InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.clone(), new op_unary<op_divergence<typename InterfaceType::numeric_type>, InterfaceType>());
  }

  /** @brief Overload of the divergence function for a ViennaMath runtime function symbol */
  template <typename InterfaceType>
  rt_unary_expr<InterfaceType> div(rt_function_symbol<InterfaceType> const & other)
  {
    return rt_unary_expr<InterfaceType>(other.clone(), new op_unary<op_divergence<typename InterfaceType::numeric_type>, InterfaceType>());
  }



  //
  // Convenience Function: Laplace operator
  //
  /** @brief For convenience, the Laplace operator (divergence of the gradient) is provided as a separate function */
  template <typename InterfaceType>
  rt_unary_expr<InterfaceType> laplace(rt_expr<InterfaceType> const & other)
  {
    return div(grad(other));
  }

  /** @brief For convenience, the Laplace operator (divergence of the gradient) is provided as a separate function */
  template <typename InterfaceType>
  rt_unary_expr<InterfaceType> laplace(rt_function_symbol<InterfaceType> const & other)
  {
    return div(grad(other));
  }


  //
  // partial derivative with respect to variable<id>:
  //
  /** @brief Implementation of the differentation of a partial derivative. This is currently not supported, thus an exception is thrown. */
  template <typename InterfaceType, typename NumericT>
  InterfaceType * diff_impl(const InterfaceType * /*e*/, op_partial_deriv<NumericT>, const InterfaceType * /*diff_var*/)
  {
    throw expression_not_differentiable_exception("Cannot evaluate formal partial derivative. Use transformations first.");
    return NULL;
  }

  //
  // integral:
  //
  /** @brief Implementation of the differentation of an integral. This is currently not supported, thus an exception is thrown. */
  template <typename InterfaceType>
  InterfaceType * diff_impl(const InterfaceType * /*e*/, op_rt_integral<InterfaceType>, const InterfaceType * /*diff_var*/)
  {
    throw expression_not_differentiable_exception("Cannot differentiate runtime integration (yet)!");
    return NULL;
  }

  /** @brief Implementation of the differentation of an integral with symbolic integration domain. This is currently not supported, thus an exception is thrown. */
  template <typename InterfaceType>
  InterfaceType * diff_impl(const InterfaceType * /*e*/, op_rt_symbolic_integral<InterfaceType>, const InterfaceType * /*diff_var*/)
  {
    throw expression_not_differentiable_exception("Cannot differentiate runtime integration (yet)!");
    return NULL;
  }

}

#endif
