#ifndef VIENNAMATH_RUNTIME_INTEGRAL_HPP
#define VIENNAMATH_RUNTIME_INTEGRAL_HPP

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
#include <math.h>
#include <iostream>
#include <sstream>
#include "viennamath/forwards.h"
#include "viennamath/exception.hpp"

/** @file   integral.hpp
    @brief  Defines concrete and symbolic interval operations at runtime.
*/

namespace viennamath
{

  //
  // Part 1: Concrete intervals
  //

  /** @brief A unary operation defining an integral over an interval. */
  template <typename InterfaceType>
  class op_rt_integral
  {
      typedef typename InterfaceType::numeric_type    NumericT;

    public:
      typedef InterfaceType         interface_type;

      op_rt_integral() : integration_variable_(0) {}

      /** @brief Creates the integral operation.
       *
       * @param interv    A runtime interval
       * @param var       The integration variable (runtime)
       */
      op_rt_integral(viennamath::rt_interval<InterfaceType> const & interv,
                     viennamath::rt_variable<InterfaceType> const & var) : interval_(interv),
                                                                           integration_variable_(var) {}

      /** @brief Creates the integral operation.
       *
       * @param interv    A runtime interval
       * @param var       The integration variable (compiletime)
       */
      template <id_type id>
      op_rt_integral(viennamath::rt_interval<InterfaceType> const & interv,
                     viennamath::ct_variable<id> const & /*var*/) : interval_(interv),
                                                                integration_variable_(id) {}

      /** @brief Returns a string representation of the integral operation. */
      std::string str() const
      {
        std::stringstream ss;

        ss << "rt_integral[" << interval_ << ", " << integration_variable_ << "]";
        return ss.str();
      }

      /** @brief Interface requirement: Applies the integral operation. No symbolic integration at runtime available, thus an exception is thrown. */
      NumericT apply(NumericT value) const
      {
        //std::cout << "TODO: Call integration here!" << std::endl;
        throw analytic_integration_not_supported_exception();
        return value;
      }

      /** @brief Interface requirement: A integral operation cannot be optimized (at least for now). */
      bool can_simplify() const { return false; }

      viennamath::rt_interval<InterfaceType> const & interval() const { return interval_; }
      viennamath::rt_variable<InterfaceType> const & variable() const { return integration_variable_; }

    private:
      viennamath::rt_interval<InterfaceType> interval_;
      viennamath::rt_variable<InterfaceType> integration_variable_;
  };



  //
  // Convenience functions with rt_interval:
  //
  /** @brief Creates an expression encoding the integral of an expression with respect to a runtime variable over a certain interval. */
  template <typename InterfaceType>
  rt_expr<InterfaceType> integral(rt_interval<InterfaceType> const & interv,
                                  rt_expr<InterfaceType> const & integrand,
                                  rt_variable<InterfaceType> const & var)
  {
    typedef op_rt_integral<InterfaceType>    OperatorT;
    OperatorT op(interv, var);

    return rt_expr<InterfaceType>(new rt_unary_expr<InterfaceType>(integrand.get()->clone(),
                                                                   new op_unary<OperatorT, InterfaceType>(op))
                                 );
  }

  /** @brief Creates an expression encoding the integral of an expression with respect to a compiletime variable over a certain interval. */
  template <typename InterfaceType, id_type id>
  rt_expr<InterfaceType> integral(rt_interval<InterfaceType> const & interv,
                                  rt_expr<InterfaceType> const & integrand,
                                  ct_variable<id> const & /*var*/)
  {
    typedef op_rt_integral<InterfaceType>    OperatorT;
    OperatorT op(interv, rt_variable<InterfaceType>(id));

    return rt_expr<InterfaceType>(new rt_unary_expr<InterfaceType>(integrand.get()->clone(),
                                                                   new op_unary<OperatorT, InterfaceType>(op))
                                 );
  }

  /** @brief Creates an expression encoding the integral of a binary expression with respect to a compiletime variable over a certain interval. */
  template <typename InterfaceType, id_type id>
  rt_expr<InterfaceType> integral(rt_interval<InterfaceType> const & interv,
                                  rt_binary_expr<InterfaceType> const & integrand,
                                  ct_variable<id> const & var)
  {
    typedef op_rt_integral<InterfaceType>    OperatorT;
    OperatorT op(interv, rt_variable<InterfaceType>(id));

    return rt_expr<InterfaceType>(new rt_unary_expr<InterfaceType>(integrand.clone(),
                                                                   new op_unary<OperatorT, InterfaceType>(op))
                                 );
  }


  //
  // Part 2: Symbolic integrals
  //

  /** @brief A unary operation encoding a symbolic interval. Cannot be evaluated and is meant to be replaced by a concrete integration later on */
  template <typename InterfaceType>
  class op_rt_symbolic_integral
  {
      typedef typename InterfaceType::numeric_type    NumericT;

