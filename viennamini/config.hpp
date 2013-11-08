#ifndef VIENNAMINI_CONFIG_HPP
#define VIENNAMINI_CONFIG_HPP

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

#include <map>
#include <vector>

namespace viennamini {

struct config
{
  typedef double                              NumericType;
  typedef int                                 IndexType;
  typedef std::vector<NumericType>            ValuesType;
  typedef std::map<std::size_t, NumericType>  SegmentValuesType;

  typedef NumericType       numeric_type;
  typedef IndexType         index_type;
  typedef ValuesType        values_type;
  typedef SegmentValuesType segmentvalues_type;

  config();

  NumericType&  temperature();
  IndexType&    nonlinear_iterations();
  NumericType&  nonlinear_breaktol();
  IndexType&    linear_iterations();
  NumericType&  linear_breaktol();
  NumericType&  damping();
  IndexType&    initial_guess_smoothing_iterations();

  void assign_contact(std::size_t segment_index, NumericType value, NumericType workfunction);

  NumericType& contact_value(std::size_t segment_index);

  NumericType& workfunction(std::size_t segment_index);

  bool& drift_diffusion_state();

private:
  IndexType         nonlinear_iterations_;
  IndexType         linear_iterations_;
  IndexType         initial_guess_smoothing_iterations_;
  NumericType       temperature_;
  NumericType       nonlinear_breaktol_;
  NumericType       linear_breaktol_;
  NumericType       damping_;
  SegmentValuesType segment_contact_values_;
  SegmentValuesType segment_contact_workfunctions_;
  bool              model_drift_diffusion_state_;
};




} // viennamini



#endif


