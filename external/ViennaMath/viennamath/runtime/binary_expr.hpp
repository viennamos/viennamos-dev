#ifndef VIENNAMATH_RUNTIME_BINARY_EXPRESSION_HPP
#define VIENNAMATH_RUNTIME_BINARY_EXPRESSION_HPP

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
#include "viennamath/runtime/binary_operators.hpp"
#include "viennamath/compiletime/ct_binary_expr.hpp"
#include "viennamath/runtime/unary_expr.hpp"
#include "viennamath/runtime/op_interface.hpp"
#include "viennamath/runtime/expression_interface.hpp"
#include "viennamath/runtime/unary_operators.hpp"

/** @file binary_expr.hpp
    @brief Defines the binary expression class.
*/

namespace viennamath
{

  /** @brief Runtime representation of a binary expression F(x,y), where F is a function of two argument (e.g. +) and x,y are expressions.
   *
   * @tparam InterfaceType    The expression runtime interface to inherit from. Usually rt_expression_interface, but extensions are possible.
   */
  template <typename InterfaceType /* see forwards.h for default argument */>
  class rt_binary_expr : public InterfaceType
  {
    typedef op_interface<InterfaceType>                                           op_interface_type;
    typedef op_unary<op_id<typename InterfaceType::numeric_type>, InterfaceType>  op_unary_id_type;

      typedef rt_binary_expr<InterfaceType>    self_type;

    public:
      typedef typename InterfaceType::numeric_type         numeric_type;
//       explicit expr(InterfaceType * lhs,
//                     op_interface         * op,
//                     InterfaceType * rhs) : lhs_(lhs), op_(op), rhs_(rhs) {}
      rt_binary_expr() {}

      explicit rt_binary_expr(InterfaceType * lhs,
                           op_interface_type         * op,
                           InterfaceType * rhs) : lhs_(lhs),
                                                         op_(op),
                                                         rhs_(rhs) {}

      template <typename LHS, typename OP, typename RHS>
      explicit rt_binary_expr(ct_binary_expr<LHS, OP, RHS> const & other) : op_(new op_binary<OP, InterfaceType>())
      {
        //std::cout << "Constructing from expression " << other << std::endl;
        lhs_ = std::auto_ptr<InterfaceType>(new rt_binary_expr<InterfaceType>(other.lhs()));
        rhs_ = std::auto_ptr<InterfaceType>(new rt_binary_expr<InterfaceType>(other.rhs()));
      }

      /////////////// special case: ct_constant involved: ////////////////////////
      template <typename LHS, typename OP, long value>
      explicit rt_binary_expr(ct_binary_expr<LHS, OP, ct_constant<value> > const & other) : op_(new op_binary<OP, InterfaceType>())
      {
        //std::cout << "Constructing from expression " << other << std::endl;
        lhs_ = std::auto_ptr<InterfaceType>(new rt_binary_expr<InterfaceType>(other.lhs()));
        rhs_ = std::auto_ptr<InterfaceType>(new rt_constant<numeric_type, InterfaceType>(value));
      }

      template <long value, typename OP, typename RHS>
      explicit rt_binary_expr(ct_binary_expr<ct_constant<value>, OP, RHS > const & other) : op_(new op_binary<OP, InterfaceType>())
      {
        //std::cout << "Constructing from expression " << other << std::endl;
        lhs_ = std::auto_ptr<InterfaceType>(new rt_constant<numeric_type, InterfaceType>(value));
        rhs_ = std::auto_ptr<InterfaceType>(new rt_binary_expr<InterfaceType>(other.rhs()));
      }

      template <long value1, typename OP, long value2>
      explicit rt_binary_expr(ct_binary_expr<ct_constant<value1>, OP, ct_constant<value2> > const & /*other*/) : op_(new op_binary<OP, InterfaceType>())
      {
        //std::cout << "Constructing from expression " << other << " to " << OP::apply(value1, value2) << std::endl;
        lhs_ = std::auto_ptr<InterfaceType>(new rt_constant<numeric_type, InterfaceType>(value1));
        rhs_ = std::auto_ptr<InterfaceType>(new rt_constant<numeric_type, InterfaceType>(value2));
      }

