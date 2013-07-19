#ifndef VIENNAMATH_MANIPULATION_SIMPLIFY_HPP
#define VIENNAMATH_MANIPULATION_SIMPLIFY_HPP

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
#include "viennamath/runtime/functor_wrapper.hpp"
#include <assert.h>

/** @file simplify.hpp
    @brief Provides routines for the simplification of expressions (remove trivial operations, collect constants, etc.)
*/

namespace viennamath
{

  //
  //   Section 1: Compile time optimization
  //

  namespace result_of
  {


    namespace detail
    {

      // general handling:
      template <typename ExpressionType>
      struct simplify_impl
      {
        //default: no optimization available, thus don't manipulate anything
        enum { value = 0 };
        typedef ExpressionType        type;
      };

      template <typename LHS, typename OP, typename RHS>
      struct simplify_impl< ct_binary_expr<LHS, OP, RHS> >
      {
        //default: no optimization available, thus don't manipulate anything
        enum { value = simplify_impl<LHS>::value + simplify_impl<RHS>::value };
        typedef ct_binary_expr<typename simplify_impl<LHS>::type,
                              OP,
                              typename simplify_impl<RHS>::type
                              >        type;
      };


      ////////// Addition of zero:

      // something plus 0 is something
      template <typename LHS, typename NumericT>
      struct simplify_impl < ct_binary_expr<LHS, op_plus<NumericT>, ct_constant<0> > >
      {
        enum { value = 1 };
        typedef LHS          type;
      };

      // 0 plus something is something
      template <typename NumericT, typename RHS>
      struct simplify_impl < ct_binary_expr<ct_constant<0>, op_plus<NumericT>, RHS> >
      {
        enum { value = 1 };
        typedef RHS          type;
      };

      // 0 plus 0 is 0 (resolve ambiguity)
      template <typename NumericT>
      struct simplify_impl < ct_binary_expr<ct_constant<0>, op_plus<NumericT>, ct_constant<0> > >
      {
        enum { value = 1 };
        typedef ct_constant<0>          type;
      };




      ////////// Subtraction of zero:

      // something minus 0 is something
      template <typename LHS, typename NumericT>
      struct simplify_impl < ct_binary_expr<LHS, op_minus<NumericT>, ct_constant<0> > >
      {
        enum { value = 1 };
        typedef LHS          type;
      };


      template <typename LHS1, typename NumericT, typename RHS2>
      struct simplify_impl < ct_binary_expr<LHS1,
                                            op_plus<NumericT>,
                                            ct_binary_expr<ct_constant<0>,
                                                          op_minus<NumericT>,
                                                          RHS2 >
                                            > >
      {
        enum { value = 1 };
        typedef ct_binary_expr<LHS1,
                              op_minus<NumericT>,
                              RHS2>                   type;
      };

      template <typename LHS1, typename NumericT, typename RHS2>
      struct simplify_impl < ct_binary_expr<LHS1,
                                            op_minus<NumericT>,
                                            ct_binary_expr<ct_constant<0>,
                                                          op_minus<NumericT>,
                                                          RHS2 >
                                            > >
      {
        enum { value = 1 };
        typedef ct_binary_expr<LHS1,
                              op_plus<NumericT>,
                              RHS2>                   type;
      };

      template <typename RHS1, typename NumericT, typename RHS2>
      struct simplify_impl < ct_binary_expr<ct_binary_expr< ct_constant<0>,
                                                            op_minus<NumericT>,
                                                            RHS1 >,
                                            op_plus<NumericT>,
                                            RHS2
                                            > >
      {
        enum { value = 1 };
        typedef ct_binary_expr<RHS2,
                              op_minus<NumericT>,
                              RHS1>                   type;
      };

      template <typename RHS1, typename NumericT, typename RHS2>
      struct simplify_impl < ct_binary_expr<ct_binary_expr< ct_constant<0>,
                                                            op_minus<NumericT>,
                                                            RHS1 >,
                                            op_minus<NumericT>,
                                            RHS2
                                            > >
      {
        enum { value = 1 };
        typedef ct_binary_expr< ct_constant<-1>,
                                op_mult<NumericT>,
                                ct_binary_expr<RHS1,
                                              op_plus<NumericT>,
                                              RHS2>
                              >                 type;
      };

