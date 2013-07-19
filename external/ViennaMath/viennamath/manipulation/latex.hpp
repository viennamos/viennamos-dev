#ifndef VIENNAMATH_MANIPULATION_LATEX_HPP
#define VIENNAMATH_MANIPULATION_LATEX_HPP

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




#include <map>

#include "viennamath/forwards.h"
#include "viennamath/runtime/unary_expr.hpp"
#include "viennamath/compiletime/unary_op_tags.hpp"

/** @file  latex.hpp
    @brief Provides routines for generating a LaTeX formula out of a ViennaMath expression.
*/

namespace viennamath
{
  //
  // LaTeX processing
  //
  template <typename InterfaceType = default_interface_type>
  class rt_latex_translator;

  template <typename InterfaceType = default_interface_type>
  class rt_latex_binary_expr_processor;

  template <typename InterfaceType = default_interface_type>
  class rt_latex_unary_expr_processor;

  template <typename InterfaceType = default_interface_type>
  class rt_latex_constant_processor;

  template <typename InterfaceType = default_interface_type>
  class rt_latex_variable_processor;

  template <typename InterfaceType = default_interface_type>
  class rt_latex_function_symbol_processor;



  /** @brief The abstract interface for each LaTeX processor module.
   *
   * @tparam InterfaceType    The interface to inherit from. Usually rt_expression_interface, but extensions are possible.
   */
  template <typename InterfaceType = default_interface_type>
  class rt_latex_processor_interface
  {
    public:
      /** @brief Returns a clone of the processor object */
      virtual rt_latex_processor_interface<InterfaceType> * clone() const = 0;

      /** @brief The interface for converting an expression 'e' to a LaTeX string.
       *
       * @param e                  Pointer to a runtime expression
       * @param use_parenthesis    A flag indicating whether the expression should be wrapped in parentheses in order to preserve priority of evaluation
       * @param trans              The LaTeX translater object for callback operations when recursively printing expressions
       */
      virtual std::string process(InterfaceType const * e,
                                  bool use_parenthesis,
                                  rt_latex_translator<InterfaceType> const & trans) const = 0;

      /** @brief The interface for customization of output. The expression 'e' is customized to return the LaTeX string 'str'.
       *
       * @param e                  Pointer to the ViennaMath runtime expression
       * @param str                The LaTeX string to be used for the expression referred to be 'e'
       */
      virtual bool customize(InterfaceType const * /*e*/, std::string const & /*str*/) { return false; }
  };


  /** @brief The main LaTeX translator class. To be used and manipulated by the ViennaMath library user.
   *
   * @tparam InterfaceType    The interface to inherit from. Usually rt_expression_interface, but extensions are possible.
   */
  template <typename InterfaceType>
  class rt_latex_translator
  {
      typedef typename InterfaceType::numeric_type                           NumericType;

      typedef viennamath::rt_binary_expr<InterfaceType>                      BinaryExpr;
      typedef viennamath::rt_unary_expr<InterfaceType>                       UnaryExpr;
      typedef viennamath::rt_constant<default_numeric_type, InterfaceType>   Constant;
      typedef viennamath::rt_variable<InterfaceType>                         Variable;
      typedef viennamath::rt_function_symbol<InterfaceType>                  FuncSymbol;
      typedef viennamath::rt_equation<InterfaceType>                         Equation;
      typedef viennamath::rt_interval<InterfaceType>                         Interval;

      typedef rt_latex_processor_interface<InterfaceType>                    ProcessorInterface;
      typedef std::vector< ProcessorInterface *>                             ProcessorArray;

    public:

      rt_latex_translator()
      {
        processors_.push_back(new rt_latex_binary_expr_processor<InterfaceType>());
        processors_.push_back(new rt_latex_unary_expr_processor<InterfaceType>());
        processors_.push_back(new rt_latex_constant_processor<InterfaceType>());
        processors_.push_back(new rt_latex_variable_processor<InterfaceType>());
        processors_.push_back(new rt_latex_function_symbol_processor<InterfaceType>());
      }

      rt_latex_translator(rt_latex_translator const & other)
      {
        for (typename ProcessorArray::const_iterator it = other.processors_.begin();
                                                     it != other.processors_.end();
                                                   ++it)
        {
          processors_.push_back( (*it)->clone() );
        }
      }

