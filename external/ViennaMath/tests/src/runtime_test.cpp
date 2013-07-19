
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

//
// A test for the proper functionality of operator overloads of runtime expressions
//


template <typename E, typename T, typename U>
void operations_test(E & e, T const & t, U const & u, double t2, double u2)
{
  e = t + u; evaluations_test(e, t2 + u2);
  e = u + t; evaluations_test(e, u2 + t2);
  e = t - u; evaluations_test(e, t2 - u2);
  e = u - t; evaluations_test(e, u2 - t2);
  e = t * u; evaluations_test(e, t2 * u2);
  e = u * t; evaluations_test(e, u2 * t2);
  e = t / u; evaluations_test(e, t2 / u2);
  e = u / t; evaluations_test(e, u2 / t2);
}

template <typename E, typename T, typename U>
void operations_test(E & e, T const & t, U const & u)
{
  operations_test(e, t, u, viennamath::eval(t, viennamath::make_vector(4, 6, 8)),
                           viennamath::eval(u, viennamath::make_vector(4, 6, 8)) );
}


template <typename E>
void evaluations_test(E & e, double ref_solution)
{
  viennamath::rt_constant<double> c4(4.0);
  viennamath::rt_constant<long> c6(6);
  viennamath::ct_constant<8> c8;

  std::vector<double> p(3);
  p[0] = 4;
  p[1] = 6;
  p[2] = 8;


  std::cout << e << " at STL (" << p[0] << ", " << p[1] << ", " << p[2] << ")"
            << " = " << e(p)
            << " = " << viennamath::eval(e,p)
            << " (reference solution: " << ref_solution << ")" << std::endl;
  assert(viennamath::eval(e, p) == ref_solution);

  std::cout << e << " at vector_3 (" << viennamath::make_vector(c4, c6, c8) << ")"
            << " = " << e(viennamath::make_vector(c4, c6, c8))
            << " = " << viennamath::eval(e, viennamath::make_vector(c4, c6, c8))
            << " (reference solution: " << ref_solution << ")" << std::endl;
  assert(viennamath::eval(e, viennamath::make_vector(c4, c6, c8)) == ref_solution);

}


template <typename E>
void unary_test(E const & e, double ref_solution)
{
  viennamath::rt_constant<double> c4(4.0);
  viennamath::rt_constant<long> c6(6);
  viennamath::ct_constant<8> c8;

  std::vector<double> p(3);
  p[0] = 4;
  p[1] = 6;
  p[2] = 8;

  std::cout << e << " at STL (" << p[0] << ", " << p[1] << ", " << p[2] << ")"
            << " = " << e(p)
            << " = " << viennamath::eval(e,p)
            << " (reference solution: " << ref_solution << ")" << std::endl;
  assert(viennamath::eval(e, p) == ref_solution);

  std::cout << e << " at vector_3 (" << viennamath::make_vector(c4, c6, c8) << ")"
            << " = " << e(viennamath::make_vector(c4, c6, c8))
            << " = " << viennamath::eval(e, viennamath::make_vector(c4, c6, c8))
            << " (reference solution: " << ref_solution << ")" << std::endl;
  assert(viennamath::eval(e, viennamath::make_vector(c4, c6, c8)) == ref_solution);
}

template <typename E>
void unary_test(E const & e)
{
  std::vector<double> p(3);
  p[0] = 4;
  p[1] = 6;
  p[2] = 8;

  unary_test(viennamath::exp(e),   exp(viennamath::eval(e,p)) );
  unary_test(viennamath::sin(e),   sin(viennamath::eval(e,p)) );
  unary_test(viennamath::cos(e),   cos(viennamath::eval(e,p)) );
  unary_test(viennamath::tan(e),   tan(viennamath::eval(e,p)) );
  unary_test(viennamath::fabs(e),  fabs(viennamath::eval(e,p)) );
  unary_test(viennamath::sqrt(e),  sqrt(viennamath::eval(e,p)) );
  unary_test(viennamath::log(e),   log(viennamath::eval(e,p)) );
  unary_test(viennamath::log10(e), log10(viennamath::eval(e,p)) );
}



