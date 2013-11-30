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

#include "viennaminiform.h"
#include "ui_viennaminiform.h"

#include <limits>

#include <QDebug>
#include <QFileDialog>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QHeaderView>

#include "forwards.hpp"
#include "device.h"

ViennaMiniForm::ViennaMiniForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ViennaMiniForm)
{
    ui->setupUi(this);

//    ui->tabWidget->setTabText(0, "Setup");
//    ui->tabWidget->setTabText(1, "Solver");

    QDoubleValidator* double_validator = new QDoubleValidator(-std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), 10, this);
    QIntValidator*    integer_validator = new QIntValidator(this);

    ui->lineEditTemp->setValidator(double_validator);
    ui->lineEditContactSingle->setValidator(double_validator);
    ui->lineEditContactRangeFrom->setValidator(double_validator);
    ui->lineEditContactRangeTo->setValidator(double_validator);
    ui->lineEditWorkfunction->setValidator(double_validator);
    ui->lineEditLinSolveIterations->setValidator(integer_validator);
    ui->lineEditNonLinSolveIterations->setValidator(integer_validator);
    ui->lineEditLinSolveTol->setValidator(double_validator);
    ui->lineEditNonLinSolveTol->setValidator(double_validator);
    ui->lineEditNonLinSolveDamping->setValidator(double_validator);

    ui->tableWidget->verticalHeader()->setVisible(false);

    // setup initial values for the UI elements
    //
//    ui->lineEditTemp->setText(QString::number(device_parameters.config().temperature()));
//    ui->lineEditLinSolveIterations->setText(QString::number(device_parameters.config().linear_iterations()));
//    ui->lineEditLinSolveTol->setText(QString::number(device_parameters.config().linear_breaktol()));
//    ui->lineEditNonLinSolveIterations->setText(QString::number(device_parameters.config().nonlinear_iterations()));
//    ui->lineEditNonLinSolveTol->setText(QString::number(device_parameters.config().nonlinear_breaktol()));
//    ui->lineEditNonLinSolveDamping->setText(QString::number(device_parameters.config().damping()));

//    QObject::connect(ui->lineEditTemp, SIGNAL(textChanged(QString)), this, SLOT(setTemperature(QString)));
//    QObject::connect(ui->lineEditLinSolveIterations, SIGNAL(textChanged(QString)), this, SLOT(setLinearIterations(QString)));
//    QObject::connect(ui->lineEditLinSolveTol, SIGNAL(textChanged(QString)), this, SLOT(setLinearTolerance(QString)));
//    QObject::connect(ui->lineEditNonLinSolveIterations, SIGNAL(textChanged(QString)), this, SLOT(setNonLinearIterations(QString)));
//    QObject::connect(ui->lineEditNonLinSolveTol, SIGNAL(textChanged(QString)), this, SLOT(setNonLinearTolerance(QString)));
//    QObject::connect(ui->lineEditNonLinSolveDamping, SIGNAL(textChanged(QString)), this, SLOT(setNonLinearDamping(QString)));
//    QObject::connect(ui->radioButtonContactSingle, SIGNAL(toggled(bool)), this, SLOT(setSegmentContactIsSingle(bool)));
//    QObject::connect(ui->lineEditContactSingle, SIGNAL(textChanged(QString)), this, SLOT(setSegmentContactContactValue(QString)));
//    QObject::connect(ui->radioButtonContactRange, SIGNAL(toggled(bool)), this, SLOT(setSegmentContactIsRange(bool)));
//    QObject::connect(ui->lineEditContactRangeFrom, SIGNAL(textChanged(QString)), this, SLOT(setSegmentContactContactFrom(QString)));
//    QObject::connect(ui->lineEditContactRangeTo, SIGNAL(textChanged(QString)), this, SLOT(setSegmentContactContactTo(QString)));
//    QObject::connect(ui->lineEditWorkfunction, SIGNAL(textChanged(QString)), this, SLOT(setSegmentContactWorkfunction(QString)));
//    QObject::connect(ui->lineEditSemiconductorAcceptors, SIGNAL(textChanged(QString)), this, SLOT(setSegmentSCAcceptors(QString)));
//    QObject::connect(ui->lineEditSemiconductorDonors, SIGNAL(textChanged(QString)), this, SLOT(setSegmentSCDonors(QString)));

