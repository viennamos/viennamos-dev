#ifndef VIENNAMATH_MANIPULATION_SUBSTITUTE_HPP
#define VIENNAMATH_MANIPULATION_SUBSTITUTE_HPP

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



#include "viennamath/forwards.h"
#include "viennamath/runtime/functor_wrapper.hpp"
#include "viennamath/manipulation/simplify.hpp"
#include <assert.h>

/** @file substitute.hpp
    @brief Defines routines for substituting (parts of) an expression by another expression.
*/

namespace viennamath
{

  //
  //   Section 1: Compile time substitution
  //

  namespace result_of
  {
    /** @brief Default case for the compiletime substitution metafunction: Do not define any return type. This provides SFINAE for the interface function. */
    template <typename SearchType,
              typename ReplacementType,
              typename ExpressionType>
    struct substitute {};  //Note: not having a default type defined serves as SFINAE for viennamath::substitute() as well (no runtime types allowed)

    //
    // generic handling of primitives
    //
    /** @brief For a binary expression, the substitution is performed for both operands. */
    template <typename SearchType,
              typename ReplacementType,
              typename LHS, typename OP, typename RHS>
    struct substitute <SearchType, ReplacementType, ct_binary_expr<LHS, OP, RHS> >
    {
      typedef ct_binary_expr< typename substitute<SearchType, ReplacementType, LHS>::type,
                              OP,
                              typename substitute<SearchType, ReplacementType, RHS>::type >   type;
    };

    /** @brief For a unary expression, the substitution is performed for the operand. */
    template <typename SearchType,
              typename ReplacementType,
              typename LHS, typename OP>
    struct substitute <SearchType, ReplacementType, ct_unary_expr<LHS, OP> >
    {
      typedef ct_unary_expr< typename substitute<SearchType, ReplacementType, LHS>::type,
                             OP >   type;
    };

    /** @brief By default, a compiletime constant is left unmodified (no match). */
    template <typename SearchType,
              typename ReplacementType,
              long value>
    struct substitute <SearchType, ReplacementType, ct_constant<value> >
    {
      typedef ct_constant<value>   type;
    };

    /** @brief By default, a compiletime function symbol is left unmodified (no match). */
    template <typename SearchType,
              typename ReplacementType,
              typename TAG>
    struct substitute <SearchType, ReplacementType, ct_function_symbol<TAG> >
    {
      typedef ct_function_symbol<TAG>   type;
    };

    /** @brief By default, a compiletime variable is left unmodified (no match). */
    template <typename SearchType,
              typename ReplacementType,
              id_type id>
    struct substitute <SearchType, ReplacementType, ct_variable<id> >
    {
      typedef ct_variable<id>   type;
    };

    //
    // replacement specializations
    //
    /** @brief If there is a match for the binary expression, the replacement is returned. */
    template <typename ReplacementType,
              typename LHS, typename OP, typename RHS>
    struct substitute <ct_binary_expr<LHS, OP, RHS>, ReplacementType, ct_binary_expr<LHS, OP, RHS> >
    {
      typedef ReplacementType   type;
    };

    /** @brief If there is a match for the unary expression, the replacement is returned. */
    template <typename ReplacementType,
              typename LHS, typename OP>
    struct substitute <ct_unary_expr<LHS, OP>, ReplacementType, ct_unary_expr<LHS, OP> >
    {
      typedef ReplacementType   type;
    };

    /** @brief If there is a match for the compiletime constant, the replacement is returned. */
    template <typename ReplacementType,
              long value>
    struct substitute <ct_constant<value>, ReplacementType, ct_constant<value> >
    {
      typedef ReplacementType   type;
    };

    /** @brief If there is a match for the compiletime function symbol, the replacement is returned. */
    template <typename ReplacementType,
              typename TAG>
    struct substitute <ct_function_symbol<TAG>, ReplacementType, ct_function_symbol<TAG> >
    {
      typedef ReplacementType   type;
    };

    /** @brief If there is a match for the compiletime variable, the replacement is returned. */
    template <typename ReplacementType,
              id_type id>
    struct substitute <ct_variable<id>, ReplacementType, ct_variable<id> >
    {
      typedef ReplacementType   type;
    };

  }

