#ifndef VIENNAMINI_CONFIG_HPP
#define VIENNAMINI_CONFIG_HPP

#include <map>
#include <vector>

namespace viennamini {



struct config
{
  typedef double Numeric;
  typedef int Index;
  typedef std::vector<Numeric> Values;
  typedef std::map<std::size_t, Numeric > SegmentValues;

  config()
  {
    local_temperature = 300.0; // K
    local_nonlinear_iterations = 100;
    local_nonlinear_breaktol = 1.E-3;
    local_linear_breaktol = 1.E-14;
    local_linear_iterations = 700;
    local_damping = 1.0;
    local_initial_guess_smoothing_iterations = 2;

    model_drift_diffusion_state = true; // on by default ..
  }

  Numeric&  temperature()                         { return local_temperature; }
  Index&    nonlinear_iterations()                { return local_nonlinear_iterations; }
  Numeric&  nonlinear_breaktol()                  { return local_nonlinear_breaktol; }
  Index&    linear_iterations()                   { return local_linear_iterations; }
  Numeric&  linear_breaktol()                     { return local_linear_breaktol; }
  Numeric&  damping()                             { return local_damping; }
  Index&    initial_guess_smoothing_iterations()  { return local_initial_guess_smoothing_iterations; }

  void assign_contact(std::size_t segment_index, Numeric value, Numeric workfunction)
  {
    segment_contact_values[segment_index] = value;
    segment_contact_workfunctions[segment_index] = workfunction;
  }

  Numeric get_contact_value(std::size_t segment_index)
  {
    return segment_contact_values[segment_index];
  }

  Numeric get_workfunction(std::size_t segment_index)
  {
      return segment_contact_workfunctions[segment_index];
  }

  void enable_drift_diffusion(bool state = true)
  {
    model_drift_diffusion_state = state;
  }

  bool has_drift_diffusion() { return model_drift_diffusion_state; }

private:
  Numeric local_temperature;

  Index       local_nonlinear_iterations;
  Numeric     local_nonlinear_breaktol;
  Index       local_linear_iterations;
  Numeric     local_linear_breaktol;
  Numeric     local_damping;

  Index       local_initial_guess_smoothing_iterations;

  SegmentValues segment_contact_values;
  SegmentValues segment_contact_workfunctions;

  bool        model_drift_diffusion_state;
};




} // viennamini



#endif


