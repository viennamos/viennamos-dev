/* =============================================================================
   Copyright (c) 2010, 2011, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
                ViennaUtils - The Vienna Utilities Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */

#ifndef VIENNAUTILS_GEOMETRY_BARYCENTER_HPP
#define VIENNAUTILS_GEOMETRY_BARYCENTER_HPP

// *** system includes
#include <vector>

// *** boost includes
#include "boost/array.hpp"

namespace viennautils {

namespace detail{

// the following barycenter specializations are only for
// two dimensional topological entities, like triangles.
// triangles can be embedded in two or three dimensional geometry spaces 
// (ofc also higher, but those cases are of no importance to us right now ..)
//
template<int DimG>
struct barycenter_topodim_2 {};
 
template<>
struct barycenter_topodim_2 <2>
{
   template<typename T, std::size_t D>
   inline static boost::array<T,D> 
   eval(boost::array<T,D> const& p1, boost::array<T,D> const& p2, boost::array<T,D> const& p3)
   {
      boost::array<T,D> temp = {{(p1[0] + p2[0] + p3[0])/3.,
                                 (p1[1] + p2[1] + p3[1])/3.}};      
      return temp;
   }

   template<typename T>
   inline static std::vector<T>
   eval(std::vector<T> const& p1, std::vector<T> const& p2, std::vector<T> const& p3)
   {
      std::vector<T> bc(p1.size());
      bc[0] = (p1[0] + p2[0] + p3[0])/3.;
      bc[1] = (p1[1] + p2[1] + p3[1])/3.;
      return bc;   
   }

   template<typename VectorT>
   inline static VectorT
   eval(VectorT const& p1, VectorT const& p2, VectorT const& p3)
   {
      return VectorT ((p1[0] + p2[0] + p3[0])/3.,
                      (p1[1] + p2[1] + p3[1])/3.);      
   }
};

template<>
struct barycenter_topodim_2 <3>
{
   template<typename T, std::size_t D>
   inline static boost::array<T,D> 
   eval(boost::array<T,D> const& p1, boost::array<T,D> const& p2, boost::array<T,D> const& p3)
   {
      boost::array<T,D> temp = {{(p1[0] + p2[0] + p3[0])/3.,
                                 (p1[1] + p2[1] + p3[1])/3.,
                                 (p1[2] + p2[2] + p3[2])/3.}};      
      return temp;
   }

   template<typename T>
   inline static std::vector<T>
   eval(std::vector<T> const& p1, std::vector<T> const& p2, std::vector<T> const& p3)
   {
      std::vector<T> bc(p1.size());
      bc[0] = (p1[0] + p2[0] + p3[0])/3.;
      bc[1] = (p1[1] + p2[1] + p3[1])/3.;
      bc[2] = (p1[2] + p2[2] + p3[2])/3.;      
      return bc;   
   }

   template<typename VectorT>
   inline static VectorT
   eval(VectorT const& p1, VectorT const& p2, VectorT const& p3)
   {
      return VectorT ((p1[0] + p2[0] + p3[0])/3.,
                      (p1[1] + p2[1] + p3[1])/3., 
                      (p1[2] + p2[2] + p3[2])/3.);      
   }
};

} // end namespace detail

// -----------------------------------------------------------------------------
//
// ## barycenter for triangles in 2/3 D

// generic runtime capable barycenter implementation
// concept requ: size() member and [] access
template<typename VectorT>
VectorT barycenter(VectorT const& p1, VectorT const& p2, VectorT const& p3)
{
   if(p1.size() == 2)
   {
      return viennautils::detail::barycenter_topodim_2<2>::eval(p1,p2,p3);
   }
   else
   {
      return viennautils::detail::barycenter_topodim_2<3>::eval(p1,p2,p3);
   }
}

template<typename T, std::size_t D>
inline boost::array<T,D> 
barycenter(boost::array<T,D> const& p1, boost::array<T,D> const& p2, boost::array<T,D> const& p3)
{
   // in case that 3 points are provided, the describing triangle 
   // can be embedded in a two or a three dimensional geometry space.
   // hence the dispatch based on the geometry dimension
   return viennautils::detail::barycenter_topodim_2<D>::eval(p1, p2, p3);
}

// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
//
// ## barycenter for tetrahedrons 
//
template<typename VectorT>
inline VectorT 
barycenter(VectorT const& p1, VectorT const& p2, VectorT const& p3, VectorT const& p4)
{
   return VectorT ((p1[0] + p2[0] + p3[0] + p4[0])/4.,
                   (p1[1] + p2[1] + p3[1] + p4[1])/4.,
                   (p1[2] + p2[2] + p3[2] + p4[2])/4.);
}

template<typename T>
inline std::vector<T> 
barycenter(std::vector<T> const& p1, std::vector<T> const& p2, std::vector<T> const& p3, std::vector<T> const& p4)
{
   std::vector<T> bc(p1.size());
   
   bc[0] = (p1[0] + p2[0] + p3[0] + p4[0])/4.;
   bc[1] = (p1[1] + p2[1] + p3[1] + p4[1])/4.;
   bc[2] = (p1[2] + p2[2] + p3[2] + p4[2])/4.;
   
   return bc;
}


template<typename T, std::size_t D>
inline boost::array<T,D> 
barycenter(boost::array<T,D> const& p1, boost::array<T,D> const& p2, boost::array<T,D> const& p3, boost::array<T,D> const& p4)
{
   // when 4 points are provided, a tetrahedron is described
   // which can only be embedded in a three dimensional geometry space.
   // hence, no need for a dispatch based on the geom dimension
   boost::array<T,D> temp = {{(p1[0] + p2[0] + p3[0] + p4[0])/4.,
                              (p1[1] + p2[1] + p3[1] + p4[1])/4.,
                              (p1[2] + p2[2] + p3[2] + p4[2])/4.}};
   return temp;
}

// -----------------------------------------------------------------------------

} // end namespace viennautils

#endif




