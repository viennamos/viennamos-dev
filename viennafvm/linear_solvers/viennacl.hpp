#ifndef VIENNAFVM_SOLVERS_CONFIG_HPP
#define VIENNAFVM_SOLVERS_CONFIG_HPP

/* =======================================================================
   Copyright (c) 2011, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
           ViennaFVM - The Vienna Finite Volume Method Library
                             -----------------

   authors:    Karl Rupp                          rupp@iue.tuwien.ac.at
               Josef Weinbub                   weinbub@iue.tuwien.ac.at
               (add your name here)

   license:    To be discussed, see file LICENSE in the ViennaFVM base directory
======================================================================= */

#include <map>
#include <vector>

#ifndef VIENNACL_HAVE_UBLAS
 #define VIENNACL_HAVE_UBLAS
#endif

#include "viennacl/linalg/cg.hpp"
#include "viennacl/linalg/bicgstab.hpp"
#include "viennacl/linalg/gmres.hpp"
#include "viennacl/linalg/norm_2.hpp"
#include "viennacl/linalg/prod.hpp"
#include "viennacl/linalg/ilu.hpp"
#include "viennacl/linalg/jacobi_precond.hpp"
#include "viennacl/linalg/row_scaling.hpp"
#include "viennafvm/timer.hpp"

namespace viennafvm {

namespace linsolv {

struct viennacl
{

  struct preconditioner_ids
  {
    enum 
    { 
      none,
      ilu0, 
      ilut, 
      block_ilu,
      jacobi, 
      row_scaling
    };
  };

  struct solver_ids
  {
    enum 
    { 
      cg,
      bicgstab, 
      gmres 
    };
  };

  viennacl() : pc_id_(viennafvm::linsolv::viennacl::preconditioner_ids::ilu0), 
               solver_id_(viennafvm::linsolv::viennacl::solver_ids::bicgstab), 
               break_tolerance_(1.0e-14),
               max_iterations_(1000)
  {
  }

  long&         preconditioner()    { return pc_id_;           }
  long&         solver()            { return solver_id_;       }
  double&       break_tolerance()   { return break_tolerance_; }
  std::size_t&  max_iterations()    { return max_iterations_;  }

  std::size_t   last_iterations()   { return last_iterations_; }
  double        last_error()        { return last_error_;      }
  float         last_pc_time()      { return last_pc_time_;    }
  float         last_solver_time()  { return last_solver_time_;}

  template <typename MatrixT, typename VectorT>
  void operator()(MatrixT& A, VectorT& b, VectorT& x)
  {
    row_normalize_system(A, b); 

    //
    // Determine the linear solver kernel and forward to an internal solve method
    // which determines the preconditioner and actually calls the solver backend
    //
    if(solver_id_ == viennafvm::linsolv::viennacl::solver_ids::bicgstab)
    {
//      std::cout << "using solver: bicgstab .. " << std::endl;
      ::viennacl::linalg::bicgstab_tag  solver_tag(break_tolerance_, max_iterations_);
      solve_intern(A, b, x, solver_tag);
    }
    else
    if(solver_id_ == viennafvm::linsolv::viennacl::solver_ids::gmres)
    {
//      std::cout << "using solver: gmres .. " << std::endl;
      ::viennacl::linalg::gmres_tag     solver_tag(break_tolerance_, max_iterations_);
      solve_intern(A, b, x, solver_tag);
    }
    else
    if(solver_id_ == viennafvm::linsolv::viennacl::solver_ids::cg)
    {
//      std::cout << "using solver: cg .. " << std::endl;
      ::viennacl::linalg::cg_tag        solver_tag(break_tolerance_, max_iterations_);
      solve_intern(A, b, x, solver_tag);
    }
    else
    {
      std::cerr << "[ERROR] ViennaFVM::LinearSolver: solver not supported .. " << std::endl;
    }
  }

private:

