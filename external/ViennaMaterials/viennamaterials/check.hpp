#ifndef VIENNAMATERIALS_CHECK_HPP
#define VIENNAMATERIALS_CHECK_HPP

/* =============================================================================
   Copyright (c) 2013, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */

#ifdef HAVE_LIBXML2

extern "C" {
#include "libxml/xmlreader.h"
}

// System includes
//
#include <iostream>
#include <fstream>
#include <cstring>

// ViennaMaterials includes
//
#include "viennamaterials/exceptions.hpp"

namespace vmat
{

bool check(std::string const& xml_filename)//, std::string const& dtd_filename)
{
  // First, load the input xml file (the input material database)
  //
  xmlDocPtr doc = xmlReadFile(xml_filename.c_str(), NULL, 0);
  if (doc == NULL) {
    throw vmat::XMLLoadError();
    return false;
  }    
  
  // Second, setup a DTD string
  //
  const char* dtd_string = "\
<!ELEMENT materials (material*)>\n\
<!ELEMENT material (id,name,category+,parameters*)>\n\
<!ELEMENT parameters (parameter*)>\n\
<!ELEMENT parameter (name,value,unit,note?)>\n\
<!ELEMENT id (#PCDATA)>\n\
<!ELEMENT name (#PCDATA)>\n\
<!ELEMENT category (#PCDATA)>\n\
<!ELEMENT value (#PCDATA)>\n\
<!ELEMENT unit (#PCDATA)>\n\
<!ELEMENT note (#PCDATA)>\n";

  // Third, read the DTD string 
  //
  xmlParserInputBufferPtr buf = xmlParserInputBufferCreateStatic (dtd_string, strlen(dtd_string),XML_CHAR_ENCODING_ASCII);
  xmlDtdPtr dtd = xmlIOParseDTD (NULL, buf, XML_CHAR_ENCODING_ASCII); 

  if(dtd == NULL) { 
    throw vmat::DTDLoadError();
    return false;
  }
  
  // Finally, check the xml document against the dtd 
  //
  xmlValidCtxt* ctx = xmlNewValidCtxt();
  
  if(xmlValidateDtd(ctx, doc, dtd) == 0) {
    throw vmat::DTDError();
    return false;
  }
  else
  {
    xmlFreeValidCtxt(ctx);  
    xmlFreeDtd(dtd);
    xmlFreeDoc(doc);
    return true;
  }
}

} // end namespace vmat

#endif
#endif
