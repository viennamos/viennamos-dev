#ifndef VIENNAMINI_INITIALGUESSACCESSORS_HPP
#define VIENNAMINI_INITIALGUESSACCESSORS_HPP



namespace viennamini {


template<typename BoundaryKey, typename InitialguessKey, typename NumericT = double>
struct initial_guess_accessor
{
  initial_guess_accessor(std::size_t const& id) : id(id) {}

  template<typename CellT>
  NumericT operator()(CellT& cell)
  {

    if(viennadata::access<BoundaryKey, bool >(BoundaryKey(id))(cell))
    {
      return viennadata::access<BoundaryKey, NumericT >(BoundaryKey(id))(cell);
    }
    else
    {
      return viennadata::access<InitialguessKey, NumericT>(InitialguessKey(id))(cell);
    }
  }
  std::size_t id;
};



}


#endif

