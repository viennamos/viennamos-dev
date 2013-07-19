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

// ViennaMath headers
#include "viennamath/expression.hpp"
#include "viennamath/manipulation/substitute.hpp"


/*
 *  Tutorial: Substitution of variables in expressions
 */


int main()
{
  viennamath::variable x(0);
  viennamath::variable y(1);
  viennamath::variable z(2);
  viennamath::rt_constant<double> c4(4.0);
  viennamath::rt_constant<long> c6(6);
  viennamath::ct_constant<8> c8;

  viennamath::expr e1 = x + y - z;
  viennamath::expr e2 = 1 + x - z;
  viennamath::expr e3 = x * y * z;
  viennamath::expr e4 = c4 + c6 - x;

  std::cout << "Substituting x with (1-y) in " << e1 << " results in: " << viennamath::substitute(x, 1-y, e1) << std::endl;
  std::cout << "Substituting y with (1-x) in " << e1 << " results in: " << viennamath::substitute(y, 1-x, e1) << std::endl;
  std::cout << std::endl;
  std::cout << "Substituting x with z in " << e2 << " results in: " << viennamath::substitute(x, z, e2) << std::endl;
  std::cout << "Substituting z with x in " << e2 << " results in: " << viennamath::substitute(z, x, e2) << std::endl;
  std::cout << std::endl;
  std::vector<viennamath::expr> search1(3); search1[0] = x; search1[1] = y; search1[2] = z;
  std::vector<viennamath::expr> repl1(3);     repl1[0] = 1;   repl1[1] = 2;   repl1[2] = 3;
  std::cout << "Substituting (x,y,z) with (1,2,3) in " << e3 << " results in: " << viennamath::substitute(search1, repl1, e3) << std::endl;

  std::cout << "****************************************************" << std::endl;
  std::cout << "*****     TUTORIAL COMPLETED SUCCESSFULLY!     *****" << std::endl;
  std::cout << "****************************************************" << std::endl;

  return EXIT_SUCCESS;
}
