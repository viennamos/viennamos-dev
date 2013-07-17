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

#include "deviceparameters.hpp"
#include "materialmanager.h"

namespace Ui {
class ViennaMiniForm;
}

class ViennaMiniForm : public QWidget
{
    Q_OBJECT

public:

    explicit ViennaMiniForm(QWidget *parent = 0);
    ~ViennaMiniForm();
    QString getMeshType();
    double getScaling();
    double getTemperature();
    void setupDevice(int number_of_segments);
    DeviceParameters& getParameters();


signals:
    void meshFileEntered(QString const& filename);

public slots:
    void saveState(QSettings& settings);
    void loadState(QSettings& settings);
    void setMaterialLibrary(MaterialManager::Library& lib);

private slots:
    void on_pushButtonLoadMesh_clicked();
    void setTemperature(QString const& value_str);
    void setLinearTolerance(QString const& value_str);
    void setLinearIterations(QString const& value_str);
    void setNonLinearTolerance(QString const& value_str);
    void setNonLinearIterations(QString const& value_str);
    void setNonLinearDamping(QString const& value_str);
    void showSegmentParameters(int row, int col = -1, int prev_row = -1, int prev_col = -1);
    void setSegmentName(QString const& name);
    void setSegmentMaterial(QString const& name);
    void setSegmentContactIsSingle(bool state);
    void setSegmentContactContactValue(QString const& value_str);
    void setSegmentContactIsRange(bool state);
    void setSegmentContactContactFrom(QString const& value_str);
    void setSegmentContactContactTo(QString const& value_str);
    void setSegmentContactWorkfunction(QString const& value_str);
    void setSegmentSCAcceptors(QString const& value_str);
    void setSegmentSCDonors(QString const& value_str);
    void makeCurrentSegmentContact(bool state);
    void makeCurrentSegmentOxide(bool state);
    void makeCurrentSegmentSemiconductor(bool state);
    void toggleSegmentContact(bool state);
    void toggleSegmentOxide(bool state);
    void toggleSegmentSemiconductor(bool state);
    void toggleParameters(bool state);

private:
    Ui::ViennaMiniForm *ui;
    DeviceParameters device_parameters;
    QString      meshfile;
    QStringList  list_oxides;
    QStringList  list_metals;
    QStringList  list_semiconductors;
    bool         resize_device_parameters;
};

#endif // VIENNAMINIFORM_H
