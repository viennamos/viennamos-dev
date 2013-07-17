/* =============================================================================
   Copyright (c) 2010, 2011, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
                ViennaUtils - The Vienna Utilities Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at               
               Rene Heinzl                        

   license:    see file LICENSE in the base directory
============================================================================= */


#ifndef VIENNAUTILS_TAGOF_HPP
#define VIENNAUTILS_TAGOF_HPP

#include <boost/array.hpp>
#include "viennagrid/point.hpp"

namespace viennautils {

namespace traits {

namespace tags {

struct none       ;   
struct boostarray ;
struct vgridpoint ;

} // end namespace tags
   
template <typename T, typename Active=void>   
struct tag_of;

template <typename T, typename Active>
struct tag_of 
{
   typedef viennautils::traits::tags::none type;
};

template <typename T, std::size_t N>
struct tag_of < boost::array<T,N> >
{
   typedef viennautils::traits::tags::boostarray  type;
};   

template <typename T, typename CooSys>
struct tag_of < viennagrid::point_t<T,CooSys> >
{
   typedef viennautils::traits::tags::vgridpoint  type;
};   

   
} // end namespace traits
   
} // end namespace viennautils

#endif   
