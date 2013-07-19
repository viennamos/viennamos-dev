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

#include <iostream>

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



void other_func()
{
  viennamath::variable x(0);
  viennamath::variable y(1);
  viennamath::variable z(2);
  std::vector<double> v(3);
  v[0] = 1.0; v[1] = 2.0; v[2] = 3.0;

  viennamath::expr e = x + y + z;

  std::cout << "Evaluation of " << e
            << " at (" << v[0] << ", " << v[1] << ", " << v[2]
            << ") results in "
            << viennamath::eval(e, v)
            << std::endl;
}
