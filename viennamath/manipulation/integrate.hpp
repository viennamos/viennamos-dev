#ifndef VIENNAMATH_MANIPULATION_INTEGRATE_HPP
#define VIENNAMATH_MANIPULATION_INTEGRATE_HPP

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
#include "viennamath/compiletime/ct_interval.hpp"
#include "viennamath/manipulation/expand.hpp"
#include "viennamath/manipulation/simplify.hpp"
#include "viennamath/manipulation/substitute.hpp"

/** @file   integrate.hpp
    @brief  Provides symbolic integration routines. No numerical quadrature is defined here -> check out viennamath/runtime/numerical_quadrature.hpp
*/

namespace viennamath
{
  //
  //////////// compile time ///////////////////
  //

  namespace result_of
  {
    /** @brief Returns the power of ct_variable<id> inside an expression. */
    template <typename VariableType, typename ExpressionType>
    struct polynomial_degree
    {
      enum { value = 0 };
    };

    /** @brief Specialization for the power of a single variable, which is known to be 1. */
    template <typename VariableType>
    struct polynomial_degree <VariableType, VariableType>
    {
      enum { value = 1 };
    };

    /** @brief Specialization for forcing a compile time error if a sum of expressions is encountered. */
    template <typename VariableType,
              typename LHS, typename NumericT, typename RHS>
    struct polynomial_degree <VariableType, ct_binary_expr<LHS, op_plus<NumericT>, RHS> >
    {
      typedef typename ct_binary_expr<LHS, op_plus<NumericT>, RHS>::ERROR_CANNOT_DEDUCE_POLYNOMIAL_DEGREE_FROM_SUM    error_type;
    };

    /** @brief Specialization for forcing a compile time error if a difference of expressions is encountered. */
    template <typename VariableType,
              typename LHS, typename NumericT, typename RHS>
    struct polynomial_degree <VariableType, ct_binary_expr<LHS, op_minus<NumericT>, RHS> >
    {
      typedef typename ct_binary_expr<LHS, op_plus<NumericT>, RHS>::ERROR_CANNOT_DEDUCE_POLYNOMIAL_DEGREE_FROM_SUM    error_type;
    };

    /** @brief Specialization for a product: The polynomial degree is given by the sum of the polynomial degree of each factor. */
    template <typename VariableType,
              typename LHS, typename NumericT, typename RHS>
    struct polynomial_degree <VariableType, ct_binary_expr<LHS, op_mult<NumericT>, RHS> >
    {
      enum { value = polynomial_degree<VariableType, LHS>::value + polynomial_degree<VariableType, RHS>::value };
    };

    /** @brief Specialization for a division: The polynomial degree is given by the difference of the polynomial degree of the numerator and the denominator. */
    template <typename VariableType,
              typename LHS, typename NumericT, typename RHS>
    struct polynomial_degree <VariableType, ct_binary_expr<LHS, op_div<NumericT>, RHS> >
    {
      enum { value = polynomial_degree<VariableType, LHS>::value - polynomial_degree<VariableType, RHS>::value };
    };


    // compute power of an expression
    /** @brief Metafunction returning the power of an expression. Tries to balance the generated expression tree. */
    template <typename T, long exponent>
    struct pow
    {
      // balance generated binary expression equally between LHS and RHS:
      typedef ct_binary_expr< typename pow<T, exponent / 2>::type,
                              op_mult<default_numeric_type>,
                              typename pow<T, exponent - exponent / 2>::type>  type;
    };

    /** @brief Specialization for the zeroth power of an expression */
    template <typename T>
    struct pow <T, 0>
    {
      typedef ct_constant<1>  type;
    };

    /** @brief Specialization for the first power of an expression */
    template <typename T>
    struct pow <T, 1>
    {
      typedef T  type;
    };

    /** @brief Specialization for the square of an expression */
    template <typename T>
    struct pow <T, 2>
    {
      typedef ct_binary_expr<T, op_mult<default_numeric_type>, T>  type;
    };



    //integrate monomial:
    /** @brief Metafunction for the integration of a monomial consisting of a single variable. */
    template <typename VariableType,
              long exponent>
    struct integrate_monomial
    {
      typedef ct_binary_expr< typename pow<VariableType, exponent + 1>::type,
                              op_div<default_numeric_type>,
                              ct_constant<exponent + 1>
                            >         type;
    };

    /** @brief Specialization for the integration of a constant: Return the variable directly */
    template <typename VariableType>
    struct integrate_monomial < VariableType, 0>
    {
      typedef VariableType      type;
    };


    ///////

    /** @brief The main metafunction for the integration of a compiletime expression.
     *
     * Note: Uses SFINAE for the default implementation in order to make the interface function integrate() visible to compile time expressions only.
     *
     * @tparam   LowerBound        Lower bound of the integration domain
     * @tparam   UpperBound        Upper bound of the integration domain
     * @tparam   IntegrandType     Type of the integrand
     * @tparam   VariableType      The integration variable
     */
    template <typename LowerBound, typename UpperBound,
              typename IntegrandType,
              typename VariableType>
    struct integrate {}; //SFINAE for interface function viennamath::integrate()


