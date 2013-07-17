/* =============================================================================
   Copyright (c) 2010, 2011, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
                ViennaUtils - The Vienna Utilities Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */


#ifndef VIENNAUTILS_FILE_HPP
#define VIENNAUTILS_FILE_HPP

#include <fstream>

namespace viennautils {

inline bool file_exists(std::string const& filename)
{
   std::ifstream ifile(filename.c_str());
   return ifile;
}

inline std::string file_extension(std::string const& filename)
{
   return filename.substr(filename.rfind(".")+1, filename.size());
}

} // end namespace viennautils

#endif
