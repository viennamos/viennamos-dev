/* =======================================================================
   Copyright (c) 2010, Institute for Microelectronics, TU Vienna.
   http://www.iue.tuwien.ac.at
                             -----------------
                ViennaUtils - The Vienna Utilities Library
                             -----------------

   authors:    Josef Weinbub                    weinbub@iue.tuwien.ac.at

   license:    MIT (X11), see file LICENSE in the ViennaGrid base directory
======================================================================= */

#ifndef VIENNAUTILS_IO_HIN
#define VIENNAUTILS_IO_HIN

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "boost/array.hpp"
#include "boost/lexical_cast.hpp"

#include "viennautils/convert.hpp"
#include "viennautils/contio.hpp"

//#define IODEBUG

namespace viennautils
{
  namespace io
  {
    struct hin_reader
    {
      static const int DIMG = 3;

      typedef double                            numeric_type;
      typedef boost::array<numeric_type, DIMG>  point_type;
      typedef std::vector<point_type>           geometry_container_type;
    
      typedef std::vector< std::size_t >        polygon_type;
      typedef std::vector< polygon_type >       polygon_container_type;
      
      typedef std::vector<polygon_container_type>   domain_type;
    
      int operator()(std::string filename)
      {
        std::ifstream reader(filename.c_str());

        if (!reader){
          std::cerr << "Cannot open file " << filename << std::endl;
          throw "Cannot open file";
          return EXIT_FAILURE;
        }
        
        try{
        
          std::string token;
            
          std::size_t vertices = 0;
        
          dim_geometry = DIMG;
        
        #ifdef IODEBUG
          std::cout << "reading geometry information .. " << std::endl;        
        #endif          
          
          while(1)
          {
            std::getline(reader, token);
            
            if(token.find("Vertices") != std::string::npos)
            {
              std::string::size_type delimiter_left = token.find("[")+1;
              std::string::size_type delimiter_right = token.find("]");              
              std::string vertices_str = token.substr(delimiter_left, delimiter_right-delimiter_left);
              vertices = viennautils::convert<std::size_t>()(vertices_str);
              break;
            }            
          }
          
        #ifdef IODEBUG
          std::cout << "vertices: " << vertices << std::endl;
        #endif

          geometry.resize(vertices);

          for(std::size_t vi = 0; vi < vertices; vi++)
          {
            point_type coords;
            for (int j=0; j < DIMG; j++)
              reader >> coords[j];
          
            geometry[vi] = coords;
         #ifdef IODEBUG
            std::cout << coords[0] << " " << coords[1] << " " << coords[2] << std::endl;
         #endif
          }
          
        #ifdef IODEBUG
          std::cout << "  finished loading point vectors .. " <<  std::endl;
        #endif                    
          
        #ifdef IODEBUG          
          std::cout << "reading polygons .. " << std::endl;
        #endif                              
          std::size_t polygon_size = 0;
          while(1)
          {
            std::getline(reader, token);

            if(token.find("Polygons") != std::string::npos)
            {
              std::string::size_type delimiter_left = token.find("[")+1;
              std::string::size_type delimiter_right = token.find("]");              
              std::string polygon_size_str = token.substr(delimiter_left, delimiter_right-delimiter_left);
              polygon_size = viennautils::convert<std::size_t>()(polygon_size_str);
              break;
            }                        
          }
        #ifdef IODEBUG
          std::cout << "polygon size: " << polygon_size << std::endl;
        #endif              
          
          for(std::size_t i = 0; i < polygon_size; i++)
          {
            std::size_t vertex_size = 0;

            while(1)
            {
              std::getline(reader, token);

              if(token.find("Polygon") != std::string::npos)
              {
                std::string::size_type delimiter_left = token.find("[")+1;
                std::string::size_type delimiter_right = token.find("]");              
                std::string vertex_size_str = token.substr(delimiter_left, delimiter_right-delimiter_left);
                vertex_size = viennautils::convert<std::size_t>()(vertex_size_str);
                break;
              }                        
            }
          #ifdef IODEBUG
            std::cout << "vertex size: " << vertex_size << std::endl;
          #endif                          
            polygon_type  polygon(vertex_size);
            for(std::size_t vi = 0; vi < vertex_size; vi++)
            {
              reader >> polygon[vi];
            }
            polygons.push_back(polygon);
          }
          
       #ifdef IODEBUG          
          std::cout << "reading polyhedra .. " << std::endl;
       #endif
          std::size_t polyhedra_size = 0;
          while(1)
          {
            std::getline(reader, token);

            if(token.find("Polyhedra") != std::string::npos)
            {
              std::string::size_type delimiter_left = token.find("[")+1;
              std::string::size_type delimiter_right = token.find("]");              
              std::string polyhedra_size_str = token.substr(delimiter_left, delimiter_right-delimiter_left);
              polyhedra_size = viennautils::convert<std::size_t>()(polyhedra_size_str);
              break;
            }                        
          }
       #ifdef IODEBUG        
          std::cout << "polyhedra size: " << polyhedra_size << std::endl;
       #endif

          // the number of polyhedras is the number of segments
          domain.resize(polyhedra_size);  

          for(std::size_t i = 0; i < polyhedra_size; i++)
          {
            std::size_t polygon_size = 0;
            while(1)
            {
              std::getline(reader, token);

              if(token.find("Polyhedron") != std::string::npos)
              {
                std::string::size_type delimiter_left = token.find("[")+1;
                std::string::size_type delimiter_right = token.find("]");              
                std::string polygon_size_str = token.substr(delimiter_left, delimiter_right-delimiter_left);
                polygon_size = viennautils::convert<std::size_t>()(polygon_size_str);
                break;
              }                        
            }
       #ifdef IODEBUG   
            std::cout << "polygon size: " << polygon_size << std::endl;
       #endif            

            long   poly_index = 0;
            long   mapped_poly_index = 0;
            // note: the polygon indices are signed integers!
            for(std::size_t pi = 0; pi < polygon_size; pi++)
            {
              reader >> poly_index;
              //std::cout << poly_index << " ";
              mapped_poly_index = map(poly_index);
              //std::cout << poly_index <<":" << mapped_poly_index << " ";
              polygon_type polygon = polygons[mapped_poly_index];
              
//              // if index is negative, reverse the orientation of the polygon, 
//              // by reversing the polygon vector
//              if(poly_index < 0)
//              {
//                std::reverse(polygon.begin(),polygon.end());
//              }
            #ifdef IODEBUG
              std::cout << polygon << std::endl;
           #endif
              domain[i].push_back(polygon);
            }
         #ifdef IODEBUG            
            std::cout << std::endl;
         #endif
          }
          
          
        } catch (...) {
          std::cerr << "Problems while reading file " << filename << std::endl;
        }
        return EXIT_SUCCESS;
      }
      
