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


//
// A check for the absence of external linkage (otherwise, library is not truly 'header-only')
//


//
// *** System
//
#include <iostream>
#include <stdlib.h>


//
// *** ViennaMath
//
#include "viennamath/expression.hpp"

#include "viennamath/manipulation/apply_coordinate_system.hpp"
#include "viennamath/manipulation/coefficient.hpp"
#include "viennamath/manipulation/diff.hpp"
#include "viennamath/manipulation/drop_dependent_terms.hpp"
#include "viennamath/manipulation/eval.hpp"
#include "viennamath/manipulation/expand.hpp"
#include "viennamath/manipulation/integrate.hpp"
#include "viennamath/manipulation/latex.hpp"
#include "viennamath/manipulation/simplify.hpp"
#include "viennamath/manipulation/substitute.hpp"



//defined in external_2.cpp
void other_func();

//
// -------------------------------------------------------------
//
int main()
{
  std::cout << "*****************" << std::endl;
  std::cout << "* Test started! *" << std::endl;
  std::cout << "*****************" << std::endl;


  //doing nothing but instantiating a few types
  viennamath::variable x(0);
  viennamath::variable y(1);
  std::vector<double> v(3);
  v[0] = 1.0; v[1] = 2.0; v[2] = 3.0;

  viennamath::expr e = x - y;

  std::cout << "Evaluation of " << e
            << " at (" << v[0] << ", " << v[1] << ", " << v[2]
            << ") results in "
            << viennamath::eval(e, v)
            << std::endl;

  //this is the external linkage check:
  other_func();



  std::cout << "*******************************" << std::endl;
  std::cout << "* Test finished successfully! *" << std::endl;
  std::cout << "*******************************" << std::endl;

  return EXIT_SUCCESS;
}
