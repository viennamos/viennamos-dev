#ifndef VIENNAMINI_SIMULATOR_HPP
#define VIENNAMINI_SIMULATOR_HPP

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

/* Come up with a simulator object similar to ViennaSHE.
   Configuration should happen in a similar manner, allowing for the selection of predefined models (DD, Hydro, ev. ET)
*/

#ifndef NDEBUG
  #define NDEBUG
#endif

// ViennaFVM includes:

#define VIENNAFVM_VERBOSE

#include "viennafvm/forwards.h"
#include "viennafvm/linear_assembler.hpp"
#include "viennafvm/io/vtk_writer.hpp"
#include "viennafvm/boundary.hpp"
#include "viennafvm/pde_solver.hpp"
#include "viennafvm/initial_guess.hpp"
#ifdef VIENNACL_WITH_OPENCL
#include "viennafvm/viennacl_support.hpp"
#endif

// ViennaGrid includes:
#include "viennagrid/config/default_configs.hpp"
#include "viennagrid/io/netgen_reader.hpp"
#include "viennagrid/io/vtk_writer.hpp"
#include "viennagrid/algorithm/interface.hpp"
#include "viennagrid/algorithm/voronoi.hpp"
#include "viennagrid/algorithm/scale.hpp"

// ViennaMath includes:
#include "viennamath/expression.hpp"

//// ViennaMaterials includes:
//#include "viennamaterials/library.hpp"
//#include "viennamaterials/kernels/pugixml.hpp"

// Boost includes:
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/operation.hpp>
#include <boost/numeric/ublas/operation_sparse.hpp>

// ViennaMini includes:
#include "viennamini/fwd.h"
#include "viennamini/physics.hpp"
#include "viennamini/constants.hpp"
#include "viennamini/config.hpp"
#include "viennamini/device.hpp"
#include "viennamini/result_accessor.hpp"

namespace viennamini
{
    template<typename DeviceT, typename MatlibT>
    class simulator
    {
      public:
        typedef DeviceT                                                                         DeviceType;
        typedef MatlibT                                                                         MatlibType;
        typedef typename DeviceT::numeric_type                                                  NumericType;
        typedef typename DeviceT::indices_type                                                  IndicesType;
        typedef typename DeviceT::mesh_type                                                     MeshType;
        typedef typename DeviceT::storage_type                                                  StorageType;
        typedef typename DeviceT::segmentation_type                                             SegmentationType;
        typedef typename DeviceT::segment_type                                                  SegmentType;

        typedef typename viennagrid::result_of::cell_tag<MeshType>::type                        CellTagType;
        typedef typename viennagrid::result_of::facet_tag<MeshType>::type                       FacetTagType;
        typedef typename viennagrid::result_of::cell<MeshType>::type                            CellType;
        typedef typename viennagrid::result_of::facet<MeshType>::type                           FacetType;

        typedef typename viennagrid::result_of::cell_range<MeshType>::type                      CellRangeType;
        typedef typename viennagrid::result_of::iterator<CellRangeType>::type                   CellIteratorType;

        typedef viennamath::function_symbol                                                     FunctionSymbolType;
        typedef viennamath::equation                                                            EquationType;

        typedef viennafvm::linsolv::viennacl                                                    LinerSolverType;
        typedef viennafvm::pde_solver<>                                                         PDESolverType;
        typedef viennafvm::linear_pde_system<>                                                  PDESystemType;
        typedef viennafvm::boundary_key                                                         BoundaryKeyType;
        typedef viennafvm::current_iterate_key                                                  IterateKeyType;
        typedef viennafvm::ncell_quantity<CellType, viennamath::expr::interface_type>           QuantityType;

        typedef boost::numeric::ublas::vector<NumericType>                                      VectorType;
        typedef std::map<std::size_t, std::size_t>                                              IndexMapType;


        /**
            @brief C'tor activates the various models in the linear pde system and
            initializes objects
        */
        simulator(DeviceT& device, MatlibT& matlib, viennamini::config& config);

        /**
            @brief Public simulator 'execute' function. Performs all required
            step for conducting the device simulation. Requires the segments
            of the 'device' to be identified as contact/oxide/semiconductor.
            Also a doping is required, which will be retrieved from the device
            during the preparations.
        */
        void operator()();

        /**
            @brief Writes the doping phi,n,p to a vtk file
        */
        void write_device_doping();

        /**
            @brief Writes the initial guess distributions phi,n,p to a vtk file
        */
        void write_device_initial_guesses();

        void write_result(std::string filename = "viennamini_result");


    private:

        /**
            @brief Method identifies metal-semiconductor/oxide interfaces
        */
        void detect_interfaces();

        /**
            @brief Method identifies for a given segment under test whether it
            shares an interface with a reference contact segment
        */
        int find_adjacent_segment(SegmentType & current_contact_segment,
                                  IndicesType & segments_under_test);


        /**
            @brief Perform final steps required for the device simulation:
            1. assign dirichlet boundary conditions
            2. disable obsolete quantities
            3. assign initial guesses
            4. smooth initial guesses
        */
        void prepare();

        /**
            @brief Test whether the contact segment under test shares an interface with an insulator
        */
        bool isContactInsulatorInterface(std::size_t contact_segment_index);

        /**
            @brief Test whether the contact segment under test shares an interface with a semiconductor
        */
        bool isContactSemiconductorInterface(std::size_t contact_segment_index);

        void add_drift_diffusion();

        /**
            @brief Perform the device simulation. The device has been assigned an initial guess
            and boundary conditions at this point.
        */
        void run();

    public:
        FunctionSymbolType quantity_potential()        const;
        FunctionSymbolType quantity_electron_density() const;
        FunctionSymbolType quantity_hole_density()     const;

        VectorType const& result();

    private:
        DeviceType            & device_;
        MatlibType            & matlib_;
        viennamini::config    & config_;

        PDESystemType           pde_system_;
        PDESolverType           pde_solver_;
        LinerSolverType         linear_solver_;

        IndexMapType contactSemiconductorInterfaces_;
        IndexMapType contactOxideInterfaces_;

        viennamini::permittivity_key       eps_key_;
        viennamini::builtin_potential_key  builtin_key_;
        viennamini::donator_doping_key     ND_key_;
        viennamini::acceptor_doping_key    NA_key_;
        viennamini::mobility_electrons_key mu_n_key_;
        viennamini::mobility_holes_key     mu_p_key_;

        QuantityType  eps_;
        QuantityType  ND_;
        QuantityType  NA_;
        QuantityType  mu_n_;
        QuantityType  mu_p_;

        int notfound_;
    };
}

#endif
