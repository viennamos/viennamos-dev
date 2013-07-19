#ifndef VIENNAMATH_RUNTIME_UNARY_EXPRESSION_HPP
#define VIENNAMATH_RUNTIME_UNARY_EXPRESSION_HPP

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
#include <ostream>
#include <sstream>
#include <memory>
#include "viennamath/forwards.h"
#include "viennamath/compiletime/unary_op_tags.hpp"
#include "viennamath/compiletime/ct_unary_expr.hpp"
#include "viennamath/runtime/constant.hpp"
#include "viennamath/runtime/op_interface.hpp"
#include "viennamath/runtime/expression_interface.hpp"

/** @file unary_expr.hpp
    @brief Defines a class representing an unary expression, i.e. an expression a unary manipulator (e.g. a function) is acting on.
*/

namespace viennamath
{

  //A run time expression
  /** @brief Runtime representation of a unary expression F(e), where F is a function of one argument (e.g. log()) and e is an expression.
   *
   * @tparam InterfaceType    The expression runtime interface to inherit from. Usually rt_expression_interface, but extensions are possible.
   */
  template <typename InterfaceType /* see forwards.h for default argument */>
  class rt_unary_expr : public InterfaceType
  {
    typedef op_interface<InterfaceType>                    op_interface_type;
    typedef op_unary<op_id<typename InterfaceType::numeric_type>, InterfaceType>  op_unary_id_type;

      typedef rt_unary_expr<InterfaceType>                    self_type;

    public:
      typedef typename InterfaceType::numeric_type         numeric_type;

      rt_unary_expr() {}

      explicit rt_unary_expr(InterfaceType * lhs,
                          op_interface_type * op) : expr_(lhs),
                                                    op_(op) {}

      explicit rt_unary_expr(InterfaceType * lhs) : expr_(lhs),
                                                 op_(new op_unary_id_type()) {}

      template <typename LHS, typename OP, typename RHS>
      explicit rt_unary_expr(ct_unary_expr<LHS, OP> const & other) : op_(new OP())
      {
        //std::cout << "Constructing from expression " << other << std::endl;
        expr_ = std::auto_ptr<InterfaceType>(other.lhs().clone());
      }

      explicit rt_unary_expr(rt_variable<InterfaceType> const & other) : expr_(other.clone()),
                                                                   op_(new op_unary_id_type())  {}

      template <typename T>
      explicit rt_unary_expr(rt_constant<T, InterfaceType> const & other) : expr_(other.clone()),
                                                                      op_(new op_unary_id_type()) {}

      template <long value>
      explicit rt_unary_expr(ct_constant<value> const & other) : expr_(other.clone()),
                                                              op_(new op_unary_id_type()) {}

      //Copy CTOR:
      rt_unary_expr(rt_unary_expr const & other) : expr_(other.expr_->clone()),
                                             op_(other.op_->clone()) {};

      //assignments:
      template <typename LHS, typename OP>
      rt_unary_expr & operator=(ct_unary_expr<LHS, OP> const & other)
      {
        expr_ = std::auto_ptr<InterfaceType>(other.lhs().clone());
        op_ = std::auto_ptr<op_interface_type>(new OP());
        return *this;
      }

      rt_unary_expr & operator=(rt_unary_expr const & other)
      {
        expr_ = std::auto_ptr<InterfaceType>(other.lhs_->clone());
        op_  = std::auto_ptr<op_interface_type>(other.op_->clone());
        return *this;
      }

      template <typename ScalarType>
      rt_unary_expr & operator=(rt_constant<ScalarType> const & other)
      {
        expr_ = std::auto_ptr<InterfaceType>(other.clone());
        op_  = std::auto_ptr<op_interface_type>(new op_unary_id_type());
        return *this;
      }

      template <long value>
      rt_unary_expr & operator=(ct_constant<value> const & other)
      {
        return *this = value;
      }

      rt_unary_expr & operator=(numeric_type value)
      {
        expr_ = std::auto_ptr<InterfaceType>(new rt_constant<numeric_type>(value));
        op_  = std::auto_ptr<op_interface_type>(new op_unary_id_type());
        return *this;
      }

