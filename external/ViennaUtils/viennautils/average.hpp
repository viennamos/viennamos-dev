/* =============================================================================
   Copyright (c) 2013, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
                ViennaUtils - The Vienna Utilities Library
                             -----------------

   license:    see file LICENSE in the base directory
============================================================================= */
   
#ifndef VIENNAUTILS_AVERAGE_HPP
#define VIENNAUTILS_AVERAGE_HPP


namespace viennautils {


/** @brief A functor which computes the arithmetic average of all entries in a container.
 *
 * The container is required to provide STL-compatible forward-iterators and a member function size() returning the number of elements.
 */
struct arithmetic_averaging
{
  template <typename ContainerType>
  double operator()(ContainerType const & cont) const
  {
    double ret = 0;
    for (typename ContainerType::const_iterator cit  = cont.begin();
                                                cit != cont.end();
                                              ++cit)
          ret += *cit / cont.size();

    return ret;
  }
};

/** @brief A functor which computes the geometric average of all entries in a container.
 *
 * The container is required to provide STL-compatible forward-iterators and a member function size() returning the number of elements.
 * Note that all entries need to be non-negative for even container sizes.
 */
struct geometric_averaging
{
  template <typename ContainerType>
  double operator()(ContainerType const & cont) const
  {
    double ret = 1.0;
    for (typename ContainerType::const_iterator cit  = cont.begin();
                                                cit != cont.end();
                                              ++cit)
    {
      ret *= std::pow(*cit, 1.0 / cont.size());
    }

    return ret;
  }
};






} // viennautils

#endif

