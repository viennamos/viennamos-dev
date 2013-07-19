#ifndef VIENNAMATH_RUNTIME_VARIABLE_HPP
#define VIENNAMATH_RUNTIME_VARIABLE_HPP

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




#include "viennamath/compiletime/ct_vector.hpp"
#include "viennamath/exception.hpp"

#include "viennamath/runtime/constant.hpp"
#include "viennamath/runtime/expression_interface.hpp"

#include <assert.h>

/** @file variable.hpp
    @brief Defines a class which behaves like a mathematical variable.
*/


namespace viennamath
{


  /** @brief Generic wrapper function for accessing the i-th entry of a vector. Provides a uniform interface for compile-time and run-time vectors.
   *
   * This is the default implementation.
   */
  template <typename VectorType>
  typename VectorType::value_type get_from_vector(VectorType const & vec, id_type i)
  {
    return vec[i];
  }

  /** @brief Generic wrapper function for accessing the i-th entry of a vector. Provides a uniform interface for compile-time and run-time vectors.
   *
   * This is the overload for a compile-time vector of size 1
   */
  template <typename T1>
  default_numeric_type get_from_vector(ct_vector_1<T1> const & vec, id_type i)
  {
    return vec[ct_index<0>()];
  }

  /** @brief Generic wrapper function for accessing the i-th entry of a vector. Provides a uniform interface for compile-time and run-time vectors.
   *
   * This is the overload for a compile-time vector of size 2
   */
  template <typename T1, typename T2>
  default_numeric_type get_from_vector(ct_vector_2<T1, T2> const & vec, id_type i)
  {
    if (i == 0)
      return vec[ct_index<0>()];
    else if (i == 1)
      return vec[ct_index<1>()];
    return 0;
  }

  /** @brief Generic wrapper function for accessing the i-th entry of a vector. Provides a uniform interface for compile-time and run-time vectors.
   *
   * This is the overload for a compile-time vector of size 3
   */
  template <typename T1, typename T2, typename T3>
  default_numeric_type get_from_vector(ct_vector_3<T1, T2, T3> const & vec, id_type i)
  {
    if (i == 0)
      return vec[ct_index<0>()];
    else if (i == 1)
      return vec[ct_index<1>()];
    else if (i == 2)
      return vec[ct_index<2>()];
    return 0;
  }


  /** @brief Representation of an variable (a variable). If the supplied argument is some vector type,
   *  a traits system accesses the id-th component
   *
   * @tparam InterfaceType    The expression runtime interface to inherit from. Usually rt_expression_interface, but extensions are possible.
   */
  template <typename InterfaceType /* see forwards.h for default argument */>
  class rt_variable : public InterfaceType
  {
      typedef rt_variable<InterfaceType>                 self_type;

    public:
      typedef typename InterfaceType::numeric_type    numeric_type;

      explicit rt_variable(id_type my_id) : id_(my_id) {};

      id_type id() const { return id_; }

      /////////////////   Basic evaluation: //////////////////////////////////

      /** @brief Evaluation of the variable by a scalar value. Only valid if the variable ID is zero. */
      numeric_type operator()(numeric_type value) const
      {
        assert(id_ == 0 && "Evaluation of variable with nonzero index by a scalar attempted!");

        return value;
      }

      /** @brief Evaluation of the variable by a ViennaMath runtime constant. Throws an exception if the variable index is larger than zero. */
      template <typename ScalarType>
      rt_constant<ScalarType> operator()(rt_constant<ScalarType> const & other) const
      {
        if (id_ > 0)
          throw variable_index_out_of_bounds_exception(id_, 0);
        return rt_constant<ScalarType>(static_cast<ScalarType>(other));
      }

      /** @brief Evaluation of the variable by a ViennaMath compiletime constant. Throws an exception if the variable index is larger than zero. */
      template <long value>
      long operator()(ct_constant<value> const & /*other*/) const
      {
        if (id_ > 0)
          throw variable_index_out_of_bounds_exception(id_, 0);
        return value;
      }

      //Vector argument (can be of type std::vector)

