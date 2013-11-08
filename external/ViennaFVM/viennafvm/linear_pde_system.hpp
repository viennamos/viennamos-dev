#ifndef VIENNAFVM_LINEARPDESYSTEM_HPP
#define VIENNAFVM_LINEARPDESYSTEM_HPP

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

#include <vector>

// *** local includes:
#include "viennafvm/forwards.h"
#include "viennafvm/linear_pde_options.hpp"

// *** vienna includes:
#include "viennamath/forwards.h"
#include "viennadata/api.hpp"
#include "viennagrid/mesh/mesh.hpp"

namespace viennafvm
{
  template <typename InterfaceType = viennamath::default_interface_type>
  struct linear_pde_system
  {
      typedef InterfaceType                                 interface_type;
      typedef viennafvm::mapping_key                        mapping_key_type;
      typedef viennafvm::boundary_key                       boundary_key_type;
      typedef viennamath::equation                          equation_type;
      typedef viennamath::function_symbol                   unknown_type;
      typedef std::vector< unknown_type >                   unknown_cont_type;
      typedef std::vector< std::string >                    key_cont_type;
      typedef viennafvm::linear_pde_options                 option_type;

      linear_pde_system() : is_linear_(true) {}

      void add_pde(equation_type      const & pde,
                   unknown_cont_type  const & unknowns,
                   key_cont_type      const & keys,
                   option_type        const & option)
      {
        pdes_.push_back(pde);
        unknowns_.push_back(unknowns);
        keys_.push_back(keys);
        options_.push_back(option);
      }

      void add_pde(equation_type      const & pde,
                   unknown_cont_type  const & unknowns)
      {
        std::size_t current_id = size();

        pdes_.push_back(pde);
        unknowns_.push_back(unknowns);

        key_cont_type key(1); key[0] = "dummy";
        keys_.push_back(key);
        options_.push_back(option_type(current_id));
      }

      void add_pde(equation_type      const & pde,
                   unknown_type       const & unknown)
      {
        std::size_t current_id = size();

        pdes_.push_back(pde);

        unknown_cont_type new_unknown(1); new_unknown[0] = unknown;
        unknowns_.push_back(new_unknown);

        key_cont_type key(1);
        key[0] = "dummy";
        keys_.push_back(key);
        options_.push_back(option_type(current_id));
      }

      equation_type const &      pde(size_t index)       const { return pdes_[index]; }
      unknown_cont_type const &  unknown(size_t index)   const { return unknowns_[index]; }
      key_cont_type const &      keys(size_t index)      const { return keys_[index]; }
      option_type const &        option(size_t index)    const { return options_[index]; }

      option_type &              option(size_t index)          { return options_[index]; }

      bool is_linear() const { return is_linear_; }
      void is_linear(bool b) { is_linear_ = b; }

      size_t size() const { return pdes_.size(); }

    private:
      std::vector< equation_type >        pdes_;
      std::vector< unknown_cont_type >    unknowns_;
      std::vector< key_cont_type >        keys_;
      std::vector< option_type >          options_;
      bool is_linear_;
  };


   template <typename InterfaceType>
   linear_pde_system<InterfaceType> make_linear_pde_system(viennamath::equation             equ_1,
                                                           viennamath::function_symbol      unknown_1,
                                                           std::string                      key_1,
                                                           viennafvm::linear_pde_options    option_1)
   {
      typedef viennafvm::linear_pde_system<InterfaceType>   linear_pde_sys_type;

      linear_pde_sys_type ret;

      typename linear_pde_sys_type::unknown_cont_type unknown_vec_1(1);
      unknown_vec_1[0] = unknown_1;

      typename linear_pde_sys_type::key_cont_type     key_vec_1(1);
      key_vec_1[0] = key_1;

      ret.add_pde(equ_1, unknown_vec_1, key_vec_1, option_1);
      return ret;
   }

   template <typename InterfaceType>
   linear_pde_system<InterfaceType> make_linear_pde_system(viennamath::rt_equation<InterfaceType>         equ_1,
                                                           viennamath::rt_function_symbol<InterfaceType>  unknown_1)
   {
      typedef viennafvm::linear_pde_system<InterfaceType>   linear_pde_sys_type;

      linear_pde_sys_type ret;

      linear_pde_options option_1;

      typename linear_pde_sys_type::unknown_cont_type unknown_vec_1(1);
      unknown_vec_1[0] = unknown_1;

      typename linear_pde_sys_type::key_cont_type     key_vec_1(1);
      key_vec_1[0] = "dummy";

      ret.add_pde(equ_1, unknown_vec_1, key_vec_1, option_1);
      return ret;
   }

   template <typename InterfaceType>
   linear_pde_system<InterfaceType> make_linear_pde_system(viennamath::rt_equation<InterfaceType>         equ_1,
                                                           viennamath::rt_function_symbol<InterfaceType>  unknown_1,
                                                           viennafvm::linear_pde_options    option_1
                                                          )
   {
      typedef viennafvm::linear_pde_system<InterfaceType>   linear_pde_sys_type;

      linear_pde_sys_type ret;

      typename linear_pde_sys_type::unknown_cont_type unknown_vec_1(1);
      unknown_vec_1[0] = unknown_1;

      typename linear_pde_sys_type::key_cont_type     key_vec_1(1);
      key_vec_1[0] = "dummy";

      ret.add_pde(equ_1, unknown_vec_1, key_vec_1, option_1);
      return ret;
   }


}
#endif

