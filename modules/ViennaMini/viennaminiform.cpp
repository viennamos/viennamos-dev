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

#include "viennamini/forwards.h"

ViennaMiniForm::ViennaMiniForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ViennaMiniForm)
{
    ui->setupUi(this);

    QDoubleValidator* double_validator = new QDoubleValidator(-std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), 10, this);
    QIntValidator*    integer_validator = new QIntValidator(this);

    ui->tabWidget->setTabText(0, "Setup");
    ui->tabWidget->setTabText(1, "Solver");

    ui->lineEditTemp->setValidator(double_validator);
//    ui->lineEditContactSingle->setValidator(double_validator);
//    ui->lineEditContactRangeFrom->setValidator(double_validator);
//    ui->lineEditContactRangeTo->setValidator(double_validator);
//    ui->lineEditWorkfunction->setValidator(double_validator);
    ui->lineEditLinSolveIterations->setValidator(integer_validator);
    ui->lineEditNonLinSolveIterations->setValidator(integer_validator);
    ui->lineEditLinSolveTol->setValidator(double_validator);
    ui->lineEditNonLinSolveTol->setValidator(double_validator);
    ui->lineEditNonLinSolveDamping->setValidator(double_validator);

    ui->tableWidgetSegmentRoles->setColumnCount(5);
    ui->tableWidgetSegmentRoles->setColumnWidth(COLOR_COLUMN, 20);
    QStringList header;
    header << "" << "ID" << "Name" << "Type" << "Material";
    ui->tableWidgetSegmentRoles->verticalHeader()->setVisible(false);
    ui->tableWidgetSegmentRoles->setHorizontalHeaderLabels(header);
    ui->tableWidgetSegmentRoles->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->comboBoxProblem->addItem("Select");
    ui->comboBoxProblem->addItem(QString::fromStdString(viennamini::id::laplace()));
    ui->comboBoxProblem->addItem(QString::fromStdString(viennamini::id::poisson_drift_diffusion_np()));
    ui->comboBoxProblem->setCurrentIndex(0);
    ui->comboBoxProblem->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    ui->comboBoxContactModel->addItem("Ohmic");
    ui->comboBoxContactModel->setCurrentIndex(0);
    ui->comboBoxContactModel->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    ui->radioButtonContactSingle->setChecked(true);

    QObject::connect(ui->tableWidgetSegmentRoles, SIGNAL(currentCellChanged(int,int,int,int)),
                     this,                        SLOT(showSegmentParameters(int, int, int, int)));
    QObject::connect(ui->lineEditTemp, SIGNAL(textChanged(QString)), this, SLOT(setTemperature(QString)));
    QObject::connect(ui->lineEditLinSolveIterations, SIGNAL(textChanged(QString)), this, SLOT(setLinearIterations(QString)));
    QObject::connect(ui->lineEditLinSolveTol, SIGNAL(textChanged(QString)), this, SLOT(setLinearTolerance(QString)));
    QObject::connect(ui->lineEditNonLinSolveIterations, SIGNAL(textChanged(QString)), this, SLOT(setNonLinearIterations(QString)));
    QObject::connect(ui->lineEditNonLinSolveTol, SIGNAL(textChanged(QString)), this, SLOT(setNonLinearTolerance(QString)));
    QObject::connect(ui->lineEditNonLinSolveDamping, SIGNAL(textChanged(QString)), this, SLOT(setNonLinearDamping(QString)));
    QObject::connect(ui->comboBoxProblem, SIGNAL(currentIndexChanged(QString)), this, SLOT(setNewProblem(QString)));


    QObject::connect(ui->radioButtonContactSingle, SIGNAL(toggled(bool)), this, SLOT(setSegmentContactIsSingle(bool)));
    QObject::connect(ui->lineEditContactSingle, SIGNAL(textChanged(QString)), this, SLOT(setSegmentContactContactValue(QString)));
    QObject::connect(ui->radioButtonContactRange, SIGNAL(toggled(bool)), this, SLOT(setSegmentContactIsRange(bool)));
    QObject::connect(ui->lineEditContactRangeFrom, SIGNAL(textChanged(QString)), this, SLOT(setSegmentContactContactFrom(QString)));
    QObject::connect(ui->lineEditContactRangeTo, SIGNAL(textChanged(QString)), this, SLOT(setSegmentContactContactTo(QString)));
    QObject::connect(ui->lineEditContactRangeDelta, SIGNAL(textChanged(QString)), this, SLOT(setSegmentContactContactDelta(QString)));
    QObject::connect(ui->lineEditWorkfunction, SIGNAL(textChanged(QString)), this, SLOT(setSegmentContactWorkfunction(QString)));
    QObject::connect(ui->checkBoxIV, SIGNAL(toggled(bool)), this, SLOT(setSegmentContactRecordIV(bool)));
    QObject::connect(ui->checkBoxScatteringLattice, SIGNAL(toggled(bool)), this, SLOT(setSegmentSemiconductorMobilityLattice(bool)));
    QObject::connect(ui->checkBoxScatteringIonizedImpurity, SIGNAL(toggled(bool)), this, SLOT(setSegmentSemiconductorMobilityIoniziedImpurity(bool)));
    QObject::connect(ui->checkBoxRecombinationSRH, SIGNAL(toggled(bool)), this, SLOT(setSegmentSemiconductorRecombinationSRH(bool)));
}

