#ifndef CHARTEDITOR_H
#define CHARTEDITOR_H

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
#include <QListWidgetItem>

#include <map>

#include "chart2d.h"
#include "charttableparameters.h"

namespace Ui {
class ChartEditor;
}

class ChartEditor : public QDialog
{
    Q_OBJECT
    
    typedef std::map<std::size_t, ChartTableParameters>   TableParameters;

public:
    explicit ChartEditor(Chart2D* chart_view, QWidget *parent = 0);
    ~ChartEditor();

private slots:
    void reset();
    void toggleUseArrayIndices();
    void toggleUseQuantity();
    void processLabel();
    void processLineThickness();
    void processLineStyle();
    void processMarkerStyle();
    void processMarkerSize();
    void updateLineProperties(int row);
    void toggleLineColor(bool state);
    void updateLineColor(QColor new_color);
    void showTableParameters(int table_row, int col = -1, int prev_row = -1, int prev_col = -1);
    void processXQuantity(int index);
    void processCheckState(QListWidgetItem* item);
    void toggleEnableAll(bool state);
    void checkQuantitySelection();
    void on_pushButtonAdopt_clicked();
    void on_pushButtonRemoveTables_clicked();

public slots:
    void update();
    void apply();

private:
    void setEnableYAxisProperties(bool state);
    QString getStyleStringFromQColor(QColor color);
    int getCurrentTableID();
    void connect_signals();
    void disconnect_signals();

protected:
    void showEvent(QShowEvent *);

 // ---------------------------------------------------------

private:
    Ui::ChartEditor *ui;
    Chart2D* chart_view;
    MultiView* multi_view;

    QColorDialog*       colorDialogLine;
    TableParameters     table_parameters;

    std::map<std::size_t, TableEntry> previous_tables;

    QString color_view_border_style;

    QColor default_line_color;
    float  default_line_width;
    int    default_line_style;
    int    default_marker_style;
    float  default_marker_size;

};

#endif // CHARTEDITOR_H