  // interface:
  /** @brief The generic interface function for compiletime substitution.
   *
   * @tparam SearchType    Type of the expression to be substituted
   * @tparam ReplacementType   Type of the expression used as replacement
   * @tparam ExpressionType    Type of the expression in which the substitution is carried out
   */
  template <typename SearchType,
            typename ReplacementType,
            typename ExpressionType>
  typename result_of::substitute< SearchType, ReplacementType, ExpressionType>::type
  substitute(SearchType const &, ReplacementType const &, ExpressionType const & /*ex*/)
  {
    return typename result_of::substitute< SearchType, ReplacementType, ExpressionType>::type();
  }



  //
  //   Section 2: Run time substitution
  //



  //public interface:
  /** @brief Replaces all occurances of the variable 'u' in the expression 'e' with 'repl'. */
  template <typename InterfaceType, typename ReplacementType, typename ExpressionType>
  rt_expr<InterfaceType> substitute(rt_variable<InterfaceType> const & u,
                                    ReplacementType const & repl,
                                    ExpressionType const & e)
  {
    rt_expr<InterfaceType> temp(e.substitute(&u, &repl));
    inplace_simplify(temp);
    return temp;
  }

  /** @brief Replaces all occurances of the variable 'u' in the expression 'e' with 'repl'. */
  template <typename InterfaceType, typename ReplacementType>
  rt_expr<InterfaceType> substitute(rt_variable<InterfaceType> const & u,
                                    ReplacementType const & repl,
                                    rt_expr<InterfaceType> const & e)
  {
    rt_expr<InterfaceType> temp(e.get()->substitute(&u, &repl));
    inplace_simplify(temp);
    return temp;
  }

  /** @brief Replaces all occurances of the variable 'u' in the expression 'e' with 'repl'. */
  template <typename InterfaceType>
  rt_expr<InterfaceType> substitute(rt_variable<InterfaceType> const & u,
                                    default_numeric_type repl,
                                    rt_expr<InterfaceType> const & e)
  {
    rt_constant<default_numeric_type, InterfaceType> c(repl);
    rt_expr<InterfaceType> temp(e.get()->substitute(&u, &c));
    inplace_simplify(temp);
    return temp;
  }

  /** @brief Replaces all occurances of the function symbol 'u' in the expression 'e' with 'repl'. */
  template <typename InterfaceType>
  rt_expr<InterfaceType> substitute(rt_function_symbol<InterfaceType> const & u,
                                    default_numeric_type repl,
                                    rt_expr<InterfaceType> const & e)
  {
    rt_constant<default_numeric_type, InterfaceType> c(repl);
    rt_expr<InterfaceType> temp(e.get()->substitute(&u, &c));
    inplace_simplify(temp);
    return temp;
  }

  /** @brief Replaces all occurances of the function symbol 'u' in the expression 'e' with 'repl'. */
  template <typename InterfaceType>
  rt_expr<InterfaceType> substitute(rt_function_symbol<InterfaceType> const & u,
                                    rt_expr<InterfaceType> const & repl,
                                    rt_expr<InterfaceType> const & e)
  {
    rt_expr<InterfaceType> temp(e.get()->substitute(&u, repl.get()));
    inplace_simplify(temp);
    return temp;
  }

  //substitute unary and  binary expressions:
  /** @brief Replaces all occurances of the unary expression 'search' in the expression 'e' with 'repl'. */
  template <typename InterfaceType, typename ReplacementType>
  rt_expr<InterfaceType> substitute(rt_unary_expr<InterfaceType> const & search,
                                    ReplacementType const & repl,
                                    rt_expr<InterfaceType> const & e)
  {
    rt_expr<InterfaceType> temp(e.get()->substitute(&search, &repl));
    inplace_simplify(temp);
    return temp;
  }

  /** @brief Replaces all occurances of the unary expression 'search' in the expression 'e' with 'repl'. */
  template <typename InterfaceType>
  rt_expr<InterfaceType> substitute(rt_unary_expr<InterfaceType> const & search,
                                    rt_expr<InterfaceType> const & repl,
                                    rt_expr<InterfaceType> const & e)
  {
    rt_expr<InterfaceType> temp(e.get()->substitute(&search, repl.get()));
    inplace_simplify(temp);
    return temp;
  }

  /** @brief Replaces all occurances of the binary expression 'search' in the expression 'e' with 'repl'. */
  template <typename InterfaceType, typename ReplacementType>
  rt_expr<InterfaceType> substitute(rt_binary_expr<InterfaceType> const & search,
                                    ReplacementType const & repl,
                                    rt_expr<InterfaceType> const & e)
  {
    rt_expr<InterfaceType> temp(e.get()->substitute(&search, &repl));
    inplace_simplify(temp);
    return temp;
  }

