#ifndef VIENNAMATH_FORWARDS_H
#define VIENNAMATH_FORWARDS_H

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

/**
 @mainpage Source Code Documentation for ViennaMath 1.0.0

 Thank you for downloading ViennaMath!
 These pages provide the Doxygen-generated source code documentation with the many details on the functions implemented in ViennaMath.

 For a general overview over the types and functionality provided by ViennaMath, please refer to the file doc/viennamath.pdf.

*/


#include <string>
#include <vector>
#include <exception>
#include <sstream>
#include <memory>

/** @file forwards.h
    @brief Contains forward declarations and definition of small classes/metafunctions required to be defined at an early stage.
*/

/** @brief The main ViennaMath namespace. All types and functions for the user reside here. */
namespace viennamath
{
  //basic numeric type (customize to your needs)
  /** @brief The underlying floating point type to be used in case of doubt (e.g. when new constants are created during manipulation) */
  typedef double              default_numeric_type;

  /** @brief Type used for any kind of IDs (variable, etc.) */
  typedef unsigned long       id_type;

  //////// utils /////
  /** @brief Simple enable-if variant that uses the SFINAE pattern */
  template <bool b, class T = void>
  struct enable_if
  {
    typedef T   type;
  };

  template <class T>
  struct enable_if<false, T> {};

  /** @brief A tag representing a Cartesian coordinate system in arbitrary dimensions
   *
   * @tparam dim     Dimension of the space
   */
  template <id_type dim>
  struct cartesian {};


  /** @brief A proxy object which allows for the use of plain default_numeric_type with template operator+, operator-, operator*, operator/ without running into problems with ambiguity. With the proxy object, one additional conversion is required, thus resolving the ambiguity.
   *
   * example:
   * operator+(expr e, numeric_t val);
   *
   * template <typename ScalarType>
   * operator+(expr e, ct_constant<value>);
   *
   * lead to ambiguity for e + 4;
   */
  template <typename NumericT>
  class numeric_type_proxy
  {
    public:
      numeric_type_proxy(NumericT value) : value_(value) {}

      NumericT get() const { return value_; }
    private:
      NumericT value_;
  };

  //
  /////// compile time expression types ///////
  //

  /** @brief Compiletime representation of an expression consisting of a binary operator and two operands
   *
   * @tparam LHS    Left hand side operand
   * @tparam OP     The binary operator tag
   * @tparam RHS    Right hand side operand
   */
  template <typename LHS,
            typename OP,
            typename RHS>
  class ct_binary_expr;

  /** @brief Compiletime representation of an expression consisting of a unary operator (e.g. modulus, x^5) and an operand
   *
   * @tparam LHS   The expression the unary operator is acting on
   * @tparam OP    The unary operator
   */
  template <typename LHS,
            typename OP>
  class ct_unary_expr;

  /** @brief Representation of a compiletime constant. Note that C++ does not allow floating point types as template arguments, thus a restriction to integer arithmetic is required
   *
   * @tparam value_    The (signed) integer value to be represented
   */
  template <long value_>
  class ct_constant;

  /** @brief Compiletime representation of a variable in the mathematical sense. If evaluated using a vector, returns the 'id'-th entry of the vector.
   *
   * @tparam id     The index in the vector passed for evaluation of the expression.
   */
  template <id_type id = 0>
  struct ct_variable;

  /** @brief Compiletime representation of a generic placeholder for functions. Cannot be evaluated and is typically substituted by functions at some later stage.
   *
   * @tparam TAG    A generic identifier tag for the function symbol.
   */
  template <typename Tag>
  class ct_function_symbol;

  /** @brief Compiletime representation of an equation.
   *
   *  @tparam LHS   Left hand side of the equation
   *  @tparam RHS   Right hand side of the equation
   */
  template <typename LHS, typename RHS>
  class ct_equation;



  /** @brief Compiletime respresentation of a vector access index.
   *
   * @tparam i     The index to be represented at compile time
   */
  template <long i>
  struct ct_index {};

  /** @brief Compiletime representation of a vector with one entry
   *
   * @tparam T0    The entry of the vector
   */
  template <typename T0>
  class ct_vector_1;

