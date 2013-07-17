#ifndef VIENNAMATERIALS_PUGIXMLDYN_HPP
#define VIENNAMATERIALS_PUGIXMLDYN_HPP


/* =============================================================================
   Copyright (c) 2013, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */

// ViennaMaterials includes
//
#include "viennamaterials/kernels/pugixml.hpp"

// ViennaUtils includes
//
#include "viennautils/xml.hpp"
#include "viennautils/file.hpp"

#ifdef HAVE_VIENNAIPD
extern "C" {
#include "ipd.h"
}
#endif


namespace vmat { 

/** 
    @brief Provides the pugixml backend in the dynamic class hierarchy
*/
class PugiXMLDyn : public LibraryDyn
{
private:
  typedef vmat::kernel::PugiXML   Library;
  typedef Library::Entries        Entries;
  typedef Library::EntryIterator  EntryIterator;

public:
  virtual bool load(std::string const& filename)
  {
    return matlib.load(filename);
  }
  
  virtual bool load(std::stringstream & stream)
  {
    return matlib.load(stream);
  }

  virtual void dump(std::ostream& stream = std::cout)
  {
    matlib.dump(stream);
  }

  virtual bool hasParameter(std::string const& material, std::string const& parameter)
  {
    return matlib.hasParameter(material, parameter);
  }

  virtual numeric_type getParameterValue(std::string const& material, std::string const& parameter)
  {
    return matlib.getParameterValue(material, parameter);
  }
  
  virtual std::string getParameterUnit(std::string const& material, std::string const& parameter)
  {
    return matlib.getParameterUnit(material, parameter);  
  }  
  
  virtual keys_type getMaterialsOfCategory(std::string const& category)
  {
    keys_type keys;
    Entries entries = matlib.getMaterialsOfCategory(category);
    for(EntryIterator iter = entries.begin(); iter != entries.end(); iter++)
      keys.push_back( vmat::id(*iter) );
    return keys;
  }
  
  virtual bool hasMaterialsOfCategory(std::string const& category)
  {
    return matlib.hasMaterialsOfCategory(category);
  }  
  
private:
  Library matlib;
};


} //namespace vmat  

#endif