//    QObject::connect(ui->checkBoxContact, SIGNAL(toggled(bool)), this, SLOT(makeCurrentSegmentContact(bool)));
//    QObject::connect(ui->checkBoxOxide, SIGNAL(toggled(bool)), this, SLOT(makeCurrentSegmentOxide(bool)));
//    QObject::connect(ui->checkBoxSemiconductor, SIGNAL(toggled(bool)), this, SLOT(makeCurrentSegmentSemiconductor(bool)));

//    this->toggleSegmentContact(false);
//    this->toggleSegmentOxide(false);
//    this->toggleSegmentSemiconductor(false);


    ui->tableWidgetSegmentRoles->setColumnCount(4);
    QStringList header;
    header << "ID" << "Name" << "Type" << "Material";
    ui->tableWidgetSegmentRoles->setHorizontalHeaderLabels(header);

}

ViennaMiniForm::~ViennaMiniForm()
{
    delete ui;
}

void ViennaMiniForm::process(viennamini::simulator_handle vmini_simulator)
{
  vmini_simulator_.reset();
  vmini_simulator_ = vmini_simulator;
  viennamini::device::IndicesType& segment_indices = vmini_simulator->device().segment_indices();
  ui->tableWidgetSegmentRoles->clearContents();
  qDebug() << "widget processing ..";
  ui->tableWidgetSegmentRoles->setRowCount(segment_indices.size());
  for(int i = 0; i < segment_indices.size(); i++)
  {
      qDebug() << "iterating segmetn " << i;
      // store the segment id, keep in mind that the viennagrid segment id
      // is probably 1-based, instead of 0-based .. so store it as 'data' on the tablewidgetitem
      // for later use
      QTableWidgetItem *id = new QTableWidgetItem(QString::number(segment_indices[i]));
      id->setFlags(id->flags() ^ Qt::ItemIsEditable); // make the table not editable
      id->setData(Qt::UserRole, qint32(segment_indices[i]));
      ui->tableWidgetSegmentRoles->setItem(i, 0, id);

      QTableWidgetItem *name = new QTableWidgetItem( QString::fromStdString(vmini_simulator_->device_handle()->get_name(segment_indices[i])) );
      ui->tableWidgetSegmentRoles->setItem(i, 1, name);

      QTableWidgetItem *type = new QTableWidgetItem;
      if(vmini_simulator_->device_handle()->is_contact(segment_indices[i]))
        type->setText("Contact");
      else
      if(vmini_simulator_->device_handle()->is_oxide(segment_indices[i]))
        type->setText("Oxide");
      else
      if(vmini_simulator_->device_handle()->is_semiconductor(segment_indices[i]))
        type->setText("Semiconductor");
      else type->setText("Unidentified");
      ui->tableWidgetSegmentRoles->setItem(i, 2, type);

      QTableWidgetItem *material = new QTableWidgetItem( QString::fromStdString(vmini_simulator_->device_handle()->get_material(segment_indices[i])) );
      ui->tableWidgetSegmentRoles->setItem(i, 3, material);
  }

  ui->tableWidgetSegmentRoles->resizeColumnsToContents();
  ui->tableWidgetSegmentRoles->horizontalHeader()->setStretchLastSection(true);
  //ui->tableWidget->verticalHeader()->setStretchLastSection(true);

  // select the first segment by default
  ui->tableWidgetSegmentRoles->setCurrentCell(segment_indices[0], 0);
////    this->toggleParameters(true);
////    this->showSegmentParameters(0,0); //show default parameters for the initial selection
}


//void ViennaMiniForm::setTemperature(QString const& value_str)
//{
//    device_parameters.config().temperature() = value_str.toDouble();
//}

