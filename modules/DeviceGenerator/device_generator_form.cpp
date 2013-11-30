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

#include <QDir>
#include <QFileDialog>
#include <QDebug>

// Framework includes
//
#include "forwards.hpp"

// Module includes
//
#include "device_generator_form.h"
#include "ui_device_generator_form.h"

#include "viennamaterials/library.hpp"
#include "viennamini/material_accessors.hpp"

DeviceGeneratorForm::DeviceGeneratorForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeviceGeneratorForm)
{
    ui->setupUi(this);

    ui->tabWidget->setTabText(0, "General");
    ui->tabWidget->setTabText(1, "Device");

    // deactivate the device tab for now:
    // note: it makes only sense to enable it, when a mesh has been loaded
    // and the number of segments is available ...
    ui->tabWidget->setTabEnabled(1, false);

    ui->comboBoxMeshType->addItem(viennamos::key::triangular2d);
    ui->comboBoxMeshType->addItem(viennamos::key::tetrahedral3d);
    ui->comboBoxMeshType->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    QDoubleValidator* double_validator = new QDoubleValidator(-std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), 10, this);
    QIntValidator*    integer_validator = new QIntValidator(this);

    ui->lineEditScalingFactor->setValidator(double_validator);
    ui->lineEditSemiconductorAcceptors->setValidator(double_validator);
    ui->lineEditSemiconductorDonors->setValidator(double_validator);

    ui->lineEditScalingFactor->setText("1.0");
    QObject::connect(ui->tableWidget, SIGNAL(currentCellChanged(int,int,int,int)),
                     this,            SLOT(showSegmentParameters(int, int, int, int)));
    QObject::connect(ui->lineEditSegmentName, SIGNAL(textChanged(QString)),
                     this,                    SLOT(setSegmentName(QString)));
    QObject::connect(ui->lineEditSemiconductorAcceptors, SIGNAL(textChanged(QString)),
                     this,                              SLOT(setSegmentSCAcceptors(QString)));
    QObject::connect(ui->lineEditSemiconductorDonors, SIGNAL(textChanged(QString)),
                     this,                            SLOT(setSegmentSCDonors(QString)));
    QObject::connect(ui->checkBoxContact, SIGNAL(toggled(bool)),
                     this,                SLOT(makeCurrentSegmentContact(bool)));
    QObject::connect(ui->checkBoxOxide, SIGNAL(toggled(bool)),
                     this,              SLOT(makeCurrentSegmentOxide(bool)));
    QObject::connect(ui->checkBoxSemiconductor, SIGNAL(toggled(bool)),
                     this,                      SLOT(makeCurrentSegmentSemiconductor(bool)));

//    ui->comboBoxDeviceTemplates->addItem(key::device_capacitor_1d);
    ui->comboBoxDeviceTemplates->addItem(key::device_capacitor_2d);
    ui->comboBoxDeviceTemplates->addItem(key::device_capacitor_3d);
    ui->comboBoxDeviceTemplates->addItem(key::device_diode_pn_2d);
    ui->comboBoxDeviceTemplates->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    this->toggleSegmentContact(false);
    this->toggleSegmentOxide(false);
    this->toggleSegmentSemiconductor(false);

    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setColumnWidth(0, 20);
    QStringList header;
    header << "" << "Index";
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
}

DeviceGeneratorForm::~DeviceGeneratorForm()
{
    delete ui;
}

void DeviceGeneratorForm::on_pushButtonLoadMesh_clicked()
{
//    device_parameters.clear();
//    resize_device_parameters = true;

    // store the relative path
    // to allow shipping the INI files (which also hold the meshinputfile) to colleagues ..
  QDir dir(QCoreApplication::applicationDirPath());

  meshfile = dir.relativeFilePath(
        QFileDialog::getOpenFileName(this, tr("Open Mesh File"), QDir::currentPath(),
                                    "All files (*.*);;Netgen mesh files (*.mesh)")
  );

  if(meshfile.isEmpty()) return; // if the cancel button has been clicked ..
  else emit meshFileEntered(meshfile);
}

QString DeviceGeneratorForm::getMeshType()
{
    return ui->comboBoxMeshType->currentText();
}