      ~rt_latex_translator()
      {
        for (typename ProcessorArray::iterator it = processors_.begin();
                                               it != processors_.end();
                                             ++it)
        {
          delete *it;
        }
      }

      //
      // Generic runtime expression. Unwrap it and then scan through types
      //
      std::string operator()(viennamath::rt_expr<InterfaceType> const & e,
                             bool use_parenthesis = false) const
      {
        return process_impl(e.get(), use_parenthesis);
      }

      std::string operator()(BinaryExpr const & e, bool use_parenthesis = false) const { return process_impl(&e, use_parenthesis); }
      std::string operator()(UnaryExpr  const & e, bool use_parenthesis = false) const { return process_impl(&e, use_parenthesis); }

      template <typename ScalarType>
      std::string operator()(viennamath::rt_constant<ScalarType, InterfaceType> const & e, bool use_parenthesis = false) const { return process_impl(e, use_parenthesis); }
      std::string operator()(Variable   const & e, bool use_parenthesis = false) const { return process_impl(&e, use_parenthesis); }
      std::string operator()(FuncSymbol const & e, bool use_parenthesis = false) const { return process_impl(&e, use_parenthesis); }
      std::string operator()(Equation   const & e, bool use_parenthesis = false) const
      {
        std::stringstream ss;
        ss << operator()(e.lhs()) << " = " << operator()(e.rhs());
        static_cast<int>(use_parenthesis); //to silence unused parameter warnings
        return ss.str();
      }
      std::string operator()(Interval   const & e, bool use_parenthesis = false) const
      {
        std::stringstream ss;
        ss << " [ " << operator()(e.lower()) << "; " << operator()(e.upper()) << " ] ";
        static_cast<int>(use_parenthesis); //to silence unused parameter warnings
        return ss.str();
      }

      /** @brief Enables customization of LaTeX output. The expression 'e' is customized to return the LaTeX string 'str'.
       *
       * @param e                  Pointer to the ViennaMath runtime expression
       * @param output             The LaTeX string to be used for the expression referred to be 'e'
       */
      template <typename ExpressionType>
      void customize(ExpressionType const & e, std::string output)
      {
        bool found = false;
        for (typename ProcessorArray::const_iterator it = processors_.begin();
                                                     it != processors_.end();
                                                   ++it)
        {
          found |= (*it)->customize(&e, output);
        }

        if (!found)
          std::cerr << "Warning: Cannot customize LaTeX translator for " << e << " (no suitable processor found)" << std::endl;
        //variable_strings_[x.id()] = output;
      }

      /** @brief Adds another LaTeX processor. Typically required only if user-defined ViennaMath expressions are in use. */
      void add_processor(ProcessorInterface * proc_ptr)
      {
        processors_.push_back(proc_ptr);
      }

    private:

      std::string process_impl(InterfaceType const * ptr,
                               bool use_parenthesis = false) const
      {
        std::string ret;

        for (typename ProcessorArray::const_iterator it = processors_.begin();
                                                     it != processors_.end();
                                                   ++it)
        {
          ret = (*it)->process(ptr, use_parenthesis, *this);
          if (ret.size() > 0)
            return ret;
        }

        std::cerr << "ViennaMath: Warning: Unknown type in latex translator: " << ptr << std::endl;
        //throw "Unknown type!";
        return " \\chi_{\\mathrm{unknown}} ";
      }

    ProcessorArray processors_;
  };

  /** @brief Convenience typedef for the latex translator in order to avoid the ugly '<>' for the default template parameter of rt_latex_translator. */
  typedef rt_latex_translator<>      latex_translator;



  //
  // Binary expression
  //
  /** @brief A LaTeX processor responsible for translating binary expressions to LaTeX code.
   *
   * @tparam InterfaceType    The interface to inherit from. Usually rt_expression_interface, but extensions are possible.
   */
  template <typename InterfaceType>
  class rt_latex_binary_expr_processor : public rt_latex_processor_interface<InterfaceType>
  {
      typedef typename InterfaceType::numeric_type           NumericType;
      typedef viennamath::rt_binary_expr<InterfaceType>      BinaryExpr;

    public:
      rt_latex_processor_interface<InterfaceType> * clone() const { return new rt_latex_binary_expr_processor(); }