ViennaMiniForm::~ViennaMiniForm()
{
    delete ui;
}

void ViennaMiniForm::process(viennamini::simulator_handle vmini_simulator, Render3D* renderer)
{
  vmini_simulator_.reset();
  vmini_simulator_ = vmini_simulator;
  viennamini::device::IndicesType& segment_indices = vmini_simulator->device().segment_indices();
  ui->tableWidgetSegmentRoles->clearContents();
  ui->tableWidgetSegmentRoles->setRowCount(segment_indices.size());
  for(int i = 0; i < segment_indices.size(); i++)
  {
      if(renderer)
      {
        Render3D::RGB rgb;
        renderer->get_segment_color(i, rgb);
        QTableWidgetItem *color = new QTableWidgetItem;
        color->setBackground(QBrush(QColor::fromRgbF(rgb[0], rgb[1], rgb[2])));
        color->setFlags(Qt::ItemIsEnabled); // make item not selectable
        ui->tableWidgetSegmentRoles->setItem(i, COLOR_COLUMN, color);
      }

      // store the segment id, keep in mind that the viennagrid segment id
      // is probably 1-based, instead of 0-based .. so store it as 'data' on the tablewidgetitem
      // for later use
      QTableWidgetItem *id = new QTableWidgetItem(QString::number(segment_indices[i]));
      id->setFlags(id->flags() ^ Qt::ItemIsEditable); // make the item not editable
      id->setData(Qt::UserRole, qint32(segment_indices[i]));
      ui->tableWidgetSegmentRoles->setItem(i, ID_COLUMN, id);

      QTableWidgetItem *name = new QTableWidgetItem( QString::fromStdString(vmini_simulator_->device_handle()->get_name(segment_indices[i])) );
      name->setFlags(name->flags() ^ Qt::ItemIsEditable); // make the item not editable
      ui->tableWidgetSegmentRoles->setItem(i, NAME_COLUMN, name);

      QTableWidgetItem *type = new QTableWidgetItem;
      type->setFlags(type->flags() ^ Qt::ItemIsEditable); // make the item not editable
      if(vmini_simulator_->device_handle()->is_contact(segment_indices[i]))
        type->setText("Contact");
      else
      if(vmini_simulator_->device_handle()->is_oxide(segment_indices[i]))
        type->setText("Oxide");
      else
      if(vmini_simulator_->device_handle()->is_semiconductor(segment_indices[i]))
        type->setText("Semiconductor");
      else type->setText("Unidentified");
      ui->tableWidgetSegmentRoles->setItem(i, TYPE_COLUMN, type);

      QTableWidgetItem *material = new QTableWidgetItem( QString::fromStdString(vmini_simulator_->device_handle()->get_material(segment_indices[i])) );
      material->setFlags(material->flags() ^ Qt::ItemIsEditable); // make the item not editable
      ui->tableWidgetSegmentRoles->setItem(i, MATERIAL_COLUMN, material);
  }

  ui->lineEditTemp->setText(QString::number(vmini_simulator_->device_handle()->temperature()));

  // make last column stretch the entire width of the table, so there is no ugly white gap
  ui->tableWidgetSegmentRoles->horizontalHeader()->setStretchLastSection(true);

  // make sure the columns are wide enough to hold the contents
  ui->tableWidgetSegmentRoles->resizeColumnsToContents();

  // select the first segment by default
  ui->tableWidgetSegmentRoles->setCurrentCell(0, ID_COLUMN);

  // load gui with parameters provided by the simulator
  ui->lineEditTemp->setText(QString::number(vmini_simulator->device_handle()->temperature()));
  ui->lineEditLinSolveIterations->setText(QString::number(vmini_simulator->config_handle()->linear_iterations()));
  ui->lineEditLinSolveTol->setText(QString::number(vmini_simulator->config_handle()->linear_breaktol()));
  ui->lineEditNonLinSolveIterations->setText(QString::number(vmini_simulator->config_handle()->nonlinear_iterations()));
  ui->lineEditNonLinSolveTol->setText(QString::number(vmini_simulator->config_handle()->nonlinear_breaktol()));
  ui->lineEditNonLinSolveDamping->setText(QString::number(vmini_simulator->config_handle()->damping()));

  // reset the 'problem' combobox, to indicate to the user that the problem has to
  // be set again for this new device.
  //
  ui->comboBoxProblem->setCurrentIndex(0);
}

