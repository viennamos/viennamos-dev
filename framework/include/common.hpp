#ifndef COMMON_HPP
#define COMMON_HPP

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

#include <QString>
#include <QMap>
#include <QDir>
#include <QCoreApplication>

//#include "module_interface.h"
//#include "viennashe/device.hpp"



enum CELL_DATA {
    VERTEX,
    EDGE,
    FACE,
    CELL
};

enum DATA_TENSOR_LVL {
    SCALAR,
    VECTOR,
    TENSOR
};



//enum QUANTITIES {
//    QUAN_POTENTIAL,
//    QUAN_ELECTRON_CONCENTRATION,
//    QUAN_HOLE_CONCENTRATION,
//    QUAN_ACCEPTOR_DOPING,
//    QUAN_DONOR_DOPING
//};

enum VCR {
    FIRST,
    PREVIOUS,
    PLAY,
    NEXT,
    LAST,
    LOOP
};


namespace viennamos {

inline std::string generateDisplayName(std::string const& value, std::string const& unit)
{
    return value + " (" + unit + ")";
}

inline QString getRelativePath(QString absolute_path)
{
    QDir dir(QCoreApplication::applicationDirPath());
    return dir.relativeFilePath(absolute_path);
}

} //  viennamos

namespace key {

//static const QString vgdomain1u = "Line1D";
//static const QString vgdomain2u = "Triangular2D";
//static const QString vgdomain2s = "Quadrilateral2D";
//static const QString vgdomain3u = "Tetrahedral3D";
//static const QString vgdomain3s = "Hexahedral3D";



static const QString surface      = "Surface";
static const QString surfaceEdges = "SurfaceWithEdges";
static const QString points       = "Points";
static const QString wireframe    = "Wireframe";

static const QString solid_color = "Solid Color";
static const QString segment_index = "Segment ID";

static const QString state_suffix = "ini";
static const QString state_file_filter = "State files (*."+key::state_suffix+");;All files (*.*)";

static const QString png_suffix = "png";
static const QString png_filter = "PNG files (*."+key::png_suffix+")";

} // namespace key

namespace tag {

struct vdevice {};
struct vdomain {};

} // namespace tag





//typedef viennashe::device<VGDomain1u>                                               VDevice1u;
//typedef viennashe::device<VGDomain2u>                                               VDevice2u;
//typedef viennashe::device<VGDomain2s>                                               VDevice2s;
//typedef viennashe::device<VGDomain3u>                                               VDevice3u;
//typedef viennashe::device<VGDomain3s>                                               VDevice3s;


#endif // COMMON_HPP