      // resolve ambituity for plus:
      template <typename RHS1, typename NumericT, typename RHS2>
      struct simplify_impl < ct_binary_expr<ct_binary_expr< ct_constant<0>,
                                                            op_minus<NumericT>,
                                                            RHS1 >,
                                            op_plus<NumericT>,
                                            ct_binary_expr< ct_constant<0>,
                                                            op_minus<NumericT>,
                                                            RHS2 >
                                            > >
      {
        enum { value = 1 };
        typedef ct_binary_expr< ct_constant<-1>,
                                op_mult<NumericT>,
                                ct_binary_expr<RHS1,
                                              op_plus<NumericT>,
                                              RHS2>
                              >                 type;
      };

      // resolve ambituity for minus:
      template <typename RHS1, typename NumericT, typename RHS2>
      struct simplify_impl < ct_binary_expr<ct_binary_expr< ct_constant<0>,
                                                            op_minus<NumericT>,
                                                            RHS1 >,
                                            op_minus<NumericT>,
                                            ct_binary_expr< ct_constant<0>,
                                                            op_minus<NumericT>,
                                                            RHS2 >
                                            > >
      {
        enum { value = 1 };
        typedef ct_binary_expr<RHS2,
                              op_minus<NumericT>,
                              RHS1>                   type;
      };



      ///////// Multiplication by zero:

      // something times 0 is 0
      template <typename LHS, typename NumericT>
      struct simplify_impl < ct_binary_expr<LHS, op_mult<NumericT>, ct_constant<0> > >
      {
        enum { value = 1 };
        typedef ct_constant<0>          type;
      };

      // 0 times something is 0
      template <typename NumericT, typename RHS>
      struct simplify_impl < ct_binary_expr<ct_constant<0>, op_mult<NumericT>, RHS> >
      {
        enum { value = 1 };
        typedef ct_constant<0>          type;
      };

      // 0 times 0 is 0 (resolve ambiguity)
      template <typename NumericT>
      struct simplify_impl < ct_binary_expr<ct_constant<0>, op_mult<NumericT>, ct_constant<0> > >
      {
        enum { value = 1 };
        typedef ct_constant<0>          type;
      };


      ///////// Multiplication by one:

      // something times 1 is something
      template <typename LHS, typename NumericT>
      struct simplify_impl < ct_binary_expr<LHS, op_mult<NumericT>, ct_constant<1> > >
      {
        enum { value = 1 };
        typedef LHS          type;
      };

      // 1 times something is something
      template <typename NumericT, typename RHS>
      struct simplify_impl < ct_binary_expr<ct_constant<1>, op_mult<NumericT>, RHS> >
      {
        enum { value = 1 };
        typedef RHS          type;
      };

      // 1 times 1 is 1 (resolve ambiguity)
      template <typename NumericT>
      struct simplify_impl < ct_binary_expr<ct_constant<1>, op_mult<NumericT>, ct_constant<1> > >
      {
        enum { value = 1 };
        typedef ct_constant<1>          type;
      };

      // resolve amgibuity for 0 * 1:
      template <typename NumericT>
      struct simplify_impl < ct_binary_expr<ct_constant<1>, op_mult<NumericT>, ct_constant<0> > >
      {
        enum { value = 1 };
        typedef ct_constant<0>          type;
      };

      template <typename NumericT>
      struct simplify_impl < ct_binary_expr<ct_constant<0>, op_mult<NumericT>, ct_constant<1> > >
      {
        enum { value = 1 };
        typedef ct_constant<0>          type;
      };


      ////////// Zero in factional expressions

      // 0 divided by something is 0
      template <typename NumericT, typename RHS>
      struct simplify_impl < ct_binary_expr<ct_constant<0>, op_div<NumericT>, RHS> >
      {
        enum { value = 1 };
        typedef ct_constant<0>          type;
      };