void ViennaMiniForm::showSegmentParameters(int row, int, int, int) // the second parameter 'column' is obsolete atm
{
  if(row < 0) return;
  if(!vmini_simulator_) return;


  QTableWidgetItem* widgetItem = ui->tableWidgetSegmentRoles->item(row, ID_COLUMN);
  int sid = widgetItem->data(Qt::UserRole).toInt();

  if(vmini_simulator_->device_handle()->is_contact(sid))
  {
    ui->groupBoxContact->setEnabled(true);
    ui->groupBoxSemiconductor->setEnabled(false);

    if(vmini_simulator_->is_contact_single(sid))
    {
      // check the radio button, make sure it's not firing a signal
      //
      ui->radioButtonContactSingle->blockSignals(true);
      ui->radioButtonContactSingle->setChecked(true);
      ui->radioButtonContactSingle->blockSignals(false);

      // enable contact single widgets
      //
      ui->lineEditContactSingle->setEnabled(true);
      ui->lineEditContactSingle->setText(QString::number(vmini_simulator_->contact_potential(sid)));
//      qDebug() << "retrieving single contact " << vmini_simulator_->contact_potential(sid) << " for sid " << sid;
      // disable contact range widgets
      //
      ui->lineEditContactRangeFrom->setEnabled(false);
      ui->lineEditContactRangeTo->setEnabled(false);
      ui->lineEditContactRangeDelta->setEnabled(false);
    }
    else if(vmini_simulator_->is_contact_range(sid))
    {
      // check the radio button, make sure it's not firing a signal
      //
      ui->radioButtonContactRange->blockSignals(true);
      ui->radioButtonContactRange->setChecked(true);
      ui->radioButtonContactRange->blockSignals(false);

      // enable contact range widgets
      //
      ui->lineEditContactRangeFrom->setEnabled((true));
      ui->lineEditContactRangeTo->setEnabled((true));
      ui->lineEditContactRangeDelta->setEnabled((true));
      ui->lineEditContactRangeFrom->setText(QString::number(vmini_simulator_->contact_potential_range_from(sid)));
      ui->lineEditContactRangeTo->setText(QString::number(vmini_simulator_->contact_potential_range_to(sid)));
      ui->lineEditContactRangeDelta->setText(QString::number(vmini_simulator_->contact_potential_range_delta(sid)));
//      qDebug() << "retrieving range contact from " << vmini_simulator_->contact_potential_range_from(sid) << " for sid " << sid;
      // disable contact single widgets
      //
      ui->lineEditContactSingle->setEnabled(false);
    }
    // set the workfunction
    //
    ui->lineEditWorkfunction->setText(QString::number(vmini_simulator_->contact_workfunction(sid)));

    // set the IV record checkbox
    //
    ui->checkBoxIV->blockSignals(true);
    ui->checkBoxIV->setChecked(vmini_simulator_->record_iv(sid));
    ui->checkBoxIV->blockSignals(false);
  }
  else
  if(vmini_simulator_->device_handle()->is_semiconductor(sid))
  {
    ui->groupBoxContact->setEnabled(false);
    ui->groupBoxSemiconductor->setEnabled(true);

    // retrieve scattering model
    //
    if(vmini_simulator_->device_handle()->get_mobility(sid) == viennamini::mobility::none)
    {
      ui->checkBoxScatteringLattice->blockSignals(true);
      ui->checkBoxScatteringLattice->setChecked(false);
      ui->checkBoxScatteringLattice->blockSignals(false);
      ui->checkBoxScatteringIonizedImpurity->blockSignals(true);
      ui->checkBoxScatteringIonizedImpurity->setChecked(false);
      ui->checkBoxScatteringIonizedImpurity->blockSignals(false);
    }
    else
    if(vmini_simulator_->device_handle()->get_mobility(sid) == viennamini::mobility::lattice)
    {
        ui->checkBoxScatteringLattice->blockSignals(true);
        ui->checkBoxScatteringLattice->setChecked(true);
        ui->checkBoxScatteringLattice->blockSignals(false);
        ui->checkBoxScatteringIonizedImpurity->blockSignals(true);
        ui->checkBoxScatteringIonizedImpurity->setChecked(false);
        ui->checkBoxScatteringIonizedImpurity->blockSignals(false);
    }
    else
    if(vmini_simulator_->device_handle()->get_mobility(sid) == viennamini::mobility::ionized_impurity)
    {
        ui->checkBoxScatteringLattice->blockSignals(true);
        ui->checkBoxScatteringLattice->setChecked(true);
        ui->checkBoxScatteringLattice->blockSignals(false);
        ui->checkBoxScatteringIonizedImpurity->blockSignals(true);
        ui->checkBoxScatteringIonizedImpurity->setChecked(true);
        ui->checkBoxScatteringIonizedImpurity->blockSignals(false);

        // indicate to the user, that he/she cannot turn lattice scattering off when ionizied impurity is switchted on
        //
        ui->checkBoxScatteringLattice->setEnabled(false);
    }
    else
    {
      QMessageBox::critical(0, QString("Error"), "Mobility model not recognized");
    }

    // retrieve recombination model
    //
    if(vmini_simulator_->device_handle()->get_recombination(sid) == viennamini::recombination::none)
    {
      ui->checkBoxRecombinationSRH->blockSignals(true);
      ui->checkBoxRecombinationSRH->setChecked(false);
      ui->checkBoxRecombinationSRH->blockSignals(false);
    }
    else
    if(vmini_simulator_->device_handle()->get_recombination(sid) == viennamini::recombination::srh)
    {
        ui->checkBoxRecombinationSRH->blockSignals(true);
        ui->checkBoxRecombinationSRH->setChecked(true);
        ui->checkBoxRecombinationSRH->blockSignals(false);
    }
    else
    {
      QMessageBox::critical(0, QString("Error"), "Mobility model not recognized");
    }
  }
  else
  {
    // deactivate all
    //
    ui->radioButtonContactSingle->blockSignals(true);
    ui->radioButtonContactSingle->setChecked(false);
    ui->radioButtonContactSingle->blockSignals(false);
    ui->radioButtonContactRange->blockSignals(true);
    ui->radioButtonContactRange->setChecked(false);
    ui->radioButtonContactRange->blockSignals(false);

    ui->lineEditContactSingle->clear();
    ui->lineEditContactRangeFrom->clear();
    ui->lineEditContactRangeTo->clear();
    ui->lineEditContactRangeDelta->clear();
    ui->lineEditWorkfunction->clear();

    ui->groupBoxContact->setEnabled(false);
    ui->groupBoxSemiconductor->setEnabled(false);

    ui->checkBoxScatteringLattice->blockSignals(true);
    ui->checkBoxScatteringLattice->setChecked(false);
    ui->checkBoxScatteringLattice->blockSignals(false);
    ui->checkBoxScatteringIonizedImpurity->blockSignals(true);
    ui->checkBoxScatteringIonizedImpurity->setChecked(false);
    ui->checkBoxScatteringIonizedImpurity->blockSignals(false);

    ui->checkBoxScatteringIonizedImpurity->blockSignals(true);
    ui->checkBoxScatteringIonizedImpurity->setChecked(false);
    ui->checkBoxScatteringIonizedImpurity->blockSignals(false);

    ui->checkBoxRecombinationSRH->blockSignals(true);
    ui->checkBoxRecombinationSRH->setChecked(false);
    ui->checkBoxRecombinationSRH->blockSignals(false);
  }
}

