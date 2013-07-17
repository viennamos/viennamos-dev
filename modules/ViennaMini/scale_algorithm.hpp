#ifndef SCALE_ALGORITHM_HPP
#define SCALE_ALGORITHM_HPP

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

#include "viennagrid/forwards.h"
#include "viennagrid/domain.hpp"
#include "viennagrid/config/simplex.hpp"
#include "viennagrid/config/others.hpp"

#include "multiview.h"




void scale_algorithm(MultiView::Grid vtkGrid, double factor)
{
    if(factor != 1.0)
    {
        vtkPoints* points = vtkGrid->GetPoints();

        for(long i = 0; i < vtkGrid->GetPoints()->GetNumberOfPoints(); i++)
        {
            points->SetPoint(i, points->GetPoint(i)[0]*factor, points->GetPoint(i)[1]*factor, points->GetPoint(i)[2]*factor);
        }
    }

    // this is important, as it triggers a reevaluation of the grid,
    // such as the 'bounds' are recomputed
    vtkGrid->GetPoints()->Modified();
}

#endif // SCALE_ALGORITHM_HPP
