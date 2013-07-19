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
#include "viennamath/runtime/vector_expr.hpp"


/*
 *  Tutorial: Using vector expressions
 */



int main()
{
  viennamath::variable x(0);
  viennamath::variable y(1);
  viennamath::variable z(2);
  viennamath::rt_constant<double> c4(4.0);
  viennamath::rt_constant<long> c6(6);
  viennamath::ct_constant<8> c8;
  viennamath::function_symbol u(0, viennamath::unknown_tag<>());
  viennamath::function_symbol v(0, viennamath::test_tag<>());
  viennamath::expr x_pow_4 = (x*x)*(x*x);

  viennamath::vector_expr  vec(3);
  vec[0] = x;
  //vec[1] = c6;             //uncomment this is you do not want to have the default constant '0' here
  //vec[2] = x*y + z - c6;   //uncomment this is you do not want to have the default constant '0' here

  viennamath::vector_expr  vec2(vec);
  vec[1] = c6;

  viennamath::vector_expr  vec3(vec2.size());
  vec3 = vec2;
  vec2[1] = c8;
  vec2[2] = c6;

  std::cout << "vec: " << vec << std::endl;
  std::cout << "vec2: " << vec2 << std::endl;
  std::cout << "vec3: " << vec3 << std::endl;
  std::cout << "----------------------" << std::endl;
  std::cout << "vec + vec2: " << vec + vec2 << std::endl;
  std::cout << "vec - vec2: " << vec - vec2 << std::endl;

  // check multiplication:
  std::cout << "z * vec2: "   << z * vec2   << std::endl;
  std::cout << "vec2 * z: "   << vec2 * z   << std::endl;

  std::cout << "c4 * vec2: "   << c4 * vec2   << std::endl;
  std::cout << "vec2 * c4: "   << vec2 * c4   << std::endl;

  std::cout << "c6 * vec2: "   << c6 * vec2   << std::endl;
  std::cout << "vec2 * c6: "   << vec2 * c6   << std::endl;

  std::cout << "c6 * vec2: "   << c8 * vec2   << std::endl;
  std::cout << "vec2 * c6: "   << vec2 * c8   << std::endl;

  // check division:
  std::cout << "c6 / vec2: "  << c6 / vec2  << std::endl;
  std::cout << "vec2 / z: "   << vec2 / c6  << std::endl;


  std::cout << "----------------------" << std::endl;
  std::cout << "vec * vec2: "   << vec * vec2  << std::endl;

  viennamath::expr vec_expr = vec;
  std::cout << "vec as expr: " << vec_expr << std::endl;


  viennamath::expr uv = u*v;
  viennamath::expr substi1 = viennamath::substitute(u, vec_expr, uv);
  viennamath::expr substi2 = viennamath::substitute(v, vec_expr, substi1);
  std::cout << "Substituted 1: " << substi1 << std::endl;
  std::cout << "Substituted 2: " << substi2 << std::endl;


  std::cout << "****************************************************" << std::endl;
  std::cout << "*****     TUTORIAL COMPLETED SUCCESSFULLY!     *****" << std::endl;
  std::cout << "****************************************************" << std::endl;

  return EXIT_SUCCESS;
}