  template <typename MatrixT, typename VectorT, typename LinerSolverT>
  void solve_intern(MatrixT& A, VectorT& b, VectorT& x, LinerSolverT& linear_solver)
  {
    viennafvm::Timer timer;

    if(pc_id_ == viennafvm::linsolv::viennacl::preconditioner_ids::none)
    {
//      std::cout << "using pc: none .. " << std::endl;
      last_pc_time_ = 0.0;
      timer.start();
      x = ::viennacl::linalg::solve(A, b, linear_solver);
      last_solver_time_ = timer.get();
    }
    else
    if(pc_id_ == viennafvm::linsolv::viennacl::preconditioner_ids::ilu0)
    {
//      std::cout << "using pc: ilu0 .. " << std::endl;
      ::viennacl::linalg::ilu0_tag pc_config;
      pc_config.use_level_scheduling(false);

      timer.start();
      ::viennacl::linalg::ilu0_precond<MatrixT>    preconditioner(A, pc_config);
      last_pc_time_ = timer.get();

      timer.start();
      x = ::viennacl::linalg::solve(A, b, linear_solver, preconditioner);
      last_solver_time_ = timer.get();
    }
    else
    if(pc_id_ == viennafvm::linsolv::viennacl::preconditioner_ids::ilut)
    {
//      std::cout << "using pc: ilut .. " << std::endl;
      ::viennacl::linalg::ilut_tag pc_config;
      pc_config.set_drop_tolerance(1.0e-4);
      pc_config.set_entries_per_row(40);
      pc_config.use_level_scheduling(false);

      timer.start();
      ::viennacl::linalg::ilut_precond<MatrixT>    preconditioner(A, pc_config);
      last_pc_time_ = timer.get();

      timer.start();
      x = ::viennacl::linalg::solve(A, b, linear_solver, preconditioner);
      last_solver_time_ = timer.get();
    }
    else
    if(pc_id_ == viennafvm::linsolv::viennacl::preconditioner_ids::block_ilu)
    {
//      std::cout << "using pc: block ilu .. " << std::endl;
      ::viennacl::linalg::ilu0_tag pc_config;
      pc_config.use_level_scheduling(false);

      timer.start();
      ::viennacl::linalg::block_ilu_precond<MatrixT, ::viennacl::linalg::ilu0_tag>    preconditioner(A, pc_config);
      last_pc_time_ = timer.get();

      timer.start();
      x = ::viennacl::linalg::solve(A, b, linear_solver, preconditioner);
      last_solver_time_ = timer.get();
    }
    else
    if(pc_id_ == viennafvm::linsolv::viennacl::preconditioner_ids::jacobi)
    {
//      std::cout << "using pc: jacobi .. " << std::endl;
      timer.start();
      ::viennacl::linalg::jacobi_precond<MatrixT>    preconditioner(A, ::viennacl::linalg::jacobi_tag());
      last_pc_time_ = timer.get();

      timer.start();
      x = ::viennacl::linalg::solve(A, b, linear_solver, preconditioner);
      last_solver_time_ = timer.get();
    }
    else
    if(pc_id_ == viennafvm::linsolv::viennacl::preconditioner_ids::row_scaling)
    {
//      std::cout << "using pc: row_scaling .. " << std::endl;
      timer.start();
      ::viennacl::linalg::row_scaling<MatrixT>    preconditioner(A, ::viennacl::linalg::row_scaling_tag());
      last_pc_time_ = timer.get();

      timer.start();
      x = ::viennacl::linalg::solve(A, b, linear_solver, preconditioner);
      last_solver_time_ = timer.get();
    }
    else
    {
      std::cerr << "[ERROR] ViennaFVM::LinearSolver: preconditioner not supported .. " << std::endl;
      return;
    }
    last_iterations_ = linear_solver.iters();
    last_error_      = linear_solver.error();
  }



  template <typename NumericT>
  void row_normalize_system(boost::numeric::ublas::compressed_matrix<NumericT> & A, 
                            boost::numeric::ublas::vector<NumericT>            & b)
  {
      typedef typename boost::numeric::ublas::compressed_matrix<NumericT>::iterator1    Iterator1;
      typedef typename boost::numeric::ublas::compressed_matrix<NumericT>::iterator2    Iterator2;

      //preprocessing: scale rows such that ||a_i|| = 1
      for (Iterator1 iter1  = A.begin1();
          iter1 != A.end1();
        ++iter1)
      {
        double row_norm = 0.0;
        for (Iterator2 iter2  = iter1.begin();
          iter2 != iter1.end();
        ++iter2)
        {
          row_norm += *iter2 * *iter2;
        }

        row_norm = sqrt(row_norm);

        if (A(iter1.index1(), iter1.index1()) < 0.0)
          row_norm *= -1.0;

        for (Iterator2 iter2  = iter1.begin();
             iter2 != iter1.end();
           ++iter2)
        {
          *iter2 /= row_norm;
        }
        b(iter1.index1()) /= row_norm;
      }
  }

private:
  long        pc_id_;
  long        solver_id_;
  double      break_tolerance_;
  std::size_t max_iterations_;

  std::size_t last_iterations_;
  double      last_error_;
  float       last_pc_time_;
  float       last_solver_time_;

};



} // end linsolv
} // viennafvm

#endif

