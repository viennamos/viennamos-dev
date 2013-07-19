#ifndef VIENNAMATH_MANIPULATION_DIFF_HPP
#define VIENNAMATH_MANIPULATION_DIFF_HPP

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
#include "viennamath/runtime/unary_expr.hpp"
#include "viennamath/manipulation/simplify.hpp"

/** @file diff.hpp
    @brief Provides symbolic differentiation routines (runtime and compiletime).
*/

namespace viennamath
{
  //
  /////////////////// Derivative of runtime expression /////////////////////
  //

  /** @brief Returns the derivative of the provided runtime binary expression
   *
   * @param e        The expression to be differentiated
   * @param var      The differentiation variable
   */
  template <typename InterfaceType>
  rt_expr<InterfaceType> diff(rt_binary_expr<InterfaceType> const & e,
                              rt_variable<InterfaceType> const & var)
  {
    rt_expr<InterfaceType> temp(e.diff(&var));
    inplace_simplify(temp);
    return temp;
  }

  /** @brief Returns the derivative of the provided runtime unary expression
   *
   * @param e        The expression to be differentiated
   * @param var      The differentiation variable
   */
  template <typename InterfaceType>
  rt_expr<InterfaceType> diff(rt_unary_expr<InterfaceType> const & e,
                              rt_variable<InterfaceType> const & var)
  {
    rt_expr<InterfaceType> temp(e.diff(&var));
    inplace_simplify(temp);
    return temp;
  }

  /** @brief Returns the derivative of the provided runtime expression wrapper
   *
   * @param e        The expression to be differentiated
   * @param var      The differentiation variable
   */
  template <typename InterfaceType>
  rt_expr<InterfaceType> diff(rt_expr<InterfaceType> const & e,
                              rt_variable<InterfaceType> const & var)
  {
    rt_expr<InterfaceType> temp(e.get()->diff(&var));
    inplace_simplify(temp);
    return temp;
  }

  //compile time variable:
  /** @brief Returns the derivative of the provided runtime binary expression
   *
   * @param e        The expression to be differentiated
   * @param var      The differentiation variable
   */
  template <typename InterfaceType, id_type id>
  rt_expr<InterfaceType> diff(rt_binary_expr<InterfaceType> const & e,
                              ct_variable<id> const & /*var*/)
  {
    rt_variable<InterfaceType> temp(id);
    return diff(e, temp);
  }

  /** @brief Returns the derivative of the provided runtime unary expression
   *
   * @param e        The expression to be differentiated
   * @param var      The differentiation variable
   */
  template <typename InterfaceType, id_type id>
  rt_expr<InterfaceType> diff(rt_unary_expr<InterfaceType> const & e,
                             ct_variable<id> const & /*var*/)
  {
    rt_variable<InterfaceType> temp(id);
    return diff(e, temp);
  }

  /** @brief Returns the derivative of the provided runtime expression wrapper
   *
   * @param e        The expression to be differentiated
   * @param var      The differentiation variable
   */
  template <typename InterfaceType, id_type id>
  rt_expr<InterfaceType> diff(rt_expr<InterfaceType> const & e,
                              ct_variable<id> const & /*var*/)
  {
    rt_variable<InterfaceType> temp(id);
    return diff(e, temp);
  }


  //
  // Symbolic differentiation:
  //
  /** @brief Returns a symbolic representation of a differentiated function */
  template <typename InterfaceType>
  rt_expr<InterfaceType> diff(rt_function_symbol<InterfaceType> const & other,
                              rt_variable<InterfaceType> const & var)
  {
    typedef op_partial_deriv<typename InterfaceType::numeric_type> d_dx_type;
    return rt_expr<InterfaceType>(new rt_unary_expr<InterfaceType>(other.clone(),
                                                                   new op_unary<d_dx_type, InterfaceType>(d_dx_type(var.id()))
                                                                  )
                                 );
  }

