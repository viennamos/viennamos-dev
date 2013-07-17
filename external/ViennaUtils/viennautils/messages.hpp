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
   
#ifndef VIENNAUTILS_MESSAGES_HPP
#define VIENNAUTILS_MESSAGES_HPP
   
#include "viennautils/logger.hpp"   
   
#include <ostream>   
   
namespace viennautils {
namespace msg {

inline void error(std::string const& str, std::ostream& stream = std::cout)
{
   viennautils::logger::make_stream<viennautils::logger::red>(stream) << str << "\n";
}

inline void warning(std::string const& str, std::ostream& stream = std::cout)
{
   viennautils::logger::make_stream<viennautils::logger::yellow>(stream) << str << "\n";
}

inline void info(std::string const& str, std::ostream& stream = std::cout)
{
   viennautils::logger::make_stream<viennautils::logger::green>(stream) << str << "\n";
}

} // end namespace msg
} // end namespace viennamos

#endif 