      // something divided by 0 is a compile time error :-)
      template <typename LHS, typename NumericT>
      struct simplify_impl < ct_binary_expr<LHS, op_div<NumericT>, ct_constant<0> > >
      {
        enum { value = 1 };
        typedef typename ct_binary_expr<LHS, op_div<NumericT>, ct_constant<0> >::ERROR_ENCOUNTERED_DIVISION_BY_ZERO          type;
      };


      // 0 divided by 0 is a compile time error :-)
      template <typename NumericT>
      struct simplify_impl < ct_binary_expr<ct_constant<0>, op_div<NumericT>, ct_constant<0> > >
      {
        enum { value = 1 };
        typedef typename ct_binary_expr<ct_constant<0>, op_div<NumericT>, ct_constant<0> >::ERROR_ENCOUNTERED_DIVISION_BY_ZERO          type;
      };


      ////////// One in denominator: Throw away

      // something divided by 1 is 1
      template <typename LHS, typename NumericT>
      struct simplify_impl < ct_binary_expr<LHS, op_div<NumericT>, ct_constant<1> > >
      {
        enum { value = 1 };
        typedef LHS          type;
      };

      ////////// (1 / X) * Y  and  Y * (1/X)   results in Y/X

      template <typename LHS, typename NumericT, typename RHS>
      struct simplify_impl < ct_binary_expr<LHS,
                                            op_mult<NumericT>,
                                            ct_binary_expr<ct_constant<1>, op_div<NumericT>, RHS>
                                          > >
      {
        enum { value = 1 };
        typedef ct_binary_expr<LHS,
                              op_div<NumericT>,
                              RHS>                             type;
      };

      template <typename RHS1, typename NumericT, typename RHS2>
      struct simplify_impl < ct_binary_expr<ct_binary_expr<ct_constant<1>, op_div<NumericT>, RHS1>,
                                            op_mult<NumericT>,
                                            RHS2
                                          > >
      {
        enum { value = 1 };
        typedef ct_binary_expr<RHS2,
                              op_div<NumericT>,
                              RHS1>                             type;
      };

      //resolve ambiguity in above two cases:
      template <typename RHS1, typename NumericT, typename RHS2>
      struct simplify_impl < ct_binary_expr<ct_binary_expr<ct_constant<1>, op_div<NumericT>, RHS1>,
                                            op_mult<NumericT>,
                                            ct_binary_expr<ct_constant<1>, op_div<NumericT>, RHS2>
                                          > >
      {
        enum { value = 1 };
        typedef ct_binary_expr<ct_constant<1>,
                              op_div<NumericT>,
                              ct_binary_expr<RHS1, op_mult<NumericT>, RHS2>
                              >                             type;
      };



      ///////////////////// Rational evaluations ////////////////////////////////

      //////////////// reduce a rational expression consistnig of ct_constants to its simplest form ///////////////////////

      // A + B:
      template <long numerator_1, long denominator_1, typename NumericT,
                long numerator_2, long denominator_2>
      struct simplify_impl< ct_binary_expr<ct_binary_expr<ct_constant<numerator_1>,
                                                          op_div<NumericT>,
                                                          ct_constant<denominator_1> >,
                                          op_plus<NumericT>,
                                          ct_binary_expr<ct_constant<numerator_2>,
                                                          op_div<NumericT>,
                                                          ct_constant<denominator_2> >
                                          >
                          >
      {
        enum { value = 1,
              numerator = numerator_1 * denominator_2 + numerator_2 * denominator_1,
              denominator = denominator_1 * denominator_2 };


        typedef ct_binary_expr< ct_constant< numerator / result_of::gcd<numerator, denominator>::value >,
                                op_div<NumericT>,
                                ct_constant<denominator / result_of::gcd<numerator, denominator>::value > >        type;
      };

