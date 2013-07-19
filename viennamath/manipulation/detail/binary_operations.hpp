#ifndef VIENNAMATH_BINARY_OPERATORS_MANIPULATION_HPP
#define VIENNAMATH_BINARY_OPERATORS_MANIPULATION_HPP

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
#include "viennamath/compiletime/binary_op_tags.hpp"
#include "viennamath/runtime/binary_expr.hpp"
#include "viennamath/runtime/unary_expr.hpp"

/** @file viennamath/manipulation/detail/binary_operations.hpp
    @brief Provides additional implementation details for binary operations.
*/

namespace viennamath
{


  //
  // addition
  //
  /** @brief Implementation of the differentation of e1 + e1, where e1 and e2 are expressions. */
  template <typename InterfaceType, typename NumericT>
  InterfaceType * diff_impl(const InterfaceType * lhs, op_plus<NumericT>, const InterfaceType * rhs, const InterfaceType * diff_var)
  {
      return new rt_binary_expr<InterfaceType>( lhs->diff(diff_var),
                                                new op_binary<op_plus<NumericT>, InterfaceType>(),
                                                rhs->diff(diff_var) );
  }

  /** @brief Implementation of the simplification procedures for an expression of the form e1 + e2 */
  template <typename InterfaceType, typename NumericT>
  InterfaceType * simplify_impl(const InterfaceType * lhs, op_plus<NumericT>, const InterfaceType * rhs)
  {
        if (lhs->is_constant())
        {
          NumericT val = lhs->unwrap();
          if (val == 0.0)
            return rhs->simplify();

          return new rt_binary_expr<InterfaceType>(new rt_constant<NumericT, InterfaceType>(val),
                                                   new op_binary<op_plus<NumericT>, InterfaceType>(),
                                                   rhs->simplify());
        }
        else if (rhs->is_constant())
        {
          NumericT val = rhs->unwrap();
          if (val == 0.0)
            return lhs->simplify();

          return new rt_binary_expr<InterfaceType>(lhs->simplify(),
                                                   new op_binary<op_plus<NumericT>, InterfaceType>(),
                                                   new rt_constant<NumericT, InterfaceType>(val));
        }

        return new rt_binary_expr<InterfaceType>(lhs->simplify(),
                                                 new op_binary<op_plus<NumericT>, InterfaceType>(),
                                                 rhs->simplify());
  }

  /** @brief Checks whether an expression of the form e1 + e2 can be simplified */
  template <typename InterfaceType, typename NumericT>
  bool can_simplify_impl(const InterfaceType * lhs, op_plus<NumericT>, const InterfaceType * rhs)
  {
      if (lhs->is_constant())
      {
        if (lhs->unwrap() == 0.0)
        {
          //std::cout << "can_simplify: true in op_plus" << std::endl;
          return true;
        }
      }
      if (rhs->is_constant())
      {
        if (rhs->unwrap() == 0.0)
        {
          //std::cout << "can_simplify: true in op_plus" << std::endl;
          return true;
        }
      }
      return lhs->can_simplify() || rhs->can_simplify();
  }




  //
  // subtraction
  //
  /** @brief Implementation of the differentation of e1 - e1, where e1 and e2 are expressions. */
  template <typename InterfaceType, typename NumericT>
  InterfaceType * diff_impl(const InterfaceType * lhs, op_minus<NumericT>, const InterfaceType * rhs, const InterfaceType * diff_var)
  {
      return new rt_binary_expr<InterfaceType>( lhs->diff(diff_var),
                                                new op_binary<op_minus<NumericT>, InterfaceType>(),
                                                rhs->diff(diff_var) );
  }

  /** @brief Implementation of the simplification procedures for an expression of the form e1 - e2 */
  template <typename InterfaceType, typename NumericT>
  InterfaceType * simplify_impl(const InterfaceType * lhs, op_minus<NumericT>, const InterfaceType * rhs)
  {
        if (lhs->is_constant())
        {
          NumericT val = lhs->unwrap();
          if (val == 0.0)
          {
            return new rt_binary_expr<InterfaceType>(new rt_constant<NumericT, InterfaceType>(-1),
                                                     new op_binary<op_mult<NumericT>, InterfaceType>(),
                                                     rhs->simplify());
          }

          return new rt_binary_expr<InterfaceType>(new rt_constant<NumericT, InterfaceType>(val),
                                                   new op_binary<op_minus<NumericT>, InterfaceType>(),
                                                   rhs->simplify());
        }
        else if (rhs->is_constant())
        {
          NumericT val = rhs->unwrap();
          if (val == 0.0)
            return lhs->simplify();

          return new rt_binary_expr<InterfaceType>(lhs->simplify(),
                                                   new op_binary<op_minus<NumericT>, InterfaceType>(),
                                                   new rt_constant<NumericT, InterfaceType>(val));
        }

        return new rt_binary_expr<InterfaceType>(lhs->simplify(),
                                                 new op_binary<op_minus<NumericT>, InterfaceType>(),
                                                 rhs->simplify());
  }

