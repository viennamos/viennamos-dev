#ifndef VIENNAMATERIALS_KERNELS_PUGIXML_HPP
#define VIENNAMATERIALS_KERNELS_PUGIXML_HPP


/* =============================================================================
   Copyright (c) 2013, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */

#include "viennautils/xml.hpp"
#include "viennautils/file.hpp"

#ifdef HAVE_VIENNAIPD
extern "C" {
#include "ipd.h"
}
#endif

namespace vmat { 

namespace tag {
struct pugixml {};
} // tag


namespace kernel {

struct PugiXML
{
  typedef viennautils::xml<viennautils::tag::pugixml>::type           MaterialDatabase;
  typedef MaterialDatabase::NodeSet                                   Entries;
  typedef MaterialDatabase::Node                                      Entry;
  typedef MaterialDatabase::NodeIterator                              EntryIterator;
  typedef double                                                      Numeric;  
  typedef bool                                                        Boolean;
  typedef std::string                                                 String;

  PugiXML()
  {
    // setup variables used as placeholdes in the precompiled queries
    vars.add(vmat::key::id.c_str(),         pugi::xpath_type_string);
    vars.add(vmat::key::category.c_str(),   pugi::xpath_type_string);
    vars.add(vmat::key::parameter.c_str(),  pugi::xpath_type_string);

    // precompile the various query strings - use placeholders for run-time parameters
    std::string material_query_string = "/materials/material[id = string($"+vmat::key::id+")]";
    query_material = new pugi::xpath_query(material_query_string.c_str(), &vars);
    
    std::string category_query_string = "/materials/material[category = string($"+vmat::key::category+")]";
    query_category = new pugi::xpath_query(category_query_string.c_str(), &vars);

    std::string parameter_query_string = "/materials/material[id = string($"+vmat::key::id+")]/parameters/parameter[name = string($"+vmat::key::parameter+")]";
    query_parameter = new pugi::xpath_query(parameter_query_string.c_str(), &vars);    

    std::string parameter_value_query_string = "/materials/material[id = string($"+vmat::key::id+")]/parameters/parameter[name = string($"+vmat::key::parameter+")]/value";
    query_parameter_value = new pugi::xpath_query(parameter_value_query_string.c_str(), &vars);    

    std::string parameter_unit_query_string = "/materials/material[id = string($"+vmat::key::id+")]/parameters/parameter[name = string($"+vmat::key::parameter+")]/unit";
    query_parameter_unit = new pugi::xpath_query(parameter_unit_query_string.c_str(), &vars);    
    
    std::string parameter_note_query_string = "/materials/material[id = string($"+vmat::key::id+")]/parameters/parameter[name = string($"+vmat::key::parameter+")]/note";
    query_parameter_note = new pugi::xpath_query(parameter_note_query_string.c_str(), &vars);      
  }
  
  ~PugiXML()
  {
    // the xpath queries are due to pugixml's impelmentation required to be allocated
    // on the heap ..
    delete query_material;
    delete query_category;
    delete query_parameter;
    delete query_parameter_value;
    delete query_parameter_unit;
    delete query_parameter_note;  
  }

  bool load(std::string const& filename)
  {
#ifdef HAVE_LIBXML2
    if(!vmat::check(filename))
    {
      return false;
    }
#endif
  
    if(!viennautils::file_exists(filename))
    { 
      throw XMLFileLoadError();
      return false;
    }

    if(viennautils::file_extension(filename) == "xml")  // native
    {
      mdb.read(filename);
      return true;
    }
#ifdef HAVE_VIENNAIPD    
    else if(viennautils::file_extension(filename) == "ipd")  // conversion required
    {
      ipdInit(NULL, NULL);
      
      // todo: 
      std::cout << "IPD support is not yet implemented .." << std::endl;
      
      return true;
    }
#endif
    else return false;
  }
  
  bool load(std::stringstream & stream)
  {
    mdb.read(stream);
    return true;
  }
  
  void dump(std::ostream& stream = std::cout)
  {
    mdb.dump(stream);
  }

  void dump(std::ostream& stream = std::cout) const
  {
    mdb.dump(stream);
  }

  Entries query(std::string const& expr)
  {
    return mdb.query_raw(expr);
  }

  Entries query(std::string const& expr) const
  {
    return mdb.query_raw(expr);
  }

  Entries getSemiconductors()
  {
    vars.set(vmat::key::category.c_str(), vmat::key::semiconductor.c_str());
    return query_category->evaluate_node_set(mdb.xml);
  }
  
//  Entries getSemiconductors() const
//  {
//    vars.set(vmat::key::category.c_str(), vmat::key::semiconductor.c_str());
//    return query_category->evaluate_node_set(mdb.xml);
//  }

