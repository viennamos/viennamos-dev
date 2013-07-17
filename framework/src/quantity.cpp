
#include "quantity.h"
#include <sstream>

#include <QDebug>



Quantity::Quantity()  {}
Quantity::~Quantity() {}

Quantity::Quantity(std::string const& name, std::string const& unit, std::string const& source,
                   int cell_level, int tensor_level) :
    name(name), unit(unit), source(source), cell_level(cell_level), tensor_level(tensor_level), handle(0)
{
    generate_id();
}

Quantity::Quantity(Quantity const& other)
{
  name          = other.name;
  unit          = other.unit;
  source        = other.source;
  cell_level    = other.cell_level;
  tensor_level  = other.tensor_level;
  handle        = other.handle;
}

bool Quantity::operator==(Quantity const& other)
{
  return
  (name          == other.name) &&
  (unit          == other.unit) &&
  (source        == other.source) &&
  (cell_level    == other.cell_level) &&
  (tensor_level  == other.tensor_level) &&
  (handle        == other.handle);
}

bool Quantity::operator!=(Quantity const& other)
{
  return !((*this) == other);
}


void Quantity::operator()(std::string const& new_name, std::string const& new_unit, std::string const& new_source,
                   int new_cell_level, int new_tensor_level)
{
    name         = new_name;
    unit         = new_unit;
    source       = new_source;
    cell_level   = new_cell_level;
    tensor_level = new_tensor_level;
    handle       = 0;
    generate_id();
}

bool Quantity::operator<(Quantity const & other) const
{
    return handle < other.handle;
}

void Quantity::generate_id()
{
    std::stringstream ss;
    ss << name << unit << source << cell_level << tensor_level;
    handle = string_hash(ss.str());
}
