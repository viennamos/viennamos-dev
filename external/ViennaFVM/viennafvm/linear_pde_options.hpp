#ifndef VIENNAFVM_LINEARPDEOPTIONS_HPP
#define VIENNAFVM_LINEARPDEOPTIONS_HPP

/* =======================================================================
   Copyright (c) 2011, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
           ViennaFVM - The Vienna Finite Volume Method Library
                             -----------------

   authors:    Karl Rupp                             rupp@iue.tuwien.ac.at
               Josef Weinbub                      weinbub@iue.tuwien.ac.at

               (add your name here)

   license:    To be discussed, see file LICENSE in the ViennaFVM base directory
======================================================================= */

// *** system includes
//
#include <vector>

// *** local includes:
//
//#include "viennafvm/forwards.h"

// *** vienna includes:
//
#include "viennadata/api.hpp"
#include "viennagrid/domain/domain.hpp"
#include "viennamath/expression.hpp"

namespace viennafvm
{

  class linear_pde_options
  {
    public:
      explicit linear_pde_options(long id = 0) : data_id_(id), check_mapping_(false), geometric_update_(false), damping_term_(viennamath::rt_constant<numeric_type>(0)) {}

      long data_id() const { return data_id_; }
      void data_id(long new_id) { data_id_ = new_id; }

      bool check_existing_mapping() const { return check_mapping_; }
      void check_existing_mapping(bool b) { check_mapping_ = b; }

      bool geometric_update() const { return geometric_update_; }
      void geometric_update(bool b) { geometric_update_ = b; }

      viennamath::expr damping_term() const { return damping_term_; }
      void damping_term(viennamath::expr const & e) { damping_term_ = e; }

    private:
      long data_id_;
      bool check_mapping_;
      bool geometric_update_;
      viennamath::expr damping_term_;
  };

  inline linear_pde_options make_linear_pde_options(long data_id, bool existing_mapping = false)
  {
    linear_pde_options options;
    options.data_id(data_id);
    options.check_existing_mapping(existing_mapping);
    return options;
  }

}
#endif

