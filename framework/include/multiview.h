#ifndef MULTIVIEW_H
#define MULTIVIEW_H

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
#include <QMainWindow> // TODO removeme?!
#include <QSplitter>
#include <QDockWidget>
#include <QObject>
#include <QHBoxLayout>

#include <vtkObject.h>  // remove me?!
#include <vtkSmartPointer.h>
#include <vtkEventQtSlotConnect.h>

#include "render3d.h"
#include "chart2d.h"
#include "newview.h"
#include "rendereditor.h"
#include "charteditor.h"
#include "QVariantPtr.hpp"
#include "table_entry.h"

#include <vector>
#include <map>


class MultiView : public QObject
{
    Q_OBJECT
    
    // allow the following classes to use private functions
    friend class Chart2D;

public:
    typedef int*                                    IndexPnt;
    typedef std::vector<IndexPnt>                   IndexPntCont;
    typedef std::map<QDockWidget*, Render3D*>       Render3DMap;
    typedef std::map<QDockWidget*, Chart2D*>        Chart2DMap;
    typedef std::map<QDockWidget*, RenderEditor*>   RenderEditorMap;
    typedef std::map<QDockWidget*, ChartEditor*>    ChartEditorMap;
    typedef vtkSmartPointer<vtkUnstructuredGrid>    Grid;
    typedef vtkSmartPointer<vtkMultiBlockDataSet>   MultiGrid;
    typedef vtkSmartPointer<vtkTable>               Table;
    typedef std::map<std::size_t, TableEntry>       Tables;
    typedef std::map<QDockWidget*, int>             DockIndexMap;

    explicit MultiView(QWidget *parent = 0);
    ~MultiView();
    QDockWidget* removeCurrentView();
    QDockWidget* removeView(int id);
    void addNewView(QDockWidget* dock);
    bool currentIsRender3D();
    bool currentIsChart2D();
    bool hasCharts2D();
    bool hasRenders3D();
    void resetTables();
    void resetGrid();
    Render3D* getCurrentRender3D();
    Chart2D*  getCurrentChart2D();
    Table getTable(std::size_t index);
    QString getTableName(std::size_t index);
    Tables getTables();
    MultiGrid  getGrid();
    RenderEditor* getCurrentRenderEditor();
    ChartEditor* getCurrentChartEditor();
    int getCurrentViewIndex();
    QDockWidget* getDock(int id);
    QDockWidget* getCurrentDock();
    int getNumberOfViews();
    void setCurrentLogScale(bool state);
    void makeCurrentViewActive();
    void update();
    std::size_t addTable(QString const& name, Table table);
    void takeScreenshotOfActiveView(QString const& filename);

public slots:
    void resetCurrentView();

private slots:
    void viewClicked(vtkObject*, unsigned long, void*, void*);
    void addRender3D();
    void addChart2D();
    void newViewClicked();

signals:
    void currentViewChanged();
    void chartUpdated();
    void tableRemoved(std::size_t id);

private:
    void removeTable(std::size_t id);

    static const int INVALID = -1;

    int                                     current_index;
    int                                     view_counter;
    IndexPntCont                            index_cont;  //
    vtkSmartPointer<vtkEventQtSlotConnect>  vtk_connections;
    Render3DMap                             render_map;
    Chart2DMap                              chart_map;
    RenderEditorMap                         render_editor_map;
    ChartEditorMap                          chart_editor_map;
    DockIndexMap                            dock_index_map;
    Tables                                  tables;
    MultiGrid                               multigrid;
    std::size_t                             table_index;    
    const char* dock_key;
};

#endif // MULTIVIEW_H