  /** @brief Checks whether an expression of the form e1 - e2 can be simplified */
  template <typename InterfaceType, typename NumericT>
  bool can_simplify_impl(const InterfaceType * lhs, op_minus<NumericT>, const InterfaceType * rhs)
  {
      if (lhs->is_constant())
      {
        if (lhs->unwrap() == 0.0)
        {
          //std::cout << "can_simplify: true in op_minus" << std::endl;
          return true;
        }
      }

      if (rhs->is_constant())
      {
        if (rhs->unwrap() == 0.0)
        {
          //std::cout << "can_simplify: true in op_minus" << std::endl;
          return true;
        }
      }

      return lhs->can_simplify() || rhs->can_simplify();
  }




  //
  // multiplication
  //
  /** @brief Implementation of the differentation of e1 * e1, where e1 and e2 are expressions. */
  template <typename InterfaceType, typename NumericT>
  InterfaceType * diff_impl(const InterfaceType * lhs, op_mult<NumericT>, const InterfaceType * rhs, const InterfaceType * diff_var)
  {
      return new rt_binary_expr<InterfaceType>( new rt_binary_expr<InterfaceType>(lhs->diff(diff_var),
                                                                                  new op_binary<op_mult<NumericT>, InterfaceType>(),
                                                                                  rhs->clone()),
                                                new op_binary<op_plus<NumericT>, InterfaceType>(),
                                                new rt_binary_expr<InterfaceType>(lhs->clone(),
                                                                                  new op_binary<op_mult<NumericT>, InterfaceType>(),
                                                                                  rhs->diff(diff_var) )
                            );
  }

  /** @brief Implementation of the simplification procedures for an expression of the form e1 * e2 */
  template <typename InterfaceType, typename NumericT>
  InterfaceType * simplify_impl(const InterfaceType * lhs, op_mult<NumericT>, const InterfaceType * rhs)
  {
        if (lhs->is_constant())
        {
          NumericT val = lhs->unwrap();
          if (val == 0.0)
            return new rt_constant<NumericT, InterfaceType>(0.0);
          if (val == 1.0)
            return rhs->simplify();

          return new rt_binary_expr<InterfaceType>(new rt_constant<NumericT, InterfaceType>(val),
                                                   new op_binary<op_mult<NumericT>, InterfaceType>(),
                                                   rhs->simplify());
        }
        else if (rhs->is_constant())
        {
          NumericT val = rhs->unwrap();
          if (val == 0.0)
            return new rt_constant<NumericT, InterfaceType>(0.0);
          if (val == 1.0)
            return lhs->simplify();

          return new rt_binary_expr<InterfaceType>(lhs->simplify(),
                                                   new op_binary<op_mult<NumericT>, InterfaceType>(),
                                                   new rt_constant<NumericT, InterfaceType>(val));
        }

        rt_vector_expr<InterfaceType> const * lhs_vec = dynamic_cast<rt_vector_expr<InterfaceType> const *>(lhs);
        rt_vector_expr<InterfaceType> const * rhs_vec = dynamic_cast<rt_vector_expr<InterfaceType> const *>(rhs);

        if (lhs_vec != NULL && rhs_vec != NULL)
          return (*lhs_vec * *rhs_vec).get()->clone();

        return new rt_binary_expr<InterfaceType>(lhs->simplify(),
                                                 new op_binary<op_mult<NumericT>, InterfaceType>(),
                                                 rhs->simplify());
  }

