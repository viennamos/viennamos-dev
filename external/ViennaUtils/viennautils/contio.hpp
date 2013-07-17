/* =============================================================================
   Copyright (c) 2010, 2011, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
                ViennaUtils - The Vienna Utilities Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */


#ifndef VIENNAUTILS_CONTIO_HPP
#define VIENNAUTILS_CONTIO_HPP

#include <iostream>
#include <vector>
#include <map>

#include "boost/array.hpp"

namespace viennautils {

struct print
{
   template<typename T1, std::size_t Size>
   void operator()(boost::array<T1, Size> const& vec, std::ostream& ostr = std::cout)
   {
      for(typename boost::array<T1, Size>::const_iterator iter = vec.begin();
          iter != vec.end(); iter++)
      {
         ostr << *iter << "  ";
      }
      std::cout << std::endl;
   }
   
   template<typename T1, std::size_t Size>
   void operator()(boost::array<T1, Size> & vec, std::ostream& ostr = std::cout)
   {
      for(typename boost::array<T1, Size>::iterator iter = vec.begin();
          iter != vec.end(); iter++)
      {
         ostr << *iter << "  ";
      }
      std::cout << std::endl;
   }

   template<typename T1>
   void operator()(std::vector<T1> const& vec, std::ostream& ostr = std::cout)
   {
      for(typename std::vector<T1>::const_iterator iter = vec.begin();
          iter != vec.end(); iter++)
      {
         ostr << *iter << "  ";
      }
      std::cout << std::endl;
   }
   
   template<typename T1>
   void operator()(std::vector<T1> & vec, std::ostream& ostr = std::cout)
   {
      for(typename std::vector<T1>::iterator iter = vec.begin();
          iter != vec.end(); iter++)
      {
         ostr << *iter << "  ";
      }
      std::cout << std::endl;
   }   
   
   template<typename T0, typename T1>
   void operator()(std::map<T0, T1> const& vec, std::ostream& ostr = std::cout)
   {
      for(typename std::map<T0, T1>::const_iterator iter = vec.begin();
          iter != vec.end(); iter++)
      {
         ostr << iter->first << " - " << iter->second << std::endl;
      }
   }
   
   template<typename T0, typename T1>
   void operator()(std::map<T0, T1> & vec, std::ostream& ostr = std::cout)
   {
      for(typename std::map<T0, T1>::iterator iter = vec.begin();
          iter != vec.end(); iter++)
      {
         ostr << iter->first << " - " << iter->second << std::endl;
      }
   }
};

} // end namespace viennautils

namespace std {

template<typename T1>
std::ostream& operator<<(std::ostream& ostr, std::vector<T1> const& vec)
{
   ostr << "size: " << vec.size() << " entries: "<< std::endl;
   viennautils::print()(vec, ostr);
   return ostr;
}

template<typename T1>
std::ostream& operator<<(std::ostream& ostr, std::vector<T1> & vec)
{
   ostr << "size: " << vec.size() << " entries: "<< std::endl;
   viennautils::print()(vec, ostr);
   return ostr;
}

template<typename T0, std::size_t S>
std::ostream& operator<<(std::ostream& ostr, boost::array<T0, S> const& vec)
{
   ostr << "size: " << vec.size() << " entries: "<< std::endl;
   viennautils::print()(vec, ostr);
   return ostr;
}

template<typename T0, std::size_t S>
std::ostream& operator<<(std::ostream& ostr, boost::array<T0, S> & vec)
{
   ostr << "size: " << vec.size() << " entries: "<< std::endl;
   viennautils::print()(vec, ostr);
   return ostr;
}

template<typename T0, typename T1>
std::ostream& operator<<(std::ostream& ostr, std::map<T0, T1> const& map)
{
   ostr << "size: " << map.size() << " entries: "<< std::endl;
   viennautils::print()(map, ostr);
   return ostr;
}

template<typename T0, typename T1>
std::ostream& operator<<(std::ostream& ostr, std::map<T0, T1> & map)
{
   ostr << "size: " << map.size() << " entries: " << std::endl;
   viennautils::print()(map, ostr);
   return ostr;
}

} // end namespace std

#endif

