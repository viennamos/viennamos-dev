#ifndef VIENNAMATERIALS_LIBRARYDYN_HPP
#define VIENNAMATERIALS_LIBRARYDYN_HPP


/* =============================================================================
   Copyright (c) 2013, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */

// System includes
//
#include <iostream>
#include <vector>

// ViennaMaterials includes
//
#include "viennamaterials/forwards.h"
#include "viennamaterials/library.hpp"
#include "viennamaterials/exceptions.hpp"
#include "viennamaterials/check.hpp"


namespace vmat { 


/** 
    @brief Class acts as a base class for dynamic utilization
*/
class LibraryDyn
{
public:
  typedef double                    numeric_type;
  typedef std::vector<std::string>  keys_type;

  /** 
      @brief Loads an input material file and polpulates the internal database
  */
  virtual bool load(std::string const& filename) = 0;

  /** 
      @brief Loads an input material stream and polpulates the internal database
  */
  virtual bool load(std::stringstream & stream) = 0;

  /** 
      @brief Writes the content of the database to the stream
  */
  virtual void dump(std::ostream& stream = std::cout) = 0;

  /** 
      @brief Checks if the database has a specific parameter for a specific material available
  */
  virtual bool hasParameter(std::string const& material_id, std::string const& parameter_id) = 0;

  /** 
      @brief Access the numeric value of a specific parameter of a material
  */
  virtual numeric_type getParameterValue(std::string const& material, std::string const& parameter) = 0;

  /** 
      @brief Access the unit string of a specific parameter a material
  */
  virtual std::string getParameterUnit(std::string const& material, std::string const& parameter) = 0;

  /** 
      @brief Retrieve all materials of a specific category
  */
  virtual keys_type getMaterialsOfCategory(std::string const& category) = 0;

  /** 
      @brief Checks if the database has materials of a specific category
  */
  virtual bool hasMaterialsOfCategory(std::string const& category_id) = 0;
};


} //namespace vmat  

#endif


