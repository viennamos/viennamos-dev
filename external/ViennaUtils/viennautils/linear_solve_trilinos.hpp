/* =============================================================================
   Copyright (c) 2012, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
                 ViennaX - The Vienna eXecution Framework
                             -----------------

   authors:    Josef Weinbub                         weinbub@iue.tuwien.ac.at
               Rene Heinzl
               Philipp Schwaha
               
   license:    see file LICENSE in the base directory
============================================================================= */


#ifndef VIENNAUTILS_LINSOLVE_TRILINOS_HPP
#define VIENNAUTILS_LINSOLVE_TRILINOS_HPP

// TRILINOS includes
//
#include "Epetra_ConfigDefs.h"
#ifdef VX_HAVE_MPI
   #include "mpi.h"
   #include "Epetra_MpiComm.h"
#else
   #include "Epetra_SerialComm.h"
#endif
#include "Epetra_Map.h"
#include "Epetra_Vector.h"
#include "Epetra_RowMatrix.h"
#include "Epetra_CrsMatrix.h"
#include "AztecOO.h"
#include "EpetraExt_CrsMatrixIn.h"
#include "EpetraExt_VectorIn.h"
#include "EpetraExt_VectorOut.h"

// Boost includes
//
#include <boost/shared_ptr.hpp>

//#define PRINT_WORK_MATRICES



namespace viennautils {

// -----------------------------------------------------------------------------
struct TrilinosLinearSolver
{
  // --------------------------------------------------------------------------
  // Trilinos specific internal datatypes ...
  typedef int                                   IndexT;
  typedef double                                NumericT;
#ifdef VX_HAVE_MPI
  typedef boost::shared_ptr<Epetra_MpiComm>     MPIComm;
#else 
  typedef boost::shared_ptr<Epetra_SerialComm>  SerialComm;
#endif
  typedef boost::shared_ptr<Epetra_Map>         Map;
  typedef boost::shared_ptr<Epetra_CrsMatrix>   TrilMatrix;
  typedef boost::shared_ptr<Epetra_Vector>      TrilVector;
  typedef boost::shared_ptr<AztecOO>            TrilSolver;
  // --------------------------------------------------------------------------
  TrilinosLinearSolver()  
  {
    this->init();
  }

  TrilinosLinearSolver(IndexT problem_size)  
  {
    this->init();

    //std::cout << "process " << (*comm).MyPID() << " of " << (*comm).NumProc() << std::endl;

    // notes on Trilinos global/local indices:
    // we provide the map object with the global number of rows, represented by our variable
    // 'problem_size'. the rows correspond to the rows of the system matrix A aswell as 
    // to the vector length (again, rows) of the rhs vector.
    // the map object automatically defers the 'local' number of responsible rows for 
    // each mpi process via the comm object.
    // therefore dumping (*map).NumMyElements() yields the local (process specific) number
    // of responsible rows. 
    //
    map = Map       (new Epetra_Map(problem_size, 0, *comm));
    //std::cout << "## my numb ele: " << (*map).NumMyElements() << std::endl;
    A_  = TrilMatrix(new Epetra_CrsMatrix(Copy, *map, 0));  
    b_  = TrilVector(new Epetra_Vector(*map));  
    x_  = TrilVector(new Epetra_Vector(*map));
    (*b_).PutScalar(0.0);
    (*x_).PutScalar(0.0);
  }
  // --------------------------------------------------------------------------

  // --------------------------------------------------------------------------
//  void read_problem_matrix_market(std::string fileA, std::string fileb)
//  {
//    EpetraExt::MatrixMarketFileToCrsMatrix(fileA.c_str(), *comm, A_.get());
//    map = Map(new Epetra_Map((*A_).NumGlobalRows(),0,*comm));      

//    EpetraExt::MatrixMarketFileToVector(fileb.c_str(), *map, b_.get());

//    x_  = TrilVector(new Epetra_Vector((*A_).Map()));
//    (*x_).PutScalar(0.0);
//  }
  // --------------------------------------------------------------------------

//  // --------------------------------------------------------------------------
//  void write_solution_matrix_market(std::string file)
//  {
//    EpetraExt::VectorToMatrixMarketFile(file.c_str(), *x_);
//  }
//  // --------------------------------------------------------------------------

