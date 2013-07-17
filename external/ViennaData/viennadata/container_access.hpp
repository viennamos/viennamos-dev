#ifndef VIENNADATA_CONTAINER_ACCESS_HPP
#define VIENNADATA_CONTAINER_ACCESS_HPP

/* =======================================================================
   Copyright (c) 2011-2013, Institute for Microelectronics,
                            Institute for Analysis and Scientific Computing,
                            TU Wien.

                            -----------------
                     ViennaData - The Vienna Data Library
                            -----------------

   Authors:      Florian Rudolf                    rudolf@iue.tuwien.ac.at
                 Karl Rupp                           rupp@iue.tuwien.ac.at

   (A list of additional contributors can be found in the PDF manual)

   License:      MIT (X11), see file LICENSE in the base directory
======================================================================= */

#include <cassert>
#include <map>

#include "viennadata/forwards.hpp"
#include "viennadata/meta/result_of.hpp"

namespace viennadata
{
  /** @brief Class that manages container access, depends on container_type, access_type and access_tag */
  template<typename ContainerType, typename AccessType, typename AccessTag>
  struct container_access;

  /** @brief Default implementation: random access container using offset */
  template<typename ContainerType, typename AccessType, typename AccessTag>
  struct container_access
  {
    typedef ContainerType                            container_type;
    typedef AccessType                              access_type;
    typedef typename container_type::value_type      value_type;
    
    typedef typename container_type::reference         reference;
    typedef typename container_type::const_reference   const_reference;
    
    typedef typename container_type::pointer           pointer;
    typedef typename container_type::const_pointer     const_pointer;

    typedef typename result_of::offset< typename access_type::id_type >::type     offset_type;
    
    
    static pointer find(container_type & container, access_type const & element)
    {
      offset_type offset = result_of::offset< typename access_type::id_type >::get(element.id());
      return (static_cast<offset_type>(container.size()) > offset) ? (&container[offset]) : NULL; // return NULL if not found
    }

    static const_pointer find(container_type const & container, access_type const & element)
    {
      offset_type offset = result_of::offset< typename access_type::id_type >::get(element.id());
      return (static_cast<offset_type>(container.size()) > offset) ? (&container[offset]) : NULL; // return NULL if not found
    }
    
    static reference lookup_unchecked(container_type & container, access_type const & element) // no offset checking
    {
      offset_type offset = result_of::offset< typename access_type::id_type >::get(element.id());
      return container[offset];
    }

    static const_reference lookup_unchecked(container_type const & container, access_type const & element)
    {
      offset_type offset = result_of::offset< typename access_type::id_type >::get(element.id());
      
      assert( static_cast<offset_type>(container.size()) > offset ); // no release-runtime check for accessing elements outside container
      return container[offset];
    }

    static reference lookup(container_type & container, access_type const & element)
    {
      offset_type offset = result_of::offset< typename access_type::id_type >::get(element.id());

      if ( static_cast<offset_type>(container.size()) <= offset) container.resize(offset+1); // ensure that container is big enough
      return container[offset];
    }

    static const_reference lookup(container_type const & container, access_type const & element)
    {
      return lookup_unchecked(container, element); // using unchecked lookup
    }

    static void copy(container_type & container, access_type const & src_element, access_type const & dst_element)
    {
      lookup(container, dst_element) = lookup(container, src_element);
    }
    
    static void erase(container_type & container, access_type const & element)
    {
      offset_type offset = result_of::offset< typename access_type::id_type >::get(element.id());

      if (offset+1 == static_cast<offset_type>(container.size())) // container is shrinked only when deleting data for last element
          container.resize(container.size()-1);
    }
    
    static void clear(container_type & container)
    {
      container.clear();
    }
    
    static void resize(container_type & container, std::size_t size)
    {
      container.resize( size );
    }
  };

  // const container specialization
  template<typename ContainerType, typename AccessType, typename AccessTag>
  struct container_access<const ContainerType, AccessType, AccessTag>
  {
    typedef ContainerType                            container_type;
    typedef AccessType                              access_type;
    typedef typename container_type::value_type      value_type;
    
    typedef typename container_type::const_reference   reference;
    typedef typename container_type::const_reference   const_reference;
    
    typedef typename container_type::const_pointer     pointer;
    typedef typename container_type::const_pointer     const_pointer;


    static const_pointer find(container_type const & container, access_type const & element)
    {
      typedef typename result_of::offset< typename access_type::id_type >::type      offset_type;

      offset_type offset = result_of::offset< typename access_type::id_type >::get(element.id());

      return (container.size() > offset) ? (&container[offset]) : NULL; // return NULL if not found
    }

