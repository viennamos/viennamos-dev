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


#include <iostream>
#include <vector>
#include <stdlib.h>
#include <assert.h>

// ViennaMath headers
#include "viennamath/expression.hpp"
#include "viennamath/manipulation/eval.hpp"
#include "viennamath/manipulation/substitute.hpp"
#include "viennamath/manipulation/diff.hpp"

#include "viennamath/runtime/equation.hpp"


/*
 *  Tutorial: How to set up a Newton solver for a system of two nonlinear equations
 */


template <typename InterfaceType>
std::vector<double> newton_solve_impl(std::vector<viennamath::rt_equation<InterfaceType> > const & equations,
                                      std::vector<double> guess)
{
  viennamath::variable x(0);
  viennamath::variable y(1);

  //
  // Bring to form
  //   f1(x,y) = 0
  //   f2(x,y) = 0
  //
  viennamath::rt_expr<InterfaceType> f1 = equations[0].lhs() - equations[0].rhs();
  viennamath::rt_expr<InterfaceType> f2 = equations[1].lhs() - equations[1].rhs();

  // compute residuals:
  double res_f1 = f1(guess);
  double res_f2 = f2(guess);

  //
  // Entries of Jacobian matrix:
  //
  viennamath::rt_expr<InterfaceType> df1_dx = viennamath::diff(f1, x);
  viennamath::rt_expr<InterfaceType> df1_dy = viennamath::diff(f1, y);
  viennamath::rt_expr<InterfaceType> df2_dx = viennamath::diff(f2, x);
  viennamath::rt_expr<InterfaceType> df2_dy = viennamath::diff(f2, y);

  //TODO Solver for general linear system here

  //In the meanwhile, we use the explicit solution formula for a 2-by-2 system:
  double det = df1_dx(guess) * df2_dy(guess) - df2_dx(guess) * df1_dy(guess);
  double update_x = (res_f1*df2_dy(guess) - df1_dy(guess)*res_f2) / det;
  double update_y = (df1_dx(guess)*res_f2 - res_f1*df2_dx(guess)) / det;

  std::vector<double> ret;
  ret.push_back( guess[0] - update_x );
  ret.push_back( guess[1] - update_y );

  return ret;
}

template <typename InterfaceType>
std::vector<double> newton_solve(std::vector<viennamath::rt_equation<InterfaceType> > const & equations,
                                 std::vector<double> init_guess)
{
  viennamath::variable x(0);
  viennamath::variable y(1);
  double norm_res = 1;

  std::vector<double> x_k = init_guess;

  //
  // Bring to form
  //   f1(x,y) = 0
  //   f2(x,y) = 0
  //
  viennamath::rt_expr<InterfaceType> f1 = equations[0].lhs() - equations[0].rhs();
  viennamath::rt_expr<InterfaceType> f2 = equations[1].lhs() - equations[1].rhs();

  //iterate until convergence:
  while (norm_res > 1e-10 && norm_res < 1e10)
  {
    x_k = newton_solve_impl(equations, x_k);
    norm_res = ::sqrt(f1(x_k)*f1(x_k) + f2(x_k)*f2(x_k));
    std::cout << "Current x_k: (" << x_k[0] << ", " << x_k[1] << "), norm of residual: " << norm_res << std::endl;
 }


  return x_k;
}



int main()
{
  viennamath::variable x(0);
  viennamath::variable y(1);
  viennamath::variable z(2);
  viennamath::rt_constant<double> c4(4.0);
  viennamath::rt_constant<long> c6(6);
  viennamath::ct_constant<8> c8;
  viennamath::expr x_pow_4 = (x*x)*(x*x);

  //
  // Set up nonlinear system:
  //   x^2 + y   = 3.0
  //   x^4 - y^2 = 3.0
  // Solution is known to be (sqrt(2), 1)
  //
  std::vector<viennamath::equation > equ_system;
  equ_system.push_back(viennamath::make_equation(x*x + y, 3.0));
  equ_system.push_back(viennamath::make_equation(x_pow_4 - y*y, 3.0));

  //
  //Initial guess:
  //
  std::vector<double> init_guess;
  init_guess.push_back(1.0);
  init_guess.push_back(2.0);

  //
  // Launch Newton solver
  //
  std::vector<double> result = newton_solve(equ_system, init_guess);

  std::cout << "Result: (" << result[0] << ", " << result[1] << ")" << std::endl;


  std::cout << "****************************************************" << std::endl;
  std::cout << "*****     TUTORIAL COMPLETED SUCCESSFULLY!     *****" << std::endl;
  std::cout << "****************************************************" << std::endl;

  return EXIT_SUCCESS;
}