      template <long value_1, typename NumericT,
                long numerator_2, long denominator_2>
      struct simplify_impl< ct_binary_expr<ct_constant<value_1>,
                                          op_plus<NumericT>,
                                          ct_binary_expr<ct_constant<numerator_2>,
                                                          op_div<NumericT>,
                                                          ct_constant<denominator_2> >
                                          >
                          >
      {
        enum { value = 1,
              numerator = value_1 * denominator_2 + numerator_2,
              denominator = denominator_2 };

        typedef ct_binary_expr< ct_constant< numerator / result_of::gcd<numerator, denominator>::value >,
                                op_div<NumericT>,
                                ct_constant<denominator / result_of::gcd<numerator, denominator>::value > >        type;
      };

      template <long numerator_1, long denominator_1, typename NumericT,
                long value_2>
      struct simplify_impl< ct_binary_expr<ct_binary_expr<ct_constant<numerator_1>,
                                                          op_div<NumericT>,
                                                          ct_constant<denominator_1> >,
                                          op_plus<NumericT>,
                                          ct_constant<value_2>
                                          >
                          >
      {
        enum { value = 1,
              numerator = numerator_1 + value_2 * denominator_1,
              denominator = denominator_1 };

        typedef ct_binary_expr< ct_constant< numerator / result_of::gcd<numerator, denominator>::value >,
                                op_div<NumericT>,
                                ct_constant<denominator / result_of::gcd<numerator, denominator>::value > >        type;
      };



      // A - B:
      template <long numerator_1, long denominator_1, typename NumericT,
                long numerator_2, long denominator_2>
      struct simplify_impl< ct_binary_expr<ct_binary_expr<ct_constant<numerator_1>,
                                                          op_div<NumericT>,
                                                          ct_constant<denominator_1> >,
                                          op_minus<NumericT>,
                                          ct_binary_expr<ct_constant<numerator_2>,
                                                          op_div<NumericT>,
                                                          ct_constant<denominator_2> >
                                          > >
      {
        enum { value = 1,
              numerator = numerator_1 * denominator_2 - numerator_2 * denominator_1,
              denominator = denominator_1 * denominator_2 };

        typedef ct_binary_expr< ct_constant< numerator / result_of::gcd<numerator, denominator>::value >,
                                op_div<NumericT>,
                                ct_constant<denominator / result_of::gcd<numerator, denominator>::value > >        type;
      };

      template <long value_1, typename NumericT,
                long numerator_2, long denominator_2>
      struct simplify_impl< ct_binary_expr<ct_constant<value_1>,
                                          op_minus<NumericT>,
                                          ct_binary_expr<ct_constant<numerator_2>,
                                                          op_div<NumericT>,
                                                          ct_constant<denominator_2> >
                                          > >
      {
        enum { value = 1,
              numerator = value_1 * denominator_2 - numerator_2,
              denominator = denominator_2 };

        typedef ct_binary_expr< ct_constant< numerator / result_of::gcd<numerator, denominator>::value >,
                                op_div<NumericT>,
                                ct_constant<denominator / result_of::gcd<numerator, denominator>::value > >        type;
      };

      template <long numerator_1, long denominator_1, typename NumericT,
                long value_2>
      struct simplify_impl< ct_binary_expr<ct_binary_expr<ct_constant<numerator_1>,
                                                          op_div<NumericT>,
                                                          ct_constant<denominator_1> >,
                                          op_minus<NumericT>,
                                          ct_constant<value_2>
                                          > >
      {
        enum { value = 1,
              numerator = numerator_1 - value_2 * denominator_1,
              denominator = denominator_1 };

        typedef ct_binary_expr< ct_constant< numerator / result_of::gcd<numerator, denominator>::value >,
                                op_div<NumericT>,
                                ct_constant<denominator / result_of::gcd<numerator, denominator>::value > >        type;
      };

      // resolve ambiguity with elimination of zero
      template <long numerator_1, long denominator_1, typename NumericT>
      struct simplify_impl< ct_binary_expr<ct_binary_expr<ct_constant<numerator_1>,
                                                          op_div<NumericT>,
                                                          ct_constant<denominator_1> >,
                                          op_minus<NumericT>,
                                          ct_constant<0>
                                          > >
      {
        enum { value = 1,
              numerator = numerator_1,
              denominator = denominator_1 };

        typedef ct_binary_expr< ct_constant< numerator / result_of::gcd<numerator, denominator>::value >,
                                op_div<NumericT>,
                                ct_constant<denominator / result_of::gcd<numerator, denominator>::value > >        type;
      };


