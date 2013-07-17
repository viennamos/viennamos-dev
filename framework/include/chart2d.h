#ifndef CHART2D_H
#define CHART2D_H

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


#include <map>

#include <QVTKWidget.h>
#include <vtkChartXY.h>
#include <vtkPlot.h>
#include <vtkPen.h>
#include <vtkTable.h>
#include <vtkContextView.h>
#include <vtkContextScene.h>
#include <vtkSmartPointer.h>
#include <vtkInteractorStyle.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkCamera.h>
#include <vtkRenderWindow.h>
#include <vtkFloatArray.h>
#include <vtkTable.h>

#include <boost/array.hpp>

#include <QStringList>

#include "chartentry.h"
#include "table_entry.h"

class MultiView;

class Chart2D : public QVTKWidget
{
    Q_OBJECT
    typedef vtkDataSetMapper        Mapper;
    typedef std::map<int, int>      IndexVTKMap;
public:
    typedef std::map<std::size_t, TableEntry>          Tables;


    typedef QStringList         StringList;

    explicit Chart2D(MultiView* multiview, Tables& tables, QWidget *parent = 0);
    void reset();
    void reset_view();
    StringList listQuantities(std::size_t table_index = 0);
    void addPlot(int x_index, ChartEntry& entry,
                 bool useIndexForXSeries, std::size_t table_index = 0);
    void setBackgroundColor(QColor color);
    void clear();
    void showLegend(bool state);
    void showGrid(bool state);
    void showLabels(bool state);
    void showAllPlots();
    std::size_t tableSize();
    QString getTableName(std::size_t index);
    Tables& getTables();
    void removeTable(std::size_t index);

    void setXAxisLabel(QString const& label);
    void setYAxisLabel(QString const& label);
    void setXAxisLogScale(bool state);
    void setYAxisLogScale(bool state);
    void setAxisLabelFontSize(int font_size);
    void setTitle(QString const& title);
    void setTitleFontSize(int font_size);
    void update();
    bool tableHasColumn(int table_id, QString const& key);

private:


signals:
    void signalShowAllPlots();

private:
    vtkSmartPointer<Mapper>                              mapper;
    vtkSmartPointer<vtkActor>                            actor;
    vtkSmartPointer<vtkRenderer>                         renderer;
    vtkSmartPointer<vtkRenderWindow>                     renderWindow;
    vtkSmartPointer<vtkRenderWindowInteractor>           interactor;
    vtkSmartPointer<vtkInteractorStyleTrackballCamera>   style;
    vtkSmartPointer<vtkContextView>                      view;
    vtkSmartPointer<vtkChartXY>                          chart;

    Tables&     tables;
    MultiView*  multiview;

    IndexVTKMap vtk_marker;
    IndexVTKMap vtk_line;



};

#endif // CHART2D_H
