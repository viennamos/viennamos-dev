#ifndef VIENNAMATERIALS_EXCEPTIONS_HPP
#define VIENNAMATERIALS_EXCEPTIONS_HPP

/* =============================================================================
   Copyright (c) 2013, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */

#include <iostream>
#include <exception>

namespace vmat
{
struct NonUniqueMaterialException: public std::exception
{
  NonUniqueMaterialException(std::string const& info) : info(info) {}
  ~NonUniqueMaterialException() throw() {}

  virtual const char* what() const throw()
  {
    return std::string("Material \""+info+"\" is not unique").c_str();
  }
  
  std::string info;
};

struct NonUniqueParameterException: public std::exception
{
  NonUniqueParameterException(std::string const& info) : info(info) {}
  ~NonUniqueParameterException() throw() {}

  virtual const char* what() const throw()
  {
    return std::string("Parameter \""+info+"\" is not unique").c_str();
  }
  
  std::string info;
};

struct DTDLoadError: public std::exception
{
  virtual const char* what() const throw()
  {
    return std::string("Input material database valiadation could not be loaded").c_str();
  }
};

struct XMLLoadError: public std::exception
{
  virtual const char* what() const throw()
  {
    return std::string("Input material database could not be loaded").c_str();
  }
};

struct DTDError: public std::exception
{
  virtual const char* what() const throw()
  {
    return std::string("Input material database is not valid").c_str();
  }
};

struct XMLFileLoadError: public std::exception
{
  virtual const char* what() const throw()
  {
    return std::string("Input material database XML file does not exist").c_str();
  }
};

} // end namespace vmat

#endif