      // A * B:
      template <long numerator_1, long denominator_1, typename NumericT,
                long numerator_2, long denominator_2>
      struct simplify_impl< ct_binary_expr<ct_binary_expr<ct_constant<numerator_1>,
                                                          op_div<NumericT>,
                                                          ct_constant<denominator_1> >,
                                          op_mult<NumericT>,
                                          ct_binary_expr<ct_constant<numerator_2>,
                                                          op_div<NumericT>,
                                                          ct_constant<denominator_2> >
                                          >  >
      {
        enum { value = 1,
              numerator = numerator_1 * numerator_2,
              denominator = denominator_1 * denominator_2 };

        typedef ct_binary_expr< ct_constant< numerator / result_of::gcd<numerator, denominator>::value >,
                                op_div<NumericT>,
                                ct_constant<denominator / result_of::gcd<numerator, denominator>::value > >        type;
      };

      template <long value_1, typename NumericT,
                long numerator_2, long denominator_2>
      struct simplify_impl< ct_binary_expr<ct_constant<value_1>,
                                          op_mult<NumericT>,
                                          ct_binary_expr<ct_constant<numerator_2>,
                                                          op_div<NumericT>,
                                                          ct_constant<denominator_2> >
                                          > >
      {
        enum { value = 1,
              numerator = value_1 * numerator_2,
              denominator = denominator_2 };

        typedef ct_binary_expr< ct_constant< numerator / result_of::gcd<numerator, denominator>::value >,
                                op_div<NumericT>,
                                ct_constant<denominator / result_of::gcd<numerator, denominator>::value > >        type;
      };

      template <long numerator_1, long denominator_1, typename NumericT,
                long value_2>
      struct simplify_impl< ct_binary_expr<ct_binary_expr<ct_constant<numerator_1>,
                                                          op_div<NumericT>,
                                                          ct_constant<denominator_1> >,
                                          op_mult<NumericT>,
                                          ct_constant<value_2>
                                          >  >
      {
        enum { value = 1,
              numerator = numerator_1 * value_2,
              denominator = denominator_1 };

        typedef ct_binary_expr< ct_constant< numerator / result_of::gcd<numerator, denominator>::value >,
                                op_div<NumericT>,
                                ct_constant<denominator / result_of::gcd<numerator, denominator>::value > >        type;
      };

      // A / B:
      template <long numerator_1, long denominator_1, typename NumericT,
                long numerator_2, long denominator_2>
      struct simplify_impl< ct_binary_expr<ct_binary_expr<ct_constant<numerator_1>,
                                                          op_div<NumericT>,
                                                          ct_constant<denominator_1> >,
                                          op_div<NumericT>,
                                          ct_binary_expr<ct_constant<numerator_2>,
                                                          op_div<NumericT>,
                                                          ct_constant<denominator_2> >
                                          >  >
      {
        enum { value = 1,
              numerator = numerator_1 * denominator_2,
              denominator = denominator_1 * numerator_2 };

        typedef ct_binary_expr< ct_constant< numerator / result_of::gcd<numerator, denominator>::value >,
                                op_div<NumericT>,
                                ct_constant<denominator / result_of::gcd<numerator, denominator>::value > >        type;
      };

      template <long value_1, typename NumericT,
                long numerator_2, long denominator_2>
      struct simplify_impl< ct_binary_expr<ct_constant<value_1>,
                                          op_div<NumericT>,
                                          ct_binary_expr<ct_constant<numerator_2>,
                                                          op_div<NumericT>,
                                                          ct_constant<denominator_2> >
                                          >  >
      {
        enum { value = 1,
              numerator = value_1 * denominator_2,
              denominator = numerator_2 };

        typedef ct_binary_expr< ct_constant< numerator / result_of::gcd<numerator, denominator>::value >,
                                op_div<NumericT>,
                                ct_constant<denominator / result_of::gcd<numerator, denominator>::value > >        type;
      };

