#ifndef VIENNAMATH_COMPILETIME_PROMOTE_TRAITS_HPP
#define VIENNAMATH_COMPILETIME_PROMOTE_TRAITS_HPP

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

/** @file promote_traits.hpp
    @brief Deduces the result type of the possibly mixed addition/subtraction/multiplication/division of integers and floating point types
*/

namespace viennamath
{
   /** @brief For two numeric types T and U with objects t and u respectively, the result type of t+u is returned. Default case: return the basic numeric type for ViennaMath. */
   template <typename T, typename OP, typename U>
   struct promote_traits
   {
     typedef default_numeric_type  result_type;
   };

   /** @brief The result type of two operands of the same type is of the same type again (cf. standard C/C++ behavior for integers) */
   template <typename T, typename OP>
   struct promote_traits <T, OP, T>
   {
      typedef T      result_type;
   };

   //to double:
   template <typename OP>
   struct promote_traits <double, OP, long>
   {
      typedef double      result_type;
   };

   template <typename OP>
   struct promote_traits <long, OP, double>
   {
      typedef double      result_type;
   };

   template <typename OP>
   struct promote_traits <double, OP, int>
   {
      typedef double      result_type;
   };

   template <typename OP>
   struct promote_traits <int, OP, double>
   {
      typedef double      result_type;
   };

   template <typename OP>
   struct promote_traits <double, OP, float>
   {
      typedef double      result_type;
   };

   template <typename OP>
   struct promote_traits <float, OP, double>
   {
      typedef double      result_type;
   };

   //to float:
   template <typename OP>
   struct promote_traits <float, OP, long>
   {
      typedef float      result_type;
   };

   template <typename OP>
   struct promote_traits <long, OP, float>
   {
      typedef float      result_type;
   };

   template <typename OP>
   struct promote_traits <float, OP, int>
   {
      typedef float      result_type;
   };

   template <typename OP>
   struct promote_traits <int, OP, float>
   {
      typedef float      result_type;
   };

   //special case: integer division:
   template <typename NumericType>
   struct promote_traits <long, op_div<NumericType>, long>
   {
      typedef default_numeric_type      result_type;
   };

   template <typename NumericType>
   struct promote_traits <long, op_div<NumericType>, int>
   {
      typedef default_numeric_type      result_type;
   };

   template <typename NumericType>
   struct promote_traits <int, op_div<NumericType>, long>
   {
      typedef default_numeric_type      result_type;
   };


}

#endif