void ViennaMiniForm::setTemperature(QString const& value_str)
{
  vmini_simulator_->device_handle()->temperature() = value_str.toDouble();
}

void ViennaMiniForm::setLinearTolerance(QString const& value_str)
{
  vmini_simulator_->config_handle()->linear_breaktol() = value_str.toDouble();
}

void ViennaMiniForm::setLinearIterations(QString const& value_str)
{
  vmini_simulator_->config_handle()->linear_iterations() = value_str.toDouble();
}

void ViennaMiniForm::setNonLinearTolerance(QString const& value_str)
{
  vmini_simulator_->config_handle()->nonlinear_breaktol() = value_str.toDouble();
}

void ViennaMiniForm::setNonLinearIterations(QString const& value_str)
{
  vmini_simulator_->config_handle()->nonlinear_iterations() = value_str.toDouble();
}

void ViennaMiniForm::setNonLinearDamping(QString const& value_str)
{
  vmini_simulator_->config_handle()->damping() = value_str.toDouble();
}

void ViennaMiniForm::setNewProblem(QString const& problem_str)
{
  vmini_simulator_->problem_id() = problem_str.toStdString();
}

void ViennaMiniForm::setSegmentContactIsSingle(bool state)
{
  vmini_simulator_->is_contact_single(ui->tableWidgetSegmentRoles->item(ui->tableWidgetSegmentRoles->currentRow(), ID_COLUMN)->data(Qt::UserRole).toInt()) = state;

  if(state)
  {
    // deactivate range widgets
    ui->lineEditContactRangeFrom->clear();
    ui->lineEditContactRangeTo->clear();
    ui->lineEditContactRangeDelta->clear();
    ui->lineEditContactRangeFrom->setEnabled(false);
    ui->lineEditContactRangeTo->setEnabled(false);
    ui->lineEditContactRangeDelta->setEnabled(false);

    // activate single widget
    ui->lineEditContactSingle->setEnabled(true);
    ui->lineEditContactSingle->setText("0.0");
  }
}