void DeviceGeneratorForm::process(viennamini::device_handle& vmini_device, Render3D* renderer)
{
    vmini_device_ = vmini_device;
    viennamini::device::IndicesType& segment_indices = vmini_device_->segment_indices();
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(segment_indices.size());
    for(int i = 0; i < segment_indices.size(); i++)
    {
        if(renderer)
        {
          Render3D::RGB rgb;
          renderer->get_segment_color(i, rgb);
          QTableWidgetItem *item = new QTableWidgetItem;
          item->setBackground(QBrush(QColor::fromRgbF(rgb[0], rgb[1], rgb[2])));
          ui->tableWidget->setItem(i, COLOR_COLUMN, item);
        }

        QTableWidgetItem *item = new QTableWidgetItem(QString::number(segment_indices[i]));
        item->setFlags(item->flags() ^ Qt::ItemIsEditable); // make the table not editable
        item->setData(Qt::UserRole, qint32(segment_indices[i]));
        ui->tableWidget->setItem(i, ID_COLUMN, item);
    }
    ui->tableWidget->resizeColumnsToContents();
//    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    //ui->tableWidget->verticalHeader()->setStretchLastSection(true);

    // now, activate the device tab!
    ui->tabWidget->setTabEnabled(1, true);

    // select the first segment by default
    ui->tableWidget->setCurrentCell(0, ID_COLUMN);
//    this->toggleParameters(true);
//    this->showSegmentParameters(0,0); //show default parameters for the initial selection

  viennamaterials::library_handle matlib = vmini_device_->material_library();

  viennamaterials::accessor_handle material_category  = matlib->register_accessor(new viennamini::xpath_material_category_accessor);
  viennamaterials::accessor_handle data               = matlib->register_accessor(new viennamini::xpath_data_accessor);

  // extract all semiconductors from the material database and populate the combobox
  //
  viennamaterials::query semiconductor_query = viennamaterials::make_query(
        viennamaterials::make_entry(material_category , viennamini::material::semiconductor()),
        viennamaterials::make_entry(data              , viennamini::material::id()));
  viennamaterials::range semiconductor_range = viennamaterials::make_range(matlib->query(semiconductor_query));
  foreach (std::string const& entry, semiconductor_range) {
      ui->comboBoxSemiconductorMaterial->addItem(QString::fromStdString(entry));
  }
  QObject::connect(ui->comboBoxSemiconductorMaterial, SIGNAL(activated(QString)),
                   this, SLOT(setSegmentMaterial(QString)));


  // extract all oxides from the material database and populate the combobox
  //
  viennamaterials::query oxide_query = viennamaterials::make_query(
        viennamaterials::make_entry(material_category , viennamini::material::oxide()),
        viennamaterials::make_entry(data              , viennamini::material::id()));
  viennamaterials::range oxide_range = viennamaterials::make_range(matlib->query(oxide_query));
  foreach (std::string const& entry, oxide_range) {
      ui->comboBoxOxideMaterial->addItem(QString::fromStdString(entry));
  }
  QObject::connect(ui->comboBoxOxideMaterial, SIGNAL(activated(QString)),
                   this, SLOT(setSegmentMaterial(QString)));


  // extract all metals from the material database and populate the combobox
  //
  viennamaterials::query metal_query = viennamaterials::make_query(
        viennamaterials::make_entry(material_category , viennamini::material::metal()),
        viennamaterials::make_entry(data              , viennamini::material::id()));
  viennamaterials::range metal_range = viennamaterials::make_range(matlib->query(metal_query));
  foreach (std::string const& entry, metal_range) {
    ui->comboBoxContactMaterial->addItem(QString::fromStdString(entry));
  }
  QObject::connect(ui->comboBoxContactMaterial, SIGNAL(activated(QString)),
                   this, SLOT(setSegmentMaterial(QString)));
}

void DeviceGeneratorForm::saveState(QSettings& settings)
{
}

void DeviceGeneratorForm::loadState(QSettings& settings)
{
}

void DeviceGeneratorForm::on_pushButtonScaleDevice_clicked()
{
  emit scaleDevice(ui->lineEditScalingFactor->text().toDouble());
}

