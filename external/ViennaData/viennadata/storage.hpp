#ifndef VIENNADATA_STORAGE_HPP
#define VIENNADATA_STORAGE_HPP

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
#include "viennadata/container_map.hpp"

namespace viennadata
{

  /** @brief The central storage class. Can be configured with container configuration and runtime type information
    *
    * Holds a std::map<runtime_key_type, container_map_base> which maps the combination <element_type/tag, key_type, value_type> to the container_map
    * Additionally holds three maps for <element_type>, <element_type, key_type>, <element_type, value_type> for supporting operation on more than one data value
    * (e.g. erase<all, all>(element))
    *
    * @tparam cont
    */
  template<typename WrappedContainerConfig /* see forwards.hpp for default argument */,
           typename RuntimeTypeInformation /* see forwards.hpp for default argument */>
  class storage
  {
   public:

    typedef RuntimeTypeInformation                               runtime_type_information;
    typedef typename WrappedContainerConfig::type                container_config;

    typedef typename RuntimeTypeInformation::runtime_key_type    runtime_key_type;
    typedef std::map<runtime_key_type, container_map_base*>      container_map_map_type;

    typedef std::map<runtime_key_type, std::vector<base_dynamic_container_map_accessor*> > dynamic_key_to_container_accessor_map;

    storage() {}
    storage(runtime_type_information const & rtti_) : rtti(rtti_) {}

    /** @brief Resets the storage, takes care of dynamically allocated objects */
    void clear()
    {
      for (typename container_map_map_type::iterator it  = container_maps.begin();
                                                     it != container_maps.end();
                                                   ++it)
        delete it->second;

      // only one out of
      //   element_type_to_container_map_accessor_map,
      //   element_and_key_type_to_container_map_accessor_map, or
      //   element_and_value_type_to_container_map_accessor_map
      // has to be deleted
      for (typename dynamic_key_to_container_accessor_map::iterator it  = element_type_to_container_map_accessor_map.begin();
                                                                    it != element_type_to_container_map_accessor_map.end();
                                                                  ++it)
      {
        for (std::vector<base_dynamic_container_map_accessor*>::iterator jt  = it->second.begin();
                                                                         jt != it->second.end();
                                                                       ++jt)
          delete *jt;
      }

      container_maps.clear();

      element_type_to_container_map_accessor_map.clear();
      element_and_key_type_to_container_map_accessor_map.clear();
      element_and_value_type_to_container_map_accessor_map.clear();
    }

    ~storage() { clear(); }


   private:

    /** @brief Base method for querying a container map based on a combination of <key_type, value_type, element_type/tag>
      *
      * Method is const but maps are all mutable
      * if container_map is not available it is created
      * maps for <element_tag>, <element_tag, key_type>, <element_tag, value_type> are updated
      */
    template<typename KeyType, typename ValueType, typename ElementTypeOrTag>
    container_map<
            KeyType,
            typename result_of::container_from_config<
                container_config,
                viennameta::static_pair<typename result_of::element_tag<ElementTypeOrTag>::type,
                                        viennameta::static_pair<KeyType, ValueType> >
            >::type,
            typename result_of::access_tag_from_config<
                container_config,
                viennameta::static_pair<typename result_of::element_tag<ElementTypeOrTag>::type,
                                        viennameta::static_pair<KeyType, ValueType> >
            >::type
        > &
    get_container_map_impl() const
    {
      typedef typename result_of::element_tag<ElementTypeOrTag>::type                                       element_tag;
      typedef viennameta::static_pair<element_tag, viennameta::static_pair<KeyType, ValueType> >            static_key_type;
      typedef typename result_of::container_from_config< container_config, static_key_type >::type          container_type;
      typedef typename result_of::access_tag_from_config< container_config, static_key_type >::type         access_tag;
      typedef container_map<KeyType, container_type, access_tag>                                            container_map_type;

      runtime_key_type runtime_key = rtti.template key< static_key_type >();

      typename container_map_map_type::iterator it = container_maps.find(runtime_key);
      if (it == container_maps.end())
      {
        container_map_type * container_map = new container_map_type();
        container_maps.insert(std::make_pair(runtime_key, static_cast<container_map_base*>(container_map)));

        base_dynamic_container_map_accessor * container_map_accessor =
            new dynamic_container_map_accessor<container_map_type, element_tag>(*container_map);

        element_type_to_container_map_accessor_map[rtti.template key<element_tag>()].
            push_back(container_map_accessor);
        element_and_key_type_to_container_map_accessor_map[rtti.template key< viennameta::static_pair<element_tag, KeyType> >()].
            push_back(container_map_accessor);
        element_and_value_type_to_container_map_accessor_map[rtti.template key< viennameta::static_pair<element_tag, ValueType> >()].
            push_back(container_map_accessor);

        return *container_map;
      }
      else
        return *static_cast<container_map_type*>(it->second);
    }

