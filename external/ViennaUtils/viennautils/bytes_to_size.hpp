/* =============================================================================
   Copyright (c) 2012, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
                ViennaUtils - The Vienna Utilities Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */

#ifndef VIENNAUTILS_BYTES2SIZE_HPP
#define VIENNAUTILS_BYTES2SIZE_HPP

#include "boost/lexical_cast.hpp"

namespace viennautils {

std::string bytes_to_size(std::size_t bytes)
{  
  std::size_t kilobyte = 1024;
  std::size_t megabyte = kilobyte * 1024;
  std::size_t gigabyte = megabyte * 1024;
  std::size_t terabyte = gigabyte * 1024;
 
  if ((bytes >= 0) && (bytes < kilobyte)) {
    return boost::lexical_cast<std::string>(bytes) + " B";

  } 
  else if ((bytes >= kilobyte) && (bytes < megabyte)) {
    return boost::lexical_cast<std::string>( bytes / kilobyte ) + " KB";

  } 
  else if ((bytes >= megabyte) && (bytes < gigabyte)) {
    return boost::lexical_cast<std::string>( bytes / megabyte ) + " MB";

  } 
  else if ((bytes >= gigabyte) && (bytes < terabyte)) {
    return boost::lexical_cast<std::string>( bytes / gigabyte) + " GB";

  } 
  else if (bytes >= terabyte) {
    return boost::lexical_cast<std::string>( bytes / terabyte) + " TB";
  } else {
    return boost::lexical_cast<std::string>( bytes ) + " B";
  }
}

} // viennautils

#endif