  /** @brief Checks whether an expression of the form e1 * e2 can be simplified */
  template <typename InterfaceType, typename NumericT>
  bool can_simplify_impl(const InterfaceType * lhs, op_mult<NumericT>, const InterfaceType * rhs)
  {
      if (lhs->is_constant())
      {
        NumericT val = lhs->unwrap();
        if (val == 0.0)
        {
          //std::cout << "can_simplify: true in op_mult" << std::endl;
          return true;
        }
        if (val == 1.0)
        {
          //std::cout << "can_simplify: true in op_mult" << std::endl;
          return true;
        }
      }
      if (rhs->is_constant())
      {
        NumericT val = rhs->unwrap();
        if (val == 0.0)
        {
          //std::cout << "can_simplify: true in op_mult" << std::endl;
          return true;
        }
        if (val == 1.0)
        {
          //std::cout << "can_simplify: true in op_mult" << std::endl;
          return true;
        }
      }

      rt_vector_expr<InterfaceType> const * lhs_vec = dynamic_cast<rt_vector_expr<InterfaceType> const *>(lhs);
      rt_vector_expr<InterfaceType> const * rhs_vec = dynamic_cast<rt_vector_expr<InterfaceType> const *>(rhs);

      if (lhs_vec != NULL && rhs_vec != NULL)
        return true;

      return lhs->can_simplify() || rhs->can_simplify();
  }




  //
  // division
  //
  /** @brief Implementation of the differentation of e1 / e1, where e1 and e2 are expressions. */
  template <typename InterfaceType, typename NumericT>
  InterfaceType * diff_impl(const InterfaceType * lhs, op_div<NumericT>, const InterfaceType * rhs, const InterfaceType * diff_var)
  {
    return new rt_binary_expr<InterfaceType>( new rt_binary_expr<InterfaceType>( new rt_binary_expr<InterfaceType>(lhs->diff(diff_var),
                                                                                                                   new op_binary<op_mult<NumericT>, InterfaceType>(),
                                                                                                                   rhs->clone()),
                                                                                 new op_binary<op_minus<NumericT>, InterfaceType>(),
                                                                                 new rt_binary_expr<InterfaceType>(lhs->clone(),
                                                                                                                   new op_binary<op_mult<NumericT>, InterfaceType>(),
                                                                                                                   rhs->diff(diff_var))
                                                                         ),
                                              new op_binary<op_div<NumericT>, InterfaceType>(),
                                              new rt_binary_expr<InterfaceType>(rhs->clone(),
                                                                                new op_binary<op_mult<NumericT>, InterfaceType>(),
                                                                                rhs->clone())
                                         );
  }

  /** @brief Implementation of the simplification procedures for an expression of the form e1 / e2 */
  template <typename InterfaceType, typename NumericT>
  InterfaceType * simplify_impl(const InterfaceType * lhs, op_div<NumericT>, const InterfaceType * rhs)
  {
    if (lhs->is_constant())
    {
      NumericT val = lhs->unwrap();
      if (val == 0.0)
        return new rt_constant<NumericT, InterfaceType>(0.0);

      return new rt_binary_expr<InterfaceType>(new rt_constant<NumericT, InterfaceType>(val),
                                               new op_binary<op_div<NumericT>, InterfaceType>(),
                                               rhs->simplify());
    }
    else if (rhs->is_constant())
    {
      NumericT val = rhs->unwrap();
      if (val == 1.0)
        return lhs->simplify();

      return new rt_binary_expr<InterfaceType>(lhs->simplify(),
                                               new op_binary<op_div<NumericT>, InterfaceType>(),
                                               new rt_constant<NumericT, InterfaceType>(val));
    }

    return new rt_binary_expr<InterfaceType>(lhs->simplify(),
                                             new op_binary<op_div<NumericT>, InterfaceType>(),
                                             rhs->simplify());
  }

  /** @brief Checks whether an expression of the form e1 / e2 can be simplified */
  template <typename InterfaceType, typename NumericT>
  bool can_simplify_impl(const InterfaceType * lhs, op_div<NumericT>, const InterfaceType * rhs)
  {
    if (lhs->is_constant())
    {
      if (lhs->unwrap() == 0.0)
      {
        //std::cout << "can_simplify: true in op_div" << std::endl;
        return true;
      }
    }
    if (rhs->is_constant())
    {
      if (rhs->unwrap() == 1.0)
      {
        //std::cout << "can_simplify: true in op_div" << std::endl;
        return true;
      }
    }
    return lhs->can_simplify() || rhs->can_simplify();
  }



}

#endif
