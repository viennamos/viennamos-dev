/* =============================================================================
   Copyright (c) 2010, 2011, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
                ViennaUtils - The Vienna Utilities Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at               
               Rene Heinzl                         heinzl@iue.tuwien.a.cat

   license:    see file LICENSE in the base directory
============================================================================= */


#ifndef VIENNAUTILS_VALUETYPE_HPP
#define VIENNAUTILS_VALUETYPE_HPP

#include "viennautils/tag_of.hpp"

namespace viennautils {

namespace result_of {   

template<typename T, typename Tag>   
struct value_impl   
{
   typedef typename viennautils::traits::tags::none   type;
};

template<typename T>   
struct value_impl  <T, viennautils::traits::tags::boostarray> 
{
   typedef typename T::value_type   type;
};

template<typename T>   
struct value_impl  <T, viennautils::traits::tags::vgridpoint> 
{
   typedef typename T::numeric_type   type;
};

template<typename T>   
struct value
{
   typedef typename value_impl<T, typename viennautils::traits::tag_of<T>::type>::type type;
};
   
} // end namespace result_of
   
} // end namespace viennautils

#endif