      template <long numerator_1, typename NumericT, long denominator_1,
                long value_2>
      struct simplify_impl< ct_binary_expr<ct_binary_expr<ct_constant<numerator_1>,
                                                          op_div<NumericT>,
                                                          ct_constant<denominator_1> >,
                                          op_div<NumericT>,
                                          ct_constant<value_2>
                                          >  >
      {
        enum { value = 1,
              numerator = numerator_1,
              denominator = denominator_1 * value_2 };

        typedef ct_binary_expr< ct_constant< numerator / result_of::gcd<numerator, denominator>::value >,
                                op_div<NumericT>,
                                ct_constant<denominator / result_of::gcd<numerator, denominator>::value > >        type;
      };

      //primitive expressions
      template <long value_1, typename NumericT, long value_2>
      struct simplify_impl< ct_binary_expr<ct_constant<value_1>,
                                          op_plus<NumericT>,
                                          ct_constant<value_2>
                                          >  >
      {
        enum { value = 1 };
        typedef ct_constant<value_1 + value_2>   type;
      };

      template <long value_1, typename NumericT>
      struct simplify_impl< ct_binary_expr<ct_constant<value_1>,
                                          op_plus<NumericT>,
                                          ct_constant<0>
                                          >  >
      {
        enum { value = 1 };
        typedef ct_constant<value_1>   type;
      };

      template <typename NumericT, long value_2>
      struct simplify_impl< ct_binary_expr<ct_constant<0>,
                                          op_plus<NumericT>,
                                          ct_constant<value_2>
                                          >  >
      {
        enum { value = 1 };
        typedef ct_constant<value_2>   type;
      };

      template <long value_1, typename NumericT, long value_2>
      struct simplify_impl< ct_binary_expr<ct_constant<value_1>,
                                          op_minus<NumericT>,
                                          ct_constant<value_2>
                                          >  >
      {
        enum { value = 1 };
        typedef ct_constant<value_1 - value_2>   type;
      };

      //resolve ambiguity:
      template <typename NumericT>
      struct simplify_impl< ct_binary_expr<ct_constant<1>,
                                          op_minus<NumericT>,
                                          ct_constant<0>
                                          >  >
      {
        enum { value = 1 };
        typedef ct_constant<1>   type;
      };

      template <typename NumericT>
      struct simplify_impl< ct_binary_expr<ct_constant<-1>,
                                          op_minus<NumericT>,
                                          ct_constant<0>
                                          >  >
      {
        enum { value = 1 };
        typedef ct_constant<-1>   type;
      };


      //////// const * const:

      template <long value_1, typename NumericT, long value_2>
      struct simplify_impl< ct_binary_expr<ct_constant<value_1>,
                                          op_mult<NumericT>,
                                          ct_constant<value_2>
                                          > >
      {
        enum { value = 1 };
        typedef ct_constant<value_1 * value_2>   type;
      };

      //resolve ambiguity
      template <typename NumericT, long value_2>
      struct simplify_impl< ct_binary_expr<ct_constant<1>,
                                          op_mult<NumericT>,
                                          ct_constant<value_2>
                                          > >
      {
        enum { value = 1 };
        typedef ct_constant<value_2>   type;
      };

      template <long value_1, typename NumericT>
      struct simplify_impl< ct_binary_expr<ct_constant<value_1>,
                                          op_mult<NumericT>,
                                          ct_constant<1>
                                          > >
      {
        enum { value = 1 };
        typedef ct_constant<value_1>   type;
      };

      template <long value_1, typename NumericT>
      struct simplify_impl< ct_binary_expr<ct_constant<value_1>,
                                          op_mult<NumericT>,
                                          ct_constant<0>
                                          > >
      {
        enum { value = 1 };
        typedef ct_constant<0>   type;
      };

      template <typename NumericT, long value_2>
      struct simplify_impl< ct_binary_expr<ct_constant<0>,
                                          op_mult<NumericT>,
                                          ct_constant<value_2>
                                          > >
      {
        enum { value = 1 };
        typedef ct_constant<0>   type;
      };


