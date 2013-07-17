/* =============================================================================
   Copyright (c) 2010, 2011, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
                ViennaUtils - The Vienna Utilities Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at
               Markus Bina                        bina@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */


#ifndef VIENNAUTILS_XML_EXCEPTIONS_HPP
#define VIENNAUTILS_XML_EXCEPTIONS_HPP

namespace viennautils
{

//! xml query exception
struct XmlQueryException : public std::runtime_error 
{
   XmlQueryException(std::string const & str) : std::runtime_error(str) {}
};

//! xml file exception
struct InvalidXmlFileException : public std::runtime_error 
{
   InvalidXmlFileException(std::string const & str) : std::runtime_error(str) {}
};

} // end namespace viennautils

#endif
