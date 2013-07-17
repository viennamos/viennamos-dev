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

#ifndef VIENNAUTILS_GEOMETRY_INCLUSION_HPP
#define VIENNAUTILS_GEOMETRY_INCLUSION_HPP

#include <cmath>
#include <vector>
#include <limits>

#include "viennagrid/algorithm/inner_prod.hpp"
#include "viennagrid/algorithm/cross_prod.hpp"
#include "viennagrid/algorithm/norm.hpp"

namespace viennautils {




// NOTE: for now, the cells have to be of simplex topology
// TODO: extend to 'arbitrary' topology

template<int D1, int D2>
struct inclusion{};


// point in/on a finite line-segment test
template<>
struct inclusion <0, 1>
{
   // pnt is the point under test
   // p1, p2 are the points of the line segment
   template<typename VectorT>
   bool operator()(VectorT& pnt, std::vector<VectorT>& pvec, double const& epsilon = 1.0E-15)
   {
      typedef typename VectorT::value_type numeric_type;
   
      VectorT p1 = pvec[0];
      VectorT p2 = pvec[1];      
   
      VectorT v = p2-p1;

      numeric_type t = viennagrid::inner_prod(v, v);
      numeric_type u = ((pnt[0]-p1[0])*v[0]+(pnt[1]-p1[1])*v[1])/t;
      
      if( u > 1.0 ) u = 1.0;
      else if(u < 0.0) u = 0.0;
      
      numeric_type x = p1[0] + u*v[0];
      numeric_type y = p1[1] + u*v[1];
      
      numeric_type dx = x - pnt[0];
      numeric_type dy = y - pnt[1];
      
      if( ((dx*dx)+(dy*dy)) < epsilon ) return true;
      else return false;
   }
};

// point in triangle test
template<>
struct inclusion <0, 2>
{
   template<typename VectorT>
   bool operator()(VectorT& pnt, std::vector<VectorT>& pvec, double const& epsilon = 1.0E-15)
   {

      typedef typename VectorT::value_type numeric_type;


////      // test if the reference point is one of the points under test
          // note: this should be not nec ...
////      //
////      std::size_t dimg = pnt.size();
////      std::size_t equal_a, equal_b, equal_c = 0;
////      for(std::size_t dim = 0; dim < dimg; dim++)
////      {
////         if(std::fabs(pnt[dim] - pvec[0][dim]) < std::numeric_limits<numeric_type>::epsilon()) equal_a++;
////         if(std::fabs(pnt[dim] - pvec[1][dim]) < std::numeric_limits<numeric_type>::epsilon()) equal_b++;
////         if(std::fabs(pnt[dim] - pvec[2][dim]) < std::numeric_limits<numeric_type>::epsilon()) equal_c++;
////      }
////      if( (equal_a == dimg) || (equal_b == dimg) || (equal_c == dimg) ) return true;



// ####################################################
      // this implementation has very nice performance, but 
      // only works if the point and the triangle are coplanar.
      
//      typedef double numeric_type;
//      
//      // compute vectors
//      VectorT v0 = p3 - p1;
//      VectorT v1 = p2 - p1;
//      VectorT v2 = pnt - p1;
//      
//      // compute dot products
//      numeric_type dot00 = viennagrid::inner_prod(v0, v0);
//      numeric_type dot01 = viennagrid::inner_prod(v0, v1);      
//      numeric_type dot02 = viennagrid::inner_prod(v0, v2);
//      numeric_type dot11 = viennagrid::inner_prod(v1, v1);            
//      numeric_type dot12 = viennagrid::inner_prod(v1, v2);            
//      
//      // compute barycentric coordinates
//      numeric_type inv_denom = 1. / (dot00 * dot11 - dot01 * dot01);
//      numeric_type u = (dot11 * dot02 - dot01 * dot12) * inv_denom;
//      numeric_type v = (dot00 * dot12 - dot01 * dot02) * inv_denom;

//      // Check if point is in triangle on the edges or corner of the triangle
//      return ((u >= 0) && (v >= 0) && (u + v <= 1));

// ####################################################

      // NOTE: this algorithm can be extended to work for an arbitrary polygon 
      // in 3d. traverse the nodes of the polygon, build the vectors pa, pb
      // based on the current node and the next node. thats it.
      
      VectorT p1 = pvec[0];
      VectorT p2 = pvec[1];
      VectorT p3 = pvec[2];                              
      
      numeric_type anglesum = 0;
      numeric_type twopi    = 6.283185307179586476925287;
      
      VectorT pa = p1-pnt;
      VectorT pb = p2-pnt;
      numeric_type m = viennagrid::norm(pa) * viennagrid::norm(pb);
      if( m <= epsilon) return true;
      else                anglesum += acos( viennagrid::inner_prod(pa,pb)/m );

      pa = p2-pnt;
      pb = p3-pnt;
      m = viennagrid::norm(pa) * viennagrid::norm(pb);
      if( (m) <= epsilon) return true;
      else                anglesum += acos( viennagrid::inner_prod(pa,pb)/m );

      pa = p3-pnt;
      pb = p1-pnt;
      m = viennagrid::norm(pa) * viennagrid::norm(pb);
      if( (m) <= epsilon) return true;
      else                anglesum += acos( viennagrid::inner_prod(pa,pb)/m );

      if( std::abs(anglesum - twopi) < 0.01 ) return true;
      else return false;
   }
};



} // end namespace viennautils

#endif