      /////////////////// special case: ct_variable involved: //////////////////////////
      template <typename LHS, typename OP, id_type id>
      explicit rt_binary_expr(ct_binary_expr<LHS, OP, ct_variable<id> > const & other) : op_(new op_binary<OP, InterfaceType>())
      {
        //std::cout << "Constructing from expression " << other << std::endl;
        lhs_ = std::auto_ptr<InterfaceType>(new rt_binary_expr<InterfaceType>(other.lhs()));
        rhs_ = std::auto_ptr<InterfaceType>(new rt_variable<InterfaceType>(id));
      }

      template <id_type id, typename OP, typename RHS>
      explicit rt_binary_expr(ct_binary_expr<ct_variable<id>, OP, RHS > const & other) : op_(new op_binary<OP, InterfaceType>())
      {
        //std::cout << "Constructing from expression " << other << std::endl;
        lhs_ = std::auto_ptr<InterfaceType>(new rt_variable<InterfaceType>(id));
        rhs_ = std::auto_ptr<InterfaceType>(new rt_binary_expr<InterfaceType>(other.rhs()));
      }

      template <id_type id1, typename OP, id_type id2>
      explicit rt_binary_expr(ct_binary_expr<ct_variable<id1>, OP, ct_variable<id2> > const & /*other*/) : op_(new op_binary<OP, InterfaceType>())
      {
        //std::cout << "Constructing from expression " << other << " to " << OP::apply(value1, value2) << std::endl;
        lhs_ = std::auto_ptr<InterfaceType>(new rt_variable<InterfaceType>(id1));
        rhs_ = std::auto_ptr<InterfaceType>(new rt_variable<InterfaceType>(id2));
      }

      //resolving ambiguities:
      template <id_type id, typename OP, long value>
      explicit rt_binary_expr(ct_binary_expr<ct_variable<id>, OP, ct_constant<value> > const & /*other*/) : op_(new op_binary<OP, InterfaceType>())
      {
        //std::cout << "Constructing from expression " << other << " to " << OP::apply(value1, value2) << std::endl;
        lhs_ = std::auto_ptr<InterfaceType>(new rt_variable<InterfaceType>(id));
        rhs_ = std::auto_ptr<InterfaceType>(new rt_constant<numeric_type, InterfaceType>(value));
      }

      template <id_type id, typename OP, long value>
      explicit rt_binary_expr(ct_binary_expr<ct_constant<value>, OP, ct_variable<id> > const & /*other*/) : op_(new op_binary<OP, InterfaceType>())
      {
        //std::cout << "Constructing from expression " << other << " to " << OP::apply(value1, value2) << std::endl;
        lhs_ = std::auto_ptr<InterfaceType>(new rt_constant<numeric_type, InterfaceType>(value));
        rhs_ = std::auto_ptr<InterfaceType>(new rt_variable<InterfaceType>(id));
      }

      /*explicit rt_binary_expr(variable<InterfaceType> const & other) : lhs_(other.clone()),
                                                                    op_(new op_unary_id_type()),
                                                                    rhs_(other.clone()) {}


      template <typename T>
      explicit rt_binary_expr(constant<T> const & other) : lhs_(other.clone()),
                                                        op_(new op_unary_id_type()),
                                                        rhs_(other.clone()) {}

      template <long value>
      explicit rt_binary_expr(ct_constant<value> const & other) : lhs_(new rt_constant<numeric_type, InterfaceType>(value)),
                                                               op_(new op_unary_id_type()),
                                                               rhs_(new rt_constant<numeric_type, InterfaceType>(value)) {}  */

      //Copy CTOR:
      rt_binary_expr(binary_expr const & other) : lhs_(other.lhs_->clone()),
                                               op_(other.op_->clone()),
                                               rhs_(other.rhs_->clone()) {}