void ViennaMiniForm::setSegmentContactContactValue(QString const& value_str)
{
  vmini_simulator_->contact_potential(ui->tableWidgetSegmentRoles->item(ui->tableWidgetSegmentRoles->currentRow(), ID_COLUMN)->data(Qt::UserRole).toInt()) = value_str.toDouble();
}

void ViennaMiniForm::setSegmentContactIsRange(bool state)
{
  vmini_simulator_->is_contact_range(ui->tableWidgetSegmentRoles->item(ui->tableWidgetSegmentRoles->currentRow(), ID_COLUMN)->data(Qt::UserRole).toInt()) = state;

  if(state)
  {
    // deactivate single widget
    ui->lineEditContactSingle->clear();
    ui->lineEditContactSingle->setEnabled(false);
    // activate range widgets
    ui->lineEditContactRangeFrom->setEnabled(true);
    ui->lineEditContactRangeTo->setEnabled(true);
    ui->lineEditContactRangeDelta->setEnabled(true);
    ui->lineEditContactRangeFrom->setText("0.0");
    ui->lineEditContactRangeTo->setText("0.0");
    ui->lineEditContactRangeDelta->setText("0.0");
  }
}

void ViennaMiniForm::setSegmentContactContactFrom(QString const& value_str)
{
  vmini_simulator_->contact_potential_range_from(ui->tableWidgetSegmentRoles->item(ui->tableWidgetSegmentRoles->currentRow(), ID_COLUMN)->data(Qt::UserRole).toInt()) = value_str.toDouble();
}

