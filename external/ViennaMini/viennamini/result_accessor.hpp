#ifndef VIENNAMINI_RESULTACCESSORS_HPP
#define VIENNAMINI_RESULTACCESSORS_HPP

#include "viennafvm/forwards.h"

namespace viennamini {


template<typename Cell, typename Storage, typename ResultVector>
struct result_accessor
{
  typedef typename ResultVector::value_type   value_type;
  typedef viennafvm::mapping_key              mapping_key;
  typedef viennafvm::boundary_key             boundary_key;

  typedef typename viennadata::result_of::accessor<Storage, mapping_key, long, Cell>::type    mapping_accessor;
  typedef typename viennadata::result_of::accessor<Storage, boundary_key, double, Cell>::type   boundary_accessor;

  result_accessor(Storage& storage, ResultVector const& result, std::size_t const& id) :
    storage(storage), result(result)
  {
    mapping_acc  = viennadata::make_accessor<mapping_key, long, Cell>(storage, mapping_key(id));
    boundary_acc = viennadata::make_accessor<boundary_key, double, Cell>(storage, boundary_key(id));
  }

  value_type operator()(Cell const& cell)
  {
    //long cur_index = viennadata::access<mapping_key_type, long>(map_key)(cell);
    long cur_index = mapping_acc(cell);

    // if interior or non-Dirichlet boundary
    if(cur_index > -1)
    {
      return result[cur_index];
    }
    else //use Dirichlet boundary data:
    {
      //std::cout << "dirichlet cell value: " << boundary_acc(cell) << std::endl;
      //return viennadata::access<boundary_key_type, double>(bnd_key)(cell);
      return boundary_acc(cell);
    }
  }

  value_type operator()(Cell const& cell) const
  {
    //long cur_index = viennadata::access<mapping_key_type, long>(map_key)(cell);
    long cur_index = mapping_acc(cell);

    // if interior or non-Dirichlet boundary
    if(cur_index > -1)
    {
      return result[cur_index];
    }
    else //use Dirichlet boundary data:
    {
      //return viennadata::access<boundary_key_type, double>(bnd_key)(cell);
      return boundary_acc(cell);
    }
  }

  Storage           &   storage;
  ResultVector const&   result;
  mapping_accessor      mapping_acc;
  boundary_accessor     boundary_acc;
};



} // viennamini


#endif