    public:
      typedef InterfaceType         interface_type;

      op_rt_symbolic_integral() {}

      op_rt_symbolic_integral(viennamath::rt_symbolic_interval<InterfaceType> const & interv) : interval_(interv) {}

      /** @brief Returns a string representation of the symbolic interval */
      std::string str() const
      {
        std::stringstream ss;

        ss << "symbolic_integral[" << interval_ << "]";
        return ss.str();
      }

      /** @brief Interface requirement: Application of the symbolic interval. This is bogus, thus a symbolic_integral_evaluation_not_possible_exception is thrown. */
      NumericT apply(NumericT value) const
      {
        //std::cout << "TODO: Call integration here!" << std::endl;
        throw symbolic_integral_evaluation_not_possible_exception();
        return value;
      }

      /** @brief Interface requirement: Since an integral over a symbolic interval cannot be further optimized, 'false' is returned */
      bool can_simplify() const { return false; }

      /** @brief Returns the integration interval */
      viennamath::rt_symbolic_interval<InterfaceType> const & interval() const { return interval_; }

    private:
      viennamath::rt_symbolic_interval<InterfaceType> interval_;
  };


  //
  // Convenience functions with symbolic integrals
  //
  /** @brief Integral generator function taking a symbolic interval and a runtime expression wrapper for the integrand */
  template <typename InterfaceType>
  rt_expr<InterfaceType> integral(rt_symbolic_interval<InterfaceType> const & interv,
                                  rt_expr<InterfaceType> const & integrand)
  {
    typedef op_rt_symbolic_integral<InterfaceType>    OperatorT;

    return rt_expr<InterfaceType>(new rt_unary_expr<InterfaceType>(integrand.get()->clone(),
                                                                   new op_unary<OperatorT, InterfaceType>(OperatorT(interv)))
                                 );
  }

  /** @brief Integral generator function taking a symbolic interval and a runtime binary expression for the integrand */
  template <typename InterfaceType>
  rt_expr<InterfaceType> integral(rt_symbolic_interval<InterfaceType> const & interv,
                                  rt_binary_expr<InterfaceType> const & integrand)
  {
    typedef op_rt_symbolic_integral<InterfaceType>    OperatorT;

    return rt_expr<InterfaceType>(new rt_unary_expr<InterfaceType>(integrand.clone(),
                                                                   new op_unary<OperatorT, InterfaceType>(OperatorT(interv)))
                                 );
  }

  /** @brief Integral generator function taking a symbolic interval and a runtime unary expression for the integrand */
  template <typename InterfaceType>
  rt_expr<InterfaceType> integral(rt_symbolic_interval<InterfaceType> const & interv,
                                  rt_unary_expr<InterfaceType> const & integrand)
  {
    typedef op_rt_symbolic_integral<InterfaceType>    OperatorT;

    return rt_expr<InterfaceType>(new rt_unary_expr<InterfaceType>(integrand.clone(),
                                                                   new op_unary<OperatorT, InterfaceType>(OperatorT(interv)))
                                 );
  }

  /** @brief Integral generator function taking a symbolic interval and a ViennaMath runtime constant for the integrand */
  template <typename InterfaceType, typename T>
  rt_expr<InterfaceType> integral(rt_symbolic_interval<InterfaceType> const & interv,
                                  rt_constant<T, InterfaceType> const & integrand)
  {
    typedef op_rt_symbolic_integral<InterfaceType>    OperatorT;

    return rt_expr<InterfaceType>(new rt_unary_expr<InterfaceType>(integrand.clone(),
                                                                   new op_unary<OperatorT, InterfaceType>(OperatorT(interv)))
                                 );
  }

  /** @brief Integral generator function taking a symbolic interval and a ViennaMath runtime variable for the integrand */
  template <typename InterfaceType>
  rt_expr<InterfaceType> integral(rt_symbolic_interval<InterfaceType> const & interv,
                                  rt_variable<InterfaceType> const & integrand)
  {
    typedef op_rt_symbolic_integral<InterfaceType>    OperatorT;

    return rt_expr<InterfaceType>(new rt_unary_expr<InterfaceType>(integrand.clone(),
                                                                   new op_unary<OperatorT, InterfaceType>(OperatorT(interv)))
                                 );
  }

  /** @brief Integral generator function taking a symbolic interval and a ViennaMath runtime function symbol for the integrand */
  template <typename InterfaceType>
  rt_expr<InterfaceType> integral(rt_symbolic_interval<InterfaceType> const & interv,
                                  rt_function_symbol<InterfaceType> const & integrand)
  {
    typedef op_rt_symbolic_integral<InterfaceType>    OperatorT;

    return rt_expr<InterfaceType>(new rt_unary_expr<InterfaceType>(integrand.clone(),
                                                                   new op_unary<OperatorT, InterfaceType>(OperatorT(interv)))
                                 );
  }

}

#endif

