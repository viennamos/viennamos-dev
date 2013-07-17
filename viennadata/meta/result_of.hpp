#ifndef VIENNADATA_META_RESULT_OF_HPP
#define VIENNADATA_META_RESULT_OF_HPP

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

#include <vector>
#include <deque>
#include <map>

#include "viennadata/forwards.hpp"

namespace viennadata
{
  namespace result_of
  {
    /** @brief Query the tag of an element, viennadata support type and type tags for configuration
      *
      * default = type identity
      */
    template<typename ElementTypeOrTag>
    struct element_tag
    {
      typedef ElementTypeOrTag         type;
    };

    // query the offset and offset type for an id, used for random access container
    // default is identity (e.g. for int ids)
    template<typename IDType>
    struct offset
    {
        typedef IDType      type;

        static type get(type const & id) { return id; }
    };

    // specialization for smart_id_t, used in ViennaGrid
    /*template<typename value_type, typename base_id_type>
    struct offset< viennagrid::storage::smart_id_t<value_type, base_id_type> >
    {
        typedef viennagrid::storage::smart_id_t<value_type, base_id_type> id_type;
        typedef base_id_type type;

        static type get(type const & id) { return id.get(); }
    };*/


    /** @brief Query the access and access_type for an element based on an access tag */
    template<typename ElementType, typename AccessTag>
    struct access_type;

    /** @brief specialization for id access */
    template<typename ElementType>
    struct access_type<ElementType, id_access_tag>
    {
      typedef id_key<ElementType>      type;
    };

    /** @brief specialization for pointer access */
    template<typename ElementType>
    struct access_type<ElementType, pointer_access_tag>
    {
      typedef pointer_key<ElementType> type;
    };



    /** @brief Query the value type of a container; default useing container_type::value_type */
    template<typename ContainerType>
    struct value_type
    {
      typedef typename ContainerType::value_type       type;
    };

    /** @brief Specialization for std::map */
    template<typename KeyType, typename ValueType, typename Compare, typename Alloc>
    struct value_type< std::map<KeyType, ValueType, Compare, Alloc> >
    {
      typedef ValueType               type;
    };

    template<typename KeyType, typename ValueType, typename Compare, typename Alloc>
    struct value_type< const std::map<KeyType, ValueType, Compare, Alloc> >
    {
      typedef ValueType               type;
    };



    /** @brief Constructs a container type based on an element type, value type a container tag and an access tag */
    template<typename ElementType, typename ValueType, typename ContainerTag, typename AccessTag>
    struct container;

    /** @brief Specialization for std::deque: only id access is supported, element_type is ignored */
    template<typename ElementType, typename ValueType>
    struct container<ElementType, ValueType, std_deque_tag, id_access_tag>
    {
      typedef std::deque<ValueType>    type;
    };

    /** @brief Specialization for std::vector: only id access is supported, element_type is ignored */
    template<typename ElementType, typename ValueType>
    struct container<ElementType, ValueType, std_vector_tag, id_access_tag>
    {
      typedef std::vector<ValueType>   type;
    };

    /** @brief Specialization for std::map, access type is created based on element type and access tag */
    template<typename ElementType, typename ValueType, typename AccessTag>
    struct container<ElementType, ValueType, std_map_tag, AccessTag>
    {
      typedef typename result_of::access_type<ElementType, AccessTag>::type      access_type;
      typedef std::map< access_type, ValueType>                                  type;
    };



    /** @brief Query the access tag for a combination of <element_type/tag, key_type, value_type> based on a configuration */
    template<typename container_config, typename pair_type>
    struct access_tag_from_config;

    /** @brief Default implementation (specialization for typemaps) */
    template<typename ContainerConfig, typename ElementTypeOrTag, typename KeyType, typename ValueType>
    struct access_tag_from_config<
            ContainerConfig,
            viennameta::static_pair<
                ElementTypeOrTag,
                viennameta::static_pair<KeyType, ValueType>
            >
        >
    {
      typedef typename result_of::element_tag<ElementTypeOrTag>::type                                 element_tag; // ensure we use the tag of the element
      // constructing the compile-time key for querying in the config
      typedef viennameta::static_pair<element_tag, viennameta::static_pair<KeyType, ValueType> >      static_key_type;
      // the search result for the compile-time key
      typedef typename viennameta::typemap::result_of::find<ContainerConfig, static_key_type>::type   search_result;
      // the default configuration
      typedef typename viennameta::typemap::result_of::find<ContainerConfig, default_tag>::type       default_container;

      // if the combination of <element_type/tag, key_type, value_type> was not found use default
      typedef typename viennameta::IF<
          !viennameta::EQUAL<search_result, viennameta::not_found>::value,
          search_result,
          default_container
      >::type container_tag_pair;

      // first ::second -> search result value-type
      // second ::second -> access_tag
      typedef typename container_tag_pair::second::second           type;
    };


