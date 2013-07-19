
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

#ifdef _MSC_VER      //Visual Studio complains about potentially dangerous things, which are perfectly legal in our context
  #pragma warning( disable : 4503 )     //truncated name decoration
#endif

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "viennamath/expression.hpp"
#include "viennamath/manipulation/eval.hpp"
#include "viennamath/manipulation/substitute.hpp"
#include "viennamath/manipulation/diff.hpp"
#include "viennamath/manipulation/expand.hpp"
#include "viennamath/manipulation/coefficient.hpp"
#include "viennamath/manipulation/drop_dependent_terms.hpp"
#include "viennamath/manipulation/integrate.hpp"

//
// A compilation test for the proper compiletime substitution
//


int main()
{
  viennamath::ct_constant<0> c0;
  viennamath::ct_constant<1> c1;
  viennamath::ct_constant<2> c2;
  viennamath::ct_constant<5> c5;
  viennamath::ct_constant<7> c7;

  viennamath::ct_variable<0> x;
  viennamath::ct_variable<1> y;
  viennamath::ct_variable<2> z;

  std::cout << "Evaluate (1-x-y)xy at (1,2,5):" << std::endl;
  std::cout << viennamath::eval( (c1 - x - y) * x * y, viennamath::make_vector(c1, c2, c5) ) << std::endl;

  std::cout << "Replace ( (1-x-y)xy, x) and evaluate at (1,2,5):" << std::endl;
  std::cout << viennamath::substitute(x, z, (c1 - x - y) * x * y) << std::endl;
  std::cout << viennamath::eval(
                  viennamath::substitute(x, z, (c1 - x - y) * x * y),
                  viennamath::make_vector(c1, c2, c5)
               ) << std::endl;

  std::cout << "Differentiate (1-x-y)xy with respect to x and evaluate the result at (1,2,5):" << std::endl;
  std::cout << viennamath::diff((c1 - x - y) * x * y, x) << std::endl;
  /*std::cout << viennamath::ct_eval(
                  viennamath::diff((c1 - x - y) * x * y, x),
                  viennamath::make_vector(c1, c2, c5)
               ) << std::endl;*/
  std::cout << viennamath::eval(
                  viennamath::diff((c1 - x - y) * x * y, x),
                  viennamath::make_vector(c1, c2, c5)
               ) << std::endl;

  std::cout << "Evaluation test of super-complicated expression:" << std::endl;
  /*std::cout << viennamath::ct_eval(
                viennamath::diff((c1 - x - y/x) * x * y / (c2 - y / (x + y)) * (x + y) / (z+x), x) - c1 / (c2  + c2),
                viennamath::make_vector(c1, c2, c5)
              ) << std::endl;*/
  std::cout << viennamath::eval(
                viennamath::diff((c1 - x - y/x) * x * y / (c2 - y / (x + y)) * (x + y) / (z+x), x) - c1 / (c2  + c2),
                viennamath::make_vector(c1, c2, c5)
              ) << std::endl;


  std::cout << "Expanding (1-x)*(1-y)*(1-z):" << std::endl;
  std::cout << viennamath::expand( (c1 - x) * (c1 - y) * (c1 - z) ) << std::endl;
  std::cout << viennamath::eval(
                (c1 - x) * (c1 - y) * (c1 - z),
                viennamath::make_vector(c1, c2, c5)
              ) << std::endl;
  std::cout << viennamath::eval(
                viennamath::expand( (c1 - x) * (c1 - y) * (c1 - z) ),
                viennamath::make_vector(c1, c2, c5)
              ) << std::endl;


  std::cout << "Extracting coefficients from (1-x)*(1-y)*(1-z):" << std::endl;
  std::cout << "x: " << viennamath::coefficient(x, (c1 - x) * (c1 - y) * (c1 - z) ) << std::endl;
  std::cout << "y: " << viennamath::coefficient(y, (c1 - x) * (c1 - y) * (c1 - z) ) << std::endl;
  std::cout << "z: " << viennamath::coefficient(z, (c1 - x) * (c1 - y) * (c1 - z) ) << std::endl;

  std::cout << "Dropping coefficients from (1-x)*(1-y)*(1-z):" << std::endl;
  std::cout << "Drop x-terms: " << viennamath::drop_dependent_terms(x, (c1 - x) * (c1 - y) * (c1 - z) ) << std::endl;
  std::cout << "Drop y-terms: " << viennamath::drop_dependent_terms(y, (c1 - x) * (c1 - y) * (c1 - z) ) << std::endl;
  std::cout << "Drop z-terms: " << viennamath::drop_dependent_terms(z, (c1 - x) * (c1 - y) * (c1 - z) ) << std::endl;


  std::cout << "Integrate x*y:" << std::endl;
  std::cout << "with respect to x on [0,1]: " << viennamath::integrate( viennamath::make_interval(c0, c1),
                                                                        x*y,
                                                                        x) << std::endl;
  std::cout << "with respect to y on [0,1]: " << viennamath::integrate( viennamath::make_interval(c0, c1),
                                                                        x*y,
                                                                        y) << std::endl;

  std::cout << "Integrate (1-x)*(1-y)*x*y: " << std::endl;

  std::cout << "with respect to x on [0,1]: " << viennamath::integrate( viennamath::make_interval(c0, c1),
                                                                         (c1-x)*(c1-y)*x*y,
                                                                         x) << std::endl;
  std::cout << "with respect to y on [0,1]: " << viennamath::integrate( viennamath::make_interval(c0, c1),
                                                                         (c1-x)*(c1-y)*x*y,
                                                                         y) << std::endl;


  std::cout << "Integration on unit triangle:" << std::endl;
  std::cout << "1: " << viennamath::integrate( viennamath::make_interval(c0, c1),
                                               viennamath::integrate( viennamath::make_interval(c0, c1 - x),
                                                                      c1,
                                                                      y),
                                               x) << std::endl;
  //linear
  std::cout << "x: " << viennamath::integrate( viennamath::make_interval(c0, c1),
                                               viennamath::integrate( viennamath::make_interval(c0, c1 - x),
                                                                      x,
                                                                      y),
                                               x) << std::endl;
  std::cout << "y: " << viennamath::integrate( viennamath::make_interval(c0, c1),
                                               viennamath::integrate( viennamath::make_interval(c0, c1 - x),
                                                                      y,
                                                                      y),
                                               x) << std::endl;
  std::cout << "1-x-y: " << viennamath::integrate( viennamath::make_interval(c0, c1),
                                                   viennamath::integrate( viennamath::make_interval(c0, c1 - x),
                                                                          c1 - x - y,
                                                                          y),
                                                    x) << std::endl;

  //quadratic:
  std::cout << "x^2: " << viennamath::integrate( viennamath::make_interval(c0, c1),
                                                 viennamath::integrate( viennamath::make_interval(c0, c1 - x),
                                                                        x*x,
                                                                        y),
                                                 x) << std::endl;
  std::cout << "y^2: " << viennamath::integrate( viennamath::make_interval(c0, c1),
                                                 viennamath::integrate( viennamath::make_interval(c0, c1 - x),
                                                                        y*y,
                                                                        y),
                                                 x) << std::endl;

  std::cout << "(1-x-y)^2: " << viennamath::integrate( viennamath::make_interval(c0, c1),
                                                       viennamath::integrate( viennamath::make_interval(c0, c1 - x),
                                                                              (c1 - x - y) * (c1 - x - y),
                                                                              y),
                                                       x) << std::endl;
  std::cout << "x*y: " << viennamath::integrate( viennamath::make_interval(c0, c1),
                                                 viennamath::integrate( viennamath::make_interval(c0, c1 - x),
                                                                        x*y,
                                                                        y),
                                                 x) << std::endl;
  std::cout << "x*(1-x-y): " << viennamath::integrate( viennamath::make_interval(c0, c1),
                                                       viennamath::integrate( viennamath::make_interval(c0, c1 - x),
                                                                              x*(c1 - x - y),
                                                                              y),
                                                       x) << std::endl;
  std::cout << "y*(1-x-y): " << viennamath::integrate( viennamath::make_interval(c0, c1),
                                                       viennamath::integrate( viennamath::make_interval(c0, c1 - x),
                                                                              y*(c1 - x - y),
                                                                              y),
                                                       x) << std::endl;

  std::cout << "************************************************" << std::endl;
  std::cout << "*****     TEST COMPLETED SUCCESSFULLY!     *****" << std::endl;
  std::cout << "************************************************" << std::endl;

  return EXIT_SUCCESS;
}