      /** @brief Returns the expression the unary function is acting on */
      const InterfaceType     * lhs() const { return expr_.get(); }

      /** @brief Returns the operation */
      const op_interface_type * op()  const { return op_.get(); }

      ///////////////// evaluation: ///////////////////////////////

      //operator() is a convenience layer:
      /** @brief Evaluation of the unary expression at val (i.e. all variables with ID 0 replaced by 'val') */
      numeric_type operator()(numeric_type val) const
      {
        return this->eval(val);
      }

      /** @brief Evaluation of the unary expression at the ViennaMath runtime constant val */
      template <typename ScalarType>
      numeric_type operator()(rt_constant<ScalarType> val) const
      {
        return this->eval(static_cast<numeric_type>(val));
      }

      /** @brief Evaluation of the unary expression at the ViennaMath compiletime constant val */
      template <long value>
      numeric_type operator()(ct_constant<value> val) const
      {
        return this->eval(value);
      }

      /** @brief Evaluation of the unary expression at the vector v */
      template <typename VectorType>
      numeric_type operator()(VectorType const & v) const
      {
        std::vector<double> stl_v(v.size());
        for (std::size_t i=0; i<v.size(); ++i)
          stl_v[i] = v[i];

        return this->eval(stl_v);
      }

      /** @brief Evaluation of the unary expression at the STL vector v */
      numeric_type operator()(std::vector<numeric_type> const & stl_v) const
      {
        return this->eval(stl_v);
      }

      /** @brief Evaluation of the unary expression at a ViennaMath compiletime vector with one entry */
      template <typename T0>
      numeric_type operator()(viennamath::ct_vector_1<T0> const & v) const
      {
        std::vector<double> stl_v(1);
        stl_v[0] = v[ct_index<0>()];
        return this->eval(stl_v);
      }

      /** @brief Evaluation of the unary expression at a ViennaMath compiletime vector with two entries */
      template <typename T0, typename T1>
      numeric_type operator()(viennamath::ct_vector_2<T0, T1> const & v) const
      {
        std::vector<double> stl_v(2);
        stl_v[0] = v[ct_index<0>()];
        stl_v[1] = v[ct_index<1>()];
        return this->eval(stl_v);
      }

      /** @brief Evaluation of the unary expression at a ViennaMath compiletime vector with three entries */
      template <typename T0, typename T1, typename T2>
      numeric_type operator()(viennamath::ct_vector_3<T0, T1, T2> const & v) const
      {
        std::vector<double> stl_v(3);
        stl_v[0] = v[ct_index<0>()];
        stl_v[1] = v[ct_index<1>()];
        stl_v[2] = v[ct_index<2>()];
        return this->eval(stl_v);
      }

      //virtual functions for evaluations
      /** @brief Evaluation of the unary expression at an STL vector. Interface requirement. */
      numeric_type eval(std::vector<double> const & v) const
      {
        return op_->apply(expr_.get()->eval(v));
      }

      /** @brief Evaluation of the unary expression for a numerical constant. Interface requirement. */
      numeric_type eval(numeric_type val) const
      {
        return op_->apply(expr_.get()->eval(val));
      }

      ///////////////////// substitution /////////////////////////////

      /** @brief Returns a simplified expression with trivial operations removed. The caller is responsible for deleting the object the returned pointer refers to. */
      InterfaceType * simplify() const
      {
        if (expr_->is_constant())
        {
          if (op_->can_simplify())
            return new rt_constant<numeric_type, InterfaceType>( op_->apply(expr_->unwrap()) );
          else
            return new rt_unary_expr(new rt_constant<numeric_type, InterfaceType>(expr_->unwrap()),
                                     op_->clone());
        }

        //TODO: Unwrap op_id()

        return new rt_unary_expr(expr_->simplify(), op_->clone());
      }

      /** @brief Returns true if the expression can be simplified */
      bool can_simplify() const
      {
        if (expr_->can_simplify())
        {
          //std::cout << "optimizable(): true in unary_expr" << std::endl;
          return true;
        }
        return false;
      }


