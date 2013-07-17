#ifndef VIENNAMATERIALS_LIBRARY_HPP
#define VIENNAMATERIALS_LIBRARY_HPP


/* =============================================================================
   Copyright (c) 2013, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */



#include "forwards.h"
#include "exceptions.hpp"
#include "check.hpp"

namespace vmat { 


// base class - is being specialized in the respective kernel implementations
//
template<typename Tag>
struct Library 
{
};


} //namespace vmat  

#endif


