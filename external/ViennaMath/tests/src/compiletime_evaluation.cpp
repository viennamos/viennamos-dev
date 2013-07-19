
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
#include <math.h>

#include "viennamath/expression.hpp"
#include "viennamath/manipulation/eval.hpp"

//
// A test for the proper functionality of operator overloads of runtime expressions
//

template <typename T, typename U>
void operations_test(T const & t, U const & u, double t2, double u2)
{
  std::cout << "LHS: " << t << ", RHS: " << u << std::endl;
  evaluations_test(t + u, t2 + u2);
  evaluations_test(u + t, u2 + t2);
  evaluations_test(t - u, t2 - u2);
  evaluations_test(u - t, u2 - t2);
  evaluations_test(t * u, t2 * u2);
  evaluations_test(u * t, u2 * t2);
  evaluations_test(t / u, t2 / u2);
  evaluations_test(u / t, u2 / t2);
}


template <typename T, typename U>
void operations_test(T const & t, U const & u)
{

  operations_test(t, u, viennamath::eval(t, viennamath::make_vector(4, 6, 8)),
                        viennamath::eval(u, viennamath::make_vector(4, 6, 8)) );
}

template <typename T>
void operations_test(T const & t)
{
  viennamath::ct_variable<0> x;
  viennamath::ct_variable<1> y;
  viennamath::ct_variable<2> z;
  viennamath::ct_constant<5> c5;
  viennamath::ct_constant<7> c7;

  operations_test(t, x);
  operations_test(t, c5);
  operations_test(t, c7);
  operations_test(t, x + y);
  operations_test(t, z * y);
  operations_test(t, c5 / x);
  operations_test(t, c7 - z);
}


void fuzzy_check(double a, double b)
{
  if (fabs(a) < 1e-8 && fabs(b) < 1e-8)
    return;

  if (fabs(a) > 1e-8)
    assert( fabs(fabs(a-b) / fabs(a)) < 1e-10);
  else
    assert( fabs(fabs(a-b) / fabs(b)) < 1e-10);
}


template <long do_print, typename E>
struct evaluation_compiletime
{
  template <typename VectorType>
  static void apply(E const & e, VectorType const & v, double ref_solution)
  {
    std::cout.precision(17);
    std::cout << viennamath::eval(e, v);
    viennamath::expr temp(viennamath::eval(e, v));
    std::cout <<  "(should: " << ref_solution << ", is: " << temp(1.0) << ")" << std::endl;
    fuzzy_check(temp(1.0), ref_solution);
  }
};

template <typename E>
struct evaluation_compiletime<0, E>
{
  template <typename VectorType>
  static void apply(E const & e, VectorType const & v, double ref_solution)
  {
    std::cout << "Not compile-time evaluable!" << std::endl;
  }
};


template <typename E>
void evaluations_test(E const & e, double ref_solution)
{
  viennamath::ct_constant<4> ct4;
  viennamath::ct_constant<6> ct6;
  viennamath::ct_constant<8> ct8;

  std::cout << e << " at compile-time (4,6,8): ";
  evaluation_compiletime<viennamath::result_of::is_ct_evaluable<E>::value,
                         E
                        >::apply(e, viennamath::make_vector(ct4, ct6, ct8), ref_solution);

}




int main()
{
  viennamath::ct_variable<0> x;
  viennamath::ct_variable<1> y;
  viennamath::ct_variable<2> z;
  viennamath::ct_constant<5> c5;
  viennamath::ct_constant<7> c7;


  std::cout << "--- Involved types ---" << std::endl;
  std::cout << "x: " << x << std::endl;
  std::cout << "y: " << y << std::endl;
  std::cout << "z: " << z << std::endl;
  std::cout << "c5: " << c5 << std::endl;
  std::cout << "c7: " << c7 << std::endl;

  std::cout << "--- Various expressions: ---" << std::endl;
  operations_test(x);
  operations_test(c5);
  operations_test(c7);
  operations_test(x + y);
  operations_test(z * y);
  operations_test(c5 / x);
  operations_test(c7 - z);


  //complicated beast:
  // (4 + 5) / 7 - (6 / 8 + 4 * 5)
  // = 9 / 7 - (20 + 6 / 8)
  // = 9 / 7 - 166 / 8
  // = (9*8 - 166 * 7) / 56
  // = (72 -  1162) / 56
  operations_test( (x + c5) / c7 - (y / z + x * c5) );

  operations_test( (x + c5) / c7 - (y / (z + c7) + x * c5) );

  std::cout << "************************************************" << std::endl;
  std::cout << "*****     TEST COMPLETED SUCCESSFULLY!     *****" << std::endl;
  std::cout << "************************************************" << std::endl;

  return EXIT_SUCCESS;
}