      std::string process(InterfaceType const * ptr, bool use_parenthesis, rt_latex_translator<InterfaceType> const & translator) const
      {
        if (dynamic_cast< const BinaryExpr * >(ptr) != NULL)
        {
          const BinaryExpr * temp = dynamic_cast< const BinaryExpr * >(ptr);
          return process_impl(*temp, use_parenthesis, translator);
        }

        return "";
      }

    private:

      std::string process_impl(BinaryExpr const & e, bool use_parenthesis, rt_latex_translator<InterfaceType> const & translator) const
      {
        std::stringstream ss;
        viennamath::rt_expr<InterfaceType> lhs(e.lhs()->clone());
        viennamath::rt_expr<InterfaceType> rhs(e.rhs()->clone());

        if (dynamic_cast< const op_binary<op_div<NumericType>, InterfaceType> * >(e.op()) != NULL)  //division -> \frac{}{}
        {
          if (use_parenthesis)
            ss << " \\left( ";
          ss << " \\frac{" << translator(lhs) << "}{" << translator(rhs) << "} ";
          if (use_parenthesis)
            ss << " \\right) ";
        }
        else
        {
          bool is_op_mult = false;
          if (dynamic_cast< const op_binary<op_mult<NumericType>, InterfaceType> * >(e.op()) != NULL)
            is_op_mult = true;

          bool is_op_minus = false;
          if (dynamic_cast< const op_binary<op_minus<NumericType>, InterfaceType> * >(e.op()) != NULL)
            is_op_minus = true;

          if (use_parenthesis)
            ss << "(";
          ss << translator(lhs, is_op_mult);
          if (is_op_mult)
            ss << " \\cdot ";
          else
            ss << " " << e.op()->str() << " ";
          ss << translator(rhs, is_op_mult || is_op_minus);
          if (use_parenthesis)
            ss << ")";
        }

        return ss.str();
      }

  };



  //
  // Unary expression
  //
  /** @brief A LaTeX processor responsible for translating unary expressions to LaTeX code.
   *
   * @tparam InterfaceType    The interface to inherit from. Usually rt_expression_interface, but extensions are possible.
   */
  template <typename InterfaceType>
  class rt_latex_unary_expr_processor : public rt_latex_processor_interface<InterfaceType>
  {
      typedef typename InterfaceType::numeric_type           NumericType;
      typedef viennamath::rt_unary_expr<InterfaceType>       UnaryExpr;

    public:
      rt_latex_processor_interface<InterfaceType> * clone() const { return new rt_latex_unary_expr_processor(); }

      std::string process(InterfaceType const * ptr, bool use_parenthesis, rt_latex_translator<InterfaceType> const & translator) const
      {
        if (dynamic_cast< const UnaryExpr * >(ptr) != NULL)
        {
          const UnaryExpr * temp = dynamic_cast< const UnaryExpr * >(ptr);
          return process_impl(*temp, use_parenthesis, translator);
        }

        return "";
      }

    private:

      std::string process_impl(UnaryExpr const & e, bool use_parenthesis, rt_latex_translator<InterfaceType> const & translator) const
      {
        typedef op_unary<op_id<NumericType>,    InterfaceType>    OpId;
        typedef op_unary<op_exp<NumericType>,   InterfaceType>    OpExp;
        typedef op_unary<op_sin<NumericType>,   InterfaceType>    OpSin;
        typedef op_unary<op_cos<NumericType>,   InterfaceType>    OpCos;
        typedef op_unary<op_tan<NumericType>,   InterfaceType>    OpTan;
        typedef op_unary<op_fabs<NumericType>,  InterfaceType>    OpFabs;
        typedef op_unary<op_sqrt<NumericType>,  InterfaceType>    OpSqrt;
        typedef op_unary<op_log<NumericType>,   InterfaceType>    OpLog;
        typedef op_unary<op_log10<NumericType>, InterfaceType>    OpLog10;

        // symbolic stuff:
        typedef op_unary<op_gradient<NumericType>, InterfaceType>              OpGradient;
        typedef op_unary<op_divergence<NumericType>, InterfaceType>            OpDivergence;
        typedef op_unary<op_partial_deriv<NumericType>, InterfaceType>         OpPartialDeriv;
        typedef op_unary<op_rt_integral<InterfaceType>, InterfaceType>         OpRuntimeIntegration;
        typedef op_unary<op_rt_symbolic_integral<InterfaceType>, InterfaceType>         OpSymbolicIntegration;

        std::stringstream ss;
        viennamath::rt_expr<InterfaceType> lhs(e.lhs()->clone());

        if (dynamic_cast< const OpId * >(e.op()) != NULL)
          ss << translator(lhs, use_parenthesis);
        else if (dynamic_cast< const OpExp * >(e.op()) != NULL)
          ss << " \\exp " << translator(lhs, true) << " ";
        else if (dynamic_cast< const OpSin * >(e.op()) != NULL)
          ss << " \\sin " << translator(lhs, true) << " ";
        else if (dynamic_cast< const OpCos * >(e.op()) != NULL)
          ss << " \\cos " << translator(lhs, true) << " ";
        else if (dynamic_cast< const OpTan * >(e.op()) != NULL)
          ss << " \\tan " << translator(lhs, true) << " ";
        else if (dynamic_cast< const OpFabs * >(e.op()) != NULL)
          ss << " \\left|" << translator(lhs) << "\\right| ";
        else if (dynamic_cast< const OpSqrt * >(e.op()) != NULL)
          ss << " \\sqrt{" << translator(lhs) << "} ";
        else if (dynamic_cast< const OpLog * >(e.op()) != NULL)
          ss << " \\ln " << translator(lhs, true) << " ";
        else if (dynamic_cast< const OpLog10 * >(e.op()) != NULL)
          ss << " \\log " << translator(lhs, true) << " ";

        else if (dynamic_cast< const OpGradient * >(e.op()) != NULL)
          ss << " \\nabla " << translator(lhs, true) << " ";
        else if (dynamic_cast< const OpDivergence * >(e.op()) != NULL)
          ss << " \\nabla \\cdot " << translator(lhs, true) << " ";
        else if (dynamic_cast< const OpPartialDeriv * >(e.op()) != NULL)
        {
          const OpPartialDeriv * tmp = dynamic_cast< const OpPartialDeriv * >(e.op());
          viennamath::rt_variable<InterfaceType> var(tmp->op().id());
          ss << " \\frac{\\partial " << translator(lhs, true) << "}{\\partial " << translator(var) << "} ";
        }
        else if (dynamic_cast< const OpRuntimeIntegration * >(e.op()) != NULL)
        {
          const OpRuntimeIntegration * op_integral = dynamic_cast< const OpRuntimeIntegration * >(e.op());

          ss << " \\int_{ " << translator(op_integral->op().interval()) << " } ";
          ss << translator(lhs) << " ";
          ss << " \\: \\mathrm{d} " << translator(op_integral->op().variable()) << " ";
        }
        else if (dynamic_cast< const OpSymbolicIntegration * >(e.op()) != NULL)
        {
          const OpSymbolicIntegration * op_integral = dynamic_cast< const OpSymbolicIntegration * >(e.op());
          // integral:
          ss << " \\int_{\\Omega_{" << op_integral->op().interval().id() << "}} ";
          ss << translator(lhs) << " ";
          ss << " \\: \\mathrm{d} \\Omega ";
        }
        else
        {
          std::cout << "Operator not supported: " << e.op()->str() << std::endl;
          throw "Not supported!";
        }

        return ss.str();
      }

  };


  //
  // Constant
  //
  /** @brief A LaTeX processor responsible for translating ViennaMath runtime constants to LaTeX code.
   *
   * @tparam InterfaceType    The interface to inherit from. Usually rt_expression_interface, but extensions are possible.
   */
  template <typename InterfaceType>
  class rt_latex_constant_processor : public rt_latex_processor_interface<InterfaceType>
  {
      typedef typename InterfaceType::numeric_type           NumericType;
      typedef viennamath::rt_constant<default_numeric_type, InterfaceType>   Constant;

    public:
      rt_latex_processor_interface<InterfaceType> * clone() const { return new rt_latex_constant_processor(); }

      std::string process(InterfaceType const * ptr, bool use_parenthesis, rt_latex_translator<InterfaceType> const & translator) const
      {
        if (dynamic_cast< const Constant * >(ptr) != NULL)
        {
          const Constant * temp = dynamic_cast< const Constant * >(ptr);
          return process_impl(*temp, use_parenthesis, translator);
        }

        return "";
      }

    private:

      std::string process_impl(Constant const & e, bool /*use_parenthesis*/, rt_latex_translator<InterfaceType> const & /*translator*/) const
      {
        std::stringstream ss;
        ss << " " << static_cast<NumericType>(e) << " ";
        return ss.str();
      }

  };


  //
  // Variable
  //
  /** @brief A LaTeX processor responsible for translating ViennaMath runtime variables to LaTeX code.
   *
   * @tparam InterfaceType    The interface to inherit from. Usually rt_expression_interface, but extensions are possible.
   */
  template <typename InterfaceType>
  class rt_latex_variable_processor : public rt_latex_processor_interface<InterfaceType>
  {
      typedef typename InterfaceType::numeric_type           NumericType;
      typedef viennamath::rt_variable<InterfaceType>                         Variable;

    public:
      rt_latex_processor_interface<InterfaceType> * clone() const { return new rt_latex_variable_processor(); }

      std::string process(InterfaceType const * ptr, bool use_parenthesis, rt_latex_translator<InterfaceType> const & translator) const
      {
        if (dynamic_cast< const Variable * >(ptr) != NULL)
        {
          const Variable * temp = dynamic_cast< const Variable * >(ptr);
          return process_impl(*temp, use_parenthesis, translator);
        }

        return "";
      }

      bool customize(InterfaceType const * ptr, std::string const & str)
      {
        if (dynamic_cast< const Variable * >(ptr) != NULL)
        {
          const Variable * temp = dynamic_cast< const Variable * >(ptr);
          variable_strings_[temp->id()] = str;
          return true;
        }
        return false;
      }

    private:

      std::string process_impl(Variable const & e, bool /*use_parenthesis*/, rt_latex_translator<InterfaceType> const & /*translator*/) const
      {
        std::stringstream ss;

        std::map<id_type, std::string>::const_iterator it = variable_strings_.find(e.id());
        if (it != variable_strings_.end())
          ss << it->second;
        else
          ss << " x_{" << e.id() << "} ";

        return ss.str();
      }

      std::map<id_type, std::string>                            variable_strings_;
  };

  //
  // Function Symbol
  //
  /** @brief A LaTeX processor responsible for translating ViennaMath runtime function symbols to LaTeX code.
   *
   * @tparam InterfaceType    The interface to inherit from. Usually rt_expression_interface, but extensions are possible.
   */
  template <typename InterfaceType>
  class rt_latex_function_symbol_processor : public rt_latex_processor_interface<InterfaceType>
  {
      typedef typename InterfaceType::numeric_type              NumericType;
      typedef viennamath::rt_function_symbol<InterfaceType>     FuncSymbol;
      typedef std::pair<id_type, id_type>           KeyType;

    public:
      rt_latex_processor_interface<InterfaceType> * clone() const { return new rt_latex_function_symbol_processor(); }

      std::string process(InterfaceType const * ptr, bool use_parenthesis, rt_latex_translator<InterfaceType> const & translator) const
      {
        if (dynamic_cast< const FuncSymbol * >(ptr) != NULL)
        {
          const FuncSymbol * temp = dynamic_cast< const FuncSymbol * >(ptr);
          return process_impl(*temp, use_parenthesis, translator);
        }

        return "";
      }

      bool customize(InterfaceType const * ptr, std::string const & str)
      {
        if (dynamic_cast< const FuncSymbol * >(ptr) != NULL)
        {
          const FuncSymbol * temp = dynamic_cast< const FuncSymbol * >(ptr);
          KeyType key(temp->id(), temp->tag_id());
          function_symbol_strings_[key] = str;
          return true;
        }
        return false;
      }

    private:

      std::string process_impl(FuncSymbol const & e, bool /*use_parenthesis*/, rt_latex_translator<InterfaceType> const & /*translator*/) const
      {
        std::stringstream ss;

        KeyType func_symb_id(e.id(), e.tag_id());
        std::map<KeyType, std::string>::const_iterator it = function_symbol_strings_.find(func_symb_id);
        if (it != function_symbol_strings_.end())
        {
          ss << it->second;
        }
        else
        {
          if (e.tag_id() == test_tag<>::tag_id())
            ss << " v_{" << e.id() << "} ";
          else
            ss << " u_{" << e.id() << "} ";
        }

        return ss.str();
      }

      std::map<KeyType, std::string>  function_symbol_strings_;

  };



}

#endif