    /** @brief Query a container by using a key and get_container_map
      *
      * Method is const but maps are all mutable
      */
    template<typename KeyType, typename ValueType, typename ElementTypeOrTag>
    typename result_of::container_from_config<
                container_config,
                viennameta::static_pair<ElementTypeOrTag, viennameta::static_pair<KeyType, ValueType> >
            >::type &
    get_container_impl(KeyType const & key) const
    {
      return get_container_map_impl<KeyType, ValueType, ElementTypeOrTag>().get(key);
    }


   public:

    /** @brief get_container_map public interface, using private implementation */
    template<typename KeyType, typename ValueType, typename ElementTypeOrTag>
    container_map<
            KeyType,
            typename result_of::container_from_config<
                container_config,
                viennameta::static_pair<ElementTypeOrTag, viennameta::static_pair<KeyType, ValueType> >
            >::type,
            typename result_of::access_tag_from_config<
                container_config,
                viennameta::static_pair<ElementTypeOrTag, viennameta::static_pair<KeyType, ValueType> >
            >::type
        > &
    get_container_map()
    {
      return get_container_map_impl<KeyType, ValueType, ElementTypeOrTag>();
    }

    /** @brief get_container_map public interface, using private implementation; const version -> returning const reference */
    template<typename KeyType, typename ValueType, typename ElementTypeOrTag>
    container_map<
            KeyType,
            typename result_of::container_from_config<
                container_config,
                viennameta::static_pair<ElementTypeOrTag, viennameta::static_pair<KeyType, ValueType> >
            >::type,
            typename result_of::access_tag_from_config<
                container_config,
                viennameta::static_pair<ElementTypeOrTag, viennameta::static_pair<KeyType, ValueType> >
            >::type
        > const &
    get_container_map() const
    {
      return get_container_map_impl<KeyType, ValueType, ElementTypeOrTag>();
    }


    /** @brief get_container public interface, using private implementation */
    template<typename KeyType, typename ValueType, typename ElementTypeOrTag>
    typename result_of::container_from_config<
                container_config,
                viennameta::static_pair<ElementTypeOrTag, viennameta::static_pair<KeyType, ValueType> >
            >::type &
    get_container(KeyType const & key)
    {
      return get_container_impl<KeyType, ValueType, ElementTypeOrTag>(key);
    }

    /** @brief get_container public interface, using private implementation; const version -> returning const reference */
    template<typename KeyType, typename ValueType, typename ElementTypeOrTag>
    typename result_of::container_from_config<
                container_config,
                viennameta::static_pair<ElementTypeOrTag, viennameta::static_pair<KeyType, ValueType> >
            >::type const &
            get_container(KeyType const & key) const
    {
      return get_container_impl<KeyType, ValueType, ElementTypeOrTag>(key);
    }



    /** @brief Returns value for <key_type, value_type, element_type> with specific key and element */
    template<typename KeyType, typename ValueType, typename ElementType>
    ValueType & lookup(KeyType const & key, ElementType const & element)
    {
      typedef viennameta::static_pair<ElementType, viennameta::static_pair<KeyType, ValueType> >    static_key_type;
      typedef typename result_of::container_from_config< container_config, static_key_type >::type     container_type;
      typedef typename result_of::access_tag_from_config< container_config, static_key_type >::type    access_tag;
      return container_access<container_type, ElementType, access_tag>::lookup(
          get_container_map<KeyType, ValueType, ElementType>().get(key), element);
    }

    /** @brief Returns value for <key_type, value_type, element_type> with specific key and element; const version */
    template<typename KeyType, typename ValueType, typename ElementType>
    ValueType const & lookup(KeyType const & key, ElementType const & element) const
    {
      typedef viennameta::static_pair<ElementType, viennameta::static_pair<KeyType, ValueType> >        static_key_type;
      typedef typename result_of::container_from_config< container_config, static_key_type >::type      container_type;
      typedef typename result_of::access_tag_from_config< container_config, static_key_type >::type     access_tag;
      return container_access<container_type, ElementType, access_tag>::lookup(
        get_container_map<KeyType, ValueType, ElementType>().get(key), element);
    }


