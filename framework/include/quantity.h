#ifndef QUANTITY_H
#define QUANTITY_H

#include <iostream>
#include <vector>

#include <boost/functional/hash.hpp>

#include <QMetaType>

struct Quantity
{
    Quantity();
    ~Quantity();

    Quantity(std::string const& name, std::string const& unit, std::string const& source,
             int cell_level, int tensor_level);

    Quantity(Quantity const& other);

    void operator()(std::string const& new_name, std::string const& new_unit, std::string const& new_source,
             int new_cell_level, int new_tensor_level);

    bool operator==(const Quantity& other);
    bool operator!=(const Quantity& other);

    bool operator<(Quantity const & other) const;

private:
    /// Create a unique integer ID for the string based keys
    void generate_id();

public:

    std::string name;
    std::string unit;
    std::string source;
    int         cell_level;
    int         tensor_level;
    std::size_t handle;

private:
    boost::hash<std::string> string_hash;
};

typedef std::vector<Quantity> QuantitySet;


// register it as a Qt type, so we can use it with QVariant
Q_DECLARE_METATYPE(Quantity)

#endif // QUANTITY_H