  /** @brief Returns a symbolic representation of a differentiated function */
  template <typename InterfaceType, id_type id>
  rt_expr<InterfaceType> diff(rt_function_symbol<InterfaceType> const & other,
                              ct_variable<id> const & var)
  {
    typedef op_partial_deriv<typename InterfaceType::numeric_type> d_dx_type;
    return rt_expr<InterfaceType>(new rt_unary_expr<InterfaceType>(other.clone(),
                                                                   new op_unary<d_dx_type, InterfaceType>(d_dx_type(id))
                                                                  )
                                 );
  }

  //
  // Gateaux derivative:
  //
  /** @brief dg/df for function symbols f and g */
  template <typename InterfaceType>
  rt_expr<InterfaceType> diff(rt_function_symbol<InterfaceType> const & e,
                              rt_function_symbol<InterfaceType> const & var)
  {
    typedef typename InterfaceType::numeric_type     numeric_type;
    if (e.deep_equal(&var))
      return rt_constant<numeric_type, InterfaceType>(1);
    return rt_constant<numeric_type, InterfaceType>(0);
  }

  /** @brief Returns a symbolic representation of a differentiated function */
  template <typename InterfaceType>
  rt_expr<InterfaceType> diff(rt_unary_expr<InterfaceType> const & e,
                              rt_function_symbol<InterfaceType> const & var)
  {
    rt_expr<InterfaceType> temp(e.diff(&var));
    inplace_simplify(temp);
    return temp;
  }

  /** @brief Returns a symbolic representation of a differentiated function */
  template <typename InterfaceType>
  rt_expr<InterfaceType> diff(rt_binary_expr<InterfaceType> const & e,
                              rt_function_symbol<InterfaceType> const & var)
  {
    rt_expr<InterfaceType> temp(e.diff(&var));
    inplace_simplify(temp);
    return temp;
  }

  /** @brief Returns a symbolic representation of a differentiated function */
  template <typename InterfaceType>
  rt_expr<InterfaceType> diff(rt_expr<InterfaceType> const & e,
                              rt_function_symbol<InterfaceType> const & var)
  {
    rt_expr<InterfaceType> temp(e.get()->diff(&var));
    inplace_simplify(temp);
    return temp;
  }


  //////////// Derivative of a constant: /////////////////////////////////

  /** @brief Overload for the derivative of a function */
  template <typename InterfaceType>
  rt_constant<typename InterfaceType::numeric_type> diff(typename InterfaceType::numeric_type value,
                                                         rt_variable<InterfaceType> const & var)
  {
    return rt_constant<typename InterfaceType::numeric_type>(0);
  }

  /** @brief Overload for the derivative of a ViennaMath runtime constant */
  template <typename OtherScalarType, typename InterfaceType>
  rt_constant<typename InterfaceType::numeric_type> diff(rt_constant<OtherScalarType, InterfaceType> const & c,
                                                         rt_variable<InterfaceType> const & var)
  {
    return rt_constant<typename InterfaceType::numeric_type>(0);
  }

  //////////// derivative of a variable: /////////////////////////////////

  /** @brief Overload for computing the derivative of a runtime variable */
  template <id_type other_id, typename InterfaceType,
            id_type id>
  rt_constant<typename InterfaceType::numeric_type> diff(rt_variable<InterfaceType> const & c,
                                                         rt_variable<InterfaceType> const & var)
  {
    if (c.id() == var.id())
      return rt_constant<typename InterfaceType::numeric_type>(1);

    return rt_constant<typename InterfaceType::numeric_type>(0);
  }


  //
  /////////////////// Derivative of compiletime expressions /////////////////////
  //


  //metafunctions
  namespace result_of
  {
    //differentiates ARG with respect to VAR
    /** @brief Returns the type of the expression after differentiation of 'ARG' with respect to the variable 'VAR'.
     *
     * @tparam ARG     Type of the expression to be differentiated
     * @tparam VAR     Type of the variable
     */
    template <typename ARG, typename VAR>
    struct diff {}; //default case: used for SFINAE with respect to the viennamath::diff() function
    //{
    //  typedef typename ARG::ERROR_INVALID_ARGUMENT_PROVIDED_TO_COMPILE_TIME_DIFFERENTIATION  error_type;
    //};


