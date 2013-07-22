#ifndef VIENNAMINI_CONSTANTS_HPP
#define VIENNAMINI_CONSTANTS_HPP

/* =======================================================================
   Copyright (c) 2011, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
                 ViennaMini - The Vienna Device Simulator
                             -----------------

   authors:    Karl Rupp                          rupp@iue.tuwien.ac.at
               Josef Weinbub                   weinbub@iue.tuwien.ac.at
               (add your name here)

   license:    see file LICENSE in the ViennaFVM base directory
======================================================================= */


namespace viennamini
{

struct pi
{
  typedef double        result_type;
  typedef std::string   string_type;

  static result_type val() {
    return  3.1415926535897932384626433832795;
  }

  static string_type unit() {
    return "1";
  }

  static string_type note() {
    return "The ratio of a circle's circumference to its diameter";
  }
};

struct c0
{
  typedef double        result_type;
  typedef std::string   string_type;

  static result_type val() {
    return  299792458.0;
  }

  static string_type unit() {
    return "m/s";
  }

  static string_type note() {
    return "Speed of light in vacuum";
  }
};

struct eps0
{
  typedef double        result_type;
  typedef std::string   string_type;

  static result_type val() {
    return 8.854187817620E-12;
  }

  static string_type unit() {
    return "As/Vm";
  }

  static string_type note() {
    return "Permittivity of vacuum";
  }
};

struct kB
{
  typedef double        result_type;
  typedef std::string   string_type;

  static result_type val() {
    return 1.3806488e-23;
  }

  static string_type unit() {
    return "J/K";
  }

  static string_type note() {
    return "Boltzmann constant";
  }
};

struct q
{
  typedef double        result_type;
  typedef std::string   string_type;

  static result_type val() {
    return 1.602176565e-19;
  }

  static string_type unit() {
    return "As";
  }

  static string_type note() {
    return "Elementary charge";
  }
};

//  /** @brief Implementations. Not intended to be used by a library user. */
//  namespace detail
//  {
//    /** @brief Implementation class holding basic physics constants */
//    template <bool dummy = true>  //template argument in order to control linkage
//    struct constants
//    {
//      /** @brief Speed of light in vacuum */
//      static const double c_0;
//      /** @brief Electron rest mass */
//      static const double mass_electron;
//      /** @brief Elementary charge */
//      static const double q;
//      /** @brief Boltzmann constant */
//      static const double kB;
//      /** @brief Planck constant */
//      static const double h;
//      /** @brief Modified Planck constant */
//      static const double hbar;
//      /** @brief Permittivity of vacuum. */
//      static const double eps_0;
//      /** @brief Permeability of vacuum. */
//      static const double mu_0;
//      /** @brief Intrinsic carrier concentration in silicon. */
//      static const double ni;
//    };

//    template <bool b>
//    const double constants<b>::c_0 = 299792458.0; // m / s
//    template <bool b>
//    const double constants<b>::mass_electron = 9.10938291e-31; // kg
//    template <bool b>
//    const double constants<b>::q      = 1.602176565e-19;     // As
//    template <bool b>
//    const double constants<b>::kB     = 1.3806488e-23;    // J/K
//    template <bool b>
//    const double constants<b>::h      = 6.62606896e-34; // Js
//    template <bool b>
//    const double constants<b>::hbar   = 6.62606896e-34 / (2.0 * 3.1415926535897932384626433832795); // Js
//    template <bool b>
//    const double constants<b>::eps_0  =  1.0e7 / ( 4.0 * 3.1415926535897932384626433832795 * 299792458.0 * 299792458.0) ; // = 10^7 / (4*pi*c_0^2) ~ 8.854e-12 As / Vm
//    template <bool b>
//    const double constants<b>::mu_0  =  ( 4.0 * 3.1415926535897932384626433832795 ) * 1.0e-7 ; // = 10^-7 * (4*pi) ~ 12.56e-7   Vs / Am
//    template <bool b>
//    const double constants<b>::ni = 1.08e16; // m^-3
//  }

//  typedef detail::constants<>   constants;

} //namespace viennashe
#endif