    namespace detail
    {
      /** @brief The worker metafunction for compile time integration. Each specialization represents an integration rule. */
      template <typename LowerBound, typename UpperBound,
                typename IntegrandType,
                typename VariableType>
      struct integrate_impl {};



      template <typename LowerBound, typename UpperBound,
                typename LHS, typename NumericT, typename RHS,
                typename VariableType>
      struct integrate_impl <LowerBound, UpperBound,
                            ct_binary_expr<LHS, op_plus<NumericT>, RHS>,
                            VariableType
                            >
      {
        typedef ct_binary_expr< typename integrate_impl<LowerBound, UpperBound, LHS, VariableType>::type,
                                op_plus<NumericT>,
                                typename integrate_impl<LowerBound, UpperBound, RHS, VariableType>::type
                              >                   integrated_type;

        typedef typename viennamath::result_of::simplify<integrated_type>::type    type;
      };

      template <typename LowerBound, typename UpperBound,
                typename LHS, typename NumericT, typename RHS,
                typename VariableType>
      struct integrate_impl <LowerBound, UpperBound,
                            ct_binary_expr<LHS, op_minus<NumericT>, RHS>,
                            VariableType
                            >
      {
        typedef ct_binary_expr< typename integrate_impl<LowerBound, UpperBound, LHS, VariableType>::type,
                                op_minus<NumericT>,
                                typename integrate_impl<LowerBound, UpperBound, RHS, VariableType>::type
                              >                   integrated_type;

        typedef typename viennamath::result_of::simplify<integrated_type>::type    type;
      };


      template <typename LowerBound, typename UpperBound,
                typename LHS, typename NumericT, typename RHS,
                typename VariableType>
      struct integrate_impl <LowerBound, UpperBound,
                            ct_binary_expr<LHS, op_mult<NumericT>, RHS>,
                            VariableType
                            >
      {
        //extract coefficient:
        typedef typename viennamath::result_of::substitute< VariableType,
                                                            ct_constant<1>,
                                                            ct_binary_expr<LHS, op_mult<NumericT>, RHS> >::type    coefficient_type;

        typedef typename viennamath::result_of::simplify<coefficient_type>::type    simplified_coefficient_type;

        enum { exponent = polynomial_degree<VariableType,
                                            ct_binary_expr<LHS, op_mult<NumericT>, RHS>
                                          >::value  };

        typedef ct_binary_expr< typename integrate_monomial<UpperBound, exponent>::type,
                                op_mult<NumericT>,
                                simplified_coefficient_type
                              >                    upper_contribution;

        typedef ct_binary_expr< typename integrate_monomial<LowerBound, exponent>::type,
                                op_mult<NumericT>,
                                simplified_coefficient_type
                              >                    lower_contribution;

        typedef ct_binary_expr< typename simplify<upper_contribution>::type,
                                op_minus<NumericT>,
                                typename simplify<lower_contribution>::type
                              >            integrated_type;

        typedef typename viennamath::result_of::simplify<integrated_type>::type    type;
      };

      template <typename LowerBound, typename UpperBound,
                typename LHS, typename NumericT, typename RHS,
                typename VariableType>
      struct integrate_impl <LowerBound, UpperBound,
                            ct_binary_expr<LHS, op_div<NumericT>, RHS>,
                            VariableType
                            >
      {
        //assume polynomial, thus integrate numerator only:
        typedef ct_binary_expr< typename integrate_impl<LowerBound, UpperBound,
                                                        LHS,
                                                        VariableType>::type,
                                op_div<NumericT>,
                                RHS>                     integrated_type;

        typedef typename viennamath::result_of::simplify<integrated_type>::type    type;
      };

      template <typename LowerBound, typename UpperBound,
                typename LHS, typename OP,
                typename VariableType>
      struct integrate_impl <LowerBound, UpperBound,
                            ct_unary_expr<LHS, OP>,
                            VariableType
                            >
      {
        typedef typename ct_unary_expr<LHS, OP>::ERROR_INTEGRATION_OF_UNARY_EXPRESSION_NOT_SUPPORTED    type;
      };

      template <typename LowerBound, typename UpperBound,
                long value,
                typename VariableType>
      struct integrate_impl <LowerBound, UpperBound,
                            ct_constant<value>,
                            VariableType
                            >
      {
        typedef ct_binary_expr<UpperBound,
                              op_minus<default_numeric_type>,
                              LowerBound
                              >                    interval_length;

        typedef ct_binary_expr< ct_constant<value>,
                                op_mult<default_numeric_type>,
                                interval_length
                              >    integrated_type;

        typedef typename viennamath::result_of::simplify<integrated_type>::type   type;
      };