  /** @brief Replaces all occurances of the expression 'search' in the expression 'e' with 'repl'. */
  template <typename InterfaceType>
  rt_expr<InterfaceType> substitute(rt_expr<InterfaceType> const & search,
                                    rt_expr<InterfaceType> const & repl,
                                    rt_expr<InterfaceType> const & e)
  {
    rt_expr<InterfaceType> temp(e.get()->substitute(search.get(), repl.get()));
    inplace_simplify(temp);
    return temp;
  }


  /** @brief Replaces all occurances of the the expressions in 'search' in the expression 'e' with the corresponding expressions in 'repl'. */
  template <typename InterfaceType>
  rt_expr<InterfaceType> substitute(std::vector<rt_expr<InterfaceType> > const & search,
                                    std::vector<rt_expr<InterfaceType> > const & repl,
                                    rt_expr<InterfaceType> const & e)
  {
    assert(search.size() == repl.size() && "Search and replace array must have the same length!");

    std::vector<const InterfaceType *> search_ptrs(search.size());
    for (size_t i=0; i<search_ptrs.size(); ++i)
      search_ptrs[i] = search[i].get();

    std::vector<const InterfaceType *> repl_ptrs(repl.size());
    for (size_t i=0; i<repl_ptrs.size(); ++i)
      repl_ptrs[i] = repl[i].get();

    rt_expr<InterfaceType> temp(e.get()->substitute(search_ptrs, repl_ptrs));
    inplace_simplify(temp);
    return temp;
  }



  // substitute intervals

  /** @brief Implementation details for functionality in ViennaMath. Not intended for direct use. */
  namespace detail
  {
    template <typename InterfaceType>
    class integral_substitution_functor : public viennamath::rt_manipulation_interface<InterfaceType>
    {
      public:
        integral_substitution_functor(op_unary<op_rt_integral<InterfaceType>, InterfaceType> const * ptr) : ptr_(ptr) {}


        InterfaceType * operator()(InterfaceType const * e) const
        {
          viennamath::rt_unary_expr<InterfaceType> const * ex_ptr = dynamic_cast< viennamath::rt_unary_expr<InterfaceType> const * >(e);
          if (ex_ptr != NULL)
          {
            typedef op_unary<op_rt_symbolic_integral<InterfaceType>, InterfaceType> const *  OpPtrType;

            OpPtrType op_ptr = dynamic_cast< OpPtrType >(ex_ptr->op());
            if (op_ptr != NULL)
            {
              //std::cout << "Modifies!" << std::endl;
              return new viennamath::rt_unary_expr<InterfaceType>(ex_ptr->lhs()->clone(), ptr_->clone());
            }
          }

          return e->clone();
        }


        bool modifies(InterfaceType const * e) const
        {
          //std::cout << "Checking " << e->shallow_str() << std::endl;
          viennamath::rt_unary_expr<InterfaceType> const * ex_ptr = dynamic_cast< viennamath::rt_unary_expr<InterfaceType> const * >(e);
          if (ex_ptr != NULL)
          {
            typedef op_unary<op_rt_symbolic_integral<InterfaceType>, InterfaceType> const *  OpPtrType;

            OpPtrType op_ptr = dynamic_cast< OpPtrType >(ex_ptr->op());
            if (op_ptr != NULL)
            {
              //std::cout << "FOUND!" << std::endl;
              return true;
            }
          }
          return false;
        }

      private:
        std::auto_ptr<const op_unary<op_rt_integral<InterfaceType>, InterfaceType> > ptr_;
    };
  } //namespace detail

  /** @brief Substitutes a symbolic interval with a concrete interval.
   *
   * @param search     The symbolic interval to be replaced
   * @param repl       A pair consisting of the integration interval and the integration variable. Must be interface compatible with std::pair<>.
   * @param e          The expression on which the substitution is carried out
   */
  template <typename InterfaceType, typename PairType>
  rt_expr<InterfaceType> substitute(rt_symbolic_interval<InterfaceType> const & /*search*/,
                                    PairType const & repl,
                                    rt_expr<InterfaceType> const & e)
  {
    typedef op_rt_integral<InterfaceType>    OperatorT;

    viennamath::rt_manipulation_wrapper<> fun( new detail::integral_substitution_functor<InterfaceType>(new op_unary<OperatorT>(OperatorT(repl.first, repl.second))) );

    rt_expr<InterfaceType> temp(e.get()->recursive_manipulation(fun));

    return temp;
  }

}

#endif
