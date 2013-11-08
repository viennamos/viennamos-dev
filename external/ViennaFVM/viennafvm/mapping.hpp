#ifndef VIENNAFVM_MAPPING_HPP
#define VIENNAFVM_MAPPING_HPP

/* =======================================================================
   Copyright (c) 2011, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
           ViennaFVM - The Vienna Finite Volume Method Library
                             -----------------

   authors:    Karl Rupp                             rupp@iue.tuwien.ac.at
               Josef Weinbub                      weinbub@iue.tuwien.ac.at

               (add your name here)

   license:    To be discussed, see file LICENSE in the ViennaFVM base directory
======================================================================= */


#include <vector>
#include "viennafvm/forwards.h"
#include "viennafvm/common.hpp"

#include "viennagrid/forwards.hpp"

namespace viennafvm
{

/** @brief If EntityType is a ViennaGrid segment, returns the domain. If EntityType is already the domain, no changes.
*/
template <typename EntityType>
struct extract_domain
{
   typedef EntityType  type;
   static EntityType       & apply(EntityType       & mesh) { return mesh; }
   static EntityType const & apply(EntityType const & mesh) { return mesh; }
};

template <typename ConfigType>
struct extract_domain<viennagrid::segmentation<ConfigType> >
{
   typedef typename viennagrid::result_of::mesh<ConfigType>::type    type;
   static type       & apply(viennagrid::segment_handle<ConfigType>       & seg) { return seg.mesh(); }
   static type const & apply(viennagrid::segment_handle<ConfigType> const & seg) { return seg.mesh(); }
};



/** @brief Distributes mapping indices over domain or segment
*
*/
template <typename LinPdeSysT, typename DomainType, typename StorageType>
long create_mapping(LinPdeSysT & pde_system,
                    std::size_t  pde_index,
                    DomainType const & domain,
                    StorageType & storage,
                    long start_index = 0)
{
  typedef typename viennagrid::result_of::cell<DomainType>::type CellTag;

  typedef typename viennagrid::result_of::point<DomainType>::type                  PointType;
  typedef typename viennagrid::result_of::element<DomainType, CellTag>::type    CellType;

  typedef typename viennagrid::result_of::const_element_range<DomainType, CellTag>::type   CellContainer;
  typedef typename viennagrid::result_of::iterator<CellContainer>::type                       CellIterator;

  typedef typename LinPdeSysT::mapping_key_type   MappingKeyType;
  typedef typename LinPdeSysT::boundary_key_type  BoundaryKeyType;

  long map_index = start_index;
  bool init_done = false;

  long unknown_id = pde_system.unknown(pde_index)[0].id();

  BoundaryKeyType bnd_key(unknown_id);
  MappingKeyType  map_key(unknown_id);

  typename viennadata::result_of::accessor<StorageType, BoundaryKeyType, bool, CellType>::type boundary_accessor =
      viennadata::make_accessor(storage, bnd_key);

  typename viennadata::result_of::accessor<StorageType, MappingKeyType, long, CellType>::type cell_mapping_accessor =
      viennadata::make_accessor(storage, map_key);

  typename viennadata::result_of::accessor<StorageType, viennafvm::disable_quantity_key, bool, CellType>::type disable_quantity_accessor =
      viennadata::make_accessor(storage, viennafvm::disable_quantity_key(unknown_id));

  //eventually, map indices need to be set to invalid first:
  if (!init_done)
  {
    typedef typename extract_domain<DomainType>::type   TrueDomainType;
    typedef typename viennagrid::result_of::const_element_range<TrueDomainType, CellTag>::type  DomainCellContainer;
    typedef typename viennagrid::result_of::iterator<DomainCellContainer>::type                    DomainCellIterator;

    DomainCellContainer cells(extract_domain<DomainType>::apply(domain));
    for (DomainCellIterator cit  = cells.begin();
                            cit != cells.end();
                          ++cit)
    {
      cell_mapping_accessor(*cit) = viennafvm::QUANTITY_DISABLED; // some negative value
    }
    viennadata::access<MappingKeyType, bool>(storage, map_key, extract_domain<DomainType>::apply(domain)) = true;
  }

  CellContainer cells(domain);
  for (CellIterator cit = cells.begin(); cit != cells.end(); ++cit)
  {
    if (boundary_accessor(*cit))  // boundary cell
    {
      //std::cout << "boundary cell" << std::endl;
      cell_mapping_accessor(*cit) = viennafvm::DIRICHLET_BOUNDARY; // some negative value
    }
    else if (!disable_quantity_accessor(*cit))
    {
      //std::cout << "interior cell" << std::endl;
      if (cell_mapping_accessor(*cit) < 0) //only assign if no dof assigned yet
      {
        cell_mapping_accessor(*cit) = map_index;
        map_index += pde_system.unknown(pde_index).size();
      }
    }
  }

  viennadata::access<MappingKeyType, long>(storage, map_key, extract_domain<DomainType>::apply(domain)) = map_index;

  return map_index;
}


/** @brief Distributes mapping indices over domain or segment
*
*/
template <typename LinPdeSysT, typename DomainType, typename StorageType>
long create_mapping(LinPdeSysT & pde_system,
                    DomainType const & domain,
                    StorageType & storage)
{
  typedef typename viennagrid::result_of::cell<DomainType>::type CellTag;

  typedef typename viennagrid::result_of::point<DomainType>::type                  PointType;
  typedef typename viennagrid::result_of::element<DomainType, CellTag>::type    CellType;

  typedef typename viennagrid::result_of::const_element_range<DomainType, CellTag>::type   CellContainer;
  typedef typename viennagrid::result_of::iterator<CellContainer>::type                       CellIterator;

  typedef typename LinPdeSysT::mapping_key_type   MappingKeyType;
  typedef typename LinPdeSysT::boundary_key_type  BoundaryKeyType;

  long next_index = 0;

  for (std::size_t pde_index = 0; pde_index < pde_system.size(); ++pde_index)
  {
    next_index = create_mapping(pde_system, pde_index, domain, storage, next_index);
  }

  return next_index;
}


} // end namespace viennafvm

#endif

