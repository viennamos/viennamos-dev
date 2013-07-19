#ifndef VIENNAMATH_RUNTIME_EXPR_HPP
#define VIENNAMATH_RUNTIME_EXPR_HPP

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
#include <memory>
#include "viennamath/forwards.h"
#include "viennamath/runtime/constant.hpp"
#include "viennamath/compiletime/ct_constant.hpp"
#include "viennamath/compiletime/ct_binary_expr.hpp"
#include "viennamath/compiletime/ct_unary_expr.hpp"

/** @file expr.hpp
    @brief Defines the ViennaMath expression wrapper. All ViennaMath expressions can be wrapped by this type.
*/

namespace viennamath
{

  /** @brief This class represents a smart pointer to all types expressions.
   *
   * Additionally provides the basic user-interface for evaluation using operator().
   *
   * @tparam InterfaceType    The expression runtime interface to inherit from. Usually rt_expression_interface, but extensions are possible.
   */
  template <typename InterfaceType /* see forwards.h for default argument */>
  class rt_expr
  {
    public:
      typedef typename InterfaceType::numeric_type       numeric_type;
      typedef InterfaceType                              interface_type;

      rt_expr() {}

      explicit rt_expr(const InterfaceType * e) : rt_expr_(e) {}

      rt_expr(rt_binary_expr<InterfaceType> const & other)
      {
        rt_expr_ = std::auto_ptr<InterfaceType>(other.clone());
      }

      rt_expr(rt_unary_expr<InterfaceType> const & other)
      {
        rt_expr_ = std::auto_ptr<InterfaceType>(other.clone());
      }

      template <typename LHS, typename OP, typename RHS>
      rt_expr(ct_binary_expr<LHS, OP, RHS> const & other)
      {
        rt_expr_ = std::auto_ptr<InterfaceType>(new rt_binary_expr<InterfaceType>(other));
      }

      template <typename LHS, typename OP>
      rt_expr(ct_unary_expr<LHS, OP> const & other)
      {
        rt_expr_ = std::auto_ptr<InterfaceType>(new rt_unary_expr<InterfaceType>(other));
      }

      rt_expr<InterfaceType>(rt_variable<InterfaceType> const & other)
      {
        rt_expr_ = std::auto_ptr<InterfaceType>(other.clone());
      }

      template <id_type id>
      rt_expr<InterfaceType>(ct_variable<id> const & /*other*/)
      {
        rt_expr_ = std::auto_ptr<InterfaceType>(new rt_variable<InterfaceType>(id));
      }

      template <typename T>
      rt_expr(rt_constant<T, InterfaceType> const & other)
      {
        rt_expr_ = std::auto_ptr<InterfaceType>(other.clone());
      }

      rt_expr(rt_function_symbol<InterfaceType> const & other)
      {
        rt_expr_ = std::auto_ptr<InterfaceType>(other.clone());
      }

      rt_expr(rt_vector_expr<InterfaceType> const & other)
      {
        rt_expr_ = std::auto_ptr<InterfaceType>(other.clone());
      }

      template <long value>
      rt_expr(ct_constant<value> const & other)
      {
        rt_expr_ = std::auto_ptr<InterfaceType>(new rt_constant<numeric_type>(value));
      }

      rt_expr(numeric_type const & other)
      {
        rt_expr_ = std::auto_ptr<InterfaceType>(new rt_constant<numeric_type, InterfaceType>(other));
      }

      //Copy CTOR:
      rt_expr(rt_expr const & other)
      {
        if (other.get() != NULL)
        rt_expr_ = std::auto_ptr<InterfaceType>(other.get()->clone());
      }

      //
      // assignments:
      //
      rt_expr & operator=(InterfaceType * other)
      {
        rt_expr_ = std::auto_ptr<InterfaceType>(other);
        return *this;
      }

      rt_expr & operator=(rt_expr const & other)
      {
        rt_expr_ = std::auto_ptr<InterfaceType>(other.get()->clone());
        return *this;
      }

      template <typename LHS, typename OP, typename RHS>
      rt_expr & operator=(ct_binary_expr<LHS, OP, RHS> const & other)
      {
        rt_expr_ = std::auto_ptr<InterfaceType>(new rt_binary_expr<InterfaceType>(other));
        return *this;
      }