      //assignments:
      template <typename LHS, typename OP, typename RHS>
      rt_binary_expr & operator=(ct_binary_expr<LHS, OP, RHS> const & other)
      {
        lhs_ = std::auto_ptr<InterfaceType>(new rt_binary_expr<InterfaceType>(other.lhs()));
        op_ = std::auto_ptr<op_interface_type>(new op_binary<OP, InterfaceType>());
        rhs_ = std::auto_ptr<InterfaceType>(new rt_binary_expr<InterfaceType>(other.rhs()));
        return *this;
      }

      template <typename LHS, typename OP, long value>
      rt_binary_expr & operator=(ct_binary_expr<LHS, OP, ct_constant<value> > const & other)
      {
        //std::cout << "Constructing from expression " << other << std::endl;
        lhs_ = std::auto_ptr<InterfaceType>(new rt_binary_expr<InterfaceType>(other.lhs()));
        op_ = std::auto_ptr<op_interface_type>(new op_binary<OP, InterfaceType>());
        rhs_ = std::auto_ptr<InterfaceType>(new rt_constant<numeric_type, InterfaceType>(value));
        return *this;
      }

      template <long value, typename OP, typename RHS>
      rt_binary_expr & operator=(ct_binary_expr<ct_constant<value>, OP, RHS > const & other)
      {
        //std::cout << "Constructing from expression " << other << std::endl;
        lhs_ = std::auto_ptr<InterfaceType>(new rt_constant<numeric_type, InterfaceType>(value));
        op_ = std::auto_ptr<op_interface_type>(new op_binary<OP, InterfaceType>());
        rhs_ = std::auto_ptr<InterfaceType>(new rt_binary_expr<InterfaceType>(other.rhs()));
        return *this;
      }

      template <long value1, typename OP, long value2>
      rt_binary_expr & operator=(ct_binary_expr<ct_constant<value1>, OP, ct_constant<value2> > const & other)
      {
        //std::cout << "Constructing from expression " << other << std::endl;
        lhs_ = std::auto_ptr<InterfaceType>(new rt_constant<numeric_type, InterfaceType>(OP().apply(value1, value2)));
        op_  = std::auto_ptr<op_interface_type>(new op_unary_id_type());
        rhs_ = std::auto_ptr<InterfaceType>(new rt_constant<numeric_type, InterfaceType>(OP().apply(value1, value2)));
        return *this;
      }


      rt_binary_expr & operator=(rt_binary_expr const & other)
      {
        lhs_ = std::auto_ptr<InterfaceType>(other.lhs()->clone());
        op_  = std::auto_ptr<op_interface_type>(other.op()->clone());
        rhs_ = std::auto_ptr<InterfaceType>(other.rhs()->clone());
        return *this;
      }

      template <typename ScalarType>
      rt_binary_expr & operator=(rt_constant<ScalarType> const & other)
      {
        lhs_ = std::auto_ptr<InterfaceType>(other.clone());
        op_  = std::auto_ptr<op_interface_type>(new op_unary_id_type());
        rhs_ = std::auto_ptr<InterfaceType>(other.clone());
        return *this;
      }

      template <long value>
      rt_binary_expr & operator=(ct_constant<value> const & other)
      {
        return *this = value;
      }

      rt_binary_expr & operator=(numeric_type value)
      {
        lhs_ = std::auto_ptr<InterfaceType>(new rt_constant<numeric_type, InterfaceType>(value));
        op_  = std::auto_ptr<op_interface_type>(new op_unary_id_type());
        rhs_ = std::auto_ptr<InterfaceType>(new rt_constant<numeric_type, InterfaceType>(value));
        return *this;
      }

      const InterfaceType      * lhs() const { return lhs_.get(); }
      const op_interface_type  * op()  const { return op_.get(); }
      const InterfaceType      * rhs() const { return rhs_.get(); }

      ///////////////// evaluation: ///////////////////////////////