//void ViennaMiniForm::setLinearTolerance(QString const& value_str)
//{
//    device_parameters.config().linear_breaktol() = value_str.toDouble();
//}

//void ViennaMiniForm::setLinearIterations(QString const& value_str)
//{
//    device_parameters.config().linear_iterations() = value_str.toInt();
//}

//void ViennaMiniForm::setNonLinearTolerance(QString const& value_str)
//{
//    device_parameters.config().nonlinear_breaktol() = value_str.toDouble();
//}

//void ViennaMiniForm::setNonLinearIterations(QString const& value_str)
//{
//    device_parameters.config().nonlinear_iterations() = value_str.toInt();
//}

//void ViennaMiniForm::setNonLinearDamping(QString const& value_str)
//{
//    device_parameters.config().damping() = value_str.toDouble();
//}



//void ViennaMiniForm::setSegmentMaterial(QString const& name)
//{
//    device_parameters[ui->tableWidget->item(ui->tableWidget->currentRow(), 0)->data(Qt::UserRole).toInt()].material = name;
//}

//void ViennaMiniForm::setSegmentContactIsSingle(bool state)
//{
//    device_parameters[ui->tableWidget->item(ui->tableWidget->currentRow(), 0)->data(Qt::UserRole).toInt()].isContactSingle = state;
//}

//void ViennaMiniForm::setSegmentContactContactValue(QString const& value_str)
//{
//    device_parameters[ui->tableWidget->item(ui->tableWidget->currentRow(), 0)->data(Qt::UserRole).toInt()].contact = value_str.toDouble();
//}

//void ViennaMiniForm::setSegmentContactIsRange(bool state)
//{
//    device_parameters[ui->tableWidget->item(ui->tableWidget->currentRow(), 0)->data(Qt::UserRole).toInt()].isContactRange = state;
//}

//void ViennaMiniForm::setSegmentContactContactFrom(QString const& value_str)
//{
//    device_parameters[ui->tableWidget->item(ui->tableWidget->currentRow(), 0)->data(Qt::UserRole).toInt()].contactFrom = value_str.toDouble();
//}

//void ViennaMiniForm::setSegmentContactContactTo(QString const& value_str)
//{
//    device_parameters[ui->tableWidget->item(ui->tableWidget->currentRow(), 0)->data(Qt::UserRole).toInt()].contactTo = value_str.toDouble();
//}

//void ViennaMiniForm::setSegmentContactWorkfunction(QString const& value_str)
//{
//    device_parameters[ui->tableWidget->item(ui->tableWidget->currentRow(), 0)->data(Qt::UserRole).toInt()].workfunction = value_str.toDouble();
//}

//void ViennaMiniForm::setSegmentSCAcceptors(QString const& value_str)
//{
//    device_parameters[ui->tableWidget->item(ui->tableWidget->currentRow(), 0)->data(Qt::UserRole).toInt()].acceptors = value_str.toDouble();
//}

//void ViennaMiniForm::setSegmentSCDonors(QString const& value_str)
//{
//  device_parameters[ui->tableWidget->item(ui->tableWidget->currentRow(), 0)->data(Qt::UserRole).toInt()].donors = value_str.toDouble();
//}

//void ViennaMiniForm::makeCurrentSegmentContact(bool state)
//{
//    device_parameters[ui->tableWidget->item(ui->tableWidget->currentRow(), 0)->data(Qt::UserRole).toInt()].isContact = ui->checkBoxContact->isChecked();
//    if(state)
//    {
//        this->toggleSegmentContact(true);
//        this->toggleSegmentOxide(false);
//        this->toggleSegmentSemiconductor(false);
//    }
//    else
//        this->toggleSegmentContact(false);
//}


//void ViennaMiniForm::makeCurrentSegmentOxide(bool state)
//{
//    device_parameters[ui->tableWidget->item(ui->tableWidget->currentRow(), 0)->data(Qt::UserRole).toInt()].isOxide = ui->checkBoxOxide->isChecked();
//    if(state)
//    {
//        this->toggleSegmentOxide(true);
//        this->toggleSegmentContact(false);
//        this->toggleSegmentSemiconductor(false);
//    }
//    else
//        this->toggleSegmentOxide(false);
//}

