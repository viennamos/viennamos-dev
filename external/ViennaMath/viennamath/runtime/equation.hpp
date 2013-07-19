#ifndef VIENNAMATH_RUNTIME_EQUATION_HPP
#define VIENNAMATH_RUNTIME_EQUATION_HPP

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




#include <ostream>
#include "viennamath/forwards.h"

/** @file equation.hpp
    @brief Defines the ViennaMath runtime equation object and provides several generator functions for convenience.
*/

namespace viennamath
{

  /** @brief Runtime representation of a equation by a left hand side and a right hand side expresssion. */
  template <typename InterfaceType>
  class rt_equation
  {

    public:
      typedef rt_expr<InterfaceType>     value_type;
      typedef InterfaceType           interface_type;

      rt_equation() {};


      rt_equation(const rt_expr<InterfaceType> & lhs,
                  const rt_expr<InterfaceType> & rhs) : lhs_(lhs.get()->clone()), rhs_(rhs.get()->clone()) {}

      /** @brief Returns the left hand side of the equation */
      rt_expr<InterfaceType> const & lhs() const { return lhs_; }
      /** @brief Returns the right hand side of the equation */
      rt_expr<InterfaceType> const & rhs() const { return rhs_; }

    private:
      rt_expr<InterfaceType> lhs_;
      rt_expr<InterfaceType> rhs_;
  };

  /** @brief Convenience operator overload for streaming a runtime equation to std::cout or any other STL-compatible output stream. */
  template <typename InterfaceType>
  std::ostream& operator<<(std::ostream & stream, rt_equation<InterfaceType> const & e)
  {
    stream << "equation(" << e.lhs() << " = " << e.rhs() << ")";
    return stream;
  }




  //////// helper functions for creating equations (uniform interface for compile time and run time: ///////////


  //
  // run time
  //

  ////// scalar is first argument:
  /** @brief Generator for an equation with a (floating point) constant as left hand side and a ViennaMath runtime constant as right hand side */
  template <typename NumericT, typename InterfaceType, typename RHSType>
  rt_equation<InterfaceType> make_equation(typename InterfaceType::numeric_type lhs, rt_constant<NumericT, InterfaceType> const & rhs)
  {
    return rt_equation<InterfaceType>(lhs, rhs);
  }

  /** @brief Generator for an equation with a (floating point) constant as left hand side and a ViennaMath runtime variable as right hand side */
  template <typename InterfaceType>
  rt_equation<InterfaceType> make_equation(typename InterfaceType::numeric_type lhs, rt_variable<InterfaceType> const & rhs)
  {
    return rt_equation<InterfaceType>(lhs, rhs);
  }

  /** @brief Generator for an equation with a (floating point) constant as left hand side and a ViennaMath compiletime variable as right hand side */
  template <id_type id>
  rt_equation<> make_equation(default_numeric_type lhs, ct_variable<id> const & rhs)
  {
    return rt_equation<>(lhs, rhs);
  }

  /** @brief Generator for an equation with a (floating point) constant as left hand side and a ViennaMath runtime unary expression as right hand side */
  template <typename InterfaceType>
  rt_equation<InterfaceType> make_equation(typename InterfaceType::numeric_type lhs, rt_unary_expr<InterfaceType> const & rhs)
  {
    return rt_equation<InterfaceType>(lhs, rhs);
  }

  /** @brief Generator for an equation with a (floating point) constant as left hand side and a ViennaMath runtime binary expression as right hand side */
  template <typename InterfaceType>
  rt_equation<InterfaceType> make_equation(typename InterfaceType::numeric_type lhs, rt_binary_expr<InterfaceType> const & rhs)
  {
    return rt_equation<InterfaceType>(lhs, rhs);
  }

  /** @brief Generator for an equation with a (floating point) constant as left hand side and a ViennaMath runtime expression wrapper as right hand side */
  template <typename InterfaceType>
  rt_equation<InterfaceType> make_equation(typename InterfaceType::numeric_type lhs, rt_expr<InterfaceType> const & rhs)
  {
    return rt_equation<InterfaceType>(lhs, rhs);
  }


