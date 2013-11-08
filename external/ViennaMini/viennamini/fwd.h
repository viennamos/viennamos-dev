#ifndef VIENNAMINI_FORWARDS_H
#define VIENNAMINI_FORWARDS_H

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

/** Forward declarations */


//#include "viennamini/device.hpp"

// ViennaData includes:
#include "viennadata/api.hpp"

#include "viennagrid/forwards.hpp"

// ViennaMaterials includes:
#include "viennamaterials/library.hpp"
#include "viennamaterials/kernels/pugixml.hpp"

namespace viennamini
{

  //
  // TODO: Think about where to move the following keys.
  //

  struct permittivity_key
  {
    // Operator< is required for compatibility with std::map
    bool operator<(permittivity_key const & ) const { return false; }
  };

  struct mobility_electrons_key
  {
    // Operator< is required for compatibility with std::map
    bool operator<(mobility_electrons_key const & ) const { return false; }
  };

  struct mobility_holes_key
  {
    // Operator< is required for compatibility with std::map
    bool operator<(mobility_holes_key const & ) const { return false; }
  };

  struct builtin_potential_key
  {
    // Operator< is required for compatibility with std::map
    bool operator<(builtin_potential_key const & ) const { return false; }
  };

  // N_D
  struct donator_doping_key
  {
    // Operator< is required for compatibility with std::map
    bool operator<(donator_doping_key const & ) const { return false; }
  };

  // N_A
  struct acceptor_doping_key
  {
    // Operator< is required for compatibility with std::map
    bool operator<(acceptor_doping_key const & ) const { return false; }
  };

  struct oxide_key
  {
    // Operator< is required for compatibility with std::map
    bool operator<(oxide_key const & ) const { return false; }
  };

  struct semiconductor_key
  {
    // Operator< is required for compatibility with std::map
    bool operator<(semiconductor_key const & ) const { return false; }
  };

  struct contact_key
  {
    // Operator< is required for compatibility with std::map
    bool operator<(contact_key const & ) const { return false; }
  };

  struct material_key
  {
    // Operator< is required for compatibility with std::map
    bool operator<(material_key const & ) const { return false; }
  };

  struct potential_key
  {
    // Operator< is required for compatibility with std::map
    bool operator<(potential_key const & ) const { return false; }
  };

  struct electron_key
  {
    // Operator< is required for compatibility with std::map
    bool operator<(electron_key const & ) const { return false; }
  };

  struct hole_key
  {
    // Operator< is required for compatibility with std::map
    bool operator<(hole_key const & ) const { return false; }
  };


  template<typename DeviceT, typename MatlibT>
  class simulator;

  template<typename MeshT, typename SegmentationT, typename StorageT>
  class device;

  typedef ::viennadata::storage<>                                                                   StorageType;
  typedef ::vmat::Library<vmat::tag::pugixml>::type                                                 MatLibPugixmlType;

  typedef ::viennagrid::mesh< viennagrid::config::triangular_2d >                                   MeshTriangular2DType;
  typedef ::viennagrid::mesh< viennagrid::config::tetrahedral_3d >                                  MeshTetrahedral3DType;

  typedef ::viennagrid::result_of::segmentation<MeshTriangular2DType>::type                         SegmentationTriangular2DType;
  typedef ::viennagrid::result_of::segmentation<MeshTetrahedral3DType>::type                        SegmentationTetrahedral3DType;

  typedef ::viennamini::device<MeshTriangular2DType,  SegmentationTriangular2DType,  StorageType>   DeviceTriangular2DType;
  typedef ::viennamini::device<MeshTetrahedral3DType, SegmentationTetrahedral3DType, StorageType>   DeviceTetrahedral3DType;

  typedef ::viennamini::simulator<DeviceTriangular2DType,  MatLibPugixmlType>                       SimulatorTriangular2DType;
  typedef ::viennamini::simulator<DeviceTetrahedral3DType,  MatLibPugixmlType>                      SimulatorTetrahedral3DType;

} // viennamini

#endif