      template <typename LowerBound, typename UpperBound,
                typename TAG,
                typename VariableType>
      struct integrate_impl <LowerBound, UpperBound,
                            ct_function_symbol<TAG>,
                            VariableType
                            >
      {
        typedef typename ct_function_symbol<TAG>::ERROR_INTEGRATION_OF_FUNCTION_SYMBOL_NOT_SUPPORTED    type;
      };

      template <typename LowerBound, typename UpperBound,
                id_type id,
                typename VariableType>
      struct integrate_impl <LowerBound, UpperBound,
                            ct_variable<id>,
                            VariableType
                            >
      {
        typedef ct_binary_expr< ct_binary_expr<UpperBound,
                                              op_mult<default_numeric_type>,
                                              UpperBound
                                              >,
                                op_div<default_numeric_type>,
                                ct_constant<2>
                              >    upper_contribution;

        typedef ct_binary_expr< ct_binary_expr<LowerBound,
                                              op_mult<default_numeric_type>,
                                              LowerBound
                                              >,
                                op_div<default_numeric_type>,
                                ct_constant<2>
                              >    lower_contribution;

        typedef ct_binary_expr<upper_contribution,
                              op_minus<default_numeric_type>,
                              lower_contribution>      integrated_type;

        typedef typename viennamath::result_of::simplify<integrated_type>::type   type;
      };

    } //namespace detail

    //
    // public metafunction overloads:

    /** @brief  Specialization for the integration of a compiletime binary expression */
    template <typename LowerBound, typename UpperBound,
              typename LHS, typename OP, typename RHS,
              typename VariableType>
    struct integrate <LowerBound, UpperBound,
                      ct_binary_expr<LHS, OP, RHS>,
                      VariableType
                     >
    {
      //prepare
      typedef ct_binary_expr<LHS, OP, RHS>                                        IntegrandType;
      typedef typename viennamath::result_of::expand<IntegrandType>::type         expanded_integrand;
      typedef typename viennamath::result_of::simplify<expanded_integrand>::type  simplified_integrand;

      typedef typename detail::integrate_impl<LowerBound, UpperBound,
                                              simplified_integrand,
                                              VariableType>::type    type;

    };

    /** @brief  Specialization for the integration of a compiletime unary expression. Not supported at the moment, thus a compile time error is forced. */
    template <typename LowerBound, typename UpperBound,
              typename LHS, typename OP,
              typename VariableType>
    struct integrate <LowerBound, UpperBound,
                      ct_unary_expr<LHS, OP>,
                      VariableType
                     >
    {
      typedef typename ct_unary_expr<LHS, OP>::ERROR_INTEGRATION_OF_UNARY_EXPRESSION_NOT_SUPPORTED    type;
    };

    /** @brief  Specialization for the integration of a compile time constant. Forwards the integration to the worker metafunction integrate_impl.  */
    template <typename LowerBound, typename UpperBound,
              long value,
              typename VariableType>
    struct integrate <LowerBound, UpperBound,
                      ct_constant<value>,
                      VariableType
                     >
    {
      typedef typename detail::integrate_impl<LowerBound, UpperBound, ct_constant<value>, VariableType >::type    type;
    };

    /** @brief  Specialization for the integration of a compiletime function symbol. This is a user error, since a function symbol should have been replaced already by another expression. Thus, a compile time error is forced. */
    template <typename LowerBound, typename UpperBound,
              typename TAG,
              typename VariableType>
    struct integrate <LowerBound, UpperBound,
                      ct_function_symbol<TAG>,
                      VariableType
                     >
    {
      typedef typename ct_function_symbol<TAG>::ERROR_INTEGRATION_OF_FUNCTION_SYMBOL_NOT_SUPPORTED    type;
    };

    /** @brief  Specialization for the integration of a compile time variable. Forwards the integration to the worker metafunction integrate_impl.  */
    template <typename LowerBound, typename UpperBound,
              id_type id,
              typename VariableType>
    struct integrate <LowerBound, UpperBound,
                      ct_variable<id>,
                      VariableType
                     >
    {
      typedef typename detail::integrate_impl<LowerBound, UpperBound, ct_variable<id>, VariableType >::type    type;
    };


  } // namespace result_of

  /** @brief The user function for the compile time integration of a compile time expression.
   *
   * @param interv         The compiletime integration interval
   * @param integrand      A compiletime integrand
   * @param var            A compiletime variable
   */
  template <typename LowerBound, typename UpperBound,
            typename IntegrandType,
            typename VariableType>
  typename result_of::integrate<LowerBound, UpperBound,
                                IntegrandType,
                                VariableType>::type
  integrate( ct_interval<LowerBound, UpperBound> const & /*interv*/,
             IntegrandType const & /*integrand*/,
             VariableType const & /*var*/)
  {
    return typename result_of::integrate<LowerBound, UpperBound,
                                         IntegrandType,
                                         VariableType>::type();
  }


  //////////// run time ///////////////////

  //TODO: Symbolic integration at run time not provided yet

}

#endif