  //constant:
  /** @brief Generator for an equation with a ViennaMath runtime constant as left hand side */
  template <typename NumericT, typename InterfaceType, typename RHSType>
  rt_equation<InterfaceType> make_equation(rt_constant<NumericT, InterfaceType> const & lhs, RHSType const & rhs)
  {
    return rt_equation<InterfaceType>(lhs, rhs);
  }

  //variable:
  /** @brief Generator for an equation with a ViennaMath runtime variable as left hand side */
  template <typename InterfaceType, typename RHSType>
  rt_equation<InterfaceType> make_equation(rt_variable<InterfaceType> const & lhs, RHSType const & rhs)
  {
    return rt_equation<InterfaceType>(lhs, rhs);
  }

  //function_symbol:
  /** @brief Generator for an equation with a ViennaMath runtime function symbol as left hand side */
  template <typename InterfaceType, typename RHSType>
  rt_equation<InterfaceType> make_equation(rt_function_symbol<InterfaceType> const & lhs, RHSType const & rhs)
  {
    return rt_equation<InterfaceType>(lhs, rhs);
  }

  //unary:
  /** @brief Generator for an equation with a ViennaMath runtime unary expression as left hand side */
  template <typename InterfaceType, typename RHSType>
  rt_equation<InterfaceType> make_equation(rt_unary_expr<InterfaceType> const & lhs, RHSType const & rhs)
  {
    return rt_equation<InterfaceType>(lhs, rhs);
  }


  //binary
  /** @brief Generator for an equation with a ViennaMath runtime binary expression as left hand side */
  template <typename InterfaceType, typename RHSType>
  rt_equation<InterfaceType> make_equation(rt_binary_expr<InterfaceType> const & lhs, RHSType const & rhs)
  {
    return rt_equation<InterfaceType>(lhs, rhs);
  }

  //expr
  /** @brief Generator for an equation with a ViennaMath runtime expression wrapper as left hand side */
  template <typename InterfaceType, typename RHSType>
  rt_equation<InterfaceType> make_equation(rt_expr<InterfaceType> const & lhs, RHSType const & rhs)
  {
    return rt_equation<InterfaceType>(lhs, rhs);
  }

  //compile time with run time stuff also possible:
  /** @brief Generator for an equation with a ViennaMath compiletime binary expression as left hand side */
  template <typename LHS, typename OP, typename RHS, typename RHSType>
  rt_equation<> make_equation(ct_binary_expr<LHS, OP, RHS> const & lhs, RHSType const & rhs)
  {
    return rt_equation<>(lhs, rhs);
  }

  /** @brief Generator for an equation with a ViennaMath compiletime unary expression as left hand side */
  template <typename LHS, typename OP, typename RHSType>
  rt_equation<> make_equation(ct_unary_expr<LHS, OP> const & lhs, RHSType const & rhs)
  {
    return rt_equation<>(lhs, rhs);
  }

  /** @brief Generator for an equation with a ViennaMath compiletime constant as left hand side */
  template <long val, typename RHSType>
  rt_equation<> make_equation(ct_constant<val> const & lhs, RHSType const & rhs)
  {
    return rt_equation<>(lhs, rhs);
  }

  /** @brief Generator for an equation with a ViennaMath compiletime function symbol as left hand side */
  template <typename TAG, typename RHSType>
  rt_equation<> make_equation(ct_function_symbol<TAG> const & lhs, RHSType const & rhs)
  {
    return rt_equation<>(lhs, rhs);
  }

  /** @brief Generator for an equation with a ViennaMath compiletime variable as left hand side */
  template <id_type id, typename RHSType>
  rt_equation<> make_equation(ct_variable<id> const & lhs, RHSType const & rhs)
  {
    return rt_equation<>(lhs, rhs);
  }

}

#endif
