#ifndef VIENNAMATH_EXCEPTION_HPP
#define VIENNAMATH_EXCEPTION_HPP

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


#include <string>
#include <vector>
#include <exception>
#include <sstream>
#include <memory>

/** @file exception.hpp
    @brief Defines all the exceptions thrown within ViennaMath.
*/

namespace viennamath
{

  /** @brief Exception for the case that an integration over expressions without integral is attempted. */
  struct integration_without_integral_exception : std::exception
  {
    const char* what() const throw() { return "Cannot integrate an expression if there is no integral."; }
  };

  /** @brief Analytic integration at runtime not supported */
  struct symbolic_integral_evaluation_not_possible_exception : std::exception
  {
    const char* what() const throw() { return "The evaluation of an integral with a symbolic integration domain is not possible."; }
  };


  /** @brief Analytic integration at runtime not supported */
  struct analytic_integration_not_supported_exception : std::exception
  {
    const char* what() const throw() { return "The current release of ViennaMath does not support analytic integrations at runtime."; }
  };


  /** @brief An exception which is thrown if no right hand side operand is passed to a binary operation. */
  struct no_rhs_provided_exception  : std::exception
  {
    const char* what() const throw() { return "Error due to wrong use of class 'op_interface': No right hand side provided for binary operator."; }
  };

  /** @brief An exception which is thrown if a right hand side argument is passed to a unary operation. */
  struct rhs_provided_for_unary_operation_exception : std::exception
  {
    const char* what() const throw() { return "Error due to wrong use of class 'op_interface': A right hand side argument was provided to an unary operator."; }
  };




  /** @brief An exception which is thrown if an expression should be evaluated to a floating point number using eval(), but at least one of the leaves in the expression tree cannot be evaluated (e.g. is a function symbol).
   *
   */
  class expression_not_evaluable_exception : public std::exception
  {
    public:
      expression_not_evaluable_exception() : message_("Expression cannot be unwrapped!") {};
      expression_not_evaluable_exception(std::string const & str) : message_(str) {};

      virtual ~expression_not_evaluable_exception() throw () {};

    private:
      const char * what() const throw() { return message_.c_str(); }

      std::string message_;
  };


  /** @brief An exception which is thrown if an expression should be reduced to a floating point number using unwrap(), but at least one of the leaves in the expression tree is not a constant (e.g. is a variable).
   *
   */
  class expression_not_unwrappable_exception : public std::exception
  {
    public:
      expression_not_unwrappable_exception() : message_("Expression cannot be unwrapped!") {};
      expression_not_unwrappable_exception(std::string const & str) : message_(str) {};

      virtual ~expression_not_unwrappable_exception() throw () {};

    private:
      const char * what() const throw() { return message_.c_str(); }

      std::string message_;
  };

  /** @brief An exception which is thrown if an expression should be differentiated, but at least one of the leaves in the expression tree cannot be differentiated (e.g. is a function symbol).
   *
   */
  class expression_not_differentiable_exception : public std::exception
  {
    public:
      expression_not_differentiable_exception() : message_("Expression not differentiable!") {};
      expression_not_differentiable_exception(std::string const & str) : message_(str) {};

      virtual ~expression_not_differentiable_exception() throw () {};

    private:
      const char * what() const throw() { return message_.c_str(); }

      std::string message_;
  };



  /** @brief An exception which is thrown if a variable index is out of bounds. Similar to the C++ STL, index checks are performed on vectors using the .at() member function only.
   *
   */
  class variable_index_out_of_bounds_exception : public std::exception
  {
    const char * what() const throw()
    {
      std::stringstream ss;
      ss << "Encountered an variable<> type with id larger or equal to the size of the supplied vector of values. id=" << id_ << ", vector_size=" << vector_size_ << std::endl;
      return ss.str().c_str();
    }

    public:
      variable_index_out_of_bounds_exception(long id, long vector_size) : id_(id), vector_size_(vector_size) {}

    private:
      long id_;
      long vector_size_;
  };


}

#endif
