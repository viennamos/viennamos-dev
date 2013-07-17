#ifndef VIENNADATA_CONTAINER_MAP_HPP
#define VIENNADATA_CONTAINER_MAP_HPP

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

#include <map>

#include "viennadata/forwards.hpp"
#include "viennadata/meta/result_of.hpp"
#include "viennadata/container_access.hpp"

namespace viennadata
{

  /** @brief the base class for container map */
  class container_map_base
  {
  public:
      virtual ~container_map_base() {}
  };

  /** @brief Class for container map, a container map holds a std::map< key_type container_type >
    *
    *   used for data for different keys
    */
  template<typename KeyType, typename ContainerType, typename AccessTag>
  class container_map : public container_map_base
  {
   public:

    typedef AccessTag                          access_tag;
    typedef KeyType                            key_type;
    typedef ContainerType                      container_type;
    typedef std::map<KeyType, container_type> container_map_type;

    // query the container for a specific key
    container_type       & get(key_type const & key)       { return container_map_[key]; }
    container_type const & get(key_type const & key) const { return container_map_[key]; }

    container_map_type       & get_map()       { return container_map_; }
    container_map_type const & get_map() const { return container_map_; }

   private:
    mutable container_map_type        container_map_;
  };




  /** @brief base class for container map accessing */
  struct base_dynamic_container_map_accessor
  {
    virtual ~base_dynamic_container_map_accessor() {}

    /** @brief Delete all values for a specific element */
    virtual void erase_all(base_dynamic_type_wrapper const & dynamic_element) = 0;

    /** @brief Delete the value for a specific element wit a specific key */
    virtual void erase(base_dynamic_type_wrapper const & dynamic_key,
                       base_dynamic_type_wrapper const & dynamic_element) = 0;

    /** @brief Copy all values for a specific element to another element */
    virtual void copy_all(base_dynamic_type_wrapper const & dynamic_src_element,
                          base_dynamic_type_wrapper const & dynamic_dst_element) = 0;

    /** @brief Copy the value for a specific element wit a specific key to another element */
    virtual void copy(base_dynamic_type_wrapper const & dynamic_key,
                      base_dynamic_type_wrapper const & dynamic_src_element,
                      base_dynamic_type_wrapper const & dynamic_dst_element) = 0;
  };

  /** @brief Dynamic container map, has reference to container_map and knows its type; also know the accessing element_type */
  template<typename ContainerMapType, typename AccessType>
  struct dynamic_container_map_accessor : public base_dynamic_container_map_accessor
  {
    dynamic_container_map_accessor(ContainerMapType & container_map_obj) : container_map_(container_map_obj) {}

    void erase_all(base_dynamic_type_wrapper const & dynamic_element)
    {
      // knows the accessing element type -> static casting the base element type
      dynamic_type_wrapper<AccessType> const & element =
        static_cast< dynamic_type_wrapper<AccessType> const & >(dynamic_element);

      // iterate over all containers and erases the data for value
      for (typename ContainerMapType::container_map_type::iterator it = container_map_.get_map().begin(); it != container_map_.get_map().end(); ++it)
      {
        container_access<typename ContainerMapType::container_type,
                         AccessType,
                         typename ContainerMapType::access_tag>::erase(it->second, element.value);
      }
    }

    void erase(base_dynamic_type_wrapper const & dynamic_key,
               base_dynamic_type_wrapper const & dynamic_element)
    {
      // knows the accessing element type -> static casting the base element type
      dynamic_type_wrapper<AccessType> const & element =
        static_cast< dynamic_type_wrapper<AccessType> const & >(dynamic_element);

      // knows the key type -> static casting the base key type
      dynamic_type_wrapper<typename ContainerMapType::key_type> const & key =
        static_cast< dynamic_type_wrapper<typename ContainerMapType::key_type> const & >(dynamic_key);

      // searching for the key, if found -> erase
      typename ContainerMapType::container_map_type::iterator it = container_map_.get_map().find(key.value);
      if (it != container_map_.get_map().end())
      {
        container_access<typename ContainerMapType::container_type,
                         AccessType,
                         typename ContainerMapType::access_tag>::erase(it->second, element.value);
      }
    }

