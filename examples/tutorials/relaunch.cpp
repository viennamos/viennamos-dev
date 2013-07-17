
#include <vector>
#include <iostream>
#include <cstdlib>

#include "viennadata/api.hpp"


struct test_tag;

struct test_struct
{
    typedef test_tag tag;
    typedef int id_type;

    test_struct() {}
    test_struct(int idd) : id_(idd) {}

    id_type id() const { return id_; }

    int id_;
};




struct test_tag2;

struct test_struct2
{
    typedef test_tag2 tag;
    typedef int id_type;

    test_struct2() {}
    test_struct2(int idd) : id_(idd) {}

    id_type id() const { return id_; }

    int id_;
};


struct my_trivial_key
{};

// When using a trivial key operator< MUST return false
bool operator<(my_trivial_key const &, my_trivial_key const &)
{
    return false;
}


// create a custom container configuration
// configuration of default_tag is used for every value-type, key-type, access-type tripple not explicit configured
// storage<> has default std::map with pointer access
// the custom config below has default std::map with pointer access and for class test_struct with key-type int and value-type double std::deque with id access
// when using id access the access-type has to provide ::id_type and ::id()
struct my_container_config
{
    typedef viennameta::make_typemap<
      viennadata::default_tag, viennameta::static_pair<viennadata::std_map_tag, viennadata::pointer_access_tag>,
      viennameta::static_pair< test_struct, viennameta::static_pair<int, double> >, viennameta::static_pair<viennadata::std_deque_tag, viennadata::id_access_tag>
    >::type type;
};


