#ifndef VIENNAMOS_FORWARDS_HPP
#define VIENNAMOS_FORWARDS_HPP

/*
 *
 * Copyright (c) 2013, Institute for Microelectronics, TU Wien.
 *
 * This file is part of ViennaMOS     http://viennamos.sourceforge.net/
 *
 * Contact: Josef Weinbub             weinbub@iue.tuwien.ac.at
 *
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

// ViennaGrid includes
//
#include "viennagrid/forwards.hpp"
#include "viennagrid/config/default_configs.hpp"
#include "viennagrid/mesh/segmented_mesh.hpp"

// ViennaFVM includes
//
#include "viennafvm/forwards.h"
#include "viennafvm/problem_description.hpp"

// Boost includes
//
#include "boost/shared_ptr.hpp"

namespace viennamos 
{
  // Tags
  //
  class null {};
  

  // Typedefs
  //
  typedef ::viennagrid::mesh< viennagrid::config::line_1d >                                                       mesh_line_1d;
  typedef ::viennagrid::mesh< viennagrid::config::triangular_2d >                                                 mesh_triangular_2d;
  typedef ::viennagrid::mesh< viennagrid::config::tetrahedral_3d >                                                mesh_tetrahedral_3d;

  typedef ::viennagrid::result_of::segmentation<mesh_line_1d>::type                                               segmentation_line_1d;
  typedef ::viennagrid::result_of::segmentation<mesh_triangular_2d>::type                                         segmentation_triangular_2d;
  typedef ::viennagrid::result_of::segmentation<mesh_tetrahedral_3d>::type                                        segmentation_tetrahedral_3d;

  typedef segmentation_line_1d::segment_handle_type                                                               segment_line_1d;
  typedef segmentation_triangular_2d::segment_handle_type                                                         segment_triangular_2d;
  typedef segmentation_tetrahedral_3d::segment_handle_type                                                        segment_tetrahedral_3d;


  typedef ::viennagrid::segmented_mesh<viennamos::mesh_line_1d,        viennamos::segmentation_line_1d>           segmesh_line_1d;
  typedef ::viennagrid::segmented_mesh<viennamos::mesh_triangular_2d,  viennamos::segmentation_triangular_2d>     segmesh_triangular_2d;
  typedef ::viennagrid::segmented_mesh<viennamos::mesh_tetrahedral_3d, viennamos::segmentation_tetrahedral_3d>    segmesh_tetrahedral_3d;

  typedef ::boost::shared_ptr<segmesh_line_1d>                                                                    segmesh_line_1d_ptr;
  typedef ::boost::shared_ptr<segmesh_triangular_2d>                                                              segmesh_triangular_2d_ptr;
  typedef ::boost::shared_ptr<segmesh_tetrahedral_3d>                                                             segmesh_tetrahedral_3d_ptr;

  typedef ::viennafvm::problem_description<mesh_line_1d>                                                          problem_description_line_1d;
  typedef ::viennafvm::problem_description<mesh_triangular_2d>                                                    problem_description_triangular_2d;
  typedef ::viennafvm::problem_description<mesh_tetrahedral_3d>                                                   problem_description_tetrahedral_3d;

  typedef std::vector<problem_description_line_1d>                                                                problem_description_line_1d_set;
  typedef std::vector<problem_description_triangular_2d>                                                          problem_description_triangular_2d_set;
  typedef std::vector<problem_description_tetrahedral_3d>                                                         problem_description_tetrahedral_3d_set;
} // viennamos

#endif

