#ifndef VIENNAMATH_COMPILETIME_CT_FUNCTION_SYMBOL_HPP
#define VIENNAMATH_COMPILETIME_CT_FUNCTION_SYMBOL_HPP

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
#include <vector>
#include "viennamath/forwards.h"
//#include "viennamath/expression_compile_time.hpp"
//#include "viennamath/expression_run_time.hpp"

/** @file viennamath/compiletime/ct_function_symbol.hpp
    @brief  Defines a function symbol for manipulation at compile time
*/

namespace viennamath
{

  /////// two common tags:

  /** @brief A compiletime tag for an unknown function: */
  template <id_type ct_id = 0>
  struct unknown_tag
  {
    static std::string str()
    {
      std::stringstream ss;
      ss << "unknown[" << ct_id << "]";
      return ss.str();
    }

    static id_type tag_id() { return 0; }
  };

  /** @brief A compiletime tag for a test function: */
  template <id_type ct_id = 0>
  struct test_tag
  {
    static std::string str()
    {
      std::stringstream ss;
      ss << "test[" << ct_id << "]";
      return ss.str();
    }

    static id_type tag_id() { return 1; }
  };







  /** @brief A function symbol. Can be used for unknown functions, test functions, etc. Cannot be evaluated, but substituted with an evaluable object
   *
   * @tparam Tag    A tag class typically used to distinguish between different function symbols. Tag requirements: 'static std::string str();' which returns an identification string
   */
  template <typename Tag>
  class ct_function_symbol
  {
      static std::string str()
      {
        return Tag::str();
      }
  };



  /** @brief Convenience overload for printing a compiletime function symbol to an output stream */
  template <typename Tag>
  std::ostream& operator<<(std::ostream & stream, ct_function_symbol<Tag> const & v)
  {
    stream << ct_function_symbol<Tag>::str();
    return stream;
  }



}

#endif