int main()
{

  // create a storage using ur custom configuration
  viennadata::storage<my_container_config> my_storage;

  // create 2 containers for testing
  std::vector<test_struct> elements;
  std::vector<test_struct2> elements2;

  int num_elements = 10;

  for (int i = 0; i < num_elements; ++i)
  {
    elements.push_back(test_struct(i));
    elements2.push_back(test_struct2(i));
  }


  // using viennadata to fill meta data for test containers
  {
    double val = 0.0;
    for (std::vector<test_struct>::iterator it = elements.begin(); it != elements.end(); ++it, val += 1.0)
      viennadata::access<int, double>(my_storage, 0, *it) = val;
  }

  {
    double val = 0.0;
    for (std::vector<test_struct2>::iterator it = elements2.begin(); it != elements2.end(); ++it, val += 1.0)
      viennadata::access<int, double>(my_storage, 0, *it) = val;
  }


  // query the underlying container for key-type double, value-type double and access-type test_struct2
  // Syntax: viennadata::result_of::container_type< STORAGE_TYPE, KEY_TYPE, VALUE_TYPE, ACCESS_TYPE>::type
  // Syntax: viennadata::container<KEY_TYPE, VALUE_TYPE, ACCESS_TYPE>(storage, key);
  viennadata::result_of::container_type< viennadata::storage<my_container_config>, double, double, test_struct2>::type & c =
      viennadata::container<double, double, test_struct2>(my_storage, 0);


  // erasing value from the storage
  // beware: erased from dense container might not actually erase a value
  std::cout << "\nErase Test\n" << std::endl;

  test_struct2 tmp(10);

  // add with temporary data for tmp
  viennadata::access< double, double >(my_storage, 0, tmp) = 1;
  viennadata::access< int, double >(my_storage, 0, tmp) = 2;
  viennadata::access< double, int >(my_storage, 0, tmp) = 3;

  // finding the data -> should be available (point is not NULL)
  // Syntax: viennadata::find<KEY_TYPE, VALUE_TYPE>(storage, key, to_access)
  std::cout << viennadata::find<double, double>(my_storage, 0, tmp) << std::endl;
  std::cout << viennadata::find<int, double>(my_storage, 0, tmp) << std::endl;
  std::cout << viennadata::find<double, int>(my_storage, 0, tmp) << std::endl;



  // query container size (should be 1)
  std::cout << "Container size = " << c.size() << std::endl;

  // erasing data for tmp, key-type double, all value types
  // Syntax: viennadata::erase<KEY_TYPE, VALUE_TYPE>(storage, key, to_access)

  viennadata::erase<double, viennadata::all>(my_storage, 0, tmp);

  // some other erase tests
  // KEY_TYPE and VALUE_TYPE can be viennadata::all if all keys and key_types/all value type should be erased
  // viennadata::erase<viennadata::all, viennadata::all>(my_storage, tmp); // no key needed
  // viennadata::erase<viennadata::all, int>(my_storage, tmp); // no key needed

  // first and third should be NULL now
  std::cout << viennadata::find<double, double>(my_storage, 0, tmp) << std::endl;
  std::cout << viennadata::find<int, double>(my_storage, 0, tmp) << std::endl;
  std::cout << viennadata::find<double, int>(my_storage, 0, tmp) << std::endl;

  // query container size (should be 0)
  std::cout << "Container size = " << c.size() << std::endl;


  // Some testing with Accessors
  std::cout << "\nAccessor Test\n" << std::endl;

  // query an accessor for key-type double, value-type double and access-type test_struct2
  // Syntax: viennadata::result_of::accessor< STORAGE_TYPE, KEY_TYPE, VALUE_TYPE, ACCESS_TYPE>::type
  // Syntax: viennadata::accessor<KEY_TYPE, VALUE_TYPE, ACCESS_TYPE>(storage, key);
  viennadata::result_of::accessor<viennadata::storage<my_container_config>, double, double, test_struct2>::type
      accessor = viennadata::acc(my_storage, 0);

//       viennadata::accessor<double, double, test_struct2>(my_storage, 0);

  // simple accessor access
  accessor(tmp) = 3.14;
  std::cout << "Accessor: " << accessor(tmp) << std::endl;

  // find return Pointer
  std::cout << "Accessor find: " << accessor.find(tmp) << std::endl;

  // erase might not work properly on dense containers
  accessor.erase(tmp);

  // should ne NULL
  std::cout << "Accessor find: " << accessor.find(tmp) << std::endl;


  std::cout << "\nCopy Test\n" << std::endl;
  viennadata::access< double, double >(my_storage, 0, tmp) = 1;
  viennadata::access< int, double >(my_storage, 0, tmp) = 2;
  viennadata::access< double, int >(my_storage, 0, tmp) = 3;

  std::cout << viennadata::access<double, double>(my_storage, 0, tmp) << std::endl;
  std::cout << viennadata::access<int, double>(my_storage, 0, tmp) << std::endl;
  std::cout << viennadata::access<double, int>(my_storage, 0, tmp) << std::endl;
  std::cout << std::endl;

  test_struct2 tmp1(11);

  // Copy data
  // Syntax: viennadata::copy<KEY_TYPE, VALUE_TYPE>(storage, key, source_access, destination_access)
  // KEY_TYPE and/or VALUE_TYPE can be viennadata::all
  viennadata::copy<double, viennadata::all>(my_storage, 0, tmp, tmp1);

  std::cout << viennadata::access<double, double>(my_storage, 0, tmp) << std::endl;
  std::cout << viennadata::access<int, double>(my_storage, 0, tmp) << std::endl;
  std::cout << viennadata::access<double, int>(my_storage, 0, tmp) << std::endl;
  std::cout << std::endl;

  // the first and the third should have been copied
  std::cout << viennadata::access<double, double>(my_storage, 0, tmp1) << std::endl;
  std::cout << viennadata::access<int, double>(my_storage, 0, tmp1) << std::endl;
  std::cout << viennadata::access<double, int>(my_storage, 0, tmp1) << std::endl;


  // Example using a trivial key (something similar to type-based dispatching)
  std::cout << "\nTrivial Key\n" << std::endl;
  viennadata::access< my_trivial_key, double >(my_storage, my_trivial_key(), tmp) = 42;
  std::cout << "my trivial key = " << viennadata::access< my_trivial_key, double >(my_storage, my_trivial_key(), tmp) << std::endl;

  return EXIT_SUCCESS;
}