  bool hasSemiconductors()
  {
    return !(this->getSemiconductors().empty());
  }  

//  bool hasSemiconductors() const
//  {
//    return !(this->getSemiconductors().empty());
//  } 

  Entries getMetals()
  {
    vars.set(vmat::key::category.c_str(), vmat::key::metal.c_str());
    return query_category->evaluate_node_set(mdb.xml);
  }

//  Entries getMetals() const
//  {
//    vars.set(vmat::key::category.c_str(), vmat::key::metal.c_str());
//    return query_category->evaluate_node_set(mdb.xml);
//  }

  bool hasMetals()
  {
    return !(this->getMetals().empty());
  }  

  Entries getOxides()
  {
    vars.set(vmat::key::category.c_str(), vmat::key::oxide.c_str());
    return query_category->evaluate_node_set(mdb.xml);
  }
  
  bool hasOxides()
  {
    return !(this->getOxides().empty());
  }  

  Entries getMaterialsOfCategory(std::string const& category_id)
  {
    vars.set(vmat::key::category.c_str(), category_id.c_str());
    return query_category->evaluate_node_set(mdb.xml);
  }
  
  bool hasMaterialsOfCategory(std::string const& category_id)
  {
    return !(this->getMaterialsOfCategory(category_id).empty());
  }  

  // -- Material Accessors -----------------------------------------------------
private:  
  Entries queryMaterial(std::string const& material_id)
  {
    vars.set(vmat::key::id.c_str(), material_id.c_str());
    return query_material->evaluate_node_set(mdb.xml);
  }
public:
  Entry getMaterial(std::string const& material_id)
  {
    pugi::xpath_node_set const& result = this->queryMaterial(material_id);
    if(result.size() > 1) // there must be only one material with this id
    {
      throw vmat::NonUniqueMaterialException(material_id);
      return Entry();
    }
    else if (result.size() == 0) return Entry();
    else return result.first(); // if success, return only a single xpath_node
  }
  
  bool hasMaterial(std::string const& material_id)
  {
    pugi::xpath_node_set const& result = this->queryMaterial(material_id);
    if(result.size() > 1) // there must be only one material with this id
    {
      throw vmat::NonUniqueMaterialException(material_id);
      return false;
    }
    else if (result.size() == 0) return false;
    else return true;
  }  
  // ---------------------------------------------------------------------------

  // -- Parameter Accessors ----------------------------------------------------
private:
  Entries queryParameter(std::string const& material_id, std::string const& parameter_id)
  {
    vars.set(vmat::key::id.c_str(),        material_id.c_str());  
    vars.set(vmat::key::parameter.c_str(), parameter_id.c_str());
    return query_parameter->evaluate_node_set(mdb.xml);
  }
public:
  Entry getParameter(std::string const& material_id, std::string const& parameter_id)
  {
    pugi::xpath_node_set const& result = this->queryParameter(material_id, parameter_id);
    if(result.size() > 1) // there must be only one parameter with this id
    {
      throw vmat::NonUniqueParameterException(parameter_id);
      return Entry();
    }
    else if (result.size() == 0) return Entry();
    else return result.first(); // if success, return only a single xpath_node
  }

  bool hasParameter(std::string const& material_id, std::string const& parameter_id)
  {
    pugi::xpath_node_set const& result = this->queryParameter(material_id, parameter_id);
    if(result.size() > 1) // there must be only one material with this id
    {
      throw vmat::NonUniqueParameterException(parameter_id);
      return false;
    }
    else if (result.size() == 0) return false;
    else return true;
  }  

  Numeric getParameterValue(std::string const& material_id, std::string const& parameter_id)
  {
    vars.set(vmat::key::id.c_str(),        material_id.c_str());  
    vars.set(vmat::key::parameter.c_str(), parameter_id.c_str());
    return query_parameter_value->evaluate_number(mdb.xml);
  }

  String getParameterUnit(std::string const& material_id, std::string const& parameter_id)
  {
    vars.set(vmat::key::id.c_str(),        material_id.c_str());  
    vars.set(vmat::key::parameter.c_str(), parameter_id.c_str());
    return query_parameter_unit->evaluate_string(mdb.xml);
  }

  String getParameterNote(std::string const& material_id, std::string const& parameter_id)
  {
    vars.set(vmat::key::id.c_str(),        material_id.c_str());  
    vars.set(vmat::key::parameter.c_str(), parameter_id.c_str());
    return query_parameter_note->evaluate_string(mdb.xml);
  }

  // ---------------------------------------------------------------------------
  
private:

  MaterialDatabase mdb;
  
  pugi::xpath_variable_set    vars;
  
  pugi::xpath_query *query_material;
  pugi::xpath_query *query_category;
  pugi::xpath_query *query_parameter;  
  pugi::xpath_query *query_parameter_value;
  pugi::xpath_query *query_parameter_unit;
  pugi::xpath_query *query_parameter_note;  

};


} // kernel


template<>
struct Library <vmat::tag::pugixml>
{
  typedef vmat::kernel::PugiXML type;
};



// TOOLS -----------------------------------------------------------------------
inline void printToStream(vmat::kernel::PugiXML::Entries const& nodeset, std::ostream& stream = std::cout)
{
  for(size_t i = 0; i < nodeset.size(); i++)
    nodeset[i].node().print(stream, "  ");
}

inline void printToStream(vmat::kernel::PugiXML::Entry const& node, std::ostream& stream = std::cout)
{
  node.node().print(stream, "  ");
}

inline vmat::kernel::PugiXML::Entries query(vmat::kernel::PugiXML::Entry const& entry, std::string const& query)
{
  return entry.node().select_nodes(query.c_str());
}

inline bool isParameter(vmat::kernel::PugiXML::Entry const& entry)
{
  if(std::string(entry.node().name()) == vmat::key::parameter)
    return true;
  else return false;
}

inline bool isMaterial(vmat::kernel::PugiXML::Entry const& entry)
{
  if(std::string(entry.node().name()) == vmat::key::material)
    return true;
  else return false;
}

inline vmat::kernel::PugiXML::Numeric value(vmat::kernel::PugiXML::Entry const& entry)
{
  return pugi::xpath_query("value").evaluate_number(entry);
}

inline vmat::kernel::PugiXML::String name(vmat::kernel::PugiXML::Entry const& entry)
{
  return pugi::xpath_query("name").evaluate_string(entry);
}

inline vmat::kernel::PugiXML::String id(vmat::kernel::PugiXML::Entry const& entry)
{
  return pugi::xpath_query("id").evaluate_string(entry);
}

inline vmat::kernel::PugiXML::String unit(vmat::kernel::PugiXML::Entry const& entry)
{
  return pugi::xpath_query("unit").evaluate_string(entry);
}

inline vmat::kernel::PugiXML::String note(vmat::kernel::PugiXML::Entry const& entry)
{
  return pugi::xpath_query("note").evaluate_string(entry);
}
// -----------------------------------------------------------------------------

struct ParameterExtractor
{
  typedef vmat::kernel::PugiXML::Entries  Entries;
  typedef vmat::kernel::PugiXML::Entry    Entry;

public:
  ParameterExtractor()
  {
    vars.add(vmat::key::parameter.c_str(), pugi::xpath_type_string);
    std::string parameter_query_string = ".//parameters/parameter[name = string($"+vmat::key::parameter+")]";
    query_parameter = new pugi::xpath_query(parameter_query_string.c_str(), &vars);    
  }

  ~ParameterExtractor()
  {
    delete query_parameter;
  }

  Entry operator()(Entry const& material, std::string const& parameter_id)
  {
    pugi::xpath_node_set const& result = this->queryParameter(material, parameter_id);
    if(result.size() > 1) // there must be only one parameter with this id
    {
      throw vmat::NonUniqueParameterException(parameter_id);
      return Entry();
    }
    else if (result.size() == 0) return Entry();
    else return result.first(); // if success, return only a single xpath_node
  }

  bool hasParameter(Entry const& material, std::string const& parameter_id)
  {
    pugi::xpath_node_set const& result = this->queryParameter(material, parameter_id);
    if(result.size() > 1) // there must be only one material with this id
    {
      throw vmat::NonUniqueParameterException(parameter_id);
      return false;
    }
    else if (result.size() == 0) return false;
    else return true;
  }  

private:
  Entries queryParameter(Entry const& material, std::string const& parameter_id)
  {
    vars.set(vmat::key::parameter.c_str(), parameter_id.c_str());
    return query_parameter->evaluate_node_set(material.node());
  }

  pugi::xpath_variable_set    vars;
  
  pugi::xpath_query *query_parameter;  
};

inline vmat::kernel::PugiXML::Entry getParameter(vmat::kernel::PugiXML::Entry const& material, std::string const& parameter_id)
{
  return vmat::ParameterExtractor()(material, parameter_id);
}

inline bool hasParameter(vmat::kernel::PugiXML::Entry const& material, std::string const& parameter_id)
{
  return vmat::ParameterExtractor().hasParameter(material, parameter_id);
}


} // vmat


#endif

