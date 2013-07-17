#ifndef VIENNADATA_API_HPP
#define VIENNADATA_API_HPP

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

#include <string>

#include "viennadata/forwards.hpp"
#include "viennadata/storage.hpp"


namespace viennadata
{

  /** @brief access function, used storage::lookup */
  template<typename KeyType, typename ValueType, typename StorageType, typename ElementType>
  ValueType & access(StorageType & storage_obj, KeyType const & key, ElementType const & element)
  {
    return storage_obj.template lookup<KeyType, ValueType, ElementType>(key, element);
  }

  /** @brief access function, used storage::lookup; const version */
  template<typename KeyType, typename ValueType, typename StorageType, typename ElementType>
  ValueType const & access(StorageType const & storage_obj, KeyType const & key, ElementType const & element)
  {
    return storage_obj.template lookup<KeyType, ValueType, ElementType>(key, element);
  }



  namespace detail
  {

    /** @brief copy helper struct; default implementation: copying one single value of element with key to another element */
    template<typename KeyType, typename ValueType>
    struct copy_helper
    {
      template<typename StorageType, typename ElementType>
      static void copy(StorageType & storage_obj, KeyType const & key, ElementType const & src_element, ElementType const & dst_element)
      {
        storage_obj.template copy<KeyType, ValueType, ElementType>(key, src_element, dst_element);
      }
    };

    /** @brief copy helper struct; specialization: copying all data values of all value_types of element with specific key to another element */
    template<typename KeyType>
    struct copy_helper<KeyType, viennadata::all>
    {
      template<typename StorageType, typename ElementType>
      static void copy(StorageType & storage_obj, ElementType const & src_element, ElementType const & dst_element)
      {
        storage_obj.template copy_all_data_from_element_with_key<KeyType>(src_element, dst_element);
      }

      template<typename StorageType, typename ElementType>
      static void copy(StorageType & storage_obj, KeyType const & key, ElementType const & src_element, ElementType const & dst_element)
      {
        storage_obj.copy_all_data_from_element_with_key(key, src_element, dst_element);
      }
    };

    /** @brief copy helper struct; specialization: copying all data values of element for all keys to another element */
    template<typename ValueType>
    struct copy_helper<viennadata::all, ValueType>
    {
      template<typename StorageType, typename ElementType>
      static void copy(StorageType & storage_obj, ElementType const & src_element, ElementType const & dst_element)
      {
        storage_obj.template copy_all_data_from_element_with_value_type<ValueType>(src_element, dst_element);
      }
    };

    /** @brief copy helper struct; specialization: copying all data values of element to another element */
    template<>
    struct copy_helper<viennadata::all, viennadata::all>
    {
      template<typename StorageType, typename ElementType>
      static void copy(StorageType & storage_obj, ElementType const & src_element, ElementType const & dst_element)
      {
        storage_obj.copy_all_data_from_element(src_element, dst_element);
      }
    };

  } // namespace detail


  /** @brief copy function with specific key, uses copy_helper */
  template<typename KeyType, typename ValueType, typename StorageType, typename ElementType>
  void copy(StorageType & storage_obj, KeyType const & key, ElementType const & src_element, ElementType const & dst_element)
  {
    detail::copy_helper<KeyType, ValueType>::copy(storage_obj, key, src_element, dst_element);
  }

  /** @brief copy function without key, uses copy_helper */
  template<typename KeyType, typename ValueType, typename StorageType, typename ElementType>
  void copy(StorageType & storage_obj, ElementType const & src_element, ElementType const & dst_element)
  {
    detail::copy_helper<KeyType, ValueType>::copy(storage_obj, src_element, dst_element);
  }




  namespace detail
  {
    /** @brief erase helper struct; default implementation: erasing one single value of element with key */
    template<typename KeyType, typename ValueType>
    struct erase_helper
    {
      template<typename StorageType, typename ElementType>
      static void erase(StorageType & storage_obj, KeyType const & key, ElementType const & element)
      {
        storage_obj.template erase<KeyType, ValueType, ElementType>(key, element);
      }
    };

