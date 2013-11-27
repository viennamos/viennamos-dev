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
 #include "device.h"


namespace viennamos {

// -----------------------------------------------------------------------------
//
// Boost Variant Visitors
//
struct is_line_1d_visitor : public boost::static_visitor<bool>
{
  bool operator()(segmesh_line_1d_ptr & ptr) const { return true; }

  template<typename T>
  bool operator()(T & ptr) const { return false; }
};

struct is_triangle_2d_visitor : public boost::static_visitor<bool>
{
  bool operator()(segmesh_triangular_2d_ptr & ptr) const { return true; }

  template<typename T>
  bool operator()(T & ptr) const { return false; }
};

struct is_tetrahedral_3d_visitor : public boost::static_visitor<bool>
{
  bool operator()(segmesh_tetrahedral_3d_ptr & ptr) const { return true; }

  template<typename T>
  bool operator()(T & ptr) const { return false; }
};


// -----------------------------------------------------------------------------
//
// Member Implementations
//

device::device()
{
}

device::~device()
{
}

void device::make_line1d()
{
  generic_mesh_                     = segmesh_line_1d_ptr(new segmesh_line_1d_ptr::element_type);
  
  generic_problem_description_set_  = problem_description_line_1d_set();
  this->get_problem_description_line_1d_set().push_back( problem_description_line_1d(get_segmesh_line_1d().mesh) );
}

void device::make_triangular2d()
{
  generic_mesh_                = segmesh_triangular_2d_ptr(new segmesh_triangular_2d_ptr::element_type);

  generic_problem_description_set_  = problem_description_triangular_2d_set();
  this->get_problem_description_triangular_2d_set().push_back( problem_description_triangular_2d(get_segmesh_triangular_2d().mesh) );
}

void device::make_tetrahedral3d()
{
  generic_mesh_                 = segmesh_tetrahedral_3d_ptr(new segmesh_tetrahedral_3d_ptr::element_type);

  generic_problem_description_set_  = problem_description_tetrahedral_3d_set();
  this->get_problem_description_tetrahedral_3d_set().push_back( problem_description_tetrahedral_3d(get_segmesh_tetrahedral_3d().mesh) );
}

bool device::is_line1d()
{
  return boost::apply_visitor(is_line_1d_visitor(), generic_mesh_);
}

bool device::is_triangular2d()
{
  return boost::apply_visitor(is_triangle_2d_visitor(), generic_mesh_);
}

bool device::is_tetrahedral3d()
{
  return boost::apply_visitor(is_tetrahedral_3d_visitor(), generic_mesh_);
}

segmesh_line_1d& device::get_segmesh_line_1d()
{
  return *boost::get<segmesh_line_1d_ptr>(generic_mesh_);
}

segmesh_triangular_2d& device::get_segmesh_triangular_2d()
{
  return *boost::get<segmesh_triangular_2d_ptr>(generic_mesh_);
}

segmesh_tetrahedral_3d& device::get_segmesh_tetrahedral_3d()
{
  return *boost::get<segmesh_tetrahedral_3d_ptr>(generic_mesh_);
}

problem_description_line_1d_set&  device::get_problem_description_line_1d_set()
{
  return boost::get<problem_description_line_1d_set>(generic_problem_description_set_);
}

problem_description_triangular_2d_set&  device::get_problem_description_triangular_2d_set()
{
  return boost::get<problem_description_triangular_2d_set>(generic_problem_description_set_);
}

problem_description_tetrahedral_3d_set& device::get_problem_description_tetrahedral_3d_set()
{
  return boost::get<problem_description_tetrahedral_3d_set>(generic_problem_description_set_);
}

problem_description_line_1d&  device::get_problem_description_line_1d(std::size_t id)
{
  return boost::get<problem_description_line_1d_set>(generic_problem_description_set_)[id];
}

problem_description_triangular_2d&  device::get_problem_description_triangular_2d(std::size_t id)
{
  return boost::get<problem_description_triangular_2d_set>(generic_problem_description_set_)[id];
}

problem_description_tetrahedral_3d& device::get_problem_description_tetrahedral_3d(std::size_t id)
{
  return boost::get<problem_description_tetrahedral_3d_set>(generic_problem_description_set_)[id];
}

} // viennamos


