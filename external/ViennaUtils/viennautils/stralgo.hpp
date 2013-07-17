/* =======================================================================
   Copyright (c) 2010, Institute for Microelectronics, TU Vienna.
   http://www.iue.tuwien.ac.at
                             -----------------
                ViennaUtils - The Vienna Utilities Library
                             -----------------
   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    MIT (X11), see file LICENSE in the ViennaGrid base directory
======================================================================= */


#ifndef VIENNAUTILS_STRALGO_HPP
#define VIENNAUTILS_STRALGO_HPP

#include <vector>

#include "boost/algorithm/string/split.hpp"
#include "boost/algorithm/string/classification.hpp"

namespace viennautils {

namespace stralgo{

std::size_t size(std::string const& str, std::string const& delimiter = "\n")
{
   std::vector<std::string> result_cont;
   boost::split(result_cont, str, boost::is_any_of(delimiter));
   return result_cont.size();
}

} // end namespace stralgo
} // end namespace viennautils

#endif




