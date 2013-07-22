#ifndef VIENNAMINI_DEVICE_HPP
#define VIENNAMINI_DEVICE_HPP

/* =======================================================================
   Copyright (c) 2011, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
                 ViennaMini - The Vienna Device Simulator
                             -----------------

   authors:    Karl Rupp                          rupp@iue.tuwien.ac.at
               Josef Weinbub                   weinbub@iue.tuwien.ac.at
               (add your name here)

   license:    see file LICENSE in the ViennaFVM base directory
======================================================================= */

// ViennaGrid includes:
#include "viennagrid/forwards.hpp"
#include "viennagrid/config/default_configs.hpp"

namespace viennamini {

template<typename Domain, typename Segmentation, typename Storage>
struct device
{
  typedef double                                      numeric_type;
  typedef std::map<std::size_t, std::string>          indexkeys_type;
  typedef std::vector<std::size_t>                    indices_type;
  typedef std::map<std::size_t, numeric_type>         index_values_type;
  typedef Domain                                      domain_type;
  typedef Segmentation                                segmentation_type;
  typedef typename segmentation_type::segment_type    segment_type;
  typedef Storage                                     storage_type;
  typedef std::map<std::size_t, std::size_t>          index_map_type;

  device(domain_type& domain, segmentation_type& segments, storage_type& storage)
    : domain(domain), segments(segments), storage(storage)
  {
    std::size_t global_id = 0;
    for(typename segmentation_type::iterator sit = segments.begin();
        sit != segments.end(); sit++)
    {
      segment_index_map[global_id++] = (*sit).id();
    }
  }

  /**
      @brief Stores a name string for a given segment index
  */
  void assign_name(std::size_t segment_index, std::string const& name)
  {
    segment_names[segment_index] = name;
  }

  /**
      @brief Stores a material ID string for a given segment index
  */
  void assign_material(std::size_t segment_index, std::string const& material_id)
  {
    segment_materials[segment_index] = material_id;
  }

  /**
      @brief Identifies the segment to be a contact
  */
  void assign_contact(std::size_t segment_index)
  {
  #ifdef VIENNAMINI_DEBUG
    std::cout << "* assign_contact(): segment " << segment_index << std::endl;
  #endif
    contact_segments.push_back(segment_index);
  }

  /**
      @brief Identifies the segment to be a oxide
  */
  void assign_oxide(std::size_t segment_index)
  {
  #ifdef VIENNAMINI_DEBUG
    std::cout << "* assign_oxide(): segment " << segment_index << std::endl;
  #endif
    oxide_segments.push_back(segment_index);
  }

  /**
      @brief Identifies the segment to be a semiconductor
  */
  template<typename NumericT>
  void assign_semiconductor(std::size_t segment_index, NumericT const& ND, NumericT const& NA)
  {
  #ifdef VIENNAMINI_DEBUG
    std::cout << "* assign_semiconductor(): segment " << segment_index << std::endl;
  #endif
    semiconductor_segments.push_back(segment_index);

    segment_donators[segment_index] = ND;
    segment_acceptors[segment_index] = NA;
  }

  indexkeys_type& get_segment_names()          { return segment_names; }
  indexkeys_type& get_segment_materials()      { return segment_materials; }
  indices_type&   get_contact_segments()       { return contact_segments; }
  indices_type&   get_oxide_segments()         { return oxide_segments; }
  indices_type&   get_semiconductor_segments() { return semiconductor_segments; }

  numeric_type get_donator(std::size_t segment_index)  { return segment_donators[segment_index]; }
  numeric_type get_acceptor(std::size_t segment_index) { return segment_acceptors[segment_index]; }


  inline domain_type&       get_domain()   { return domain;   }
  inline segmentation_type& get_segments() { return segments; }
  inline storage_type&      get_storage()  { return storage;  }
  inline segment_type&      get_segment(std::size_t si) { return segments(segment_index_map[si]); }

  // -----
private:
  domain_type       & domain;
  segmentation_type & segments;
  storage_type      & storage;

  indexkeys_type               segment_names;
  indexkeys_type               segment_materials;
  indices_type                 contact_segments;
  indices_type                 oxide_segments;
  indices_type                 semiconductor_segments;
  index_values_type            segment_donators;
  index_values_type            segment_acceptors;
  index_map_type               segment_index_map;
};

} // viennamini

#endif