      ///////// other interface requirements ////////////////////////
      /** @brief Returns a copy of the unary expression. The caller is responsible for deleting the returned object. */
      InterfaceType * clone() const { return new rt_unary_expr(expr_->clone(), op_->clone()); }

      /** @brief Returns a detailed string describing the unary expression. Acts recursively. */
      std::string deep_str() const
      {
        std::stringstream ss;
        ss << op_->str();
        ss << "(";
        ss << expr_->deep_str();
        ss << ")";
        return ss.str();
      }

      /** @brief Returns a short string describing the unary expression. Does not act recursively. */
      std::string shallow_str() const
      {
        return std::string("unary_expr");
      }

      /** @brief Reduces the expression to a constant. If this is not possible, an expression_not_unwrappable_exception is thrown. */
      numeric_type unwrap() const
      {
        //if (op_->is_unary())
        //  return lhs_->unwrap();
        return op_->apply(expr_->unwrap());
      }

      /** @brief If 'e' equals to this unary expression, the replacement 'repl' is returned. Otherwise, the unary expression is cloned. Note that the caller must ensure proper deletion of the returned object.  */
      InterfaceType * substitute(const InterfaceType * e,
                                 const InterfaceType * repl) const
      {
        if (deep_equal(e))
          return repl->clone();

        return new rt_unary_expr(expr_->substitute(e, repl),
                              op_->clone());
      };

      /** @brief If one of the expressions in 'e' equals to this unary expression, the respective replacement 'repl' is returned. Otherwise, a copy of *this is returned. Note that the caller must ensure proper deletion of the returned object. */
      InterfaceType * substitute(std::vector<const InterfaceType *> const &  e,
                                 std::vector<const InterfaceType *> const &  repl) const
      {
        for (std::size_t i=0; i<e.size(); ++i)
          if (deep_equal(e[i]))
            return repl[i]->clone();

        return new rt_unary_expr(expr_->substitute(e, repl),
                              op_->clone());
      };

      /** @brief Performs a detailed comparison of the unary expression with the passed expression. Returns true if they represent the same expression (equal IDs, etc.) */
      bool deep_equal(const InterfaceType * other) const
      {
        if (dynamic_cast<const rt_unary_expr *>(other) != NULL)
        {
           const rt_unary_expr * temp = dynamic_cast<const rt_unary_expr *>(other);

           return expr_->deep_equal(temp->expr_.get())
                  && op_->equal(temp->op_.get());
        }
        return false;
      }

      /** @brief Returns true if the passed expression is of the same type as this expression. */
      bool shallow_equal(const InterfaceType * other) const
      {
        return dynamic_cast< const self_type * >(other) != NULL;
      }

      /** @brief Returns the derivative of this unary expression with respect to the variable 'diff_var'. The returned object must be deleted by the caller. */
      InterfaceType * diff(const InterfaceType * diff_var) const
      {
        return op_->diff(expr_.get(), diff_var);
      }

      /** @brief Recursively manipulates the expression. If this expression is manipulated, the result is returned directly. Otherwise, the manipulator continues with the expression the unary operation is acting on. The returned object must be deleted by the caller. */
      InterfaceType * recursive_manipulation(rt_manipulation_wrapper<InterfaceType> const & fw) const
      {
        if (fw.modifies(this))
          return fw(this);

        return new rt_unary_expr(expr_->recursive_manipulation(fw),
                              op_->clone() );
      }

      /** @brief Recursively traverses the expression by calling the wrapper for each object without manipulating it. */
      void recursive_traversal(rt_traversal_wrapper<InterfaceType> const & fw) const
      {
        fw(this);
        expr_->recursive_traversal(fw);
      }

    private:
      std::auto_ptr<InterfaceType>      expr_;
      std::auto_ptr<op_interface_type>  op_;
  };

  /** @brief Overload of the stream operator in order to allow for a convenient conversion of a unary expression to a string. */
  template <typename InterfaceType>
  std::ostream& operator<<(std::ostream & stream, rt_unary_expr<InterfaceType> const & e)
  {
    stream << "unary"
           << e.deep_str()
           << "";
    return stream;
  }


}

#endif