      template <typename LHS, typename OP>
      rt_expr & operator=(ct_unary_expr<LHS, OP> const & other)
      {
        rt_expr_ = std::auto_ptr<InterfaceType>(new rt_unary_expr<InterfaceType>(other));
        return *this;
      }

      rt_expr & operator=(rt_binary_expr<InterfaceType> const & other)
      {
        rt_expr_ = std::auto_ptr<InterfaceType>(other.clone());
        return *this;
      }

      rt_expr & operator=(rt_variable<InterfaceType> const & other)
      {
        rt_expr_ = std::auto_ptr<InterfaceType>(other.clone());
        return *this;
      }

      template <id_type id>
      rt_expr & operator=(ct_variable<id> const & other)
      {
        rt_expr_ = std::auto_ptr<InterfaceType>(new rt_constant<InterfaceType>(id));
        return *this;
      }


      template <typename ScalarType>
      rt_expr & operator=(rt_constant<ScalarType, InterfaceType> const & other)
      {
        rt_expr_ = std::auto_ptr<InterfaceType>(other.clone());
        return *this;
      }

      template <long value>
      rt_expr & operator=(ct_constant<value> const & /*other*/)
      {
        rt_expr_ = std::auto_ptr<InterfaceType>(new rt_constant<numeric_type>(value));
        return *this;
      }

      rt_expr & operator=(rt_function_symbol<InterfaceType> const & other)
      {
        rt_expr_ = std::auto_ptr<InterfaceType>(other.clone());
        return *this;
      }

      rt_expr & operator=(rt_vector_expr<InterfaceType> const & other)
      {
        rt_expr_ = std::auto_ptr<InterfaceType>(other.clone());
        return *this;
      }

      rt_expr & operator=(numeric_type value)
      {
        rt_expr_ = std::auto_ptr<InterfaceType>(new rt_constant<numeric_type>(value));
        return *this;
      }

      const InterfaceType   * get() const  { return rt_expr_.get(); }

      ///////////////// evaluation: ///////////////////////////////

      //operator() is a convenience layer:
      numeric_type operator()(numeric_type val) const
      {
        return rt_expr_.get()->eval(val);
      }

      template <typename ScalarType>
      numeric_type operator()(rt_constant<ScalarType, InterfaceType> val) const
      {
        return rt_expr_.get()->eval(static_cast<numeric_type>(val));
      }

      template <long value>
      numeric_type operator()(ct_constant<value> /*val*/) const
      {
        return rt_expr_.get()->eval(value);
      }

      template <typename VectorType>
      numeric_type operator()(VectorType const & v) const
      {
        std::vector<double> stl_v(v.size());
        for (size_t i=0; i<v.size(); ++i)
          stl_v[i] = v[i];

        return rt_expr_.get()->eval(stl_v);
      }

      numeric_type operator()(std::vector<numeric_type> const & stl_v) const
      {
        //std::cout << "operator() with STL vector!" << std::endl;
        return rt_expr_.get()->eval(stl_v);
      }

      template <typename T0>
      numeric_type operator()(viennamath::ct_vector_1<T0> const & v) const
      {
        std::vector<double> stl_v(1);
        stl_v[0] = v[ct_index<0>()];
        return rt_expr_.get()->eval(stl_v);
      }

      template <typename T0, typename T1>
      numeric_type operator()(viennamath::ct_vector_2<T0, T1> const & v) const
      {
        std::vector<double> stl_v(2);
        stl_v[0] = v[ct_index<0>()];
        stl_v[1] = v[ct_index<1>()];
        return rt_expr_.get()->eval(stl_v);
      }

      template <typename T0, typename T1, typename T2>
      numeric_type operator()(viennamath::ct_vector_3<T0, T1, T2> const & v) const
      {
        std::vector<double> stl_v(3);
        stl_v[0] = v[ct_index<0>()];
        stl_v[1] = v[ct_index<1>()];
        stl_v[2] = v[ct_index<2>()];
        return rt_expr_.get()->eval(stl_v);
      }

    private:
      std::auto_ptr<const InterfaceType>  rt_expr_;
  };


  template <typename InterfaceType>
  std::ostream& operator<<(std::ostream & stream, rt_expr<InterfaceType> const & e)
  {
    if (e.get()->is_unary())
    {
      stream << "expr"
            << "("
            << e.get()->deep_str()
            << ")";
    }
    else
    {
      stream << "expr"
            << e.get()->deep_str();
    }

    return stream;
  }

}

#endif