void ViennaMiniForm::setSegmentContactContactTo(QString const& value_str)
{
  vmini_simulator_->contact_potential_range_to(ui->tableWidgetSegmentRoles->item(ui->tableWidgetSegmentRoles->currentRow(), ID_COLUMN)->data(Qt::UserRole).toInt()) = value_str.toDouble();
}

void ViennaMiniForm::setSegmentContactContactDelta(QString const& value_str)
{
  vmini_simulator_->contact_potential_range_delta(ui->tableWidgetSegmentRoles->item(ui->tableWidgetSegmentRoles->currentRow(), ID_COLUMN)->data(Qt::UserRole).toInt()) = value_str.toDouble();
}

void ViennaMiniForm::setSegmentContactWorkfunction(QString const& value_str)
{
  vmini_simulator_->contact_workfunction(ui->tableWidgetSegmentRoles->item(ui->tableWidgetSegmentRoles->currentRow(), ID_COLUMN)->data(Qt::UserRole).toInt()) = value_str.toDouble();
}

void ViennaMiniForm::setSegmentContactRecordIV(bool state)
{
  vmini_simulator_->record_iv(ui->tableWidgetSegmentRoles->item(ui->tableWidgetSegmentRoles->currentRow(), ID_COLUMN)->data(Qt::UserRole).toInt()) = state;
}


void ViennaMiniForm::setSegmentSemiconductorMobilityLattice(bool state)
{
  if(state)
  {
    vmini_simulator_->device_handle()->set_mobility(
          ui->tableWidgetSegmentRoles->item(ui->tableWidgetSegmentRoles->currentRow(), ID_COLUMN)->data(Qt::UserRole).toInt(),
          viennamini::mobility::lattice);
  }
  else
  {
    vmini_simulator_->device_handle()->set_mobility(
          ui->tableWidgetSegmentRoles->item(ui->tableWidgetSegmentRoles->currentRow(), ID_COLUMN)->data(Qt::UserRole).toInt(),
          viennamini::mobility::none);
  }
}

void ViennaMiniForm::setSegmentSemiconductorMobilityIoniziedImpurity(bool state)
{
  if(state)
  {
    vmini_simulator_->device_handle()->set_mobility(
          ui->tableWidgetSegmentRoles->item(ui->tableWidgetSegmentRoles->currentRow(), ID_COLUMN)->data(Qt::UserRole).toInt(),
          viennamini::mobility::ionized_impurity);

    // ionizied impurities include the lattice scattering, so
    // check the corresponding checkbox to indiciate this to the user
    //
    ui->checkBoxScatteringLattice->blockSignals(true);
    ui->checkBoxScatteringLattice->setChecked(true);
    ui->checkBoxScatteringLattice->blockSignals(false);

    // indicate to the user, that he/she cannot turn lattice scattering off when ionizied impurity is switchted on
    //
    ui->checkBoxScatteringLattice->setEnabled(false);
  }
  else
  {
    // if the user unchecks ionizied impurity scattering,
    // leave lattice scattering on
    //
    vmini_simulator_->device_handle()->set_mobility(
          ui->tableWidgetSegmentRoles->item(ui->tableWidgetSegmentRoles->currentRow(), ID_COLUMN)->data(Qt::UserRole).toInt(),
          viennamini::mobility::lattice);

    ui->checkBoxScatteringLattice->setEnabled(true);
  }
}

void ViennaMiniForm::setSegmentSemiconductorRecombinationSRH(bool state)
{
  if(state) // TODO with recombination, there are severla different independent models ..
  {
    vmini_simulator_->device_handle()->set_recombination(
          ui->tableWidgetSegmentRoles->item(ui->tableWidgetSegmentRoles->currentRow(), ID_COLUMN)->data(Qt::UserRole).toInt(),
          viennamini::recombination::srh);
  }
  else
  {
    vmini_simulator_->device_handle()->set_recombination(
          ui->tableWidgetSegmentRoles->item(ui->tableWidgetSegmentRoles->currentRow(), ID_COLUMN)->data(Qt::UserRole).toInt(),
          viennamini::recombination::none);
  }
}


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