    /** @brief Query the availability of a value for <key_type, value_type, element_type> with specific key and element; returns NULL if not found */
    template<typename KeyType, typename ValueType, typename ElementType>
    ValueType * find(KeyType const & key, ElementType const & element)
    {
      typedef viennameta::static_pair<ElementType, viennameta::static_pair<KeyType, ValueType> >        static_key_type;
      typedef typename result_of::container_from_config< container_config, static_key_type >::type      container_type;
      typedef typename result_of::access_tag_from_config< container_config, static_key_type >::type     access_tag;
      return container_access<container_type, ElementType, access_tag>::find(
        get_container_map<KeyType, ValueType, ElementType>().get(key), element);
    }

    /** @brief Query the availability of a value for <key_type, value_type, element_type> with specific key and element; returns NULL if not found; const version */
    template<typename KeyType, typename ValueType, typename ElementType>
    ValueType const * find(KeyType const & key, ElementType const & element) const
    {
      typedef viennameta::static_pair<ElementType, viennameta::static_pair<KeyType, ValueType> >        static_key_type;
      typedef typename result_of::container_from_config< container_config, static_key_type >::type      container_type;
      typedef typename result_of::access_tag_from_config< container_config, static_key_type >::type     access_tag;
      return container_access<container_type, ElementType, access_tag>::find(
        get_container_map<KeyType, ValueType, ElementType>().get(key), element);
    }


    /** @brief Erases the value for <key_type, value_type, element_type> with specific key and element */
    template<typename KeyType, typename ValueType, typename ElementType>
    void erase(KeyType const & key, ElementType const & element)
    {
      typedef viennameta::static_pair<ElementType, viennameta::static_pair<KeyType, ValueType> >        static_key_type;
      typedef typename result_of::container_from_config< container_config, static_key_type >::type      container_type;
      typedef typename result_of::access_tag_from_config< container_config, static_key_type >::type     access_tag;
      container_access<container_type, ElementType, access_tag>::erase(
        get_container_map<KeyType, ValueType, ElementType>().get(key), element);
    }

    /** @brief Erases all values the specific element */
    template<typename ElementType>
    void erase_all_data_from_element(ElementType const & element)
    {
      typename dynamic_key_to_container_accessor_map::iterator it =
        element_type_to_container_map_accessor_map.find(rtti.template key<ElementType>());
      if (it == element_type_to_container_map_accessor_map.end()) return;

      dynamic_type_wrapper<ElementType> dynamic_element(element);

      for (std::vector<base_dynamic_container_map_accessor*>::iterator jt = it->second.begin(); jt != it->second.end(); ++jt)
        (*jt)->erase_all(dynamic_element);
    }

    /** @brief Erases all values for specific element with a specific key_type */
    template<typename KeyType, typename ElementType>
    void erase_all_data_from_element_with_key(ElementType const & element)
    {
      typename dynamic_key_to_container_accessor_map::iterator it =
        element_and_key_type_to_container_map_accessor_map.find(rtti.template key< viennameta::static_pair<ElementType,KeyType> >());
      if (it == element_and_key_type_to_container_map_accessor_map.end()) return;

      dynamic_type_wrapper<ElementType> dynamic_element(element);

      for (std::vector<base_dynamic_container_map_accessor*>::iterator jt = it->second.begin(); jt != it->second.end(); ++jt)
        (*jt)->erase_all(dynamic_element);
    }

    /** @brief Erases all values for specific element with a specific key_type and key */
    template<typename KeyType, typename ElementType>
    void erase_all_data_from_element_with_key(KeyType const & key, ElementType const & element)
    {
      typename dynamic_key_to_container_accessor_map::iterator it =
        element_and_key_type_to_container_map_accessor_map.find(rtti.template key< viennameta::static_pair<ElementType,KeyType> >());
      if (it == element_and_key_type_to_container_map_accessor_map.end()) return;

      dynamic_type_wrapper<ElementType> dynamic_element(element);
      dynamic_type_wrapper<KeyType> dynamic_key(key);

      for (std::vector<base_dynamic_container_map_accessor*>::iterator jt = it->second.begin(); jt != it->second.end(); ++jt)
        (*jt)->erase(dynamic_key, dynamic_element);
    }

    /** @brief erases all values for specific element with a specific value_type */
    template<typename ValueType, typename ElementType>
    void erase_all_data_from_element_with_value_type(ElementType const & element)
    {
      typename dynamic_key_to_container_accessor_map::iterator it =
        element_and_value_type_to_container_map_accessor_map.find(rtti.template key< viennameta::static_pair<ElementType, ValueType> >());
      if (it == element_and_value_type_to_container_map_accessor_map.end()) return;

      dynamic_type_wrapper<ElementType> dynamic_element(element);

      for (std::vector<base_dynamic_container_map_accessor*>::iterator jt = it->second.begin(); jt != it->second.end(); ++jt)
        (*jt)->erase_all(dynamic_element);
    }