    static const_reference lookup_unchecked(container_type const & container, access_type const & element)
    {
      typedef typename result_of::offset< typename access_type::id_type >::type      offset_type;
      
      offset_type offset = result_of::offset< typename access_type::id_type >::get(element.id());
      
      assert( static_cast<offset_type>(container.size()) > offset ); // no release-runtime check for accessing elements outside container
      return container[offset];
    }

    static const_reference lookup(container_type const & container, access_type const & element)
    {
      return lookup_unchecked(container, element); // using unchecked lookup
    }

    static void copy(container_type & container, access_type const & src_element, access_type const & dst_element);
    static void erase(container_type & container, access_type const & element);
    static void clear(container_type & container);
    static void resize(container_type & container, std::size_t size);
  };
  
  


  // specialization for std::map
  template<typename KeyType, typename ValueType , typename Compare, typename Alloc, typename AccessType, typename AccessTag>
  struct container_access<std::map<KeyType, ValueType, Compare, Alloc>, AccessType, AccessTag>
  {
    typedef KeyType                                             key_type;
    typedef std::map<KeyType, ValueType, Compare, Alloc>        container_type;
    typedef AccessType                                         access_type;
    typedef ValueType                                           value_type;
    
    typedef value_type &         reference;
    typedef value_type const &   const_reference;
    
    typedef value_type *           pointer;
    typedef value_type const *     const_pointer;

    static pointer find(container_type & container, access_type const & element)
    {
      typename result_of::access_type<access_type, AccessTag>::type access(element);

      typename container_type::iterator it = container.find(access);
      return (it != container.end()) ? &it->second : NULL; // return NULL if not found
    }

    static const_pointer find(container_type const & container, access_type const & element)
    {
      typename result_of::access_type<access_type, AccessTag>::type access(element);

      typename container_type::const_iterator it = container.find(access);
      return (it != container.end()) ? &it->second : NULL; // return NULL if not found
    }

    static reference lookup_unchecked(container_type & container, access_type const & element)
    {
      typename result_of::access_type<access_type, AccessTag>::type access(element);
      return container[access];
    }

    static const_reference lookup_unchecked(container_type const & container, access_type const & element)
    {
      typename result_of::access_type<access_type, AccessTag>::type access(element);
      typename container_type::const_iterator it = container.find(access);

      assert(it != container.end()); // no release-runtime check for accessing elements outside container

      return it->second;
    }
    
    static reference lookup(container_type & container, access_type const & element)
    {
      return lookup_unchecked(container, element); // using unchecked lookup
    }

    static const_reference lookup(container_type const & container, access_type const & element)
    {
      return lookup_unchecked(container, element); // using unchecked lookup
    }

    static void copy(container_type & container, access_type const & src_element, access_type const & dst_element)
    {
      lookup(container, dst_element) = lookup(container, src_element);
    }
    
    static void erase(container_type & container, access_type const & element)
    {
      typename result_of::access_type<access_type, AccessTag>::type access(element);

      typename container_type::iterator it = container.find(access);
      if (it != container.end())
        container.erase(it);
    }
    
    static void clear(container_type & container)
    {
      container.clear();
    }
    
    static void resize(container_type &, std::size_t) {} // not supported
  };
  
  // const map specialization
  template<typename KeyType, typename ValueType , typename Compare, typename Alloc, typename AccessType, typename AccessTag>
  struct container_access<const std::map<KeyType, ValueType, Compare, Alloc>, AccessType, AccessTag>
  {
    typedef KeyType                                             key_type;
    typedef std::map<KeyType, ValueType, Compare, Alloc>        container_type;
    typedef AccessType                                         access_type;
    typedef ValueType                                           value_type;
    
    typedef value_type const &         reference;
    typedef value_type const &   const_reference;
    
    typedef value_type const *           pointer;
    typedef value_type const *     const_pointer;

    static const_pointer find(container_type const & container, access_type const & element)
    {
      typename result_of::access_type<access_type, AccessTag>::type access(element);

      typename container_type::const_iterator it = container.find(access);
      return (it != container.end()) ? &it->second : NULL; // return NULL if not found
    }

    static const_reference lookup_unchecked(container_type const & container, access_type const & element)
    {
      typename result_of::access_type<access_type, AccessTag>::type access(element);
      typename container_type::const_iterator it = container.find(access);

      assert(it != container.end()); // no release-runtime check for accessing elements outside container

      return it->second;
    }

    static const_reference lookup(container_type const & container, access_type const & element)
    {
      return lookup_unchecked(container, element); // using unchecked lookup
    }

    static void copy(container_type & container, access_type const & src_element, access_type const & dst_element);
    static void erase(container_type & container, access_type const & element);    
    static void clear(container_type & container);
    static void resize(container_type & container, std::size_t size);
  };



} // namespace viennadata
#endif
