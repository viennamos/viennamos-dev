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

// Main ViennaMath header:
#include "viennamath/expression.hpp"

/*
 *   Tutorial: Basic handling and evaluation of types in ViennaMath
 */


int main()
{
  viennamath::variable x(0);
  viennamath::variable y(1);
  viennamath::variable z(2);
  viennamath::rt_constant<double> c4(4.0);
  viennamath::rt_constant<long> c6(6);
  viennamath::ct_constant<8> c8;

  std::vector<double> p(3);
  p[0] = 1;
  p[1] = 2;
  p[2] = 3;

  std::cout << "--- Involved types ---" << std::endl;
  std::cout << "x: " << x << std::endl;
  std::cout << "y: " << y << std::endl;
  std::cout << "z: " << z << std::endl;
  std::cout << "c4: " << c4 << std::endl;
  std::cout << "c6: " << c6 << std::endl;
  std::cout << "c8: " << c8 << std::endl;

  std::cout << "--- Evaluation of constants ---" << std::endl;
  std::cout << "c4(" << p[0] << "," << p[1] << ", " << p[2] << ") = " << c4(p) << std::endl;
  std::cout << "c4(" << c4 << ") = " << c4(c4) << std::endl;
  std::cout << "c4(" << c8 << ") = " << c4(c8) << std::endl;

  std::cout << "c8(" << p[0] << "," << p[1] << ", " << p[2] << ") = " << c8(p) << std::endl;
  std::cout << "c8(" << c4 << ") = " << c8(c4) << std::endl;
  std::cout << "c8(" << c8 << ") = " << c8(c8) << std::endl;

  std::cout << "--- Evaluation of variables via std::vector ---" << std::endl;
  std::cout << "x(" << p[0] << "," << p[1] << ", " << p[2] << ") = " << x(p) << std::endl;
  std::cout << "y(" << p[0] << "," << p[1] << ", " << p[2] << ") = " << y(p) << std::endl;
  std::cout << "z(" << p[0] << "," << p[1] << ", " << p[2] << ") = " << z(p) << std::endl;

  std::cout << "--- Evaluation of variables via constant c4 ---" << std::endl;
  std::cout << "x(" << c4 << ") = " << x(c4) << std::endl;
  //std::cout << "y(" << c4 << ") = " << y(c4) << std::endl;

  std::cout << "--- Evaluation of variables via constant c8 ---" << std::endl;
  std::cout << "x(" << c8 << ") = " << x(c8) << std::endl;
  //std::cout << "y(" << c8 << ") = " << y(c8) << std::endl;

  std::cout << "--- Evaluation of variables via vector (c4, c8) ---" << std::endl;
  std::cout << "x(" << viennamath::make_vector(c4, c6, c8) << ") = " << x( viennamath::make_vector(c4, c6, c8) ) << std::endl;
  std::cout << "y(" << viennamath::make_vector(c4, c6, c8) << ") = " << y( viennamath::make_vector(c4, c6, c8) ) << std::endl;
  std::cout << "z(" << viennamath::make_vector(c4, c6, c8) << ") = " << z( viennamath::make_vector(c4, c6, c8) ) << std::endl;

  long long_x = x( viennamath::make_vector(c4, c6, c8) );
  long long_y = y( viennamath::make_vector(c4, c6, c8) );
  long long_z = z( viennamath::make_vector(c4, c6, c8) );
  std::cout << "Long variables: " << long_x << ", " << long_y << ", " << long_z << std::endl;

  long double_x = x( viennamath::make_vector(c4, c6, c8) );
  long double_y = y( viennamath::make_vector(c4, c6, c8) );
  long double_z = z( viennamath::make_vector(c4, c6, c8) );
  std::cout << "Double variables: " << double_x << ", " << double_y << ", " << double_z << std::endl;

  std::cout << "****************************************************" << std::endl;
  std::cout << "*****     TUTORIAL COMPLETED SUCCESSFULLY!     *****" << std::endl;
  std::cout << "****************************************************" << std::endl;

  return EXIT_SUCCESS;
}
