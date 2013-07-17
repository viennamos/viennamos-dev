#ifndef VIENNAMATH_COMPILETIME_VARIABLE_HPP
#define VIENNAMATH_COMPILETIME_VARIABLE_HPP

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

#include <assert.h>

/** @file   viennamath/compiletime/ct_variable.hpp
    @brief  Implementation of a compiletime variable (in the mathematical sense).
*/

namespace viennamath
{
  /********** evaluation at ViennaMath vector types ***************/
  /** @brief Helper metafunction for returning the 'id'-th entry of a compiletime vector */
  template <typename vmath_vector, long id>
  struct type_by_index {};

  /** @brief Accesses the first entry in a compiletime vector */
  template <typename vmath_vector>
  struct type_by_index <vmath_vector, 0>
  {
    typedef typename vmath_vector::type_0   type;
  };

  /** @brief Accesses the second entry in a compiletime vector */
  template <typename vmath_vector>
  struct type_by_index <vmath_vector, 1>
  {
    typedef typename vmath_vector::type_1   type;
  };

  /** @brief Accesses the third entry in a compiletime vector */
  template <typename vmath_vector>
  struct type_by_index <vmath_vector, 2>
  {
    typedef typename vmath_vector::type_2   type;
  };

  /********** evaluation at STL vector types ***************/
  /** @brief Helper struct for accessing the i-th element of a vector at runtime */
  template <typename VectorType,
            long id>
  struct variable_traits
  {
    typedef default_numeric_type          return_type;

    //per default, access id-th element;
    static return_type get(VectorType const & v)
    {
      return v[id];
    }
  };


  //vector_1:
  /** @brief Helper struct for accessing the i-th element of a vector at runtime. Specialization for a ViennaMath compiletime vector with one entry. */
  template <typename T0,
            long id>
  struct variable_traits <ct_vector_1<T0>,
                          id>
  {
    typedef typename type_by_index<ct_vector_1<T0>, id>::type   return_type;

    //per default, access id-th element;
    static return_type get(ct_vector_1<T0> const & v)
    {
      return v[ct_index<id>()];
    }
  };

  //vector_2:
  /** @brief Helper struct for accessing the i-th element of a vector at runtime. Specialization for a ViennaMath compiletime vector with two entries. */
  template <typename T0, typename T1,
            long id>
  struct variable_traits <ct_vector_2<T0, T1>,
                          id>
  {
    typedef typename type_by_index<ct_vector_2<T0, T1>, id>::type   return_type;

    //per default, access id-th element;
    static return_type get(ct_vector_2<T0, T1> const & v)
    {
      return v[ct_index<id>()];
    }
  };

  //vector_3:
  /** @brief Helper struct for accessing the i-th element of a vector at runtime. Specialization for a ViennaMath compiletime vector with three entries. */
  template <typename T0, typename T1, typename T2,
            long id>
  struct variable_traits <ct_vector_3<T0, T1, T2>,
                          id>
  {
    typedef typename type_by_index<ct_vector_3<T0, T1, T2>, id>::type   return_type;

    //per default, access id-th element;
    static return_type get(ct_vector_3<T0, T1, T2> const & v)
    {
      return v[ct_index<id>()];
    }
  };




  /************* evaluation of a compile time constant *****************/
  /** @brief Helper struct for accessing the 0-th element of a constant at runtime. */
  template <long value_>
  struct variable_traits <ct_constant<value_>,
                          0>
  {
    typedef viennamath::ct_constant<value_>   return_type;

    //per default, access id-th element;
    static return_type get(viennamath::ct_constant<value_> const & v)
    {
      return v;
    }
  };

  //guard: something like (x*y - z)(constant<long, ct_constant<4> >()) is not allowed
  /** @brief Compiletime guard for index out of bounds */
  template <long value_, long id>
  struct variable_traits <ct_constant<value_>,
                          id>
  {
    //try to provide a good compiler error message:
    typedef typename ct_constant<value_>::ERROR_PROVIDED_NOT_ENOUGH_ARGUMENTS_TO_EXPRESSION   error_type;

    //providing a non-vector type is bogus -> force linker error!
  };

  // further specialize variable_traits::get() here as required.



  /** @brief Representation of a mathematical variable. If the supplied argument is some vector type, a traits system accesses the id-th component
   *
   * @tparam id                the component of the vector for which 'variable' is evaluated
   */
  template <id_type id /* see forwards.h for default argument */>
  struct ct_variable
  {
    typedef default_numeric_type   numeric_type;

    explicit ct_variable() {};

    std::ostream & operator<<(std::ostream & stream) const
    {
      stream << "variable<" << id << ">";
      return stream;
    }

    /////////////////   Basic evaluation: //////////////////////////////////
    //template <typename NumericT>
    //NumericT operator()(NumericT value) const { return value; }

    template <typename ScalarType>
    rt_constant<ScalarType> operator()(rt_constant<ScalarType> const & other) const
    {
      if (id > 0)
        throw variable_index_out_of_bounds_exception(id, 0);
      return rt_constant<ScalarType>(static_cast<ScalarType>(other));
    }

    template <long value>
    long operator()(ct_constant<value> const & other) const
    {
      if (id > 0)
        throw variable_index_out_of_bounds_exception(id, 0);
      return value;
    }

    //Vector argument (can be of type std::vector)
    template <typename VectorType>
    typename variable_traits<VectorType, id>::return_type
    //numeric_type
    operator()(VectorType const & v) const
    {
      if(id >= v.size())
        throw variable_index_out_of_bounds_exception(id, v.size());
      return variable_traits<VectorType, id>::get(v);
    }

  }; //variable


  /** @brief Convenience overload for printing a variable to an output stream. */
  template <id_type id>
  std::ostream& operator<<(std::ostream & stream, ct_variable<id> const & /*u*/)
  {
    stream << "ct_variable<" << id << ">";
    return stream;
  }


}

#endif
