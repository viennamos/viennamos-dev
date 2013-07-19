
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

#include "viennamath/runtime/equation.hpp"
#include "viennamath/compiletime/ct_equation.hpp"

//
// A compilation test for ViennaMath equations
//


int main()
{
  viennamath::ct_variable<0> x;
  viennamath::ct_variable<1> y;
  viennamath::ct_variable<2> z;
  viennamath::rt_constant<double> c4(4.0);
  viennamath::rt_constant<long> c6(6);
  viennamath::ct_constant<8> c8;
  viennamath::expr x_pow_4 = (x*x)*(x*x);
  viennamath::expr test(2.0);

  //compile time:
  std::cout << viennamath::make_equation(x*x + y, x + y) << std::endl;
  std::cout << viennamath::make_equation(x*x + y, x) << std::endl;
  std::cout << viennamath::make_equation(x*x + y, c8) << std::endl;

  std::cout << viennamath::make_equation(x, x + y) << std::endl;
  std::cout << viennamath::make_equation(x, y) << std::endl;
  std::cout << viennamath::make_equation(x, c8) << std::endl;

  std::cout << viennamath::make_equation(c8, x + y) << std::endl;
  std::cout << viennamath::make_equation(c8, x) << std::endl;
  std::cout << viennamath::make_equation(c8, c8) << std::endl;

  //run time:
  std::cout << viennamath::make_equation(x, 3.0) << std::endl;
  std::cout << viennamath::make_equation(viennamath::exp(x), 3.0) << std::endl;
  //std::cout << viennamath::make_equation(5.0, 3.0) << std::endl; //is not allowed
  std::cout << viennamath::make_equation(c4, 3.0) << std::endl; //this is mathematical nonsense! How to handle?
  std::cout << viennamath::make_equation(x*x + y, 3.0) << std::endl;
  std::cout << viennamath::make_equation(x_pow_4 + y*y, 5.0) << std::endl;
  std::cout << viennamath::make_equation(x_pow_4 + y*y, x*x + y) << std::endl;


  std::cout << "************************************************" << std::endl;
  std::cout << "*****     TEST COMPLETED SUCCESSFULLY!     *****" << std::endl;
  std::cout << "************************************************" << std::endl;

  return EXIT_SUCCESS;
}
