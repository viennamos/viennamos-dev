
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

#include "viennamath/expression.hpp"
#include "viennamath/manipulation/eval.hpp"
#include "viennamath/manipulation/substitute.hpp"
#include "viennamath/manipulation/diff.hpp"

//
// A compilation test for the proper functionality of differentiation of runtime expressions
//


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
  viennamath::expr e;

  //compile time:
  std::cout << viennamath::diff(x+y, x) << std::endl;
  std::cout << viennamath::diff(x+y, y) << std::endl;
  std::cout << viennamath::diff(x+y, z) << std::endl;

  std::cout << viennamath::diff(x*y+c8, x) << std::endl;
  std::cout << viennamath::diff(x*y+c8, y) << std::endl;
  std::cout << viennamath::diff(x*y+c8, z) << std::endl;

  std::cout << viennamath::diff(x/y-c8, x) << std::endl;
  std::cout << viennamath::diff(x/y-c8, y) << std::endl;
  std::cout << viennamath::diff(x/y-c8, z) << std::endl;

  //run time:
  std::cout << viennamath::diff(e1, x) << std::endl;
  std::cout << viennamath::diff(e1, y) << std::endl;
  std::cout << viennamath::diff(e1, z) << std::endl;

  std::cout << viennamath::diff(e2, x) << std::endl;
  std::cout << viennamath::diff(e2, y) << std::endl;
  std::cout << viennamath::diff(e2, z) << std::endl;

  std::cout << viennamath::diff(e3, x) << std::endl;
  std::cout << viennamath::diff(e3, y) << std::endl;
  std::cout << viennamath::diff(e3, z) << std::endl;


  std::cout << viennamath::diff(viennamath::exp(x), x) << std::endl;
  std::cout << viennamath::diff(viennamath::exp(e1), x) << std::endl;

  std::cout << viennamath::diff(viennamath::sin(x), x) << std::endl;
  std::cout << viennamath::diff(viennamath::sin(e1), x) << std::endl;

  std::cout << viennamath::diff(viennamath::cos(x), x) << std::endl;
  std::cout << viennamath::diff(viennamath::cos(e1), x) << std::endl;

  std::cout << viennamath::diff(viennamath::tan(x), x) << std::endl;
  std::cout << viennamath::diff(viennamath::tan(e1), x) << std::endl;

  std::cout << viennamath::diff(viennamath::sqrt(x), x) << std::endl;
  std::cout << viennamath::diff(viennamath::sqrt(e1), x) << std::endl;

  std::cout << viennamath::diff(viennamath::log(x), x) << std::endl;
  std::cout << viennamath::diff(viennamath::log(e1), x) << std::endl;

  std::cout << viennamath::diff(viennamath::log10(x), x) << std::endl;
  std::cout << viennamath::diff(viennamath::log10(e1), x) << std::endl;

  //
  // Gateaux derivatives
  //
  viennamath::function_symbol f(0);
  viennamath::function_symbol g(1);

  std::cout << "-- Gateaux derivatives --" << std::endl;
  std::cout << viennamath::diff(f, f) << std::endl;
  std::cout << viennamath::diff(g, f) << std::endl;
  std::cout << viennamath::diff(f + g, g) << std::endl;
  std::cout << viennamath::diff(f - g, g) << std::endl;
  std::cout << viennamath::diff(f * g, f) << std::endl;
  std::cout << viennamath::diff(f / g, f) << std::endl;
  std::cout << viennamath::diff(4.0 * f * g, f) << std::endl;
  std::cout << viennamath::diff(viennamath::grad(f), f) << std::endl;
  std::cout << viennamath::diff(viennamath::grad(g), f) << std::endl;
  std::cout << viennamath::diff(f*viennamath::grad(f), f) << std::endl;
  std::cout << viennamath::diff(g*viennamath::grad(f), f) << std::endl;

  std::cout << "************************************************" << std::endl;
  std::cout << "*****     TEST COMPLETED SUCCESSFULLY!     *****" << std::endl;
  std::cout << "************************************************" << std::endl;

  return EXIT_SUCCESS;
}