  // --------------------------------------------------------------------------
  template<typename NumericVectorT, typename IndexVectorT>
  inline void insert_row(IndexT const& row_index, NumericVectorT const& row, 
                         IndexVectorT const& colindices)
  {
    // here it is very important to work with the global insertion method, 
    // to ensure that the input data is correctly distributed between the 
    // processes' datastructures
    (*A_).InsertGlobalValues(row_index, row.size(), &(row[0]), &(colindices[0]));
    //(*A_).InsertMyValues(local_matrix_row_counter++, row.size(), &(row[0]), &(colindices[0]));
  }
  // --------------------------------------------------------------------------
  template<typename NumericVectorT, typename IndexVectorT>
  void load_rhs(NumericVectorT const& rhs, IndexVectorT const& indices)
  {
    // here it is very important to work with the global insertion method, 
    // to ensure that the input data is correctly distributed between the 
    // processes' datastructures
    (*b_).ReplaceGlobalValues(rhs.size(), &(rhs[0]), &(indices[0]));
    
//    IndexVectorT tempindices((*map).NumMyElements());
//    for(std::size_t i = 0; i < (*map).NumMyElements(); i++)
//      tempindices[i] = i;
//    (*b_).ReplaceMyValues(rhs.size(), &(rhs[0]), &(tempindices[0]));
  }

  template<typename NumericVectorT>
  void extract_solution(NumericVectorT & solution)
  {
    (*x_).ExtractCopy(&(solution[0]));
  }

  // --------------------------------------------------------------------------
  inline std::size_t local_row_size()  { return (*map).NumMyElements(); }
  // --------------------------------------------------------------------------

  // --------------------------------------------------------------------------
  inline NumericT& b(IndexT const& row)  { return (*b_)[row]; }
  // --------------------------------------------------------------------------

  // --------------------------------------------------------------------------
  inline NumericT& x(IndexT const& row)  { return (*x_)[row]; }
  // --------------------------------------------------------------------------

  // --------------------------------------------------------------------------
  void dump_sol(std::ostream& stream = std::cout)
  {
    (*x_).Print(stream);
  }

  void dump_sol(std::string xfile)
  {
    std::ofstream fileout(xfile.c_str());
    (*x_).Print(fileout);
    fileout.close();
  }

  void dump_linsys(std::ostream& stream = std::cout)
  {
    (*A_).Print(stream);
    (*b_).Print(stream);
  }

  void dump_linsys(std::string Afile, std::string bfile)
  {
    {
    std::ofstream fileout(Afile.c_str());
    (*A_).Print(fileout);
    fileout.close();
    }
    {
    std::ofstream fileout(bfile.c_str());
    (*b_).Print(fileout);
    fileout.close();
    }
  }
  // --------------------------------------------------------------------------

  // --------------------------------------------------------------------------
  NumericT    true_residual()   { return (*solver).TrueResidual(); }
  IndexT&     maxiters()        { return maxiters_; }
  NumericT&   maxaccur()        { return maxaccur_; }
  // --------------------------------------------------------------------------

  // --------------------------------------------------------------------------
  bool solve()
  {
    this->prepare();

    // aztec options according to Aztec User's Guide

    // ------------------------------------------------------------
    // scaling
    //   
    //solver->SetAztecOption(AZ_scaling,   AZ_none);
    //solver->SetAztecOption(AZ_scaling,   AZ_Jacobi);
    //solver->SetAztecOption(AZ_scaling,   AZ_BJacobi);
    solver->SetAztecOption(AZ_scaling,   AZ_row_sum);
    //solver->SetAztecOption(AZ_scaling,   AZ_sym_diag);
    //solver->SetAztecOption(AZ_scaling,   AZ_sym_row_sum);
    // ------------------------------------------------------------   

    // ------------------------------------------------------------
    // iterative solvers
    //
    //solver->SetAztecOption(AZ_solver,   AZ_cg);            
    //solver->SetAztecOption(AZ_solver,   AZ_gmres);
    //solver->SetAztecOption(AZ_solver,   AZ_cgs);      
    //solver->SetAztecOption(AZ_solver,   AZ_tfqmr);      
    solver->SetAztecOption(AZ_solver,   AZ_bicgstab);      
    //solver->SetAztecOption(AZ_solver,   AZ_lu);            
    // ------------------------------------------------------------

    // ------------------------------------------------------------
    // preconditioners
    //      
    //solver->SetAztecOption(AZ_precond,  AZ_none);   
    //solver->SetAztecOption(AZ_precond,  AZ_Jacobi);
    //solver->SetAztecOption(AZ_precond,  AZ_Neumann);            
    //solver->SetAztecOption(AZ_precond,  AZ_ls);         
    //solver->SetAztecOption(AZ_precond,  AZ_lu);         
    //solver->SetAztecOption(AZ_precond,  AZ_ilu);         
    //solver->SetAztecOption(AZ_precond,  AZ_bilu);         
    //solver->SetAztecOption(AZ_precond,  AZ_sym_GS);         
    // ------------------------------------------------------------   

    // ------------------------------------------------------------
    // residual for convergence checks
    //         
    solver->SetAztecOption(AZ_conv,  AZ_r0);   
    //solver->SetAztecOption(AZ_conv,  AZ_rhs);
    //solver->SetAztecOption(AZ_conv,  AZ_Anorm);            
    //solver->SetAztecOption(AZ_conv,  AZ_sol);         
    //solver->SetAztecOption(AZ_conv,  AZ_weighted);         
    // ------------------------------------------------------------

    // debug output!
    solver->SetAztecOption(AZ_output,  AZ_none);
    //solver->SetAztecOption(AZ_output,  AZ_last);      


    int err = (*solver).Iterate(maxiters_, maxaccur_);
    if (err != 0) 
    {
      std::cerr << "solver error .." << std::endl;
      return false;
    }
    if ((*solver).NumIters() ==0 || (*solver).NumIters() >= maxiters_) 
    {
      std::cerr << "solver iterations are bad .." << std::endl;
      return false;
    }
    return true;
  }
  // --------------------------------------------------------------------------

//private:

  // --------------------------------------------------------------------------
  void init()
  {
    #ifdef VX_HAVE_MPI
      comm = MPIComm(new Epetra_MpiComm(MPI_COMM_WORLD));
//      std::cout << "[Trilinos][MPI] proc: " << MPI::COMM_WORLD.Get_rank() << " of " 
//                                            << MPI::COMM_WORLD.Get_size() << std::endl;
    #else
//      std::cout << "[Trilinos][SERIAL] " << std::endl;
      comm = SerialComm(new Epetra_SerialComm());
    #endif

    maxiters_ = 2000;
    maxaccur_ = 1.0E-12;  
    local_matrix_row_counter  = 0; 
    local_rhs_row_counter = 0;
  }
  // --------------------------------------------------------------------------
  
  // --------------------------------------------------------------------------
  void prepare()
  {
    // process the matrix for efficient computation and communication
    // internally calls matrix optimization method
    //
    (*A_).FillComplete();       
    solver = TrilSolver(new AztecOO(Epetra_LinearProblem(A_.get(), x_.get(), b_.get())));
  }
  // --------------------------------------------------------------------------
#ifdef VX_HAVE_MPI
  MPIComm comm;
#else
  SerialComm comm;
#endif
  Map               map; 
  TrilMatrix        A_; 
  TrilVector        x_, b_;
  TrilSolver        solver;
  IndexT            maxiters_;
  NumericT          maxaccur_;
  std::size_t       local_matrix_row_counter, local_rhs_row_counter;
};
// -----------------------------------------------------------------------------


template <typename MatrixType, typename VectorType> // Boost.Ublas !
VectorType solve_trilinos(MatrixType & system_matrix,
                          VectorType & load_vector,
                          std::size_t const& max_iters, 
                          double      const& breaktol)
{
    std::cout << "* solve(): Normalizing rows... " << std::endl;
    viennafvm::row_normalize_system(system_matrix, load_vector);

  // setup the trilinos solver object
  //
  int rows = load_vector.size();
  TrilinosLinearSolver  solver(rows);
  solver.maxiters() = max_iters;
  solver.maxaccur() = breaktol;

  // transfer the matrix to trilinos
  //
  for(typename MatrixType::const_iterator1 row_iter = system_matrix.begin1(); row_iter != system_matrix.end1(); row_iter++)
  {
    std::vector<double>   row_data;
    std::vector<int>      col_indices;
  
    for(typename MatrixType::const_iterator2 col_iter = row_iter.begin(); col_iter != row_iter.end(); col_iter++)
    {
      row_data.push_back( *col_iter );
      col_indices.push_back( col_iter.index2() );
    }
    
    solver.insert_row(row_iter.index1(), row_data, col_indices);
  }

  // transfer the rhs to trilinos
  //
  std::vector<int>      indices(rows);
  for(int i = 0; i < rows; i++) indices[i] = i;
  solver.load_rhs(load_vector, indices);
  
  solver.solve();
  
  VectorType result(rows);
  solver.extract_solution(result);
  
  return result;
}

} // viennautils


#endif