int main()
{
  viennamath::variable x(0);
  viennamath::variable y(1);
  viennamath::variable z(2);
  viennamath::rt_constant<double> c4(4.0);
  viennamath::rt_constant<long> c6(6);
  viennamath::ct_constant<8> c8;
  viennamath::expr e1 = x + y;
  viennamath::expr e2 = c4 + c8;
  viennamath::expr e3 = e1 + e2;
  viennamath::expr e;

  //compilation checks:
  operations_test(e, e1, e2);
  operations_test(e, e1, x);
  operations_test(e, e1, c4);
  operations_test(e, e1, c6);
  operations_test(e, e1, c8);
  operations_test(e, e1, 2.7172);

  operations_test(e, x, e2);
  operations_test(e, x, x);
  operations_test(e, x, c4);
  operations_test(e, x, c6);
  operations_test(e, x, c8);
  operations_test(e, x, 2.7172);

  operations_test(e, y, e2);
  operations_test(e, y, x);
  operations_test(e, y, c4);
  operations_test(e, y, c6);
  operations_test(e, y, c8);
  operations_test(e, y, 2.7172);

  operations_test(e, c4, e2);
  operations_test(e, c4, x);
  operations_test(e, c4, c4);
  operations_test(e, c4, c6);
  operations_test(e, c4, c8);
  operations_test(e, c4, 2.7172);

  operations_test(e, c6, e2);
  operations_test(e, c6, x);
  operations_test(e, c6, c4);
  operations_test(e, c6, c6);
  operations_test(e, c6, c8);
  operations_test(e, c6, 2.7172);

  operations_test(e, c8, e2);
  operations_test(e, c8, x);
  operations_test(e, c8, c4);
  operations_test(e, c8, c6);
  operations_test(e, c8, c8);
  operations_test(e, c8, 2.7172);

  operations_test(e, 2.7172, e2);
  operations_test(e, 2.7172, x);
  operations_test(e, 2.7172, c4);
  operations_test(e, 2.7172, c6);
  operations_test(e, 2.7172, c8);
  operations_test(e, 2.7172, 2.7172);

  std::cout << "************************************************" << std::endl;
  std::cout << "*****     TEST COMPLETED SUCCESSFULLY!     *****" << std::endl;
  std::cout << "************************************************" << std::endl;
  std::cout << std::endl;


  std::cout << x << ": " << viennamath::eval(x, c4) << std::endl;
  std::cout << e2  << ": " << viennamath::eval(e2, c4) << std::endl;
  e = x + e2;
  std::cout << e << ": " << viennamath::eval(e, c4) << std::endl;
  std::cout << e << ": " << viennamath::eval(e, c6) << std::endl;
  std::cout << e << ": " << viennamath::eval(e, c8) << std::endl;

  unary_test(x);
  unary_test(x+c4);
  unary_test(y*c6);
  unary_test(y/x);
  unary_test(c6+c8);



  e = x - c4 - y;
  std::cout << "Replacing x in " << e
            << " with " << c6
            << " leads to " << viennamath::substitute(x, c6, e) << std::endl;

  e = viennamath::exp(x) - c4 - y;
  std::cout << "Replacing x in " << e
            << " with " << c6
            << " leads to " << viennamath::substitute(x, c6, e) << std::endl;

  std::cout << "--- Involved types ---" << std::endl;
  std::cout << "x: " << x << std::endl;
  std::cout << "y: " << y << std::endl;
  std::cout << "z: " << z << std::endl;
  std::cout << "c4: " << c4 << std::endl;
  std::cout << "c6: " << c6 << std::endl;
  std::cout << "c8: " << c8 << std::endl;
  std::cout << "e1: " << e1 << std::endl;
  std::cout << "e2: " << e2 << std::endl;
  std::cout << "e3: " << e3 << std::endl;
  std::cout << "e: " << e << std::endl;

  std::cout << std::endl;

  std::cout << "************************************************" << std::endl;
  std::cout << "*****     TEST COMPLETED SUCCESSFULLY!     *****" << std::endl;
  std::cout << "************************************************" << std::endl;


  return EXIT_SUCCESS;
}
