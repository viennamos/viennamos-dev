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


// Since this is a benchmark, make sure that no assert() statements are included:
#ifndef NDEBUG
  #define NDEBUG
#endif

// System headers
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <math.h>

// ViennaMath headers
#include "viennamath/expression.hpp"
#include "viennamath/manipulation/eval.hpp"
#include "viennamath/manipulation/substitute.hpp"

#include "examples/benchmark-utils.hpp"

#define BENCHMARK_SIZE   1000000

/*
 *  Tutorial: A small benchmark demonstrating that ViennaMath can avoid the repeated evaluation of constants in an expression
 */


void print_stats(double result, double elapsed)
{
  std::cout << "|" << std::endl;
  std::cout << "|   * result: " << result << std::endl;
  std::cout << "|   * time: " << elapsed << std::endl;
  std::cout << "|" << std::endl;
  std::cout << std::endl;
}

int main()
{
  Timer timer;
  double elapsed;
  double result = 0;
  double x_ref = 2.0;
  double T_ref = 300;
  double kb = 1.61;
  double voltage = 3.0;
  double prefactor = 2.0;
  double eps = 8.854e-12;

  double mult;

  std::cout << "**** ViennaMath performance benchmark ****" << std::endl;
  std::cout << "Make sure that optimization flags (e.g. -O2) are set when benchmarking!" << std::endl;

  //
  // First step: disable precomputations by the compiler for naive implementation
  // If not specified at runtime, compiler might detect voltage and prefactor to be constants and precompute results
  std::cout << "Enter '1': ";
  std::cin >> mult;
  voltage *= mult;
  prefactor *= mult;
  T_ref *= mult;
  kb *= mult;

  std::cout << "--------- Naive (standard) implementation --------- " << std::endl;

  //the reference implementation:
  timer.start();
  for (int i=0; i<BENCHMARK_SIZE; ++i)
    result += (T_ref * eps * x_ref * ::exp( prefactor * voltage / (kb * T_ref) ) );
  elapsed = timer.get();


  print_stats(result, elapsed);


  std::cout << "--------- Hand tuned implementation (best possible, but will never be achieved within a simulator) --------- " << std::endl;

  //the reference implementation:
  result = 0;
  timer.start();
  for (int i=0; i<BENCHMARK_SIZE; ++i)
    result += 2.6894024e-9 * x_ref;
  elapsed = timer.get();

  print_stats(result, elapsed);



  std::cout << "--------- ViennaMath (runtime) --------- " << std::endl;

  //ViennaMath:
  viennamath::variable  x(0);
  viennamath::variable  T(1);
  viennamath::expr e = (T * eps * x * viennamath::exp( prefactor * voltage / (kb * T) ) );
  std::cout << "Expression before substitution: " << e << std::endl;
  viennamath::expr e_opt = viennamath::substitute(T, 300.0, e);
  std::cout << "Expression after substitution: " << e_opt << std::endl;
  result = 0;
  timer.start();
  for (int i=0; i<BENCHMARK_SIZE; ++i)
    result += viennamath::eval(e_opt, 2.0);
  elapsed = timer.get();

  print_stats(result, elapsed);


  std::cout << "--------- ViennaMath (compiletime) --------- " << std::endl;
  std::cout << "|" << std::endl;
  std::cout << "|  * (not possible in this case)" << std::endl;
  std::cout << "|" << std::endl;

  std::cout << "****************************************************" << std::endl;
  std::cout << "*****     TUTORIAL COMPLETED SUCCESSFULLY!     *****" << std::endl;
  std::cout << "****************************************************" << std::endl;

  return EXIT_SUCCESS;
}