//void ViennaMiniForm::makeCurrentSegmentSemiconductor(bool state)
//{
//    device_parameters[ui->tableWidget->item(ui->tableWidget->currentRow(), 0)->data(Qt::UserRole).toInt()].isSemiconductor = ui->checkBoxSemiconductor->isChecked();
//    if(state)
//    {
//        this->toggleSegmentSemiconductor(true);
//        this->toggleSegmentContact(false);
//        this->toggleSegmentOxide(false);
//    }
//    else
//        this->toggleSegmentSemiconductor(false);
//}

//void ViennaMiniForm::toggleSegmentSemiconductor(bool state)
//{
//    ui->comboBoxSemiconductorMaterial->setEnabled(state);
//    ui->lineEditSemiconductorAcceptors->setEnabled(state);
//    ui->lineEditSemiconductorDonors->setEnabled(state);
//    if(!state) ui->checkBoxSemiconductor->setChecked(false);
//}

//void ViennaMiniForm::toggleSegmentOxide(bool state)
//{
//    ui->comboBoxOxideMaterial->setEnabled(state);
//    if(!state) ui->checkBoxOxide->setChecked(false);
//}

//void ViennaMiniForm::toggleSegmentContact(bool state)
//{
//    ui->comboBoxContactMaterial->setEnabled(state);
//    ui->radioButtonContactSingle->setEnabled(state);
//    ui->radioButtonContactRange->setEnabled(state);
//    ui->lineEditContactSingle->setEnabled(state);
//    ui->lineEditContactRangeFrom->setEnabled(state);
//    ui->lineEditContactRangeTo->setEnabled(state);
//    ui->lineEditWorkfunction->setEnabled(state);
//    if(!state) ui->checkBoxContact->setChecked(false);
//}

//void ViennaMiniForm::toggleParameters(bool state)
//{
//    this->toggleSegmentContact(state);
//    this->toggleSegmentOxide(state);
//    this->toggleSegmentSemiconductor(state);
//    ui->lineEditSegmentName->setEnabled(state);
//    ui->checkBoxContact->setEnabled(state);
//    ui->checkBoxOxide->setEnabled(state);
//    ui->checkBoxSemiconductor->setEnabled(state);
//}

//DeviceParameters& ViennaMiniForm::getParameters()
//{
//    return device_parameters;
//}

//void ViennaMiniForm::saveState(QSettings& settings)
//{
//    settings.beginGroup("general");
//    settings.setValue("meshfile",      this->meshfile);
//    settings.setValue("meshscaling",   ui->lineEditScalingFactor->text());
//    settings.setValue("meshtype",      ui->comboBoxMeshType->currentIndex());
//    settings.setValue("temperature",   device_parameters.config().temperature());
//    settings.setValue("linsolve_iter", device_parameters.config().linear_iterations());
//    settings.setValue("linsolve_tol",  device_parameters.config().linear_breaktol());
//    settings.setValue("nonlinsolve_iter", device_parameters.config().nonlinear_iterations());
//    settings.setValue("nonlinsolve_tol", device_parameters.config().nonlinear_breaktol());
//    settings.setValue("nonlinsolve_damping", device_parameters.config().damping());
//    settings.endGroup();

//    settings.beginGroup("device");
//    settings.setValue("segmentsize", int(device_parameters.size()));

//    int si = 0;
//    for(DeviceParameters::iterator iter = device_parameters.begin();
//        iter != device_parameters.end(); iter++)
//    {
//        int segment_index = iter->first;
//        SegmentParameters& segpara = iter->second;
//        settings.beginGroup("segment"+QString::number(si++));
//        settings.setValue("id", segment_index);
//        settings.setValue("name", segpara.name);
//        settings.setValue("material", segpara.material);