  /** @brief Compiletime representation of a vector with two entries
   *
   * @tparam T0    The entry of the vector
   * @tparam T1    Second entry of the vector
   */
  template <typename T0, typename T1>
  class ct_vector_2;

  /** @brief Compiletime representation of a vector with three entries
   *
   * @tparam T0    First entry of the vector
   * @tparam T1    Second entry of the vector
   * @tparam T2    Third entry of the vector
   */
  template <typename T0, typename T1, typename T2>
  class ct_vector_3;

  //
  /////// runtime expression types ///////
  //

  /** @brief The abstract expression interface, from which all runtime expression types inherit */
  template <typename NumericT>
  class rt_expression_interface;

  /** @brief Convenience type definition for the default expression interface */
  typedef rt_expression_interface<default_numeric_type>     default_interface_type;

  /** @brief Runtime representation of a variable in the mathematical sense. Represents the 'i'-th coordinate in the Euclidian space, where 'i'
   *
   * @tparam InterfaceType    The interface to inherit from. Usually rt_expression_interface, but extensions are possible.
   */
  template <typename InterfaceType = default_interface_type>
  class rt_variable;
  typedef rt_variable<>                variable;

  /** @brief Runtime representation of a constant value expression
   *
   * @tparam InterfaceType    The interface to inherit from. Usually rt_expression_interface, but extensions are possible.
   */
  template <typename ScalarType = default_numeric_type, typename InterfaceType = default_interface_type>
  class rt_constant;
  typedef rt_constant<>                constant;

  /** @brief Runtime representation of an expression consisting of a binary operator and two operands. Usually not instantiated by the library user.
   *
   * @tparam InterfaceType    The interface to inherit from. Usually rt_expression_interface, but extensions are possible.
   */
  template <typename InterfaceType = default_interface_type>
  class rt_binary_expr;
  typedef rt_binary_expr<>             binary_expr;

  /** @brief Runtime representation of an expression consisting of a unary operator and one operand. Usually not instantiated by the library user.
   *
   * @tparam InterfaceType    The interface to inherit from. Usually rt_expression_interface, but extensions are possible.
   */
  template <typename InterfaceType = default_interface_type>
  class rt_unary_expr;
  typedef rt_unary_expr<>              unary_expr;

  /** @brief Generic runtime expression class. Can hold any other runtime expression type and simplifies the handling of ViennaMath expressions for the library user.
   *
   * Can be seen as a smart-pointer for objects which inherit from InterfaceType.
   *
   * @tparam InterfaceType    The interface to inherit from. Usually rt_expression_interface, but extensions are possible.
   */
  template <typename InterfaceType = default_interface_type>
  class rt_expr;
  typedef rt_expr<>                    expr;

  /** @brief
   *
   * @tparam InterfaceType    The interface to inherit from. Usually rt_expression_interface, but extensions are possible.
   */
  template <typename InterfaceType = default_interface_type>
  class rt_vector_expr;
  typedef rt_vector_expr<>             vector_expr;

  /** @brief Runtime representation of a generic placeholder for functions. Cannot be evaluated and is typically substituted by functions at some later stage.
   *
   * @tparam InterfaceType    The interface to inherit from. Usually rt_expression_interface, but extensions are possible.
   */
  template <typename InterfaceType = default_interface_type>
  class rt_function_symbol;
  typedef rt_function_symbol<>         function_symbol;

  /** @brief Runtime representation of an equation.
   *
   * @tparam InterfaceType    The interface to inherit from. Usually rt_expression_interface, but extensions are possible.
   */
  template <typename InterfaceType = default_interface_type>
  class rt_equation;
  typedef rt_equation<>                equation;

  /** @brief Runtime representation of an interval [a,b].
   *
   * @tparam InterfaceType    The interface to inherit from. Usually rt_expression_interface, but extensions are possible.
   */
  template <typename InterfaceType = default_interface_type>
  class rt_interval;
  typedef rt_interval<>                interval;

  /** @brief Runtime representation of a symbolic interval. Similar to function symbols, symbolic intervals are later on replaced by rt_interval objects.
   *
   * @tparam InterfaceType    The interface to inherit from. Usually rt_expression_interface, but extensions are possible.
   */
  template <typename InterfaceType = default_interface_type>
  class rt_symbolic_interval;
  typedef rt_symbolic_interval<>       symbolic_interval;


