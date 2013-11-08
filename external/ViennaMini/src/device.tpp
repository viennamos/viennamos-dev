
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

#include "viennamini/device.hpp"

namespace viennamini {

template<typename MeshT, typename SegmentationT, typename StorageT>
device<MeshT,SegmentationT,StorageT>::device(MeshT& mesh, SegmentationT& segments, StorageT& storage)
    : mesh_(mesh), segments_(segments), storage_(storage)
{
}

template<typename MeshT, typename SegmentationT, typename StorageT>
void device<MeshT,SegmentationT,StorageT>::assign_name(std::size_t segment_index, std::string const& name)
{
  segment_names_[segment_index] = name;
}

template<typename MeshT, typename SegmentationT, typename StorageT>
void device<MeshT,SegmentationT,StorageT>::assign_material(std::size_t segment_index, std::string const& material_id)
{
  segment_materials_[segment_index] = material_id;
}

template<typename MeshT, typename SegmentationT, typename StorageT>
void device<MeshT,SegmentationT,StorageT>::assign_contact(std::size_t segment_index)
{
#ifdef VIENNAMINI_DEBUG
  std::cout << "* assign_contact(): segment " << segment_index << std::endl;
#endif
  contact_segments_.push_back(segment_index);
}

template<typename MeshT, typename SegmentationT, typename StorageT>
void device<MeshT,SegmentationT,StorageT>::assign_oxide(std::size_t segment_index)
{
#ifdef VIENNAMINI_DEBUG
  std::cout << "* assign_oxide(): segment " << segment_index << std::endl;
#endif
  oxide_segments_.push_back(segment_index);
}

template<typename MeshT, typename SegmentationT, typename StorageT>
void device<MeshT,SegmentationT,StorageT>::assign_semiconductor(std::size_t segment_index, NumericType const& ND, NumericType const& NA)
{
#ifdef VIENNAMINI_DEBUG
  std::cout << "* assign_semiconductor(): segment " << segment_index << std::endl;
#endif
  semiconductor_segments_.push_back(segment_index);

  segment_donators_[segment_index] = ND;
  segment_acceptors_[segment_index] = NA;
}

template<typename MeshT, typename SegmentationT, typename StorageT>
typename device<MeshT,SegmentationT,StorageT>::IndexKeysType&     device<MeshT,SegmentationT,StorageT>::segment_names()          { return segment_names_; }

template<typename MeshT, typename SegmentationT, typename StorageT>
typename device<MeshT,SegmentationT,StorageT>::IndexKeysType&     device<MeshT,SegmentationT,StorageT>::segment_materials()      { return segment_materials_; }

template<typename MeshT, typename SegmentationT, typename StorageT>
typename device<MeshT,SegmentationT,StorageT>::IndicesType&       device<MeshT,SegmentationT,StorageT>::contact_segments()       { return contact_segments_; }

template<typename MeshT, typename SegmentationT, typename StorageT>
typename device<MeshT,SegmentationT,StorageT>::IndicesType&       device<MeshT,SegmentationT,StorageT>::oxide_segments()         { return oxide_segments_; }

template<typename MeshT, typename SegmentationT, typename StorageT>
typename device<MeshT,SegmentationT,StorageT>::IndicesType&       device<MeshT,SegmentationT,StorageT>::semiconductor_segments() { return semiconductor_segments_; }

template<typename MeshT, typename SegmentationT, typename StorageT>
typename device<MeshT,SegmentationT,StorageT>::NumericType&       device<MeshT,SegmentationT,StorageT>::donator (std::size_t segment_index)  { return segment_donators_[segment_index]; }

template<typename MeshT, typename SegmentationT, typename StorageT>
typename device<MeshT,SegmentationT,StorageT>::NumericType&       device<MeshT,SegmentationT,StorageT>::acceptor(std::size_t segment_index)  { return segment_acceptors_[segment_index]; }

template<typename MeshT, typename SegmentationT, typename StorageT>
typename device<MeshT,SegmentationT,StorageT>::MeshType&          device<MeshT,SegmentationT,StorageT>::mesh()                  { return mesh_;   }

template<typename MeshT, typename SegmentationT, typename StorageT>
typename device<MeshT,SegmentationT,StorageT>::SegmentationType&  device<MeshT,SegmentationT,StorageT>::segments()              { return segments_; }

template<typename MeshT, typename SegmentationT, typename StorageT>
typename device<MeshT,SegmentationT,StorageT>::StorageType&       device<MeshT,SegmentationT,StorageT>::storage()               { return storage_;  }

template<typename MeshT, typename SegmentationT, typename StorageT>
typename device<MeshT,SegmentationT,StorageT>::SegmentType&       device<MeshT,SegmentationT,StorageT>::segment(std::size_t si) { return segments_(si); }

} // viennamini



