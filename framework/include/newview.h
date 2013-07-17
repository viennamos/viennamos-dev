#ifndef NEWVIEW_H
#define NEWVIEW_H

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
#include <QPushButton>
#include <QMouseEvent>

namespace Ui {
class NewView;
}



class NewView : public QWidget
{
    Q_OBJECT
    
public:
    explicit NewView(QWidget *parent = 0);
    ~NewView();

    QPushButton* getPushButtonRender3d();
    QPushButton* getPushButtonChart2d();
    
signals:
    void clicked();

protected:
    // for emitting a signal on a mouseclick
    void mousePressEvent(QMouseEvent *event);

    // required to support stylesheets with this class
    void paintEvent(QPaintEvent *);

private:
    Ui::NewView *ui;
};

#endif // NEWVIEW_H