      //operator() is a convenience layer:
      numeric_type operator()(numeric_type val) const
      {
        return this->eval(val);
      }

      template <typename ScalarType>
      numeric_type operator()(rt_constant<ScalarType> val) const
      {
        return this->eval(static_cast<numeric_type>(val));
      }

      template <long value>
      numeric_type operator()(ct_constant<value> val) const
      {
        return this->eval(value);
      }

      template <typename VectorType>
      numeric_type operator()(VectorType const & v) const
      {
        std::vector<double> stl_v(v.size());
        for (std::size_t i=0; i<v.size(); ++i)
          stl_v[i] = v[i];

        return this->eval(stl_v);
      }

      numeric_type operator()(std::vector<numeric_type> const & stl_v) const
      {
        return this->eval(stl_v);
      }

      template <typename T0>
      numeric_type operator()(viennamath::ct_vector_1<T0> const & v) const
      {
        std::vector<double> stl_v(1);
        stl_v[0] = v[ct_index<0>()];
        return this->eval(stl_v);
      }

      template <typename T0, typename T1>
      numeric_type operator()(viennamath::ct_vector_2<T0, T1> const & v) const
      {
        std::vector<double> stl_v(2);
        stl_v[0] = v[ct_index<0>()];
        stl_v[1] = v[ct_index<1>()];
        return this->eval(stl_v);
      }

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
      numeric_type eval(std::vector<double> const & v) const
      {
        return op_->apply(lhs_.get()->eval(v), rhs_.get()->eval(v));
      }

      numeric_type eval(numeric_type val) const
      {
        return op_->apply(lhs_.get()->eval(val), rhs_.get()->eval(val));
      }

      ///////////////////// substitution /////////////////////////////


      /** @brief Returns a simplified expression with trivial operations removed. The caller is responsible for deleting the object the returned pointer refers to. */
      InterfaceType * simplify() const
      {
        if (lhs_->is_constant() && rhs_->is_constant())
          return new rt_constant<numeric_type, InterfaceType>( unwrap() );

        //std::cout << "Optimization forwarded to " << op_->str() << std::endl;
        return op_->simplify(lhs_.get(), rhs_.get());
      }

      /** @brief Returns true if the expression can be simplified */
      bool can_simplify() const
      {
        if (lhs_->is_constant() && rhs_->is_constant())
        {
          //std::cout << "optimizable(): true in rt_binary_expr" << std::endl;
          return true;
        }
        return op_->can_simplify(lhs_.get(), rhs_.get());
      }

      ///////// other interface requirements ////////////////////////
      /** @brief Returns a copy of the binary expression. The caller is responsible for deleting the returned object. */
      InterfaceType * clone() const { return new rt_binary_expr(lhs_->clone(), op_->clone(), rhs_->clone()); }

      /** @brief Returns a detailed string describing the binary expression. Acts recursively. */
      std::string deep_str() const
      {
        std::stringstream ss;
        ss << "(";
        ss << lhs_->deep_str();
        ss << op_->str();
        ss << rhs_->deep_str();
        ss << ")";
        return ss.str();
      }

      /** @brief Returns a short string describing the unary expression. Does not act recursively. */
      std::string shallow_str() const
      {
        return std::string("binary_expr");
      }

      /** @brief Returns whether the expression is a unary expression, which is not the case (-> false) */
      bool is_unary() const { return false; }

      /** @brief Reduces the expression to a constant. If this is not possible, an expression_not_unwrappable_exception is thrown. */
      numeric_type unwrap() const
      {
        //if (op_->is_unary())
        //  return lhs_->unwrap();
        return op_->apply(lhs_->unwrap(), rhs_->unwrap());
      }

      /** @brief Returns true if the expression merely represents a constant value */
      bool is_constant() const { return lhs_->is_constant() && rhs_->is_constant(); };

