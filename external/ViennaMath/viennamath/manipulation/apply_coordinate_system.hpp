#ifndef VIENNAMATH_MANIPULATION_APPLY_COORDINATE_SYSTEM_HPP
#define VIENNAMATH_MANIPULATION_APPLY_COORDINATE_SYSTEM_HPP

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
#include "viennamath/manipulation/detail/unary_operations.hpp"
#include "viennamath/manipulation/substitute.hpp"
#include "viennamath/manipulation/diff.hpp"
#include "viennamath/runtime/equation.hpp"
#include "viennamath/runtime/function_symbol.hpp"

/** @file apply_coordinate_system.hpp
    @brief Applies a coordinate system to a coordinate system free form (using gradient, divergence, etc.) of an expression.
*/

namespace viennamath
{

  /** @brief Transforms the expression to a Cartesian coordinate system in one dimension
   *
   * @param ex     The runtime expression wrapper to be transformed
   */
  template <typename InterfaceType>
  rt_expr<InterfaceType> apply_coordinate_system(cartesian<1>, rt_expr<InterfaceType> const & ex)
  {
    rt_function_symbol<InterfaceType> u(0, unknown_tag<>());
    rt_function_symbol<InterfaceType> v(0, test_tag<>());
    rt_variable<InterfaceType> x(0);

    //replace grad:
    rt_expr<InterfaceType> new_ex = ex;
    rt_expr<InterfaceType> new_ex1(substitute(grad(u), diff(u, x), new_ex) );
    rt_expr<InterfaceType> new_ex2(substitute(grad(v), diff(v, x), new_ex1) );

    //replace div:
    rt_expr<InterfaceType> new_ex3(substitute(div(u), diff(u, x), new_ex2) );
    rt_expr<InterfaceType> new_ex4(substitute(div(v), diff(v, x), new_ex3) );

    return new_ex4;
  }


  /** @brief Transforms the expression to a Cartesian coordinate system in one dimension
   *
   * @param ex     The runtime unary expression to be transformed
   */
  template <typename InterfaceType>
  rt_expr<InterfaceType> apply_coordinate_system(cartesian<1>, rt_unary_expr<InterfaceType> const & ex)
  {
    rt_expr<InterfaceType> temp(ex);
    return apply_coordinate_system(cartesian<1>(), temp);
  }

  /** @brief Transforms the expression to a Cartesian coordinate system in one dimension
   *
   * @param ex     The runtime binary expression to be transformed
   */
  template <typename InterfaceType>
  rt_expr<InterfaceType> apply_coordinate_system(cartesian<1>, rt_binary_expr<InterfaceType> const & ex)
  {
    rt_expr<InterfaceType> temp(ex);
    return apply_coordinate_system(cartesian<1>(), temp);
  }


  /********* 2d *********/

  /** @brief Transforms the expression to a Cartesian coordinate system in two dimensions
   *
   * @param ex     The runtime expression wrapper to be transformed
   */
  template <typename InterfaceType>
  rt_expr<InterfaceType> apply_coordinate_system(cartesian<2>, rt_expr<InterfaceType> const & ex)
  {
    rt_function_symbol<InterfaceType> u(0, unknown_tag<>());
    rt_function_symbol<InterfaceType> v(0, test_tag<>());
    rt_variable<InterfaceType> x(0);
    rt_variable<InterfaceType> y(1);

    rt_vector_expr<InterfaceType> grad_u(2);
    grad_u[0] = diff(u, x);
    grad_u[1] = diff(u, y);

    rt_vector_expr<InterfaceType> grad_v(2);
    grad_v[0] = diff(v, x);
    grad_v[1] = diff(v, y);

    //replace grad:
    rt_expr<InterfaceType> new_ex1(substitute(grad(u), grad_u, ex) );
    rt_expr<InterfaceType> new_ex2(substitute(grad(v), grad_v, new_ex1) );

    //replace div:
    rt_expr<InterfaceType> new_ex3(substitute(div(u), diff(u, x) + diff(u, y), new_ex2) );
    rt_expr<InterfaceType> new_ex4(substitute(div(v), diff(v, x) + diff(v, y), new_ex3) );

    return new_ex4;
  }

