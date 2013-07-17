
#ifndef VIENNAMINI_PHYSICS_HPP
#define VIENNAMINI_PHYSICS_HPP

#include "constants.hpp"

namespace viennamini
{

  inline double get_thermal_potential(double T)
  {
    return (viennamini::kB::val() * T) / viennamini::q::val();
  }

  inline double built_in_potential(double temp, double doping_n, double doping_p)
  {
    const double ni = 1.e16; // TODO!!
    const double net_doping = doping_n - doping_p;
    const double x = std::abs(net_doping) / (2.0 * ni);

    // builtin_pot =         V_T              * arsinh( net_doping/(2 n_i))
    double bpot = get_thermal_potential(temp) * std::log(x + std::sqrt( 1.0 + x*x ) );


    if ( net_doping < 0) //above formula does not yet consider the doping polarity
      bpot *= -1.0;

    return bpot;
  }

} // viennamini



#endif
