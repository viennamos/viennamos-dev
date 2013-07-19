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
#include "viennamath/runtime/numerical_quadrature.hpp"

/*
 *   Tutorial: How to handle integrals and numerical quadrature
 */


int main()
{
  viennamath::ct_variable<0> x;
  viennamath::ct_variable<1> y;
  viennamath::ct_variable<2> z;
  viennamath::rt_constant<double> c4(4.0);
  viennamath::rt_constant<long> c6(6);
  viennamath::ct_constant<8> c8;
  viennamath::expr e1 = x + y;
  viennamath::expr e2 = x*y+c8;
  viennamath::expr e3 = x/y-c8;
  viennamath::expr e4 = x + c8;

  //
  // Define a few intervals (runtime and compiletime)
  //

  viennamath::interval rt_int01(0, 1);
  std::cout << "Interval: " << rt_int01 << std::endl;

  viennamath::interval rt_intxy(x, y);
  std::cout << "Interval: " << rt_intxy << std::endl;

  viennamath::ct_interval<viennamath::ct_constant<0>,
                          viennamath::ct_constant<1> > ct_int01;
  std::cout << "Interval: " << ct_int01 << std::endl;

  viennamath::ct_interval<viennamath::ct_variable<0>,
                          viennamath::ct_variable<1> > ct_intxy;
  std::cout << "Interval: " << ct_intxy << std::endl;

  //
  // Set up the integral \int_0^1 x+8 dx and evaluate it using a 1-point-Gauss rule:
  //

  viennamath::expr e4_int = viennamath::integral( viennamath::interval(0, 1), e4, x );

  std::cout << "Integral: " << e4_int << std::endl;

  std::cout << "Starting evaluation..." << std::endl;
  viennamath::numerical_quadrature integrator(new viennamath::gauss_quad_1());
  std::cout << "Evaluated, type 1: " << integrator(e4_int) << std::endl;
  std::cout << "Evaluated, type 2: " << integrator(viennamath::interval(0, 1), e4, x) << std::endl;

  //
  // Binary integral expression:
  //
  viennamath::expr e44_int = viennamath::integral( viennamath::interval(0, 1), e4, x )
                            - viennamath::integral( viennamath::interval(0, 1), e4, x );

  std::cout << "Integral: " << e44_int << std::endl;
  std::cout << "Evaluated, type 1: " << integrator(e44_int) << std::endl;


  //
  // Handling of a symbolic integration interval, which is then substituted to [0,1] with integration over x:
  //

  std::cout << std::endl;
  viennamath::expr my_integral = viennamath::integral(viennamath::symbolic_interval(), e4);
  std::cout << "Symbolic integral: " << my_integral << std::endl;

  viennamath::expr my_integral2 = viennamath::substitute(viennamath::symbolic_interval(),
                                                         std::make_pair(viennamath::interval(0, 1), x),
                                                         my_integral);
  std::cout << "Symbolic integral, substituted: " << my_integral2 << std::endl;


  std::cout << "****************************************************" << std::endl;
  std::cout << "*****     TUTORIAL COMPLETED SUCCESSFULLY!     *****" << std::endl;
  std::cout << "****************************************************" << std::endl;

  return EXIT_SUCCESS;
}