    /** @brief erase helper struct; specialization: erasing all values of all value_types of element with specific key */
    template<typename KeyType>
    struct erase_helper<KeyType, viennadata::all>
    {
      template<typename StorageType, typename ElementType>
      static void erase(StorageType & storage_obj, ElementType & element)
      {
        storage_obj.template erase_all_data_from_element_with_key<KeyType>(element);
      }

      template<typename StorageType, typename ElementType>
      static void erase(StorageType & storage_obj, KeyType const & key, ElementType const & element)
      {
        storage_obj.erase_all_data_from_element_with_key(key, element);
      }
    };

    /** @brief  erase helper struct; specialization: erasing all values of element for all keys */
    template<typename ValueType>
    struct erase_helper<viennadata::all, ValueType>
    {
      template<typename StorageType, typename ElementType>
      static void erase(StorageType & storage_obj, ElementType const & element)
      {
        storage_obj.template erase_all_data_from_element_with_value_type<ValueType>(element);
      }
    };

    /** @brief erase helper struct; specialization: erasing all values of element */
    template<>
    struct erase_helper<viennadata::all, viennadata::all>
    {
      template<typename StorageType, typename ElementType>
      static void erase(StorageType & storage_obj, ElementType const & element)
      {
        storage_obj.erase_all_data_from_element(element);
      }
    };

  } // namespace detail

  /** @brief erase function with specific key, uses erase_helper */
  template<typename KeyType, typename ValueType, typename StorageType, typename ElementType>
  void erase(StorageType & storage_obj, KeyType const & key, ElementType const & element)
  {
    detail::erase_helper<KeyType, ValueType>::erase(storage_obj, key, element);
  }

  /** @brief erase function without key, uses erase_helper */
  template<typename KeyType, typename ValueType, typename StorageType, typename ElementType>
  void erase(StorageType & storage_obj, ElementType const & element)
  {
    detail::erase_helper<KeyType, ValueType>::erase(storage_obj, element);
  }


  /** @brief move function with specific key, uses copy and erase */
  template<typename KeyType, typename ValueType, typename StorageType, typename ElementType>
  void move(StorageType & storage_obj, KeyType const & key, ElementType const & src_element, ElementType const & dst_element)
  {
    copy<KeyType, ValueType>(storage_obj, key, src_element, dst_element);
    erase<KeyType, ValueType>(storage_obj, key, src_element);
  }

  /** @brief copy function without key, uses copy_helper */
  template<typename KeyType, typename ValueType, typename StorageType, typename ElementType>
  void move(StorageType & storage_obj, ElementType const & src_element, ElementType const & dst_element)
  {
    copy<KeyType, ValueType>(storage_obj, src_element, dst_element);
    erase<KeyType, ValueType>(storage_obj, src_element);
  }




  /** @brief find function, used storage::find */
  template<typename KeyType, typename ValueType, typename StorageType, typename ElementType>
  ValueType * find(StorageType & storage_obj, KeyType const & key, ElementType const & element)
  {
    return storage_obj.template find<KeyType, ValueType, ElementType>(key, element);
  }

  /** @brief find function, used storage::find; const version */
  template<typename KeyType, typename ValueType, typename StorageType, typename ElementType>
  ValueType const * find(StorageType const & storage_obj, KeyType const & key, ElementType const & element)
  {
    return storage_obj.template find<KeyType, ValueType, ElementType>(key, element);
  }


  /** @brief returns the container for a combination of <key_type, value_type, element_type/tag> */
  template<typename KeyType, typename ValueType, typename ElementTypeOrTag, typename StorageType>
  typename result_of::container_type<StorageType, KeyType, ValueType, ElementTypeOrTag>::type &
  container(StorageType & storage_obj, KeyType const & key)
  {
    return storage_obj.template get_container<KeyType, ValueType, ElementTypeOrTag>(key);
  }

