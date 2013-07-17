#ifndef VIENNAMATH_RUNTIME_FUNCTOR_WRAPPER_HPP
#define VIENNAMATH_RUNTIME_FUNCTOR_WRAPPER_HPP

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

/** @file functor_wrapper.hpp
    @brief Defines a manipulator and a traversal functionality for working with symbolic expressions at runtime.
*/

namespace viennamath
{

  //
  // Modification of expressions
  //

  /** @brief The abstract runtime interface for all expression manipulators */
  template <typename InterfaceType /* see forwards.h for default argument */>
  class rt_manipulation_interface
  {
    public:
      /** @brief Returns the manipulated (runtime) expression. The caller has to ensure the proper deletion of the object the pointer is referring to. */
      virtual InterfaceType * operator()(InterfaceType const * e) const { return e->clone(); };

      /** @brief Returns true if 'e' is modified directly and not just the leaves of 'e'. Otherwise, returns 'false'. */
      virtual bool modifies(InterfaceType const * /*e*/) const { return false; }
  };

  /** @brief A wrapper using type erasure for manipulation functors acting on expressions */
  template <typename InterfaceType /* default argument in forwards.h */>
  class rt_manipulation_wrapper
  {
    public:
      template <typename T>
      rt_manipulation_wrapper(T const * t) : functor_(t) {}

      InterfaceType * operator()(InterfaceType const * e) const
      {
        return functor_.get()->operator()(e);
      }

      bool modifies(InterfaceType const * e) const { return functor_->modifies(e); }

    private:
      std::auto_ptr< const rt_manipulation_interface<InterfaceType> > functor_;
  };


  //
  // Traversal of expressions (no modification)
  //


  /** @brief The abstract runtime interface for all expression traversal routines.
   *
   * @tparam InterfaceType   The common expression interface for all runtime classes.
   */
  template <typename InterfaceType /* see forwards.h for default argument */>
  class rt_traversal_interface
  {
    public:
      /** @brief Triggers the processing of the supplied expression. */
      virtual void operator()(InterfaceType const * /*e*/) const = 0;

      /** @brief If 'true' is returned, the traversal continues the recursion. */
      virtual bool step_into(InterfaceType const * /*e*/) const { return true; }
  };

  /** @brief A wrapper using type erasure for traversal functions acting on expressions */
  template <typename InterfaceType /* default argument in forwards.h */>
  class rt_traversal_wrapper
  {
    public:
      template <typename T>
      rt_traversal_wrapper(T const * t) : functor_(t) {}

      void operator()(InterfaceType const * e) const
      {
        functor_.get()->operator()(e);
      }

      bool step_into(InterfaceType const * e) const { return functor_.get()->step_into(e); }

    private:
      std::auto_ptr< const rt_traversal_interface<InterfaceType> > functor_;
  };


  /** @brief A helper class which calls a provided functor if the supplied expression pointer can be cast to the target type.
   *
   * @tparam CastToType    The type to which a dynamic cast should be attempted.
   */
  template <typename CastToType>
  struct callback_if_castable
  {
    /** @brief Attempts to cast the expression referred to be 'e' to 'CastToType'. If successful, the provided functor is called. */
    template <typename InterfaceType, typename FunctorType>
    static bool apply(InterfaceType const * e, FunctorType const & functor)
    {
      CastToType const * ptr = dynamic_cast< CastToType const *>(e);
      if (ptr != NULL)
      {
        functor(*ptr);
        return true;
      }
      return false;
    }

  };


}

#endif