      // a / b


      template <long value_1, typename NumericT, long value_2>
      struct simplify_impl< ct_binary_expr<ct_constant<value_1>,
                                          op_div<NumericT>,
                                          ct_constant<value_2>
                                          > >
      {
        enum { value = 0 };  //Note: this is not a change that needs to trigger an optimization, thus keep it at 0.
        typedef ct_binary_expr<ct_constant<value_1 / result_of::gcd<value_1, value_2>::value >,
                              op_div<NumericT>,
                              ct_constant<value_2 / result_of::gcd<value_1, value_2>::value >
                              >                  type;
      };

      template <typename NumericT, long value_2>
      struct simplify_impl< ct_binary_expr<ct_constant<0>,
                                          op_div<NumericT>,
                                          ct_constant<value_2>
                                          > >
      {
        enum { value = 1 };
        typedef ct_constant<0>                   type;
      };

      template <typename NumericT>
      struct simplify_impl< ct_binary_expr<ct_constant<0>,
                                          op_div<NumericT>,
                                          ct_constant<1>
                                          > >
      {
        enum { value = 1 };
        typedef ct_constant<0>                   type;
      };

      template <long value_1, typename NumericT>
      struct simplify_impl< ct_binary_expr<ct_constant<value_1>,
                                          op_div<NumericT>,
                                          ct_constant<1>
                                          > >
      {
        enum { value = 1 };
        typedef ct_constant<value_1>            type;
      };


      template <long value_1, typename NumericT>
      struct simplify_impl< ct_binary_expr<ct_constant<value_1>,
                                          op_div<NumericT>,
                                          ct_constant<-1>
                                          > >
      {
        enum { value = 1 };
        typedef ct_constant<-value_1>            type;
      };


    } //namespace detail

    ////////////////////////////////////////////////////////////////////////////////////

    //
    // interfacing metafunction: loops over simplification routines until no more optimization is possible
    //
    /** @brief Top-level metafunction for the simplification of expressions. Loops over simplification rules until no more simplifications can be applied.
     *
     * Note that the default case provides SFINAE for the compile time interface function simplify()
     *
     * @tparam ExpressionType          The compile time expression to be simplified
     * @tparam optimization_possible   Internal flag, which is evaluated to true as long as optimizations can be applied. Should not be passed by the user.
     */
    template <typename ExpressionType,
              bool optimization_possible = (detail::simplify_impl<ExpressionType>::value != 0)
             >
    struct simplify {}; //provides SFINAE for the compile time interface function simplify()

    // binary expression
    /** @brief Specialization for a binary expression, for which optimizations can be applied. */
    template <typename LHS, typename OP, typename RHS>
    struct simplify < ct_binary_expr<LHS, OP, RHS>, true >
    {
      typedef typename detail::simplify_impl< ct_binary_expr<LHS, OP, RHS> >::type    simplified_type;
      typedef typename simplify<simplified_type>::type       type;
      //typedef simplifyd_type          type;
    };

    /** @brief Specialization for a binary expression, for which no optimizations can be applied: Return the unmodified expression. */
    template <typename LHS, typename OP, typename RHS>
    struct simplify < ct_binary_expr<LHS, OP, RHS>, false >
    {
      typedef ct_binary_expr<LHS, OP, RHS>          type;
    };

    // unary expression
    /** @brief Specialization for a unary expression, for which optimizations can be applied. */
    template <typename LHS, typename OP>
    struct simplify < ct_unary_expr<LHS, OP>, true >
    {
      typedef typename detail::simplify_impl< ct_unary_expr<LHS, OP> >::type    simplified_type;
      typedef typename simplify<simplified_type>::type       type;
    };

    /** @brief Specialization for a unary expression, for which no optimizations can be applied: Return the unmodified expression. */
    template <typename LHS, typename OP>
    struct simplify < ct_unary_expr<LHS, OP>, false >
    {
      typedef ct_unary_expr<LHS, OP>       type;
    };