    /** @brief Specialization implementing the rule (u + v)' = u' + v' */
    template <typename LHS, typename RHS, id_type id>
    struct diff<ct_binary_expr<LHS, op_plus<default_numeric_type>, RHS>,
                ct_variable<id> >
    {
      typedef ct_binary_expr< typename diff<LHS, ct_variable<id> >::result_type,
                              op_plus<default_numeric_type>,
                              typename diff<RHS, ct_variable<id> >::result_type >     result_type;
    };

    /** @brief Specialization implementing the rule (u - v)' = u' - v' */
    template <typename LHS, typename RHS, id_type id>
    struct diff<ct_binary_expr<LHS, op_minus<default_numeric_type>, RHS>,
                ct_variable<id> >
    {
      typedef ct_binary_expr< typename diff<LHS, ct_variable<id> >::result_type,
                              op_minus<default_numeric_type>,
                              typename diff<RHS, ct_variable<id> >::result_type >     result_type;
    };

    /** @brief Specialization implementing the rule (u * v)' = u'*v + u*v' */
    template <typename LHS, typename RHS, id_type id>
    struct diff<ct_binary_expr<LHS, op_mult<default_numeric_type>, RHS>,
                ct_variable<id> >
    {
      typedef ct_binary_expr< ct_binary_expr< typename diff<LHS, ct_variable<id> >::result_type,
                                              op_mult<default_numeric_type>,
                                              RHS>,
                              op_plus<default_numeric_type>,
                              ct_binary_expr< LHS,
                                              op_mult<default_numeric_type>,
                                              typename diff<RHS, ct_variable<id> >::result_type >
                            >                                                      result_type;
    };

    /** @brief Specialization implementing the rule (u/v)' = (u'*v - u*v') / v^2 */
    template <typename LHS, typename RHS, id_type id>
    struct diff<ct_binary_expr<LHS, op_div<default_numeric_type>, RHS>,
                ct_variable<id> >
    {
      typedef ct_binary_expr< ct_binary_expr< ct_binary_expr< typename diff<LHS, ct_variable<id> >::result_type,
                                                              op_mult<default_numeric_type>,
                                                              RHS>,
                                              op_minus<default_numeric_type>,
                                              ct_binary_expr< LHS,
                                                              op_mult<default_numeric_type>,
                                                              typename diff<RHS, ct_variable<id> >::result_type >
                                            >,
                              op_div<default_numeric_type>,
                              ct_binary_expr< RHS,
                                              op_mult<default_numeric_type>,
                                              RHS >
                            >                 result_type;
    };

    /** @brief Specialization of (d x_i) / (d x_j) for i != j  */
    template <id_type other_id,
              id_type id>
    struct diff< ct_variable<other_id>,
                 ct_variable<id> >
    {
      typedef ct_constant<0>    result_type;
    };

    /** @brief Specialization for (d x) / (d x) = 1 */
    template <id_type id>
    struct diff< ct_variable<id>,
                 ct_variable<id> >
    {
      typedef ct_constant<1>    result_type;
    };

    /** @brief Specialization: The derivative of a constant is zero */
    template <long value, id_type id>
    struct diff< ct_constant<value>,
                 ct_variable<id> >
    {
      typedef ct_constant<0>    result_type;
    };

  } // namespace result_of


  //
  // interface functions
  //
  /** @brief The user function for differentiation of a compile time expression with respect to a compile time variable */
  template <typename ExpressionType,
            id_type id>
  typename result_of::diff<ExpressionType,
                           ct_variable<id> >::result_type
  diff(ExpressionType const & /*c*/,
       ct_variable<id> const & /*var*/)
  {
    return typename result_of::diff<ExpressionType,
                                    ct_variable<id> >::result_type();
  }



}

#endif