//        settings.setValue("iscontact", segpara.isContact);
//        settings.setValue("iscontactsingle", segpara.isContactSingle);
//        settings.setValue("contact", segpara.contact);
//        settings.setValue("iscontactrange", segpara.isContactRange);
//        settings.setValue("contactfrom", segpara.contactFrom);
//        settings.setValue("contactto", segpara.contactTo);
//        settings.setValue("workfunction", segpara.workfunction);

//        settings.setValue("isoxide", segpara.isOxide);

//        settings.setValue("issemiconductor", segpara.isSemiconductor);
//        settings.setValue("donors", segpara.donors);
//        settings.setValue("acceptors", segpara.acceptors);
//        settings.endGroup();
//    }
//    settings.endGroup();
//}

//void ViennaMiniForm::loadState(QSettings& settings)
//{
//    device_parameters.clear();
//    // first we restore the underlying device parameters object ..
//    //
//    settings.beginGroup("general");
//    this->meshfile = settings.value("meshfile").toString();
//    ui->lineEditScalingFactor->setText(settings.value("meshscaling").toString());
//    ui->comboBoxMeshType->setCurrentIndex(settings.value("meshtype").toInt());

//    device_parameters.config().temperature() = settings.value("temperature").toDouble();
//    device_parameters.config().linear_iterations() = settings.value("linsolve_iter").toInt();
//    device_parameters.config().linear_breaktol() = settings.value("linsolve_tol").toDouble();
//    device_parameters.config().nonlinear_iterations() = settings.value("nonlinsolve_iter").toInt();
//    device_parameters.config().nonlinear_breaktol() = settings.value("nonlinsolve_tol").toDouble();
//    device_parameters.config().damping() = settings.value("nonlinsolve_damping").toDouble();
//    settings.endGroup();

//    // now, we update the UI too!
//    //
//    ui->lineEditTemp->setText(QString::number(device_parameters.config().temperature()));
//    ui->lineEditLinSolveIterations->setText(QString::number(device_parameters.config().linear_iterations()));
//    ui->lineEditLinSolveTol->setText(QString::number(device_parameters.config().linear_breaktol()));
//    ui->lineEditNonLinSolveIterations->setText(QString::number(device_parameters.config().nonlinear_iterations()));
//    ui->lineEditNonLinSolveTol->setText(QString::number(device_parameters.config().nonlinear_breaktol()));
//    ui->lineEditNonLinSolveDamping->setText(QString::number(device_parameters.config().damping()));

//    settings.beginGroup("device");
//    int segment_size = settings.value("segmentsize").toInt();

//    //device_parameters.resize(segment_size);

//    for(int si = 0; si < segment_size; si++)
//    {
//        settings.beginGroup("segment"+QString::number(si));
//        int segment_index = settings.value("id").toInt();
//        device_parameters[segment_index].name = settings.value("name").toString();
//        device_parameters[segment_index].material = settings.value("material").toString();
//        device_parameters[segment_index].isContact = settings.value("iscontact").toBool();
//        device_parameters[segment_index].isContactSingle = settings.value("iscontactsingle").toBool();
//        device_parameters[segment_index].contact = settings.value("contact").toDouble();
//        device_parameters[segment_index].isContactRange = settings.value("iscontactrange").toBool();
//        device_parameters[segment_index].contactFrom = settings.value("contactfrom").toDouble();
//        device_parameters[segment_index].contactTo = settings.value("contactto").toDouble();
//        device_parameters[segment_index].workfunction = settings.value("workfunction").toDouble();
//        device_parameters[segment_index].isOxide = settings.value("isoxide").toBool();
//        device_parameters[segment_index].isSemiconductor = settings.value("issemiconductor").toBool();
//        device_parameters[segment_index].donors = settings.value("donors").toDouble();
//        device_parameters[segment_index].acceptors = settings.value("acceptors").toDouble();
//        settings.endGroup();
//    }
//    settings.endGroup();

//    resize_device_parameters = false;

//    // actually load the mesh

//    emit meshFileEntered(meshfile);

//}
