#ifndef DEVICE_GENERATOR_FORM_H
#define DEVICE_GENERATOR_FORM_H

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

#include "multiview.h"

// ViennaMini includes
//
#include "viennamini/device.hpp"

// Local includes
//
#include "forwards.h"

namespace Ui {
class DeviceGeneratorForm;
}

class DeviceGeneratorForm : public QWidget
{
  Q_OBJECT

  enum column_ids
  {
    COLOR_COLUMN,
    ID_COLUMN
  };


public:

//  typedef std::vector<std::std::string >

  explicit DeviceGeneratorForm(QWidget *parent = 0);
  ~DeviceGeneratorForm();
  void process(viennamini::device_handle& vmini_device, Render3D* renderer);
  QString getMeshType();

public slots:
    void saveState(QSettings& settings);
    void loadState(QSettings& settings);

signals:
    void meshFileEntered(QString const& filename);
    void csggeneratorTriggered();
    void scaleDevice(double factor);

private slots:
    void on_pushButtonLoadMesh_clicked();
    void on_pushButtonScaleDevice_clicked();

    void showSegmentParameters(int row, int col = -1, int prev_row = -1, int prev_col = -1);
    void setSegmentName(QString const& name);
    void setSegmentMaterial(QString const& name);
    void setSegmentSCAcceptors(QString const& value_str);
    void setSegmentSCDonors(QString const& value_str);
    void makeCurrentSegmentContact(bool state);
    void makeCurrentSegmentOxide(bool state);
    void makeCurrentSegmentSemiconductor(bool state);
    void toggleSegmentContact(bool state);
    void toggleSegmentOxide(bool state);
    void toggleSegmentSemiconductor(bool state);
    void toggleParameters(bool state);
    void on_pushButtonCSGEditor_clicked();

private:
  Ui::DeviceGeneratorForm *ui;
  QString             meshfile;

  std::map<std::string, std::map<int, std::map<std::string, double> > >   device_aux_quantity;

  viennamini::device_handle vmini_device_;
};

#endif // DEVICE_GENERATOR_H
