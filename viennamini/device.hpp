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

#include "viennamini/fwd.h"

namespace viennamini {

template<typename MeshT, typename SegmentationT, typename StorageT>
struct device
{
  typedef MeshT                                           MeshType;
  typedef SegmentationT                                   SegmentationType;
  typedef StorageT                                        StorageType;
  typedef double                                          NumericType; // TODO derive ..
  typedef std::map<std::size_t, std::string>              IndexKeysType;
  typedef std::vector<std::size_t>                        IndicesType;
  typedef std::map<std::size_t, NumericType>              IndexValuesType;
  typedef std::map<std::size_t, std::size_t>              IndexMapType;
  typedef typename SegmentationType::segment_handle_type  SegmentType;

  typedef MeshType              mesh_type;
  typedef SegmentationType      segmentation_type;
  typedef StorageType           storage_type;
  typedef NumericType           numeric_type;
  typedef IndexKeysType         indexkeys_type;
  typedef IndicesType           indices_type;
  typedef IndexValuesType       indexvalues_type;
  typedef IndexMapType          indexmap_type;
  typedef SegmentType           segment_type;

  device(MeshT& mesh, SegmentationT& segments, StorageT& storage);

  /**
      @brief Stores a name string for a given segment index
  */
  void assign_name(std::size_t segment_index, std::string const& name);
//  {
//    segment_names_[segment_index] = name;
//  }

  /**
      @brief Stores a material ID string for a given segment index
  */
  void assign_material(std::size_t segment_index, std::string const& material_id);
//  {
//    segment_materials_[segment_index] = material_id;
//  }

  /**
      @brief Identifies the segment to be a contact
  */
  void assign_contact(std::size_t segment_index);
//  {
//  #ifdef VIENNAMINI_DEBUG
//    std::cout << "* assign_contact(): segment " << segment_index << std::endl;
//  #endif
//    contact_segments_.push_back(segment_index);
//  }

  /**
      @brief Identifies the segment to be a oxide
  */
  void assign_oxide(std::size_t segment_index);
//  {
//  #ifdef VIENNAMINI_DEBUG
//    std::cout << "* assign_oxide(): segment " << segment_index << std::endl;
//  #endif
//    oxide_segments_.push_back(segment_index);
//  }

  /**
      @brief Identifies the segment to be a semiconductor
  */
  void assign_semiconductor(std::size_t segment_index, NumericType const& ND, NumericType const& NA);
//  {
//  #ifdef VIENNAMINI_DEBUG
//    std::cout << "* assign_semiconductor(): segment " << segment_index << std::endl;
//  #endif
//    semiconductor_segments_.push_back(segment_index);

//    segment_donators_[segment_index] = ND;
//    segment_acceptors_[segment_index] = NA;
//  }

IndexKeysType& segment_names();
IndexKeysType& segment_materials();
IndicesType&   contact_segments();
IndicesType&   oxide_segments();
IndicesType&   semiconductor_segments();

NumericType& donator (std::size_t segment_index);
NumericType& acceptor(std::size_t segment_index);


MeshType&          mesh();
SegmentationType&  segments();
StorageType&       storage();
SegmentType&       segment(std::size_t si);

//  inline IndexKeysType& segment_names()          { return segment_names_; }
//  inline IndexKeysType& segment_materials()      { return segment_materials_; }
//  inline IndicesType&   contact_segments()       { return contact_segments_; }
//  inline IndicesType&   oxide_segments()         { return oxide_segments_; }
//  inline IndicesType&   semiconductor_segments() { return semiconductor_segments_; }

//  inline NumericType& donator (std::size_t segment_index)  { return segment_donators_[segment_index]; }
//  inline NumericType& acceptor(std::size_t segment_index)  { return segment_acceptors_[segment_index]; }


//  inline MeshType&          mesh()                  { return mesh_;   }
//  inline SegmentationType&  segments()              { return segments_; }
//  inline StorageType&       storage()               { return storage_;  }
//  inline SegmentType&       segment(std::size_t si) { return segments_(si); }

  // -----
private:
  MeshType                   & mesh_;
  SegmentationType           & segments_;
  StorageType                & storage_;

  IndexKeysType               segment_names_;
  IndexKeysType               segment_materials_;
  IndicesType                 contact_segments_;
  IndicesType                 oxide_segments_;
  IndicesType                 semiconductor_segments_;
  IndexValuesType             segment_donators_;
  IndexValuesType             segment_acceptors_;
};

} // viennamini

#endif