  /** @brief The common base class for expression manipulators (top to bottom).
   *
   * @tparam InterfaceType    The interface to inherit from. Usually rt_expression_interface, but extensions are possible.
   */
  template <typename InterfaceType = default_interface_type>   //Note: No convenience typedef needed here
  class rt_manipulation_interface;

  /** @brief A wrapper class for convenient handling of expression manipulators.
   *
   * @tparam InterfaceType    The interface to inherit from. Usually rt_expression_interface, but extensions are possible.
   */
  template <typename InterfaceType = default_interface_type>   //Note: No convenience typedef needed here
  class rt_manipulation_wrapper;


  /** @brief The common base class for traversal of an expression from bottom to top. Does not modify the expression.
   *
   * @tparam InterfaceType    The interface to inherit from. Usually rt_expression_interface, but extensions are possible.
   */
  template <typename InterfaceType = default_interface_type>   //Note: No convenience typedef needed here
  class rt_traversal_interface;

  /** @brief A wrapper class for convenient handling of expression traversal objects.
   *
   * @tparam InterfaceType    The interface to inherit from. Usually rt_expression_interface, but extensions are possible.
   */
  template <typename InterfaceType = default_interface_type>   //Note: No convenience typedef needed here
  class rt_traversal_wrapper;

  //
  /////// interface for op_tags: ///////
  //

  /** @brief Common base class for all operators (binary and unary).
   *
   * @tparam InterfaceType    The interface to inherit from. Usually rt_expression_interface, but extensions are possible.
   */
  template <typename InterfaceType = default_interface_type>
  class rt_op_interface;

  //binary operator tags:
  /** @brief Runtime representation of an operator taking two arguments.
   *
   * @tparam BinaryOperation  A tag type identifying the binary operation.
   * @tparam InterfaceType    The interface to inherit from. Usually rt_expression_interface, but extensions are possible.
   */
  template <typename BinaryOperation, typename InterfaceType = default_interface_type>
  class op_binary;

  /** @brief A compiletime tag for encoding the action of the addition operator */
  template <typename NumericT>
  struct op_plus;

  /** @brief A compiletime tag for encoding the action of the subtraction operator */
  template <typename NumericT>
  struct op_minus;

  /** @brief A compiletime tag for encoding the action of the multiplication operator */
  template <typename NumericT>
  struct op_mult;

  /** @brief A compiletime tag for encoding the action of the division operator */
  template <typename NumericT>
  struct op_div;

  /** @brief A compiletime tag for encoding the action of the exponent operator */
  template <typename NumericT>
  struct op_pow;



  //
  ////// unary operator tags:
  //

  /** @brief Runtime representation of a function taking one argument.
   *
   * @tparam UnaryOperation   A tag type identifying the unary operation.
   * @tparam InterfaceType    The interface to inherit from. Usually rt_expression_interface, but extensions are possible.
   */
  template <typename UnaryOperation, typename InterfaceType = default_interface_type>
  class op_unary;


  /** @brief A compiletime tag for encoding the action of the identity function x -> x. Can be useful when manipulating expressions. */
  template <typename NumericT>
  struct op_id;

  /** @brief A compiletime tag for encoding the action of the exponential function */
  template <typename NumericT>
  struct op_exp;

  /** @brief A compiletime tag for encoding the action of the sine function */
  template <typename NumericT>
  struct op_sin;

  /** @brief A compiletime tag for encoding the action of the cosine function */
  template <typename NumericT>
  struct op_cos;

  /** @brief A compiletime tag for encoding the action of the tangens function */
  template <typename NumericT>
  struct op_tan;

  /** @brief A compiletime tag for encoding the action of the modulus function */
  template <typename NumericT>
  struct op_fabs;

  /** @brief A compiletime tag for encoding the action of the sqrt function */
  template <typename NumericT>
  struct op_sqrt;

  /** @brief A compiletime tag for encoding the action of the natural logarithm */
  template <typename NumericT>
  struct op_log; //natural logarithm