    /** @brief copy value for <key_type, value_type, element_type> with specific key and element to another element */
    template<typename KeyType, typename ValueType, typename ElementType>
    void copy(KeyType const & key, ElementType const & src_element, ElementType const & dst_element)
    {
      typedef viennameta::static_pair<ElementType, viennameta::static_pair<KeyType, ValueType> >        static_key_type;
      typedef typename result_of::container_from_config< container_config, static_key_type >::type      container_type;
      typedef typename result_of::access_tag_from_config< container_config, static_key_type >::type     access_tag;
      return container_access<container_type, ElementType, access_tag>::copy(
        get_container_map<KeyType, ValueType, ElementType>().get(key), src_element, dst_element);
    }


    /** @brief copy all values the specific element to another element  */
    template<typename ElementType>
    void copy_all_data_from_element(ElementType const & src_element, ElementType const & dst_element)
    {
      typename dynamic_key_to_container_accessor_map::iterator it =
        element_type_to_container_map_accessor_map.find(rtti.template key<ElementType>());
      if (it == element_type_to_container_map_accessor_map.end()) return;

      dynamic_type_wrapper<ElementType> dynamic_src_element(src_element);
      dynamic_type_wrapper<ElementType> dynamic_dst_element(dst_element);

      for (std::vector<base_dynamic_container_map_accessor*>::iterator jt = it->second.begin(); jt != it->second.end(); ++jt)
        (*jt)->copy_all(dynamic_src_element, dynamic_dst_element);
    }

    /** @brief copy all values for specific element with a specific key_type to another element */
    template<typename KeyType, typename ElementType>
    void copy_all_data_from_element_with_key(ElementType const & src_element, ElementType const & dst_element)
    {
      typename dynamic_key_to_container_accessor_map::iterator it =
        element_and_key_type_to_container_map_accessor_map.find(rtti.template key< viennameta::static_pair<ElementType,KeyType> >());
      if (it == element_and_key_type_to_container_map_accessor_map.end()) return;

      dynamic_type_wrapper<ElementType> dynamic_src_element(src_element);
      dynamic_type_wrapper<ElementType> dynamic_dst_element(dst_element);

      for (std::vector<base_dynamic_container_map_accessor*>::iterator jt = it->second.begin(); jt != it->second.end(); ++jt)
        (*jt)->copy_all(dynamic_src_element, dynamic_dst_element);
    }

    /** @brief copy all values for specific element with a specific key_type and key to another element */
    template<typename KeyType, typename ElementType>
    void copy_all_data_from_element_with_key(KeyType const & key, ElementType const & src_element, ElementType const & dst_element)
    {
      typename dynamic_key_to_container_accessor_map::iterator it =
        element_and_key_type_to_container_map_accessor_map.find(rtti.template key< viennameta::static_pair<ElementType,KeyType> >());
      if (it == element_and_key_type_to_container_map_accessor_map.end()) return;

      dynamic_type_wrapper<ElementType> dynamic_src_element(src_element);
      dynamic_type_wrapper<ElementType> dynamic_dst_element(dst_element);
      dynamic_type_wrapper<KeyType> dynamic_key(key);

      for (std::vector<base_dynamic_container_map_accessor*>::iterator jt = it->second.begin(); jt != it->second.end(); ++jt)
        (*jt)->copy(dynamic_key, dynamic_src_element, dynamic_dst_element);
    }

    /** @brief erases all values for specific element with a specific value_type  */
    template<typename ValueType, typename ElementType>
    void copy_all_data_from_element_with_value_type(ElementType const & src_element, ElementType const & dst_element)
    {
      typename dynamic_key_to_container_accessor_map::iterator it =
        element_and_value_type_to_container_map_accessor_map.find(rtti.template key< viennameta::static_pair<ElementType, ValueType> >());
      if (it == element_and_value_type_to_container_map_accessor_map.end()) return;

      dynamic_type_wrapper<ElementType> dynamic_src_element(src_element);
      dynamic_type_wrapper<ElementType> dynamic_dst_element(dst_element);

      for (std::vector<base_dynamic_container_map_accessor*>::iterator jt = it->second.begin(); jt != it->second.end(); ++jt)
        (*jt)->copy_all(dynamic_src_element, dynamic_dst_element);
    }


   private:

    runtime_type_information                      rtti;
    mutable container_map_map_type                container_maps;

    mutable dynamic_key_to_container_accessor_map element_type_to_container_map_accessor_map;
    mutable dynamic_key_to_container_accessor_map element_and_key_type_to_container_map_accessor_map;
    mutable dynamic_key_to_container_accessor_map element_and_value_type_to_container_map_accessor_map;
  };



} // namespace viennadata


#endif