  /** @brief Transforms the expression to a Cartesian coordinate system in two dimensions
   *
   * @param ex     The runtime unary expression to be transformed
   */
  template <typename InterfaceType>
  rt_expr<InterfaceType> apply_coordinate_system(cartesian<2>, rt_unary_expr<InterfaceType> const & ex)
  {
    rt_expr<InterfaceType> temp(ex);
    return apply_coordinate_system(cartesian<2>(), temp);
  }

  /** @brief Transforms the expression to a Cartesian coordinate system in two dimensions
   *
   * @param ex     The runtime binary expression to be transformed
   */
  template <typename InterfaceType>
  rt_expr<InterfaceType> apply_coordinate_system(cartesian<2>, rt_binary_expr<InterfaceType> const & ex)
  {
    rt_expr<InterfaceType> temp(ex);
    return apply_coordinate_system(cartesian<2>(), temp);
  }


  /******** 3d ***********/

  /** @brief Transforms the expression to a Cartesian coordinate system in two dimensions
   *
   * @param ex     The runtime expression wrapper to be transformed
   */
  template <typename InterfaceType>
  rt_expr<InterfaceType> apply_coordinate_system(cartesian<3>, rt_expr<InterfaceType> const & ex)
  {
    rt_function_symbol<InterfaceType> u(0, unknown_tag<>());
    rt_function_symbol<InterfaceType> v(0, test_tag<>());
    rt_variable<InterfaceType> x(0);
    rt_variable<InterfaceType> y(1);
    rt_variable<InterfaceType> z(2);

    rt_vector_expr<InterfaceType> grad_u(3);
    grad_u[0] = diff(u, x);
    grad_u[1] = diff(u, y);
    grad_u[2] = diff(u, z);

    rt_vector_expr<InterfaceType> grad_v(3);
    grad_v[0] = diff(v, x);
    grad_v[1] = diff(v, y);
    grad_v[2] = diff(v, z);

    //replace grad:
    rt_expr<InterfaceType> new_ex1(substitute(grad(u), grad_u, ex) );
    rt_expr<InterfaceType> new_ex2(substitute(grad(v), grad_v, new_ex1) );

    //replace div:
    rt_expr<InterfaceType> new_ex3(substitute(div(u), diff(u, x) + diff(u, y) + diff(u, z), new_ex2) );
    rt_expr<InterfaceType> new_ex4(substitute(div(v), diff(v, x) + diff(v, y) + diff(v, z), new_ex3) );

    return new_ex4;
  }

  /** @brief Transforms the expression to a Cartesian coordinate system in three dimensions
   *
   * @param ex     The runtime unary expression to be transformed
   */
  template <typename InterfaceType>
  rt_expr<InterfaceType> apply_coordinate_system(cartesian<3>, rt_unary_expr<InterfaceType> const & ex)
  {
    rt_expr<InterfaceType> temp(ex);
    return apply_coordinate_system(cartesian<3>(), temp);
  }

  /** @brief Transforms the expression to a Cartesian coordinate system in three dimensions
   *
   * @param ex     The runtime binary expression to be transformed
   */
  template <typename InterfaceType>
  rt_expr<InterfaceType> apply_coordinate_system(cartesian<3>, rt_binary_expr<InterfaceType> const & ex)
  {
    rt_expr<InterfaceType> temp(ex);
    return apply_coordinate_system(cartesian<3>(), temp);
  }


  //TODO: more and better compile time derivation






  //tries to automatically derive the weak formulation from the strong formulation
  /** @brief Convenience overload which automatically applies the coordinate transformation to the right hand left hand side of an equation */
  template <typename InterfaceType, id_type dim>
  rt_equation<InterfaceType> apply_coordinate_system(cartesian<dim>,
                                                     rt_equation<InterfaceType> const & equ)
  {
    return rt_equation<InterfaceType>( apply_coordinate_system(cartesian<dim>(), equ.lhs()),
                                       apply_coordinate_system(cartesian<dim>(), equ.rhs())
                                     );
  }

}

#endif
