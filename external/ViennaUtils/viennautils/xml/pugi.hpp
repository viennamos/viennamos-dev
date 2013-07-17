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


#ifndef VIENNAUTILS_XML_PUGIXML_HPP
#define VIENNAUTILS_XML_PUGIXML_HPP

#include <sstream>

// local includes
#include "exceptions.hpp"

// *** boost includes
#include "boost/lexical_cast.hpp"
#include "boost/archive/text_iarchive.hpp"
#include "boost/archive/text_oarchive.hpp"
#include "boost/algorithm/string/find.hpp"

namespace viennautils
{

//! pugixml wrapper class
struct pugixml
{
  typedef pugi::xpath_node_set      NodeSet;
  typedef pugi::xpath_node          Node;
  typedef const pugi::xpath_node *  NodeIterator;

   //! default constructor
   pugixml() : indent_string("  ") { }

   //! constructor
   pugixml(const std::string & filename, const std::string indent = "  ") : indent_string(indent)
   {
      read(filename);
   }

   //! delete the stored data
   void reset()
   {
      xml.reset();
   }

   //! read an input file
   void read(std::ifstream& stream)
   {
      pugi::xml_parse_result result = xml.load(stream);
      if(!result)
      {
        //std::cout << "Error offset: " << result.offset << " (error at [..." << (source + result.offset) << "])" << std::endl;
         throw InvalidXmlFileException("Exception in XmlReader::read -> " + std::string(result.description()) );
      }            
   }

   //! read an input file
   void read(const std::string & filename)
   {
      pugi::xml_parse_result result = xml.load_file(filename.c_str());
      if(!result)
      {
        //std::cout << "Error offset: " << result.offset << " (error at [..." << (source + result.offset) << "])" << std::endl;
         throw InvalidXmlFileException("Exception in XmlReader::read -> " + std::string(result.description()) );
      }      
   }

   //! read a stringstream
   void read(std::stringstream& sstream)
   {
      pugi::xml_parse_result result = xml.load(sstream);
      if(!result)
      {
        //std::cout << "Error offset: " << result.offset << " (error at [..." << (source + result.offset) << "])" << std::endl;
         throw InvalidXmlFileException("Exception in XmlReader::read -> " + std::string(result.description()) );
      }            
   }

   //! write an input file
   void write(std::ofstream& ostream)
   {
      dump(ostream);
   }

   //! write an input file
   void write(const std::string & filename)
   {
      std::ofstream stream(filename.c_str());
      dump(stream);
      stream.close();
   }

   //! dump the whole tree to the stream
   void dump(std::ostream & stream = std::cout)
   {
      xml.save(stream, indent_string.c_str());
   }

   //! dump the whole tree to the stream
   void dump(std::ostream & stream = std::cout) const
   {
      xml.save(stream, indent_string.c_str());
   }
   void clone(std::string const& path)
   {
     pugi::xpath_node xpnode = xml.select_single_node(path.c_str());
     pugi::xml_node node = xpnode.node();
     //node.parent().append_copy(node);
     node.parent().insert_copy_after(node, node);
   }

   void set_value(std::string const& path, std::size_t value)
   {
    //std::cout << "## path: " << path << std::endl;
    pugi::xpath_node_set xpnodes = xml.select_nodes(path.c_str());
    std::size_t i = 0;
    for (pugi::xpath_node_set::const_iterator it = xpnodes.begin(); it != xpnodes.end(); ++it)
    {
      pugi::xml_node node = it->node();
      node.child("id").first_child().set_value(boost::lexical_cast<std::string>(value++).c_str());
      //std::cout << "### node value: " << node.child("id").first_child().value() << std::endl;
      i++;
    }
   }

   //! perform a query for a path and return the query result
   NodeSet query_raw(std::string const& xpath_query_str)
   {
      try
      {
         return xml.select_nodes(xpath_query_str.c_str());
      }
      catch (const pugi::xpath_exception& e)
      {
         std::cerr << "Exception caught in XmlReader::query ->  " << e.what() << std::endl;
         throw XmlQueryException("Exception in XmlReader::query ->  " + std::string(e.what()));
         return NodeSet();
      }
   }
   
   //! perform a query for a path and return the query result
   NodeSet query_raw(std::string const& xpath_query_str) const
   {
      try
      {
         return xml.select_nodes(xpath_query_str.c_str());
      }
      catch (const pugi::xpath_exception& e)
      {
         std::cerr << "Exception caught in XmlReader::query ->  " << e.what() << std::endl;
         throw XmlQueryException("Exception in XmlReader::query ->  " + std::string(e.what()));
         return NodeSet();
      }
   }   

   //! perform a query for a path and return the query result
   std::string query(std::string xpath_query_str)
   {
      std::stringstream result_stream;
      try
      {
         pugi::xpath_node_set query_result = xml.select_nodes(xpath_query_str.c_str());
         for(size_t i = 0; i < query_result.size(); i++)
         {
           query_result[i].node().print(result_stream, indent_string.c_str());
         }
      }
      catch (const pugi::xpath_exception& e)
      {
         std::cerr << "Exception caught in XmlReader::query ->  " << e.what() << std::endl;
         throw XmlQueryException("Exception in XmlReader::query ->  " + std::string(e.what()));
      }
      std::string result = result_stream.str();
      boost::trim(result);  // remove front/trailing whitespaces
      return result;
   }

   //! perform a query and return the query result
   std::string query(std::string xpath_query_str) const
   {
      std::stringstream result_stream;
      try
      {
         pugi::xpath_node_set query_result = xml.select_nodes(xpath_query_str.c_str());
         for(size_t i = 0; i < query_result.size(); i++)
         {
           query_result[i].node().print(result_stream, indent_string.c_str());
         }
      }
      catch (const pugi::xpath_exception& e)
      {
         std::cerr << "Exception caught in XmlReader::query ->  " << e.what() << std::endl;
         throw XmlQueryException("Exception in XmlReader::query ->  " + std::string(e.what()));
      }
      std::string result = result_stream.str();
      boost::trim(result);  // remove front/trailing whitespaces
      return result;
   }

  //! test if the node exists
  bool exists(std::string xpath_query_str) 
  { 
    pugi::xpath_node_set query_result = xml.select_nodes(xpath_query_str.c_str());
    if(query_result.size() == 0) return false;
    else return true;
  }

  //! model the Boost Serialization concept
  friend class boost::serialization::access;

  template<class Archive>
  void save(Archive & ar, const unsigned int version) const
  {
    std::stringstream sstream;
    xml.save(sstream);
    std::string xmlstring= sstream.str();
    ar & xmlstring; 
  }

  template<class Archive>
  void load(Archive & ar, const unsigned int version)
  {
    std::string xmlstring;
    ar & xmlstring;
    std::stringstream sstream(xmlstring);
    xml.load(sstream);
  }

  BOOST_SERIALIZATION_SPLIT_MEMBER()


   //! the master xml object
   pugi::xml_document   xml;

   //! string which contains the whitespaces for identation
   std::string    indent_string;
};


} //namespace viennautils  


#endif