void DeviceGeneratorForm::showSegmentParameters(int row, int, int, int) // the second parameter 'column' is obsolete atm
{
    if(row < 0) return;
    if(!vmini_device_) return;


    QTableWidgetItem* widgetItem = ui->tableWidget->item(row, ID_COLUMN);
    int sid = widgetItem->data(Qt::UserRole).toInt();

//    qDebug() << "showing segment " << sid;

    ui->lineEditSegmentName->setText(QString::fromStdString(vmini_device_->get_name(sid)));

    ui->checkBoxContact->blockSignals(true);
    ui->checkBoxContact->setChecked(vmini_device_->is_contact(sid));
    ui->checkBoxContact->blockSignals(false);

    ui->checkBoxOxide->blockSignals(true);
    ui->checkBoxOxide->setChecked(vmini_device_->is_oxide(sid));
    ui->checkBoxOxide->blockSignals(false);

    ui->checkBoxSemiconductor->blockSignals(true);
    ui->checkBoxSemiconductor->setChecked(vmini_device_->is_semiconductor(sid));
    ui->checkBoxSemiconductor->blockSignals(false);

    ui->lineEditSemiconductorDonors->setText(QString::number(vmini_device_->get_donator_doping(sid)));
    ui->lineEditSemiconductorAcceptors->setText(QString::number(vmini_device_->get_acceptor_doping(sid)));

    // based on is{contact,oxide,semiconductor} deactivate/activate the corresponding parameter ui elements
    if(vmini_device_->is_contact(sid))
    {
//        qDebug() << "  -> contact: " << QString::fromStdString(vmini_device_->get_material(sid));
        this->toggleSegmentContact(true);
        ui->comboBoxContactMaterial->setCurrentIndex(ui->comboBoxContactMaterial->findText(QString::fromStdString(vmini_device_->get_material(sid))));
    }
    else this->toggleSegmentContact(false);

    if(vmini_device_->is_oxide(sid))
    {
//        qDebug() << "  -> oxide: " << QString::fromStdString(vmini_device_->get_material(sid));
        this->toggleSegmentOxide(true);
        ui->comboBoxOxideMaterial->setCurrentIndex(ui->comboBoxOxideMaterial->findText(QString::fromStdString(vmini_device_->get_material(sid))));
    }
    else this->toggleSegmentOxide(false);

    if(vmini_device_->is_semiconductor(sid))
    {
//        qDebug() << "  -> semiconductor: " << QString::fromStdString(vmini_device_->get_material(sid));
        this->toggleSegmentSemiconductor(true);
        ui->comboBoxSemiconductorMaterial->setCurrentIndex(ui->comboBoxSemiconductorMaterial->findText(QString::fromStdString(vmini_device_->get_material(sid))));
    }
    else this->toggleSegmentSemiconductor(false);

}

void DeviceGeneratorForm::setSegmentName(QString const& name)
{
  if(!vmini_device_) return;

//  qDebug() << "setting segment name " << ui->tableWidget->item(ui->tableWidget->currentRow(), 0)->data(Qt::UserRole).toInt() << " name " << name;
//    device_parameters[ui->tableWidget->item(ui->tableWidget->currentRow(), 0)->data(Qt::UserRole).toInt()].name = name;
  vmini_device_->set_name(
    ui->tableWidget->item(ui->tableWidget->currentRow(), ID_COLUMN)->data(Qt::UserRole).toInt(),
    name.toStdString()
  );
}

void DeviceGeneratorForm::setSegmentMaterial(QString const& name)
{
  vmini_device_->set_material(ui->tableWidget->item(ui->tableWidget->currentRow(), ID_COLUMN)->data(Qt::UserRole).toInt(), name.toStdString());
}

void DeviceGeneratorForm::setSegmentSCAcceptors(QString const& value_str)
{
  vmini_device_->set_acceptor_doping(ui->tableWidget->item(ui->tableWidget->currentRow(), ID_COLUMN)->data(Qt::UserRole).toInt(), value_str.toDouble());
}

void DeviceGeneratorForm::setSegmentSCDonors(QString const& value_str)
{
  vmini_device_->set_donator_doping(ui->tableWidget->item(ui->tableWidget->currentRow(), ID_COLUMN)->data(Qt::UserRole).toInt(), value_str.toDouble());
}

void DeviceGeneratorForm::makeCurrentSegmentContact(bool state)
{
  int sid = ui->tableWidget->item(ui->tableWidget->currentRow(), ID_COLUMN)->data(Qt::UserRole).toInt();
  if(state == Qt::Unchecked)
  {
//      qDebug() << "contact unsetting segment " << sid;
    vmini_device_->make_neutral(sid);
    this->toggleSegmentContact(false);
  }
  else
  {
//      qDebug() << "making segment " << sid << " a contact";
    vmini_device_->make_contact(sid);
    ui->checkBoxSemiconductor->blockSignals(true);
    ui->checkBoxSemiconductor->setChecked(false);
    ui->checkBoxSemiconductor->blockSignals(false);

    ui->checkBoxOxide->blockSignals(true);
    ui->checkBoxOxide->setChecked(false);
    ui->checkBoxOxide->blockSignals(false);

    this->toggleSegmentContact(true);
    this->toggleSegmentOxide(false);
    this->toggleSegmentSemiconductor(false);
  }
}

