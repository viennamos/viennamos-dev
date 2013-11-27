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



//typedef viennagrid::mesh<viennagrid::config::line_1d>            CellComplex1u;
//typedef viennagrid::mesh<viennagrid::config::triangular_2d>      CellComplex2u;
//typedef viennagrid::mesh<viennagrid::config::quadrilateral_2d>   CellComplex2s;
//typedef viennagrid::mesh<viennagrid::config::tetrahedral_3d>     CellComplex3u;
//typedef viennagrid::mesh<viennagrid::config::hexahedral_3d>      CellComplex3s;

//typedef viennagrid::result_of::segmentation<CellComplex1u>::type     Segmentation1u;
//typedef viennagrid::result_of::segmentation<CellComplex2u>::type     Segmentation2u;
//typedef viennagrid::result_of::segmentation<CellComplex2s>::type     Segmentation2s;
//typedef viennagrid::result_of::segmentation<CellComplex3u>::type     Segmentation3u;
//typedef viennagrid::result_of::segmentation<CellComplex3s>::type     Segmentation3s;

//typedef viennadata::storage<>                                        QuanComplex;


//namespace key {
//static const QString vdevice1u = "Line1D";
//static const QString vdevice2u = "Triangular2D";
//static const QString vdevice2s = "Quadrilateral2D";
//static const QString vdevice3u = "Tetrahedral3D";
//static const QString vdevice3s = "Hexahedral3D";
//} // key


//enum DEVICES_ID {
//    DEVICE_2U,
//    DEVICE_2S,
//    DEVICE_3U,
//    DEVICE_3S
//};


//struct Device2u
//{
//public:
//    typedef viennamos::QuanComplex        QuantityComplex;
//    typedef viennamos::CellComplex2u      CellComplex;
//    typedef viennamos::Segmentation2u     Segmentation;

//    Device2u() : segmentation_(domain_) {};
//    static int         ID()  { return DEVICE_2U; }
//    static std::string KEY() { return key::vdevice2u.toStdString(); }

//    QuantityComplex& getQuantityComplex() { return quantities_;}
//    CellComplex&     getCellComplex    () { return domain_;    }
//    Segmentation&    getSegmentation   () { return segmentation_; }

//private:
//    QuantityComplex      quantities_;
//    CellComplex          domain_;
//    Segmentation         segmentation_;
//};

//struct Device2s
//{
//public:
//    typedef viennamos::QuanComplex        QuantityComplex;
//    typedef viennamos::CellComplex2s      CellComplex;
//    typedef viennamos::Segmentation2s     Segmentation;

//    Device2s() : segmentation_(domain_) {};
//    static int         ID()  { return DEVICE_2S; }
//    static std::string KEY() { return key::vdevice2s.toStdString(); }

//    QuantityComplex& getQuantityComplex() { return quantities_;}
//    CellComplex&     getCellComplex    () { return domain_;    }
//    Segmentation&    getSegmentation   () { return segmentation_; }

//private:
//    QuantityComplex      quantities_;
//    CellComplex          domain_;
//    Segmentation         segmentation_;
//};

//struct Device3u
//{
//public:
//    typedef viennamos::QuanComplex        QuantityComplex;
//    typedef viennamos::CellComplex3u      CellComplex;
//    typedef viennamos::Segmentation3u     Segmentation;

//    Device3u() : segmentation_(domain_) {};
//    static int         ID()  { return DEVICE_3U; }
//    static std::string KEY() { return key::vdevice3u.toStdString(); }

//    QuantityComplex& getQuantityComplex() { return quantities_;}
//    CellComplex&     getCellComplex    () { return domain_;    }
//    Segmentation&    getSegmentation   () { return segmentation_; }

//private:
//    QuantityComplex      quantities_;
//    CellComplex          domain_;
//    Segmentation         segmentation_;
//};

//struct Device3s
//{
//public:
//    typedef viennamos::QuanComplex        QuantityComplex;
//    typedef viennamos::CellComplex3s      CellComplex;
//    typedef viennamos::Segmentation3s     Segmentation;

//    Device3s() : segmentation_(domain_) {};
//    static int         ID()  { return DEVICE_3S; }
//    static std::string KEY() { return key::vdevice3s.toStdString(); }

//    QuantityComplex& getQuantityComplex() { return quantities_;}
//    CellComplex&     getCellComplex    () { return domain_;    }
//    Segmentation&    getSegmentation   () { return segmentation_; }

//private:
//    QuantityComplex      quantities_;
//    CellComplex          domain_;
//    Segmentation         segmentation_;
//};

} // viennamos

#endif // DEVICE_HPP





