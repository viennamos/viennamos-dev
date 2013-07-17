/* =============================================================================
   Copyright (c) 2010, 2011, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
                ViennaUtils - The Vienna Utilities Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at
               Philipp Schwaha                    schwaha@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */


#ifndef VIENNAUTILS_DUMPTYPE_HPP
#define VIENNAUTILS_DUMPTYPE_HPP

// system includes
#include <iostream>
#include <ostream>

// clang include guard
#ifndef __clang__
#include "cxxabi.h"
#endif

// boost includes
#include <boost/lexical_cast.hpp>

namespace viennautils 
{
   //! function transforms type into a string
   template<typename T>
   std::string
   type_to_string(std::ostream& stream = std::cout) 
   {
      int          stat = 0;
      
   #ifndef __clang__
      std::size_t  len = 0;
      char *demangled = abi::__cxa_demangle(typeid(T).name(), NULL, &len, &stat);
   #else
      const char *demangled = "currently not available with this compiler";
   #endif
            
      std::string type_string("");
               
      if(stat ==  0) // all went fine ..
      { 
         type_string = boost::lexical_cast< std::string >( demangled );
      #ifndef __clang__
         if(demangled)                   
            free(demangled);           
      #endif
         return type_string; 
      }
      if(stat == -1) 
         std::cout << "viennautils::type_to_string: memory allocation failure!" << std::endl;
      
      if(stat == -2)
         std::cout << "viennautils::type_to_string: the requested name is not valid under the C++ ABI mangling rules!" << std::endl;
      
      if(stat == -3)
         std::cout << "viennautils::type_to_string: one of the arguments is invalid!" << std::endl;
      return type_string;
   }

   //! dumptype function to be used with a provided type
   template<typename T>
   void dumptype(std::ostream& stream = std::cout) 
   {
      stream << "\033[1;32m" << viennautils::type_to_string<T>() << "\033[0m"  << std::endl;
   }  

   //! dumptype specialization derives type from ref object
   template<typename T>
   void dumptype(T & t)  
   {
      viennautils::dumptype<T>();
   }
   
   //! dumptype specialization derives type from const ref object
   template<typename T>
   void dumptype(T const& t)  
   {
      viennautils::dumptype<T>();
   }  


} // namespace viennautils
#endif 
