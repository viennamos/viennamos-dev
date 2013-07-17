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

#include "newview.h"
#include "ui_newview.h"

#include <QPainter>

NewView::NewView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewView)
{
    ui->setupUi(this);
}

NewView::~NewView()
{
    delete ui;
}

QPushButton* NewView::getPushButtonRender3d()
{
    return ui->pushButtonRender3d;
}

QPushButton* NewView::getPushButtonChart2d()
{
    return ui->pushButtonChart2d;
}

// for emitting a signal on a mouseclick
void NewView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit clicked();
    }
}

// required to support stylesheets with this class
void NewView::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
