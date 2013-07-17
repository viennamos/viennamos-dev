/* =============================================================================
   Copyright (c) 2010, 2011, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
                ViennaUtils - The Vienna Utilities Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at
               Markus Bina                        bina@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */

#ifndef VIENNAUTILS_CONFIG_HPP
#define VIENNAUTILS_CONFIG_HPP

// local includes
#include "xml.hpp"

namespace viennautils
{

//! a generic configuration class 
//! the default xml implementation is used as a default
template<typename Tag = viennautils::tag::none>
struct config 
{
   typedef viennautils::xml<>::type   type;
};

//! explicit specialization to use xml
//! the default xml implementation is used
template<>
struct config <viennautils::tag::xml>
{
   typedef viennautils::xml<>::type   type;
};


} // end namespace viennautils

#endif