    // constant
    /** @brief Specialization for a constant, for which optimizations can be applied. */
    template <long value>
    struct simplify < ct_constant<value>, true >
    {
      typedef typename detail::simplify_impl< ct_constant<value> >::type    simplified_type;
      typedef typename simplify<simplified_type>::type       type;
    };

    /** @brief Specialization for a constant, for which no optimizations can be applied: Return the unmodified constant. */
    template <long value>
    struct simplify < ct_constant<value>, false >
    {
      typedef ct_constant<value>       type;
    };

    // function symbol
    /** @brief Specialization for a function symbol, for which optimizations can be applied. */
    template <typename TAG>
    struct simplify < ct_function_symbol<TAG>, true >
    {
      typedef typename detail::simplify_impl< ct_function_symbol<TAG> >::type    simplified_type;
      typedef typename simplify<simplified_type>::type       type;
    };

    /** @brief Specialization for a function symbol, for which no optimizations can be applied: Return the unmodified constant. */
    template <typename TAG>
    struct simplify < ct_function_symbol<TAG>, false >
    {
      typedef ct_function_symbol<TAG>       type;
    };

    // variable
    /** @brief Specialization for a variable, for which optimizations can be applied. */
    template <id_type id>
    struct simplify < ct_variable<id>, true >
    {
      typedef typename detail::simplify_impl< ct_variable<id> >::type    simplified_type;
      typedef typename simplify<simplified_type>::type       type;
    };

    /** @brief Specialization for a compiletime variable, for which no optimizations can be applied: Return the unmodified constant. */
    template <id_type id>
    struct simplify < ct_variable<id>, false >
    {
      typedef ct_variable<id>       type;
    };

  } //namespace result_of


  /** @brief The generic interface function for compiletime manipulations */
  template <typename ExpressionType>
  typename result_of::simplify<ExpressionType>::type
  simplify(ExpressionType const & e)
  {
    return typename result_of::simplify<ExpressionType>::type();
  }





  //
  //   Section 2: Run time simplification
  //



  //public interface:
  /** @brief Simplifies an expression. Directly manipulates the provided expression. */
  template <typename InterfaceType>
  void inplace_simplify(rt_expr<InterfaceType> & e)
  {
    while (e.get()->can_simplify())
      e = e.get()->simplify();
  }

  /** @brief Simplifies an expression. Directly manipulates the provided expression. */
  template <typename InterfaceType>
  void inplace_simplify(rt_unary_expr<InterfaceType> & e)
  {
    while (e->can_simplify())
      e = e->simplify();
  }

  /** @brief Simplifies an expression. Directly manipulates the provided expression. */
  template <typename InterfaceType>
  void inplace_simplify(rt_binary_expr<InterfaceType> & e)
  {
    while (e->can_simplify())
      e = e->simplify();
  }

  /** @brief Simplifies an expression. Directly manipulates the provided expression. */
  template <typename T>
  void inplace_simplify(T & e)
  {
    //do nothing, since primitive types cannot be simplified
  }



  /** @brief Returns a new, simplified expression. */
  template <typename InterfaceType>
  rt_expr<InterfaceType> simplify(rt_expr<InterfaceType> const & e)
  {
    rt_expr<InterfaceType> ret(e);
    inplace_simplify(ret);
    return ret;
  }

  /** @brief Returns a new, simplified expression. */
  template <typename InterfaceType>
  rt_expr<InterfaceType> simplify(rt_unary_expr<InterfaceType> const & e)
  {
    rt_expr<InterfaceType> ret(e);
    inplace_simplify(ret);
    return ret;
  }

  /** @brief Returns a new, simplified expression. */
  template <typename InterfaceType>
  rt_expr<InterfaceType> simplify(rt_binary_expr<InterfaceType> const & e)
  {
    rt_expr<InterfaceType> ret(e);
    inplace_simplify(ret);
    return ret;
  }

  /** @brief Returns a new, simplified expression. */
  template <typename T>
  T simplify(T const & e)
  {
    //do nothing, since primitive types cannot be simplified
    return e;
  }

}

#endif