      /** @brief If 'e' equals to this binary expression, the replacement 'repl' is returned. Otherwise, the unary expression is cloned. Note that the caller must ensure proper deletion of the returned object.  */
      InterfaceType * substitute(const InterfaceType * e,
                                 const InterfaceType * repl) const
      {
        if (deep_equal(e))
          return repl->clone();

        return new rt_binary_expr(lhs_->substitute(e, repl),
                               op_->clone(),
                               rhs_->substitute(e, repl) );
      };

      /** @brief If one of the expressions in 'e' equals to this binary expression, the respective replacement 'repl' is returned. Otherwise, a copy of *this is returned. Note that the caller must ensure proper deletion of the returned object. */
      InterfaceType * substitute(std::vector<const InterfaceType *> const &  e,
                                 std::vector<const InterfaceType *> const &  repl) const
      {
        for (size_t i=0; i<e.size(); ++i)
          if (deep_equal(e[i]))
            return repl[i]->clone();

        return new rt_binary_expr(lhs_->substitute(e, repl),
                               op_->clone(),
                               rhs_->substitute(e, repl) );
      };


      /** @brief Performs a detailed comparison of the binary expression with the passed expression. Returns true if they represent the same expression (equal operands, equal operations, etc.) */
      bool deep_equal(const InterfaceType * other) const
      {
        if (dynamic_cast< const rt_binary_expr * >(other) != NULL)
        {
          const rt_binary_expr * temp = dynamic_cast< const rt_binary_expr * >(other);
          return lhs_->deep_equal(temp->lhs())
                 && op_->equal(temp->op())
                 && rhs_->deep_equal(temp->rhs());
        }
        return false;
        //return lhs_->deep_equal(other) && rhs_->deep_equal(other);
      }

      /** @brief Returns true if the passed expression is any binary expression. */
      bool shallow_equal(const InterfaceType * other) const
      {
        return dynamic_cast< const self_type * >(other) != NULL;
      }

      /** @brief Returns the derivative of this binary expression with respect to the variable 'diff_var'. The returned object must be deleted by the caller. */
      InterfaceType * diff(const InterfaceType * diff_var) const
      {
        return op_->diff(lhs_.get(), rhs_.get(), diff_var);
      }

      /** @brief Recursively manipulates the expression. If this expression is manipulated, the result is returned directly. Otherwise, the manipulator continues with the expression the unary operation is acting on. The returned object must be deleted by the caller. */
      InterfaceType * recursive_manipulation(rt_manipulation_wrapper<InterfaceType> const & fw) const
      {
        if (fw.modifies(this))
          return fw(this);

        return new rt_binary_expr(lhs_->recursive_manipulation(fw),
                               op_->clone(),
                               rhs_->recursive_manipulation(fw) );
      }

      /** @brief Recursively traverses the expression by calling the wrapper for each object without manipulating it. */
      void recursive_traversal(rt_traversal_wrapper<InterfaceType> const & fw) const
      {
        if (fw.step_into(this))
        {
          lhs_->recursive_traversal(fw);
          fw(this);
          rhs_->recursive_traversal(fw);
        }
        else
          fw(this);
      }

    private:
      /** @brief The left hand side operand */
      std::auto_ptr<InterfaceType>         lhs_;
      /** @brief The operation */
      std::auto_ptr<op_interface_type>     op_;
      /** @brief The right hand side operand */
      std::auto_ptr<InterfaceType>         rhs_;
  };


  /** @brief Overload of the stream operator in order to allow for a convenient conversion of a unary expression to a string. */
  template <typename InterfaceType>
  std::ostream& operator<<(std::ostream & stream, rt_binary_expr<InterfaceType> const & e)
  {
    stream << "expr"
           << e.deep_str()
           << "";
    return stream;
  }

  template <typename T, typename InterfaceType>
  InterfaceType * op_unary<T, InterfaceType>::simplify(const InterfaceType * lhs,
                                                       const InterfaceType * rhs) const
  {
    return new rt_binary_expr<InterfaceType>(lhs->clone(),
                                          new op_unary<T, InterfaceType>(),
                                          rhs->clone());
  }

}

#endif