    /** @brief Query the access tag for a combination of <element_type/tag, key_type, value_type> based on a configuration */
    template<typename PairType, typename ContainerConfig>
    struct container_from_config;

    /** @brief Default implementation (specialization for typemaps) */
    template<typename ContainerConfig, typename ElementTypeOrTag, typename KeyType, typename ValueType>
    struct container_from_config<
            ContainerConfig,
            viennameta::static_pair<
                ElementTypeOrTag,
                viennameta::static_pair<KeyType, ValueType>
            >
        >
    {
      typedef typename result_of::element_tag<ElementTypeOrTag>::type                                         element_tag; // ensure we use the tag of the element
      // constructing the compile-time key for querying in the config
      typedef viennameta::static_pair<ElementTypeOrTag, viennameta::static_pair<KeyType, ValueType> >         static_key_type;
      // the search result for the compile-time key
      typedef typename viennameta::typemap::result_of::find<ContainerConfig, static_key_type>::type           search_result;
      // the default configuration
      typedef typename viennameta::typemap::result_of::find<ContainerConfig, default_tag>::type               default_container;

      // if the combination of <element_type/tag, key_type, value_type> was not found use default
      typedef typename viennameta::IF<
          !viennameta::EQUAL<search_result, viennameta::not_found>::value,
          search_result,
          default_container
      >::type container_tag_pair;

      // first container_tag_pair::second::first -> container tag
      // first container_tag_pair::second::first -> access tag
      typedef typename container<
              ElementTypeOrTag,
              ValueType,
              typename container_tag_pair::second::first,
              typename container_tag_pair::second::second
          >::type                                                   type;
    };

    // query the container_type within a storage for a specific combination of  <key_type, value_type, element_type/tag>
    template<typename StorageType, typename KeyType, typename ValueType, typename ElementTypeOrTag>
    struct container_type;

    template<typename WrappedContainerConfig, typename RuntimeTypeInformation, typename KeyType, typename ValueType, typename ElementTypeOrTag>
    struct container_type< storage<WrappedContainerConfig, RuntimeTypeInformation>, KeyType, ValueType, ElementTypeOrTag>
    {
      typedef typename WrappedContainerConfig::type                                                   ContainerConfig;
      typedef typename element_tag<ElementTypeOrTag>::type                                            element_tag;
      typedef viennameta::static_pair<element_tag, viennameta::static_pair<KeyType, ValueType> >      static_key_type;

      typedef typename result_of::container_from_config<
              ContainerConfig,
              static_key_type
          >::type                                          type;
    };


    // query an accessor using a storage type and a combination of <key_type, value_type, element_type>
    template<typename StorageType, typename KeyType, typename ValueType, typename ElementType>
    struct accessor {};

    template<typename WrappedContainerConfig, typename RuntimeTypeInformation, typename KeyType, typename ValueType, typename ElementType>
    struct accessor< storage<WrappedContainerConfig, RuntimeTypeInformation>, KeyType, ValueType, ElementType >
    {
      typedef typename WrappedContainerConfig::type                                                  ContainerConfig;
      typedef viennameta::static_pair< ElementType, viennameta::static_pair<KeyType, ValueType> >    to_query;
      typedef typename container_from_config<ContainerConfig, to_query>::type                        container_type;
      typedef typename access_tag_from_config<ContainerConfig, to_query>::type                       access_tag;
      typedef container_accessor<container_type, ElementType, access_tag>                            type;
    };

    template<typename WrappedContainerConfig, typename RuntimeTypeInformation, typename KeyType, typename ValueType, typename ElementType>
    struct accessor< const storage<WrappedContainerConfig, RuntimeTypeInformation>, KeyType, ValueType, ElementType >
    {
      typedef typename WrappedContainerConfig::type                                                  ContainerConfig;
      typedef viennameta::static_pair< ElementType, viennameta::static_pair<KeyType, ValueType> >    to_query;
      typedef typename container_from_config<ContainerConfig, to_query>::type                        container_type;
      typedef typename access_tag_from_config<ContainerConfig, to_query>::type                       access_tag;
      typedef container_accessor<const container_type, ElementType, access_tag>                            type;
    };

  } // namespace result_of
} // namespace viennadata

#endif
