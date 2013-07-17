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

#include "license.h"
#include "ui_license.h"

License::License(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::License)
{
    ui->setupUi(this);
    this->setWindowTitle("License");
    ui->textEdit->setReadOnly(true);
    QString license_text;
    license_text = ""
        "Copyright (c) 2013, Institute for Microelectronics, TU Wien."
        "\n \n"
        "This user interface is part of ViennaMOS: http://viennamos.sourceforge.net/"
        "\n \n"
        "Contact: Josef Weinbub             weinbub@iue.tuwien.ac.at"
        "\n \n"
        "This program is free software; you can redistribute it and/or modify"
        "it under the terms of the GNU General Public License as published by"
        "the Free Software Foundation; either version 3, or (at your option)"
        "any later version."
        "\n \n"
        "This program is distributed in the hope that it will be useful,"
        "but WITHOUT ANY WARRANTY; without even the implied warranty of"
        "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the"
        "GNU General Public License for more details."
        "\n \n"
        "You should have received a copy of the GNU General Public License"
        "along with this program.  If not, see <http://www.gnu.org/licenses/>.";
    ui->textEdit->setText(license_text);
    ui->textEdit->updateGeometry();
}

License::~License()
{
    delete ui;
}
