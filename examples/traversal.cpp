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
 *  Tutorial: How to traverse an expression
 */



// A sample functor. Does not do anything useful, but shows how to traverse the individual nodes of an expression
struct cout_functor : public viennamath::rt_traversal_interface<>
{
  typedef viennamath::default_interface_type   InterfaceType;

  // The main entrance function for the functor.
  // Since the type of the expression is unknown, a double-dispatch is carried out.
  void operator()(InterfaceType const * e) const
  {
    //calls the overload for operator()(binary_expr) below if 'e' is a binary expression:
    if (viennamath::callback_if_castable< viennamath::binary_expr >::apply(e, *this))
      return;

    //calls the overload for operator()(unary_expr) below if 'e' is a unary expression:
    if (viennamath::callback_if_castable< viennamath::variable >::apply(e, *this))
      return;

    //calls the overload for operator()(variable) below if 'e' is a variable:
    if (viennamath::callback_if_castable< viennamath::constant >::apply(e, *this))
      return;

    //calls the overload for operator()(constant) below if 'e' is a constant (with numeric type 'double'):
    if (viennamath::callback_if_castable< viennamath::unary_expr >::apply(e, *this))
      return;

    //unknown:
    std::cout << "non-dispatched type: " << e->shallow_str() << std::endl;
  }

  void operator()(viennamath::binary_expr const & /*x*/) const
  {
    std::cout << "binary_expr, dispatched!" << std::endl;
  }

  void operator()(viennamath::unary_expr const & /*x*/) const
  {
    std::cout << "unary_expr, dispatched!" << std::endl;
  }

  void operator()(viennamath::variable const & /*x*/) const
  {
    std::cout << "variable, dispatched!" << std::endl;
  }

  void operator()(viennamath::constant const & /*x*/) const
  {
    std::cout << "constant, dispatched!" << std::endl;
  }

};


int main()
{
  viennamath::variable x(0);
  viennamath::variable y(1);
  viennamath::variable z(2);
  viennamath::rt_constant<double> c4(4.0);
  viennamath::rt_constant<long> c6(6);


  viennamath::expr e1 = x + y - z - c4;
  viennamath::expr e2 = viennamath::sin(x);


  viennamath::rt_traversal_wrapper<> fun(new cout_functor());

  std::cout << "---------------" << std::endl;
  e1.get()->recursive_traversal( fun );

  std::cout << "---------------" << std::endl;
  e2.get()->recursive_traversal( fun );

  std::cout << "****************************************************" << std::endl;
  std::cout << "*****     TUTORIAL COMPLETED SUCCESSFULLY!     *****" << std::endl;
  std::cout << "****************************************************" << std::endl;

  return EXIT_SUCCESS;
}