      void dump(std::ostream& ostr = std::cout)
      {
        ostr << "## GEOMETRY" << std::endl;
        for(std::size_t gi = 0; gi < geometry.size(); gi++)
        {
          ostr << "  point - id: " << gi << " - data: "; 
          viennautils::print()(geometry[gi], ostr);
        }
        
        ostr << "## TOPOLOGY" << std::endl;
        for(std::size_t si = 0; si < domain.size(); si++)
        {
          ostr << "  segment: " << si << std::endl;
          for(std::size_t fi = 0; fi < domain[si].size(); fi++)
          {
            ostr << "    face - id: " << fi << " - size: " << domain[si][fi].size() << " - eles: ";
            viennautils::print()(domain[si][fi], ostr);
          }
        }
      }
      
      point_type& point(std::size_t index)
      {
         return geometry[index];
      }
      
      std::size_t geometry_size()
      {
         return geometry.size();
      }
      
      std::size_t segment_size()
      {  
         return domain.size();
      }
      
      polygon_container_type& segment(std::size_t index)
      {
         return domain[index];
      }      
      
      std::size_t dim_geom()
      {
         return dim_geometry;
      }      
      
    private:
      long map(long index)
      {
        if(index >= 0) return index;
        else           return -(index)-1;        
      }      
      
      geometry_container_type   geometry;
      polygon_container_type    polygons;
      domain_type               domain;
      std::size_t               dim_geometry;      
      
    };
  }
}

#endif

