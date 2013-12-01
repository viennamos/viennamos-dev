 #ifndef VIENNAMINIFORM_H
#define VIENNAMINIFORM_H

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

#include <QWidget>
#include <QSettings>

#include <vector>

#include "render3d.h"

#include "viennamini/simulator.hpp"


namespace Ui {
class ViennaMiniForm;
}

class ViennaMiniForm : public QWidget
{
    Q_OBJECT

  enum column_ids
  {
    COLOR_COLUMN,
    ID_COLUMN,
    NAME_COLUMN,
    TYPE_COLUMN,
    MATERIAL_COLUMN
  };

public:
    explicit ViennaMiniForm(QWidget *parent = 0);
    ~ViennaMiniForm();
    void process(viennamini::simulator_handle vmini_simulator, Render3D* renderer);

public slots:
    void saveState(QSettings& settings);
    void loadState(QSettings& settings);

private slots:
    void showSegmentParameters(int row, int col = -1, int prev_row = -1, int prev_col = -1);
    void setTemperature(QString const& value_str);
    void setLinearTolerance(QString const& value_str);
    void setLinearIterations(QString const& value_str);
    void setNonLinearTolerance(QString const& value_str);
    void setNonLinearIterations(QString const& value_str);
    void setNonLinearDamping(QString const& value_str);
    void setNewProblem(QString const& problem_str);
    void setSegmentContactIsSingle(bool state);
    void setSegmentContactContactValue(QString const& value_str);
    void setSegmentContactIsRange(bool state);
    void setSegmentContactContactFrom(QString const& value_str);
    void setSegmentContactContactTo(QString const& value_str);
    void setSegmentContactContactDelta(QString const& value_str);
    void setSegmentContactWorkfunction(QString const& value_str);
    void setSegmentContactRecordIV(bool state);
    void setSegmentSemiconductorMobilityLattice(bool state);
    void setSegmentSemiconductorMobilityIoniziedImpurity(bool state);
    void setSegmentSemiconductorRecombinationSRH(bool state);

private:
    Ui::ViennaMiniForm           *ui;
    viennamini::simulator_handle  vmini_simulator_;

};

#endif // VIENNAMINIFORM_H
