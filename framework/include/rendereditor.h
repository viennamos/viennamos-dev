#ifndef RENDEREDITOR_H
#define RENDEREDITOR_H

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

#include <QDialog>
#include <QColorDialog>
#include <QLineEdit>

#include "render3d.h"

namespace Ui {
class RenderEditor;
}

class RenderEditor : public QDialog
{
    Q_OBJECT
    
public:
    explicit RenderEditor(Render3D* render_view, QWidget* parent = 0);
    ~RenderEditor();

private slots:
    void reset_dialog();

public slots:
    void apply();
    void setLogState(bool state);
    void update();

private:
    void reset();
    void showEvent(QShowEvent *);

    Ui::RenderEditor*   ui;
    Render3D*           render_view;
    QColorDialog*       colorDialogBackground;
    QColorDialog*       colorDialogSolid;
    QColorDialog*       colorDialogEdges;
    QColorDialog*       colorDialogBarTitle;
    QColorDialog*       colorDialogBarLabels;
    QColorDialog*       colorDialogAxes;


    double background_color[3];
    double solid_color[3];
    double edge_color[3];
    double title_color[3];
    double labels_color[3];
    double axes_color[3];
    bool   current_log_state;

    QString             current_bar_title;

    typedef std::map<std::string, int> FontMap;
    FontMap  font_map;
};

#endif // RENDEREDITOR_H
