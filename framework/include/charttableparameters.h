#ifndef CHARTTABLEPARAMETERS_H
#define CHARTTABLEPARAMETERS_H

#include <vector>

#include "chartentry.h"

struct ChartTableParameters
{
    typedef std::vector<ChartEntry>     ChartEntries;

    ChartTableParameters();

    ChartEntries        chart_entries;
    bool useIndexForXSeries;
    bool enableAll;
    int x_quantity_index;
};

#endif // CHARTTABLEPARAMETERS_H
