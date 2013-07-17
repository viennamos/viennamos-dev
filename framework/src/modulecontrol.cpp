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

#include "modulecontrol.h"
#include "ui_modulecontrol.h"

#include <QSpacerItem>

ModuleControl::ModuleControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModuleControl)
{
    ui->setupUi(this);
    current_widget = NULL;
}

ModuleControl::~ModuleControl()
{
    delete ui;
}

void ModuleControl::addModuleParameterWidget(QWidget *widget)
{
    QGridLayout* layout = new QGridLayout(ui->groupBoxParameters);
    layout->addWidget(widget);
    current_widget = widget;
}

QWidget* ModuleControl::getWidget()
{
    return current_widget;
}

QPushButton* ModuleControl::getButtonRun()
{
    return ui->pushButtonRun;
}

QPushButton* ModuleControl::getButtonSave()
{
    return ui->pushButtonSave;
}

QPushButton* ModuleControl::getButtonLoad()
{
    return ui->pushButtonLoad;
}

void ModuleControl::deactivate()
{
    ui->pushButtonRun->setEnabled(false);
    ui->pushButtonSave->setEnabled(false);
    ui->pushButtonLoad->setEnabled(false);
    if(current_widget) current_widget->setEnabled(false);
}

void ModuleControl::activate()
{
    ui->pushButtonRun->setEnabled(true);
    ui->pushButtonSave->setEnabled(true);
    ui->pushButtonLoad->setEnabled(true);
    if(current_widget) current_widget->setEnabled(true);
}
