#ifndef DEVICE_H
#define DEVICE_H

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


// Local includes
//
#include "forwards.hpp"

// Boost includes
//
#include "boost/variant.hpp"

namespace viennamos {


class device
{
private:
  typedef boost::variant<null, segmesh_line_1d_ptr,             segmesh_triangular_2d_ptr,             segmesh_tetrahedral_3d_ptr>             GenericMeshType;
  typedef boost::variant<null, problem_description_line_1d_set, problem_description_triangular_2d_set, problem_description_tetrahedral_3d_set> GenericProblemDescriptionType;

public:

  typedef GenericMeshType                                                                generic_mesh_type;
  typedef GenericProblemDescriptionType                                                  generic_problem_description_type;

  device();
  ~device();

  /*! \brief Returns a static string identification for this class
  *
  * Required to model ViennaMOS's database concept, i.e., this class
  * can be stored in the database
  */
  static const std::string ID() { return "vmos_device"; }

  void make_line1d();
  void make_triangular2d();
  void make_tetrahedral3d();

  bool is_line1d();
  bool is_triangular2d();
  bool is_tetrahedral3d();

  segmesh_line_1d&        get_segmesh_line_1d();
  segmesh_triangular_2d&  get_segmesh_triangular_2d();
  segmesh_tetrahedral_3d& get_segmesh_tetrahedral_3d();

  problem_description_line_1d&        get_problem_description_line_1d        (std::size_t id = 0);
  problem_description_triangular_2d&  get_problem_description_triangular_2d  (std::size_t id = 0);
  problem_description_tetrahedral_3d& get_problem_description_tetrahedral_3d (std::size_t id = 0);

  problem_description_line_1d_set&        get_problem_description_line_1d_set        ();
  problem_description_triangular_2d_set&  get_problem_description_triangular_2d_set  ();
  problem_description_tetrahedral_3d_set& get_problem_description_tetrahedral_3d_set ();

private:
  GenericMeshType               generic_mesh_;
  GenericProblemDescriptionType generic_problem_description_set_;
};

} // viennamos

#endif // DEVICE_HPP