    void copy_all(base_dynamic_type_wrapper const & dynamic_src_element,
                  base_dynamic_type_wrapper const & dynamic_dst_element)
    {
      // knows the accessing element type -> static casting the base element type
      dynamic_type_wrapper<AccessType> const & src_element =
        static_cast< dynamic_type_wrapper<AccessType> const & >(dynamic_src_element);
      dynamic_type_wrapper<AccessType> const & dst_element =
        static_cast< dynamic_type_wrapper<AccessType> const & >(dynamic_dst_element);

      // iterate over all containers and erases the data for value
      for (typename ContainerMapType::container_map_type::iterator it  = container_map_.get_map().begin();
                                                                     it != container_map_.get_map().end();
                                                                   ++it)
      {
        container_access<typename ContainerMapType::container_type,
                         AccessType,
                         typename ContainerMapType::access_tag>::copy(it->second, src_element.value, dst_element.value);
      }
    }

    void copy(base_dynamic_type_wrapper const & dynamic_key,
              base_dynamic_type_wrapper const & dynamic_src_element,
              base_dynamic_type_wrapper const & dynamic_dst_element)
    {
      // knows the accessing element type -> static casting the base element type
      dynamic_type_wrapper<AccessType> const & src_element =
        static_cast< dynamic_type_wrapper<AccessType> const & >(dynamic_src_element);
      dynamic_type_wrapper<AccessType> const & dst_element =
        static_cast< dynamic_type_wrapper<AccessType> const & >(dynamic_dst_element);

      // knows the key type -> static casting the base key type
      dynamic_type_wrapper<typename ContainerMapType::key_type> const & key =
        static_cast< dynamic_type_wrapper<typename ContainerMapType::key_type> const & >(dynamic_key);

      // searching for the key, if found -> erase
      typename ContainerMapType::container_map_type::iterator it = container_map_.get_map().find(key.value);
      if (it != container_map_.get_map().end())
      {
        container_access<typename ContainerMapType::container_type,
                         AccessType,
                         typename ContainerMapType::access_tag>::copy(it->second, src_element.value, dst_element.value);
      }
    }

    ContainerMapType & container_map_;
  };



  /** @brief Accessors are used to wrap containers (container is hold by reference)
    *
    * when possible, use accessors instead of direct access to storage like viennadata::access
    */
  template<typename ContainerType, typename AccessType, typename AccessTag>
  class container_accessor
  {
   public:
    typedef ContainerType                                             container_type;
    typedef typename result_of::value_type<container_type>::type      value_type;
    typedef AccessType                                               access_type;
    
    typedef typename container_access<container_type, AccessType, AccessTag>::reference       reference;
    typedef typename container_access<container_type, AccessType, AccessTag>::const_reference const_reference;

    typedef typename container_access<container_type, AccessType, AccessTag>::pointer         pointer;
    typedef typename container_access<container_type, AccessType, AccessTag>::const_pointer   const_pointer;

    
    container_accessor() : container_(NULL) {}
    container_accessor( container_type & container_obj ) : container_(&container_obj) {}

    template<typename StorageType, typename KeyType>
    container_accessor( storage_container_accessor_proxy<StorageType, KeyType> proxy )
    {
        *this = container_accessor( proxy.storage().template get_container<KeyType, value_type, access_type>(proxy.key()) );
    }
    
    bool is_valid() const { return container_ != NULL; }

    pointer find(AccessType const & element)
    {
      return container_access<container_type, AccessType, AccessTag>::find(*container_, element);
    }

    const_pointer find(AccessType const & element) const
    {
      return container_access<container_type, AccessType, AccessTag>::find(*container_, element);
    }

    reference access_unchecked(AccessType const & element)
    {
      return container_access<container_type, AccessType, AccessTag>::lookup_unchecked(*container_, element);
    }

    const_reference access_unchecked(AccessType const & element) const
    {
      return container_access<container_type, AccessType, AccessTag>::lookup_unchecked(*container_, element);
    }
    
    reference access(AccessType const & element)
    {
      return container_access<container_type, AccessType, AccessTag>::lookup(*container_, element);
    }

    const_reference access(AccessType const & element) const
    {
      return container_access<container_type, AccessType, AccessTag>::lookup(*container_, element);
    }

    reference       operator()(AccessType const & element)       { return access(element); }
    const_reference operator()(AccessType const & element) const { return access(element); }


    void erase(AccessType const & element)
    {
      container_access<container_type, AccessType, AccessTag>::erase(*container_, element);
    }
    
    void clear()
    {
      container_access<container_type, AccessType, AccessTag>::clear(*container_);
    }
    
    void resize( std::size_t size )
    {
      container_access<container_type, AccessType, AccessTag>::resize(*container_, size);
    }

   private:
    container_type * container_;
  };


} // namespace viennadata
#endif