  /** @brief A compiletime tag for encoding the action of the logarithm with base 10 */
  template <typename NumericT>
  struct op_log10;

  /** @brief A compiletime tag for encoding the partial derivative with respect to a variable */
  template <typename NumericT>
  class op_partial_deriv;

  /** @brief A compiletime tag for encoding the partial derivative with respect to a function */
  template <typename InterfaceType>
  class op_rt_integral;


  /** @brief A runtime representation of a symbolic integral. Allows to specify the integration domain later (useful in e.g. Finite Element Methods).
   *
   * @tparam InterfaceType    The interface to inherit from. Usually rt_expression_interface, but extensions are possible.
   */
  template <typename InterfaceType>
  class op_rt_symbolic_integral;



  /** @brief The namespace containing a set of metafunctions which can be used by the ViennaMath library user */
  namespace result_of
  {
    /** @brief A metafunction with value 'true' if the provided argument is a ViennaMath expression type. Otherwise, false is returned.
     *
     * Should be used in the form   result_of::is_viennamath<myType>::value  in order to check for a type being from ViennaMath.
     *
     * @tparam T    The type to be checked
     */
    template <typename T>
    struct is_viennamath
    {
      enum { value = false };
    };

    /** @brief Specialization of the metafunction for a ViennaMath compiletime constant. */
    template <long val>
    struct is_viennamath<ct_constant<val> >
    {
      enum { value = true };
    };

    /** @brief Specialization of the metafunction for a ViennaMath compiletime variable. */
    template <id_type id>
    struct is_viennamath<ct_variable<id> >
    {
      enum { value = true };
    };

    /** @brief Specialization of the metafunction for a ViennaMath compiletime function symbol. */
    template <typename TAG>
    struct is_viennamath<ct_function_symbol<TAG> >
    {
      enum { value = true };
    };

    /** @brief Specialization of the metafunction for a ViennaMath compiletime binary expression. */
    template <typename LHS, typename OP, typename RHS>
    struct is_viennamath<ct_binary_expr<LHS, OP, RHS> >
    {
      enum { value = true };
    };

    /** @brief Specialization of the metafunction for a ViennaMath compiletime unary expression. */
    template <typename LHS, typename OP>
    struct is_viennamath<ct_unary_expr<LHS, OP> >
    {
      enum { value = true };
    };



    /** @brief Specialization of the metafunction for a ViennaMath runtime constant. */
    template <typename NumericType, typename InterfaceType>
    struct is_viennamath<rt_constant<NumericType, InterfaceType> >
    {
      enum { value = true };
    };

    /** @brief Specialization of the metafunction for a ViennaMath runtime variable. */
    template <typename InterfaceType>
    struct is_viennamath<rt_variable<InterfaceType> >
    {
      enum { value = true };
    };

    /** @brief Specialization of the metafunction for a ViennaMath runtime function symbol. */
    template <typename InterfaceType>
    struct is_viennamath<rt_function_symbol<InterfaceType> >
    {
      enum { value = true };
    };

    /** @brief Specialization of the metafunction for a ViennaMath runtime unary expression. */
    template <typename InterfaceType>
    struct is_viennamath<rt_unary_expr<InterfaceType> >
    {
      enum { value = true };
    };

    /** @brief Specialization of the metafunction for a ViennaMath runtime binary expression. */
    template <typename InterfaceType>
    struct is_viennamath<rt_binary_expr<InterfaceType> >
    {
      enum { value = true };
    };

    /** @brief Specialization of the metafunction for a ViennaMath runtime expression. */
    template <typename InterfaceType>
    struct is_viennamath<rt_expr<InterfaceType> >
    {
      enum { value = true };
    };

    ///////// is_compiletime

    /** @brief A metafunction returning a value 'true' if the provided argument is a ViennaMath compiletime expression type. Otherwise, false is returned.
     *
     * Should be used in the form   result_of::is_compiletime<myType>::value  in order to check for a type being a compile time type.
     *
     * @tparam T    The type to be checked
     */
    template <typename T>
    struct is_compiletime
    {
      enum { value = false };
    };

    /** @brief Specialization of the metafunction for a ViennaMath compiletime constant. */
    template <long val>
    struct is_compiletime<ct_constant<val> >
    {
      enum { value = true };
    };

