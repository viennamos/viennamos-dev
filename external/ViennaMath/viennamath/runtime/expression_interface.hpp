#ifndef VIENNAMATH_RUNTIME_EXPRESSION_INTERFACE_HPP
#define VIENNAMATH_RUNTIME_EXPRESSION_INTERFACE_HPP

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




#include <vector>
#include "viennamath/forwards.h"
#include "viennamath/runtime/functor_wrapper.hpp"

/** @file expression_interface.hpp
    @brief Defines the common abstract runtime interface of all ViennaMath (runtime) expression types.
*/

namespace viennamath
{

  //class rt_expression_interface;
  //interface for runtime dispatches:
  /** @brief Abstract interface for all ViennaMath runtime expressions */
  template <typename NumericT>
  class rt_expression_interface
  {
    public:
      /** @brief The underlying numeric type of all arithmetical operations (typically 'double'). */
      typedef NumericT                             numeric_type;
      /** @brief The interface type of the respective expression */
      typedef rt_expression_interface<NumericT>    interface_type;

      virtual ~rt_expression_interface() {}

      /** @brief Returns a pointer to a copy of the expression. The caller must ensure the deletion of the returned copy. */
      virtual interface_type * clone() const = 0;  //receiver owns pointer!

      /** @brief Returns a detailed string fully identifying the expression */
      virtual std::string deep_str() const = 0;

      /** @brief Returns a short string identifying just the type of the expression */
      virtual std::string shallow_str() const { return this->deep_str(); }
      /** @brief Evaluates the expression at the given vector */
      virtual NumericT eval(std::vector<NumericT> const & v) const = 0;

      /** @brief Evaluates an expression taking one variable only */
      virtual NumericT eval(NumericT val) const = 0;

      /** @brief Returns true, if the expression is a unary expression */
      virtual bool is_unary() const { return true; }

      /** @brief Returns true, if the expression can be evaluated without providing values for variables (i.e. the expression is a constant) */
      virtual bool is_constant() const { return false; }

      /** @brief Returns the numeric value of the expression. */
      virtual NumericT unwrap() const = 0;

      /** @brief Checks the current expression for being equal to 'other'. Checks for same type only, does not check members. */
      virtual bool shallow_equal(const interface_type * other) const = 0;

      /** @brief Checks the current expression for being equal to 'other'. Performs a deep check for equality of members. */
      virtual bool deep_equal(const interface_type * other) const = 0;

      /** @brief Provides a common interface for top-down manipulations of the expression. */
      virtual interface_type * recursive_manipulation(rt_manipulation_wrapper<interface_type> const & fw) const { return fw(this); }
      /** @brief Provides a common interface for all top-down traversals of the expression. */
      virtual void recursive_traversal(rt_traversal_wrapper<interface_type> const & fw) const { fw(this); }

      ////// Deprecated section: ////////////////

      /** @brief Returns a new expression with all occurrences of 'e' replaced by 'repl'. The caller must ensure the deletion of the object the returned pointer is referring to. */
      virtual interface_type * substitute(const interface_type * e,
                                          const interface_type * repl) const = 0;  //receiver owns pointer! Function parameters must not be manipulated!

      /** @brief Returns a expression with all occurrances of 'e' in the expression substituted by the respective replacement in 'repl'. The caller must ensure the deletion of the object the returned pointer is referring to. */
      virtual interface_type * substitute(std::vector<const interface_type *> const & e,
                                          std::vector<const interface_type *> const & repl) const = 0;  //receiver owns pointer! Function parameters must not be manipulated!

      /** @brief Returns an optimized expression, where trivial operations such as multiplications by unity are removed. The caller must ensure the deletion of the object the returned pointer is referring to. */
      virtual interface_type * simplify() const { return clone(); }  //receiver owns pointer!
      /** @brief Returns true if the experssion can be further optimized */
      virtual bool can_simplify() const { return false; }
      /** @brief Returns the expression differentiated with respect to the provided variable. The caller must ensure the deletion of the object the returned pointer is referring to.  */
      virtual interface_type * diff(const interface_type * diff_var) const = 0;   //receiver owns pointer! Function parameter diff_var not be manipulated!


  };




}

#endif