      /** @brief Evaluation of the variable by a point (vector). Any vector type with .size() member function is supported (eventully, get_from_vector() needs to be overloaded) */
      template <typename VectorType>
      numeric_type
      operator()(VectorType const & v) const
      {
        if(id_ >= v.size())
          throw variable_index_out_of_bounds_exception(id_, v.size());
        return get_from_vector(v, id_);
      }

      //
      // interface requirements:
      //

      /** @brief Returns a copy of the variable. The returned pointer is not automatically deleted, thus the caller needs to ensure deletion.
       */
      InterfaceType * clone() const { return new variable(id_); }

      /** @brief Interface function for evaluating the variable. Returns the 'id_'-th entry of the vector. */
      numeric_type eval(std::vector<double> const & v) const
      {
        if (id_ >= v.size())
          throw variable_index_out_of_bounds_exception(id_, v.size());

        return (*this)(v);
      }

      /** @brief Interface function for evaluating the variable. Throws an exception if the id_ member is larger than zero. */
      numeric_type eval(numeric_type val) const
      {
        if (id_ > 0)
          throw variable_index_out_of_bounds_exception(id_, 1);

        return val;
      }

      /** @brief Returns a detailed string fully describing the variable. */
      std::string deep_str() const
      {
        std::stringstream ss;
        ss << "variable(" << id_ << ")";
        return ss.str();
      }

      /** @brief Tries to evaluate the scalar value of the expression, but a variable cannot be converted to a constant and thus an error is thrown. */
      numeric_type unwrap() const
      {
        throw expression_not_unwrappable_exception();
        return 0;
      }

      //protected:
      /** @brief If 'e' is equal to this variable, the corresponding replacement in 'repl' is returned. Otherwise, a copy of the variable is returned.  The object referred by the pointer is not automatically deleted, thus the caller needs to ensure deletion. */
      InterfaceType * substitute(const InterfaceType * e,
                                 const InterfaceType * repl) const
      {
        //std::cout << "Comparing variable<" << id << "> with " << e->str() << ", result: ";
        if (deep_equal(e))
          return repl->clone();

        //std::cout << "FALSE" << std::endl;
        return clone();
      };

      /** @brief Checks whether one of the expressions in 'e' are equal to the variable. If yes, then the corresponding replacement in 'repl' is returned. Otherwise, a copy of the variable is returned.  The object referred by the pointer is not automatically deleted, thus the caller needs to ensure deletion. */
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

      /** @brief Returns true if the variable is strictly equal to the expression given by 'other' (both type and variable-ID match). */
      bool deep_equal(const InterfaceType * other) const
      {
        const self_type * ptr = dynamic_cast< const self_type *>(other);
        if (ptr != NULL)
          return ptr->id() == id_;

        return false;
      }

      /** @brief Returns a short string specifying that this object is a variable. */
      bool shallow_equal(const InterfaceType * other) const
      {
        return dynamic_cast< const self_type * >(other) != NULL;
      }

      /** @brief Differentiates the variable with respect to 'diff_var' and returns the resulting expression. The object referred by the pointer is not automatically deleted, thus the caller needs to ensure deletion. */
      InterfaceType * diff(const InterfaceType * diff_var) const
      {
        const rt_variable<InterfaceType> * ptr = dynamic_cast< const rt_variable<InterfaceType> *>(diff_var);
        if (ptr != NULL)
        {
          //std::cout << "diff variable<" << id << ">: TRUE" << std::endl;
          if (ptr->id() == id_)
            return new rt_constant<numeric_type, InterfaceType>(1);
        }
        //std::cout << "diff variable<" << id << ">: FALSE, is: " << diff_var.get()->str() << std::endl;
        return new rt_constant<numeric_type, InterfaceType>(0);
      }

    private:
      id_type id_;
  }; //variable


  /** @brief Output stream overload, mostly used for printing an expression to screen. */
  template <typename InterfaceType>
  std::ostream& operator<<(std::ostream & stream, rt_variable<InterfaceType> const & u)
  {
    stream << "variable(" << u.id() << ")";
    return stream;
  }


}

#endif
