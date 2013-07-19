#ifndef VIENNAMATH_RUNTIME_FUNCTION_SYMBOL_HPP
#define VIENNAMATH_RUNTIME_FUNCTION_SYMBOL_HPP

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
#include <sstream>
#include "viennamath/forwards.h"
#include "viennamath/expression.hpp"
#include "viennamath/compiletime/ct_function_symbol.hpp"
#include "viennamath/runtime/expression_interface.hpp"

/** @file viennamath/runtime/function_symbol.hpp
    @brief Defines a symbolic representation of a function at runtime.
*/

namespace viennamath
{

  /** @brief A function symbol. Can be used for unknown functions, test functions, etc. Cannot be evaluated, but substituted with an evaluable object
   *
   * @tparam InterfaceType    The expression runtime interface to inherit from. Usually rt_expression_interface, but extensions are possible.
   */
  template <typename InterfaceType>
  class rt_function_symbol : public InterfaceType
  {
      typedef rt_function_symbol<InterfaceType>     self_type;
    public:
      typedef typename InterfaceType::numeric_type      numeric_type;
      typedef InterfaceType                             interface_type;

      /** @brief CTOR assigning the ID 'i' for the class of function symbols identified by the tag 't'
       *
       * @param i     A numerical ID
       * @param t     A tag class used for the compiletime ct_function_symbol
       */
      template <typename Tag>
      rt_function_symbol(id_type i, Tag const & /*t*/) : id_(i), tag_id_(Tag::tag_id()) {}

      /** @brief CTOR assigning the ID 'i' for the class of function symbols identified by the ID 't'
       *
       * @param i     A numerical ID
       * @param t     An ID for the family of function symbols
       */
      rt_function_symbol(id_type i, id_type t = 0) : id_(i), tag_id_(t) {}

      rt_function_symbol() : id_(0), tag_id_(unknown_tag<>::tag_id()) {}

      id_type id() const { return id_; }
      id_type tag_id() const { return tag_id_; }

      //
      // interface requirements:
      //

      /** @brief Returns a copy of the function symbol. The caller needs to ensure that the object the pointer is referring to is deleted. */
      InterfaceType * clone() const { return new self_type(id_, tag_id_); }

      /** @brief Interface requirement: Evaluates the expression. This is illegal for a function symbol */
      numeric_type eval(std::vector<double> const & /*v*/) const { throw expression_not_evaluable_exception("Cannot evaluate rt_function_symbol!"); return 0; }
      /** @brief Interface requirement: Evaluates the expression. This is illegal for a function symbol */
      numeric_type eval(numeric_type /*v*/) const { throw expression_not_evaluable_exception("Cannot evaluate rt_function_symbol!"); return 0; }

      /** @brief Returns a detailed string with all informations about the function symbol */
      std::string deep_str() const
      {
        std::stringstream ss;
        ss << "function_symbol<";
        if (tag_id_ == unknown_tag<>::tag_id())
          ss << "unknown(" << id_ << ")>";
        else if (tag_id_ == test_tag<>::tag_id())
          ss << "test(" << id_ << ")>";
        else
          ss << "invalid>";

        return ss.str();
      }

      /** @brief Interface requirement: Evaluate to a constant. This is illegal for a function symbol */
      numeric_type unwrap() const { throw expression_not_unwrappable_exception("Cannot evaluate rt_function_symbol to a number!"); }

      /** @brief If this expression is equal to 'e', the replacement 'repl' is returned. Otherwise, a copy of this object is returned. Note that the caller is responsible for deleting the object the returned pointer is referring to. */
      InterfaceType * substitute(const InterfaceType * e,
                                 const InterfaceType * repl) const
      {
        if (deep_equal(e))
            return repl->clone();

        return clone();
      };

      /** @brief If this expression is equal to one of the elements of 'e', the corresponding replacement 'repl' is returned. Otherwise, a copy of this object is returned. Note that the caller is responsible for deleting the object the returned pointer is referring to. */
      InterfaceType * substitute(std::vector<const InterfaceType *> const &  e,
                                 std::vector<const InterfaceType *> const &  repl) const
      {
        //std::cout << "Comparing variable<" << id << "> with " << e->str() << ", result: ";
        for (std::size_t i=0; i<e.size(); ++i)
          if (deep_equal(e[i]))
            return repl[i]->clone();

        //std::cout << "FALSE" << std::endl;
        return clone();
      };

      /** @brief Returns true if the other function is semantically equal (same ID, same tag). */
      bool deep_equal(const InterfaceType * other) const
      {
        const self_type * ptr = dynamic_cast< const self_type *>(other);
        if (ptr != NULL)
          return (ptr->id() == id_ && ptr->tag_id_ == tag_id_);

        return false;
      }

      /** @brief Returns true if the type of 'other' is also a function symbol (with possibly different ID). */
      bool shallow_equal(const InterfaceType * other) const
      {
        return dynamic_cast< const self_type * >(other) != NULL;
      }

      /** @brief Interface requirement. The differentiation of a function symbol is not allowed, thus an exception is thrown. */
      InterfaceType * diff(const InterfaceType * diff_var) const
      {
        const self_type * ptr = dynamic_cast< const self_type *>(diff_var);
        if (ptr != NULL)
        {
          //std::cout << "diff variable<" << id << ">: TRUE" << std::endl;
          if (ptr->id() == id_ && ptr->tag_id() == tag_id_)
            return new rt_constant<numeric_type, InterfaceType>(1);
          else
            //std::cout << "diff variable<" << id << ">: FALSE, is: " << diff_var.get()->str() << std::endl;
            return new rt_constant<numeric_type, InterfaceType>(0);
        }

        // the derivative of a function symbol with respect to a variable should not be triggered here. TODO: Maybe move higher-level code here?
        throw expression_not_differentiable_exception("Cannot differentiate rt_function_symbol!");
        return NULL;
      }

    private:
      /** @brief A numerical ID identifying the function symbol for a given tag */
      id_type id_;
      /** @brief A tag ID allowing for distinguishing between different groups of function symbols. */
      id_type tag_id_;
  };



  template <typename InterfaceType>
  std::ostream& operator<<(std::ostream & stream, rt_function_symbol<InterfaceType> const & v)
  {
    stream << v.deep_str();
    return stream;
  }



}

#endif
