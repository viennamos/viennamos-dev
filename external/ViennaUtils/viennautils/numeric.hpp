/* =======================================================================
   Copyright (c) 2010, Institute for Microelectronics, TU Vienna.
   http://www.iue.tuwien.ac.at
                             -----------------
                ViennaUtils - The Vienna Utilities Library
                             -----------------
   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    MIT (X11), see file LICENSE in the ViennaGrid base directory
======================================================================= */

#ifndef VIENNAUTILS_NUMERIC_HPP
#define VIENNAUTILS_NUMERIC_HPP

#include <limits>
#include <cmath>

namespace viennautils {

template<typename T>
inline bool equals(T const& a, T const& b)
{
   return !(std::abs(a-b) > std::numeric_limits<T>::epsilon());
}



}

#endif
