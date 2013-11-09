#ifndef DEVICEPARAMETERS_HPP
#define DEVICEPARAMETERS_HPP

#include <map>
#include "segmentparameters.hpp"

#include "viennamini/config.hpp"

struct DeviceParameters
{
    typedef double Numeric;
    typedef std::map<int, SegmentParameters>  DomainParameters;
    typedef viennamini::config      SimConfig;

    typedef DomainParameters::iterator iterator;

    SegmentParameters& operator[](int i)
    {
        return domain_paras[i];
    }

    void reset()
    {
        temperature = 300;
        local_config = SimConfig();
    }

    void clear()
    {
        domain_paras.clear();
        this->reset();
    }
    std::size_t size()  { return domain_paras.size(); }
//    void resize(std::size_t const& size) { domain_paras.resize(size); }
    iterator begin() { return domain_paras.begin(); }
    iterator end()   { return domain_paras.end(); }

    SimConfig& config() { return local_config; }

    DomainParameters    domain_paras;
    Numeric             temperature;
    SimConfig           local_config; // offers default parameters!

};



#endif // DEVICEPARAMETERS_H