void DeviceGeneratorForm::makeCurrentSegmentOxide(bool state)
{
  int sid = ui->tableWidget->item(ui->tableWidget->currentRow(), ID_COLUMN)->data(Qt::UserRole).toInt();
  if(state == Qt::Unchecked)
  {
//      qDebug() << "oxide unsetting segment " << sid;
    vmini_device_->make_neutral(sid);
    this->toggleSegmentOxide(false);
  }
  else
  {
//      qDebug() << "making segment " << sid << " an oxide";
    vmini_device_->make_oxide(sid);

    ui->checkBoxSemiconductor->blockSignals(true);
    ui->checkBoxSemiconductor->setChecked(false);
    ui->checkBoxSemiconductor->blockSignals(false);

    ui->checkBoxContact->blockSignals(true);
    ui->checkBoxContact->setChecked(false);
    ui->checkBoxContact->blockSignals(false);
    this->toggleSegmentContact(false);
    this->toggleSegmentOxide(true);
    this->toggleSegmentSemiconductor(false);
  }
}

void DeviceGeneratorForm::makeCurrentSegmentSemiconductor(bool state)
{
  int sid = ui->tableWidget->item(ui->tableWidget->currentRow(), ID_COLUMN)->data(Qt::UserRole).toInt();
  if(state == Qt::Unchecked)
  {
//    qDebug() << "semiconductor unsetting segment " << sid;
    vmini_device_->make_neutral(sid);
    this->toggleSegmentSemiconductor(false);
  }
  else
  {
//      qDebug() << "making segment " << sid << " a semiconductor";

    vmini_device_->make_semiconductor(sid);
    ui->checkBoxContact->blockSignals(true);
    ui->checkBoxContact->setChecked(false);
    ui->checkBoxContact->blockSignals(false);

    ui->checkBoxOxide->blockSignals(true);
    ui->checkBoxOxide->setChecked(false);
    ui->checkBoxOxide->blockSignals(false);
    this->toggleSegmentContact(false);
    this->toggleSegmentOxide(false);
    this->toggleSegmentSemiconductor(true);
  }
}

void DeviceGeneratorForm::toggleSegmentContact(bool state)
{
//  qDebug() << "toggling contact " << state;

  if(state == Qt::Unchecked)
  {
    ui->comboBoxContactMaterial->setEnabled(false);
  }
  else
  {
    ui->comboBoxContactMaterial->setEnabled(true);
  }

}

void DeviceGeneratorForm::toggleSegmentOxide(bool state)
{
//  qDebug() << "toggling oxide " << state;

  if(state == Qt::Unchecked)
  {
    ui->comboBoxOxideMaterial->setEnabled(false);
  }
  else
  {
    ui->comboBoxOxideMaterial->setEnabled(true);
  }

}

void DeviceGeneratorForm::toggleSegmentSemiconductor(bool state)
{
//  qDebug() << "toggling semiconductor " << state;
  if(state == Qt::Unchecked)
  {
    ui->comboBoxSemiconductorMaterial->setEnabled(false);
    ui->lineEditSemiconductorAcceptors->setEnabled(false);
    ui->lineEditSemiconductorDonors->setEnabled(false);
  }
  else
  {
    ui->comboBoxSemiconductorMaterial->setEnabled(true);
    ui->lineEditSemiconductorAcceptors->setEnabled(true);
    ui->lineEditSemiconductorDonors->setEnabled(true);
  }

}

void DeviceGeneratorForm::toggleParameters(bool state)
{
//  qDebug() << "toggling parameters " << state;
//  this->toggleSegmentContact(state);
//  this->toggleSegmentOxide(state);
//  this->toggleSegmentSemiconductor(state);
//  ui->lineEditSegmentName->setEnabled(state);
//  ui->checkBoxContact->setEnabled(state);
//  ui->checkBoxOxide->setEnabled(state);
//  ui->checkBoxSemiconductor->setEnabled(state);
}


void DeviceGeneratorForm::on_pushButtonLoadTemplate_clicked()
{
  emit deviceTemplateEntered(ui->comboBoxDeviceTemplates->currentText());
}

void DeviceGeneratorForm::on_pushButtonCSGEditor_clicked()
{

}
