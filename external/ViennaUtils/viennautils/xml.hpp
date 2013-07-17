/* =============================================================================
   Copyright (c) 2010, 2011, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
                ViennaUtils - The Vienna Utilities Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at
               Markus Bina                        bina@iue.tuwien.ac.at
               Karl Rupp                          rupp@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */


#ifndef VIENNAUTILS_XML_HPP
#define VIENNAUTILS_XML_HPP

// system includes
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>

// includes in external/
#include "boost/shared_ptr.hpp"
#include "boost/algorithm/string/trim.hpp"
//#include "pugixml/src/pugixml.hpp"
#include "../external/pugixml/pugixml.hpp"

// local includes
#include "xml/pugi.hpp"
#include "xml/tags.hpp"

namespace viennautils
{

//! a generic xml class 
//! pugixml is used as the default xml library
template<typename XmlLibTag = viennautils::tag::none>
struct xml 
{
   typedef viennautils::pugixml  type;
};

//! explicit specialization for PugiXML
template<>
struct xml <viennautils::tag::pugixml>
{
   typedef viennautils::pugixml  type;
};

} // end namespace viennautils

#endif

