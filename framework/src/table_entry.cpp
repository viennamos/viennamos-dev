
#include "table_entry.h"

TableEntry::TableEntry(QString const& name, Table table) :
    name(name), table(table)
{

}

bool TableEntry::operator==(TableEntry      & other)
{
    if((name == other.name) && (table == other.table)) return true;
    else return false;
}

bool TableEntry::operator==(TableEntry const& other)
{
    if((name == other.name) && (table == other.table)) return true;
    else return false;
}

bool TableEntry::operator!=(TableEntry      & other)
{
    return !(*this == other);
}

bool TableEntry::operator!=(TableEntry const& other)
{
    return !(*this == other);
}