    /** @brief Specialization of the metafunction for a ViennaMath compiletime variable. */
    template <id_type id>
    struct is_compiletime<ct_variable<id> >
    {
      enum { value = true };
    };

    /** @brief Specialization of the metafunction for a ViennaMath compiletime function symbol. */
    template <typename TAG>
    struct is_compiletime<ct_function_symbol<TAG> >
    {
      enum { value = true };
    };

    /** @brief Specialization of the metafunction for a ViennaMath compiletime binary expression. */
    template <typename LHS, typename OP, typename RHS>
    struct is_compiletime<ct_binary_expr<LHS, OP, RHS> >
    {
      enum { value = true };
    };

    /** @brief Specialization of the metafunction for a ViennaMath compiletime unary expression. */
    template <typename LHS, typename OP>
    struct is_compiletime<ct_unary_expr<LHS, OP> >
    {
      enum { value = true };
    };

    ////////////// interface

    /** @brief A metafunction deducing the ViennaMath runtime expression interface (base class) from two types. The first argument has priority.
    *
    * Mostly used for binary operator overloads.
    */
    template <typename LHS, typename RHS>
    struct interface
    {
      typedef default_interface_type  type; //for compatibility with enable_if
    };

    /** @brief Specialization of the runtime expression interface deduction for a ViennaMath runtime constant. */
    template <typename T, typename U, typename RHS>
    struct interface< rt_constant<T, U>, RHS >
    {
      typedef U    type;
    };

    /** @brief Specialization of the runtime expression interface deduction for a ViennaMath runtime variable. */
    template <typename T, typename RHS>
    struct interface< rt_variable<T>, RHS >
    {
      typedef T    type;
    };

    /** @brief Specialization of the runtime expression interface deduction for a ViennaMath runtime variable. */
    template <typename T>
    struct interface< default_numeric_type, rt_variable<T> >
    {
      typedef T    type;
    };

    /** @brief Specialization of the runtime expression interface deduction for a ViennaMath runtime binary expression. */
    template <typename T, typename RHS>
    struct interface< rt_binary_expr<T>, RHS >
    {
      typedef T    type;
    };

    /** @brief Specialization of the runtime expression interface deduction for a ViennaMath runtime binary expression. */
    template <typename T>
    struct interface< default_numeric_type, rt_binary_expr<T> >
    {
      typedef T    type;
    };

    /** @brief Specialization of the runtime expression interface deduction for a ViennaMath runtime unary expression. */
    template <typename T, typename RHS>
    struct interface< rt_unary_expr<T>, RHS >
    {
      typedef T    type;
    };

    /** @brief Specialization of the runtime expression interface deduction for the ViennaMath runtime expression wrapper. */
    template <typename T, typename RHS>
    struct interface< rt_expr<T>, RHS >
    {
      typedef T    type;
    };

    /** @brief Specialization of the runtime expression interface deduction for the ViennaMath runtime expression wrapper. */
    template <typename T>
    struct interface< double,  rt_expr<T> >
    {
      typedef T    type;
    };

    /** @brief A metafunction returning the resulting expression type when adding objects of type LHS and objects of type RHS.
    *
    */
    template <typename LHS,
              typename RHS,
              bool lhs_is_ct = is_compiletime<LHS>::value,
              bool rhs_is_ct = is_compiletime<RHS>::value>
    struct add;

    /** @brief A metafunction returning the resulting expression type when subtracting objects of type LHS and objects of type RHS.
    *
    */
    template <typename LHS,
              typename RHS,
              bool lhs_is_ct = is_compiletime<LHS>::value,
              bool rhs_is_ct = is_compiletime<RHS>::value>
    struct subtract;

    /** @brief A metafunction returning the resulting expression type when multiplying objects of type LHS and objects of type RHS.
    *
    */
    template <typename LHS,
              typename RHS,
              bool lhs_is_ct = is_compiletime<LHS>::value,
              bool rhs_is_ct = is_compiletime<RHS>::value>
    struct mult;

    /** @brief A metafunction returning the resulting expression type when dividing objects of type LHS and objects of type RHS.
    *
    */
    template <typename LHS,
              typename RHS,
              bool lhs_is_ct = is_compiletime<LHS>::value,
              bool rhs_is_ct = is_compiletime<RHS>::value>
    struct div;


