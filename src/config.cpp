/* =======================================================================
   Copyright (c) 2011-2013, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
                 ViennaMini - The Vienna Device Simulator
                             -----------------

   authors:    Karl Rupp                          rupp@iue.tuwien.ac.at
               Josef Weinbub                   weinbub@iue.tuwien.ac.at
               (add your name here)

   license:    see file LICENSE in the ViennaFVM base directory
======================================================================= */


#include "viennamini/config.hpp"


namespace viennamini {

config::config()
{
  temperature_                         = 300.0;
  nonlinear_iterations_                = 100;
  nonlinear_breaktol_                  = 1.E-3;
  linear_breaktol_                     = 1.E-14;
  linear_iterations_                   = 1000;
  damping_                             = 1.0;
  initial_guess_smoothing_iterations_  = 0;
  model_drift_diffusion_state_         = true;
}


config::NumericType&  config::temperature()
{
  return temperature_;
}

config::IndexType&    config::nonlinear_iterations()
{
  return nonlinear_iterations_;
}

config::NumericType&  config::nonlinear_breaktol()
{
  return nonlinear_breaktol_;
}

config::IndexType&    config::linear_iterations()
{
  return linear_iterations_;
}

config::NumericType&  config::linear_breaktol()
{
  return linear_breaktol_;
}

config::NumericType&  config::damping()
{
  return damping_;
}

config::IndexType&    config::initial_guess_smoothing_iterations()
{
  return initial_guess_smoothing_iterations_;
}

void config::assign_contact(std::size_t segment_index, config::NumericType value, config::NumericType workfunction)
{
  segment_contact_values_       [segment_index] = value;
  segment_contact_workfunctions_[segment_index] = workfunction;
}

config::NumericType& config::contact_value(std::size_t segment_index)
{
  return segment_contact_values_[segment_index];
}

config::NumericType& config::workfunction(std::size_t segment_index)
{
    return segment_contact_workfunctions_[segment_index];
}

bool& config::drift_diffusion_state()
{
  return model_drift_diffusion_state_;
}

} // viennamini

