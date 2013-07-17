#ifndef TABLE_ENTRY_H
#define TABLE_ENTRY_H

#include <vtkSmartPointer.h>
#include <vtkTable.h>

#include <QString>

struct TableEntry
{
    typedef vtkSmartPointer<vtkTable>               Table;

    TableEntry(QString const& name, Table table);

    bool operator==(TableEntry      & other);
    bool operator==(TableEntry const& other);

    bool operator!=(TableEntry      & other);
    bool operator!=(TableEntry const& other);

    QString     name;
    Table       table;
};


#endif // TABLE_ENTRY_H