  /** @brief returns the container for a combination of <key_type, value_type, element_type/tag>; const version */
  template<typename KeyType, typename ValueType, typename ElementTypeOrTag, typename StorageType>
  typename result_of::container_type<StorageType, KeyType, ValueType, ElementTypeOrTag>::type const &
  container(StorageType const & storage_obj, KeyType const & key)
  {
    return storage_obj.template get_container<KeyType, ValueType, ElementTypeOrTag>(key);
  }



  /** @brief reserves a specific amount of entries for the container associated with <key_type, value_type, element_type_or_tag> */
  template<typename KeyType, typename ValueType, typename ElementTypeOrTag, typename StorageType>
  void reserve(StorageType & storage, KeyType const & key, std::size_t size)
  {
    container<KeyType, ValueType, ElementTypeOrTag>(storage, key).reserve(size);
  }




  /** @brief returns an accessor for a combination of <key_type, value_type, element_type/tag> */
  template<typename AccessType, typename AccessTag, typename ContainerType>
  container_accessor<ContainerType, AccessType, AccessTag> accessor(ContainerType & container)
  {
      return container;
  }

  /** @brief returns an accessor for a combination of <key_type, value_type, element_type/tag> */
  template<typename AccessType, typename AccessTag, typename ContainerType>
  container_accessor<const ContainerType, AccessType, AccessTag> accessor(ContainerType const & container)
  {
      return container;
  }




  /** @brief returns an accessor for a combination of <key_type, value_type, element_type/tag> */
  template<typename KeyType, typename ValueType, typename ElementTypeOrTag, typename ContainerConfig, typename RuntimeTypeInformation>
  typename result_of::accessor<storage<ContainerConfig, RuntimeTypeInformation>, KeyType, ValueType, ElementTypeOrTag>::type
      accessor(storage<ContainerConfig, RuntimeTypeInformation> & storage_obj, KeyType const & key)
  {
      return container<KeyType, ValueType, ElementTypeOrTag>(storage_obj, key);
  }

  /** @brief returns an accessor for a combination of <key_type, value_type, element_type/tag> */
  template<typename KeyType, typename ValueType, typename ElementTypeOrTag, typename ContainerConfig, typename RuntimeTypeInformation>
  typename result_of::accessor<const storage<ContainerConfig, RuntimeTypeInformation>, KeyType, ValueType, ElementTypeOrTag>::type
      accessor(storage<ContainerConfig, RuntimeTypeInformation> const & storage_obj, KeyType const & key )
  {
      return container<KeyType, ValueType, ElementTypeOrTag>(storage_obj, key);
  }



  /** @brief returns an accessor for a combination of <key_type, value_type, element_type/tag> */
  template<typename ContainerConfig, typename RuntimeTypeInformation, typename KeyType>
  storage_container_accessor_proxy< storage<ContainerConfig, RuntimeTypeInformation>, KeyType >
      acc(storage<ContainerConfig, RuntimeTypeInformation> & storage_obj, KeyType const & key)
  {
      return storage_container_accessor_proxy< storage<ContainerConfig, RuntimeTypeInformation>, KeyType >(storage_obj, key);
  }

  /** @brief returns an accessor for a combination of <key_type, value_type, element_type/tag> */
  template<typename ContainerConfig, typename RuntimeTypeInformation, typename KeyType>
  storage_container_accessor_proxy< const storage<ContainerConfig, RuntimeTypeInformation>, KeyType >
      acc(storage<ContainerConfig, RuntimeTypeInformation> const & storage_obj, KeyType const & key )
  {
      return storage_container_accessor_proxy< const storage<ContainerConfig, RuntimeTypeInformation>, KeyType >(storage_obj, key);
  }



} // namespace viennadata

#endif

