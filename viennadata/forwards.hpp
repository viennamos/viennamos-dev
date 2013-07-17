#ifndef VIENNADATA_FORWARDS_HPP
#define VIENNADATA_FORWARDS_HPP

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
#include <typeinfo>

#include "viennadata/meta/typemap.hpp"
#include "viennadata/meta/utils.hpp"

namespace viennadata
{
  /** @brief viennadata::all for erase, move and copy */
  struct all;

  /** @brief A tag which indicates that object id is used to access the data */
  struct id_access_tag {};
  /** @brief A tag which indicates that object pointer is used to access the data */
  struct pointer_access_tag {};

  /** @brief Associative container key type for id, constructible from ElementType */
  template<typename ElementType>
  struct id_key
  {
      typedef typename ElementType::id_type id_type;

      id_key(ElementType const & element) : id(element.id()) {}

      id_type id;
  };

  /** @brief compare operator for id_key (less comparable) */
  template<typename ElementType>
  bool operator<(id_key<ElementType> const & lhs, id_key<ElementType> const & rhs)
  {
      return lhs.id < rhs.id;
  }


  /** @brief Associative container key type for pointer, constructable from element_type */
  template<typename ElementType>
  struct pointer_key
  {
      pointer_key(ElementType const & element) : ptr(&element) {}

      ElementType const * ptr;
  };

  /** @brief Compare operator for pointer_key (less comparable) */
  template<typename ElementType>
  bool operator<(pointer_key<ElementType> const & lhs, pointer_key<ElementType> const & rhs)
  {
      return lhs.ptr < rhs.ptr;
  }




  /** @brief Default tag for configuration */
  struct default_tag {};

  /** @brief Indicates the use of std::vector for data container (dense storage) */
  struct std_vector_tag {};

  /** @brief Indicates the use of std::deque for data container (dense storage) */
  struct std_deque_tag {};

  /** Indicates the use of std::map for data container (sparse storage) */
  struct std_map_tag {};

  /** @brief runtime information classes are used to map the compile-time type to a runt-time key_type */
  struct typeid_string_runtime_information
  {
    typedef std::string runtime_key_type;

    template<typename type>
    runtime_key_type key() const
    {
      return typeid(type).name();
    }
  };



  /** @brief Default container config; using std::map and pointer access */
  typedef viennameta::make_typemap<
      default_tag,
      viennameta::static_pair<std_map_tag, pointer_access_tag>
  >::type default_container_config;

  /** @brief Default config wrapping the default configuration typemap default_container_config */
  struct wrapped_default_container_config
  {
    typedef default_container_config   type;
  };

  /** @brief Base class for dynamic type wrapper */
  struct base_dynamic_type_wrapper
  {
      virtual ~base_dynamic_type_wrapper() {}
  };

  /** @brief Dynamic type wrapper, used for dynamic type searching (e.g. which types are stored on an element), knows its element type */
  template<typename ElementType>
  struct dynamic_type_wrapper : public base_dynamic_type_wrapper
  {
      dynamic_type_wrapper(ElementType const & val) : value(val) {}
      ElementType const & value;
  };

  template<typename ContainerConfig = wrapped_default_container_config, typename RuntimeTypeInformation = typeid_string_runtime_information>
  class storage;


  template<typename StorageType, typename KeyType>
  class storage_container_accessor_proxy
  {
  public:
    storage_container_accessor_proxy(StorageType & s, KeyType const & k) : storage_(s), key_(k) {}

    StorageType & storage() { return storage_; }
    KeyType const & key() const { return key_; }

  private:
    StorageType & storage_;
    KeyType const & key_;
  };

  template<typename ContainerType, typename ElementType, typename AccessTag>
  class container_accessor;
}

#endif // FORWARDS_HPP