    //////// greatest common divisor:

    /** @brief Helper metafunction for computing the greatest common divisor of two numbers. */
    template <long a, long b>
    struct gcd
    {
      enum { value = gcd<b, a % b>::value };
    };

    /** @brief Specialization for the computation of the greatest common divisor of a and 0, which is a */
    template <long a>
    struct gcd <a, 0>
    {
      enum { value = a};
    };

    /** @brief Specialization forcing a compile time error, since the greatest common divisor of 0 and 0 is not defined. */
    template <>
    struct gcd <0, 0>
    {
      enum { error = 1};
    };


  } //namespace result_of



  /** @brief A traits system for expressions. For now provides a deduction of the const reference type only.*/
  template <typename T>
  struct expression_traits
  {
    typedef T   const_reference_type;
  };

  //for compile time constants, we have to copy in order to circumvent problems with temporaries
  /** @brief Specialization of the expression traits system: A compiletime constant can be copied with no extra cost, thus no need for const reference
   *
   * Note: There seems to be a minor design flaw here, because the return type does not provide what the name suggests.
   *       This will be improved in one of the future releases.
   *
   */
  template <long value>
  struct expression_traits < ct_constant<value> >
  {
     typedef ct_constant<value>    const_reference_type;
  };

  /** @brief Specialization of the expression traits system: A runtime constant does not allow for copying, otherwise one runs into problems with tempories.
   *
   * Note: There seems to be a minor design flaw here, because the return type does not provide what the name suggests.
   *       This will be improved in one of the future releases.
   *
   */
  template <typename T, typename InterfaceType>
  struct expression_traits < rt_constant<T, InterfaceType> >
  {
     typedef rt_constant<T, InterfaceType>    const_reference_type;
  };




  /** @brief Generic overload of the addition operator for ViennaMath types. Uses SFINAE to control the scope, otherwise one may run into very tricky problems with operator+ being considered outside the viennamath namespace.
  *
  */
  template <typename LHS, typename RHS>
  typename enable_if< result_of::is_viennamath<LHS>::value || result_of::is_viennamath<RHS>::value,
                      typename result_of::add<LHS, RHS>::type >::type
  operator+(LHS const & lhs, RHS const & rhs)
  {
    return result_of::add<LHS, RHS>::instance(lhs, rhs);
  }

  /** @brief Generic overload of the subtraction operator for ViennaMath types. Uses SFINAE to control the scope, otherwise one may run into very tricky problems with operator- being considered outside the viennamath namespace.
  *
  */
  template <typename LHS, typename RHS>
  typename enable_if< result_of::is_viennamath<LHS>::value || result_of::is_viennamath<RHS>::value,
                      typename result_of::subtract<LHS, RHS>::type >::type
  operator-(LHS const & lhs, RHS const & rhs)
  {
    return result_of::subtract<LHS, RHS>::instance(lhs, rhs);
  }

  /** @brief Generic overload of the multiplication operator for ViennaMath types. Uses SFINAE to control the scope, otherwise one may run into very tricky problems with operator* being considered outside the viennamath namespace.
  *
  */
  template <typename LHS, typename RHS>
  typename enable_if< result_of::is_viennamath<LHS>::value || result_of::is_viennamath<RHS>::value,
                      typename result_of::mult<LHS, RHS>::type >::type
  operator*(LHS const & lhs, RHS const & rhs)
  {
    return result_of::mult<LHS, RHS>::instance(lhs, rhs);
  }

  /** @brief Generic overload of the division operator for ViennaMath types. Uses SFINAE to control the scope, otherwise one may run into very tricky problems with operator/ being considered outside the viennamath namespace.
  *
  */
  template <typename LHS, typename RHS>
  typename enable_if< result_of::is_viennamath<LHS>::value || result_of::is_viennamath<RHS>::value,
                      typename result_of::div<LHS, RHS>::type >::type
  operator/(LHS const & lhs, RHS const & rhs)
  {
    return result_of::div<LHS, RHS>::instance(lhs, rhs);
  }

}

#endif
