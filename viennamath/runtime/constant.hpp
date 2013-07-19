#ifndef VIENNAMATH_RUNTIME_CONSTANT_HPP
#define VIENNAMATH_RUNTIME_CONSTANT_HPP

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




#include <ostream>
#include "viennamath/forwards.h"
//#include "viennamath/expression_compile_time.hpp"
#include "viennamath/runtime/expression_interface.hpp"
//#include "viennamath/expression_run_time.hpp"


/** @file viennamath/runtime/constant.hpp
    @brief Defines the ViennaMath runtime constant class.
*/

namespace viennamath
{

  //per default, we assume floating point constants, which cannot be tackled with template arguments
  /** @brief Representation of a constant within the expression setting of ViennaMath.
   *
   * @tparam InterfaceType    The expression runtime interface to inherit from. Usually rt_expression_interface, but extensions are possible.
   */
  template <typename ScalarType, /* see forwards.h for default argument */
            typename InterfaceType /* see forwards.h for default argument */>
  class rt_constant : public InterfaceType
  {
      typedef rt_constant<ScalarType, InterfaceType>     self_type;
    public:
      typedef typename InterfaceType::numeric_type    numeric_type;

      explicit rt_constant(ScalarType s_) : s(s_) {};

      /** @brief Evaluates the constant. */
      self_type operator() () const
      {
        return *this;
      }

      /** @brief Evaluates the constant by returning its value. */
      template <typename VectorType>
      self_type operator() (const VectorType & /*p*/) const
      {
        return *this;
      }

      /** @brief A ViennaMath constant is implicity convertible to its underlying scalar. Since the constructor is explicit, this operation is safe. */
      operator ScalarType() const { return s; }

      //interface requirements:
      /** @brief Returns a copy of the constant. The object referred by the pointer is not automatically deleted, thus the caller needs to ensure deletion. */
      InterfaceType * clone() const { return new self_type(s); }

      /** @brief (Trivially) Evaluates the constant. */
      numeric_type eval(std::vector<numeric_type> const & /*v*/) const { return s; }
      /** @brief (Trivially) Evaluates the constant. */
      numeric_type eval(numeric_type /*v*/) const { return s; }

      /** @brief A constant is a constant :-) */
      bool is_constant() const { return true; }

      /** @brief Returns a textual representation of the constant */
      std::string deep_str() const
      {
        std::stringstream ss;
        ss << "constant(" << s << ")";
        return ss.str();
      }

      /** @brief Interface requirement: Returns the value of the constant */
      numeric_type unwrap() const { return s; }

      /** @brief Substitutes the constant with an expression 'repl' if it matches the expression 'e'. The object referred by the pointer is not automatically deleted, thus the caller needs to ensure deletion. */
      InterfaceType * substitute(const InterfaceType * e,
                                 const InterfaceType * repl) const
      {
        if (deep_equal(e))
          return repl->clone();

        //std::cout << "FALSE" << std::endl;
        return clone();
      };

      /** @brief If any of the expressions in 'e' match the constant, the expression in 'repl' is returned. Otherwise, a copy of the constant is returned. The object referred by the pointer is not automatically deleted, thus the caller needs to ensure deletion. */
      InterfaceType * substitute(std::vector<const InterfaceType *> const &  e,
                                 std::vector<const InterfaceType *> const &  repl) const
      {
        //std::cout << "Comparing variable<" << id << "> with " << e->str() << ", result: ";
        for (size_t i=0; i<e.size(); ++i)
          if (deep_equal(e[i]))
            return repl[i]->clone();

        //std::cout << "FALSE" << std::endl;
        return clone();
      };

      /** @brief Returns true if 'other' points to a constant with the same value */
      bool deep_equal(const InterfaceType * other) const
      {
        const self_type * ptr = dynamic_cast< const self_type *>(other);
        if (ptr != NULL)
          return ptr->s == s;

        return false;
      }

      /** @brief Returns true if 'other' is also a constant */
      bool shallow_equal(const InterfaceType * other) const
      {
        return dynamic_cast< const self_type * >(other) != NULL;
      }

      /** @brief Returns the result of differentiating the constant with respect to a variable, i.e. zero. */
      InterfaceType * diff(const InterfaceType * /*diff_var*/) const
      {
        return new self_type(0);
      }

    private:
      ScalarType s;
  };

  /** @brief Convenience operator overload for streaming a runtime constant to std::cout or any other STL-compatible output stream. */
  template <typename ScalarType, typename InterfaceType>
  std::ostream& operator<<(std::ostream & stream,
                           rt_constant<ScalarType, InterfaceType> const & c)
  {
    stream << c.deep_str();
    return stream;
  }




}

#endif
