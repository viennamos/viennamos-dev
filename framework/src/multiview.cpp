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

#include "multiview.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>

#include <vtkWindowToImageFilter.h>
#include <vtkPNGWriter.h>

#include "newview.h"

MultiView::MultiView(QWidget *parent) :
    QObject(parent)
{
    vtk_connections = vtkSmartPointer<vtkEventQtSlotConnect>::New();

    // create the central datastructures
    multigrid = MultiGrid::New();

    current_index = 0;
    view_counter  = 0;
    table_index = 0;

    dock_key = "dock";
}

MultiView::~MultiView()
{
}

void MultiView::viewClicked(vtkObject*, unsigned long, void* client_data, void*)
{
    current_index = *(static_cast<IndexPnt> (client_data));
    //std::cout << "view number " << current_index << " has been clicked .." << std::endl;

    for(DockIndexMap::iterator iter = dock_index_map.begin(); iter != dock_index_map.end(); iter++)
    {
        if(iter->second == current_index)
            iter->first->widget()->setStyleSheet(".QWidget { border: 3px solid blue }");
        else
            iter->first->widget()->setStyleSheet(".QWidget { border: }");
    }

    emit currentViewChanged();
}

void MultiView::newViewClicked()
{
    //std::cout << "newview clicked" << std::endl;
    NewView* view = qobject_cast<NewView *>(sender());
    QDockWidget* dock = QVariantPtr<QDockWidget>::asPtr(view->property(dock_key));
    current_index = dock_index_map[dock];
    //std::cout << "view number " << current_index << " has been clicked .." << std::endl;

    this->makeCurrentViewActive();
}

void MultiView::makeCurrentViewActive()
{
    for(DockIndexMap::iterator iter = dock_index_map.begin(); iter != dock_index_map.end(); iter++)
    {
        if(iter->second == current_index)
            iter->first->widget()->setStyleSheet(".QWidget { background: gray; border: 3px solid blue }");
        else
            iter->first->widget()->setStyleSheet(".QWidget { background: gray; border: }");
    }

    emit currentViewChanged();
}

QDockWidget* MultiView::getCurrentDock()
{
    return this->getDock(current_index);
}

QDockWidget* MultiView::getDock(int id)
{
    for(DockIndexMap::iterator iter = dock_index_map.begin(); iter != dock_index_map.end(); iter++)
    {
        if(iter->second == id)
            return iter->first;
    }
    return NULL;
}

void MultiView::addNewView(QDockWidget *dock)
{
    int new_view_index = dock_index_map.size();
//    std::cout << "adding new view with index: " << new_view_index << " for dock: " << dock << std::endl;
    dock_index_map[dock] = new_view_index;

    // wrap the 'newview' widget into another widget
    // to enable border coloring
    //
    QWidget* widget  = new QWidget;
    NewView* newview = new NewView;
    QHBoxLayout* layout = new QHBoxLayout;
    layout->setContentsMargins(3, 3, 3, 3); // leave room for the border!
    layout->addWidget(newview);
    widget->setLayout(layout);
    dock->setWidget(widget);
    // the '.' restricts the stylesheet to be applied only on the widget, and not on the children, e.g., buttons
    widget->setStyleSheet(".QWidget { background: gray }");

    // now, store the index of this dock window into the newview (so we know
    // in the slot later, which dock the view belongs to
    newview->setProperty(dock_key, QVariantPtr<QDockWidget>::asQVariant(dock));
    QObject::connect(newview, SIGNAL(clicked()), this, SLOT(newViewClicked()));

    //newview->resize(800, 800);
    //dock->setWidget(newview);

    newview->getPushButtonRender3d()->setProperty(dock_key, QVariantPtr<QDockWidget>::asQVariant(dock));
    newview->getPushButtonChart2d()->setProperty(dock_key, QVariantPtr<QDockWidget>::asQVariant(dock));

    QObject::connect(newview->getPushButtonRender3d(), SIGNAL(clicked()), this, SLOT(addRender3D()));
    QObject::connect(newview->getPushButtonChart2d(), SIGNAL(clicked()), this, SLOT(addChart2D()));

    current_index = dock_index_map[dock];

    this->makeCurrentViewActive();
}

void MultiView::addRender3D()
{
    QDockWidget* dock = QVariantPtr<QDockWidget>::asPtr(sender()->property(dock_key));
    if(!dock)
    {
        QMessageBox::critical(0, QString("Error"), QString("Adding a Renderer raised an error - please contact the developers."));
        return;
    }

//    std::cout << "adding render 3d with index: " << dock_index_map[dock] << " for dock: " << dock << std::endl;

    QWidget*     widget = new QWidget;
    Render3D*    view   = new Render3D(multigrid, dock);
    QHBoxLayout* layout = new QHBoxLayout;
    layout->setContentsMargins(3, 3, 3, 3); // leave room for the border!
    layout->addWidget(view);
    widget->setLayout(layout);
    delete dock->widget(); // remove the previous 'newview' widget
    dock->setWidget(widget);

    render_editor_map[dock] = new RenderEditor(view, dock);
    render_editor_map[dock]->apply();

    QObject::connect(view, SIGNAL(grid_updated()), render_editor_map[dock], SLOT(update()));

    render_map[dock] = view;

    vtk_connections->Connect(view->GetRenderWindow()->GetInteractor(),
                             vtkCommand::LeftButtonPressEvent,
                             this,
                             SLOT(viewClicked(vtkObject*, unsigned long, void*, void*)), &dock_index_map[dock]);

    current_index = dock_index_map[dock];

    this->makeCurrentViewActive();
}

void MultiView::addChart2D()
{
    QDockWidget* dock = QVariantPtr<QDockWidget>::asPtr(sender()->property(dock_key));

    if(!dock)
    {
        QMessageBox::critical(0, QString("Error"), QString("Adding a Chart raised an error - please contact the developers."));
        return;
    }

//    std::cout << "adding chart 2d with index: " << dock_index_map[dock] << " for dock: " << dock << std::endl;

    QWidget*     widget = new QWidget;
    Chart2D*     view   = new Chart2D(this, tables, dock);
    QHBoxLayout* layout = new QHBoxLayout;
    layout->setContentsMargins(3, 3, 3, 3); // leave room for the border!
    layout->addWidget(view);
    widget->setLayout(layout);
    delete dock->widget(); // remove the previous 'newview' widget
    dock->setWidget(widget);

    chart_editor_map[dock] = new ChartEditor(view, dock);

    QObject::connect(this, SIGNAL(chartUpdated()), chart_editor_map[dock], SLOT(update()));
    QObject::connect(view, SIGNAL(signalShowAllPlots()), chart_editor_map[dock], SLOT(apply()));

    chart_map[dock] = view;

    vtk_connections->Connect(view->GetRenderWindow()->GetInteractor(),
                             vtkCommand::LeftButtonPressEvent,
                             this,
                             SLOT(viewClicked(vtkObject*, unsigned long, void*, void*)), &dock_index_map[dock]);

    current_index = dock_index_map[dock];

    this->makeCurrentViewActive();
}

bool MultiView::currentIsRender3D()
{
    if(render_map.find(this->getDock(current_index)) != render_map.end()) return true;
    else return false;
}

bool MultiView::currentIsChart2D()
{
    if(chart_map.find(this->getDock(current_index)) != chart_map.end()) return true;
    else return false;
}

bool MultiView::hasCharts2D()
{
    return !chart_map.empty();
}

bool MultiView::hasRenders3D()
{
    return !render_map.empty();
}

QDockWidget* MultiView::removeCurrentView()
{
    return this->removeView(current_index);
}

QDockWidget* MultiView::removeView(int id)
{
    //std::cout << "removing view: " << id << std::endl;

    if(id == INVALID) return NULL; // there are no views, so do nothing

    QDockWidget* dock = this->getDock(id);

    // fix dock indices of subsequent docks
    dock_index_map.erase(dock);
    for(DockIndexMap::iterator iter = dock_index_map.begin(); iter != dock_index_map.end(); iter++)
    {
        if(iter->second > id)
            (iter->second)--;
    }

    if(currentIsRender3D())
    {
        vtk_connections->Disconnect(render_map[dock]->GetRenderWindow()->GetInteractor());
        render_editor_map.erase(dock);
        render_map.erase(dock);
    }
    else if(currentIsChart2D())
    {
        vtk_connections->Disconnect(chart_map[dock]->GetRenderWindow()->GetInteractor());
        chart_editor_map.erase(dock);
        chart_map.erase(dock);
    }

    if(id > 0) current_index = id-1;
    else current_index = INVALID; // otherwise, index_cont is empty .. so set the current_index to 'invalid'

    return dock;
}

void MultiView::resetTables()
{
    tables.clear();
}

void MultiView::resetGrid()
{
//    grid->Initialize();
//    grid->Allocate(1000,1000); // default allocate method used by constructor

    multigrid->PrepareForNewData();
}

Render3D* MultiView::getCurrentRender3D()
{
    if(!this->currentIsRender3D()) return NULL;
    else return render_map[this->getDock(current_index)];
}

Chart2D*  MultiView::getCurrentChart2D()
{
    if(!this->currentIsChart2D()) return NULL;
    else return chart_map[this->getDock(current_index)];
}

MultiView::Table MultiView::getTable(std::size_t index)
{
    return tables.at(index).table;
}

QString MultiView::getTableName(std::size_t index)
{
    return tables.at(index).name;
}

MultiView::Tables MultiView::getTables()
{
    return tables;
}

MultiView::MultiGrid MultiView::getGrid()
{
    return multigrid;
}

RenderEditor* MultiView::getCurrentRenderEditor()
{
    if(!this->currentIsRender3D()) return NULL;
    else return render_editor_map[this->getDock(current_index)];
}

ChartEditor* MultiView::getCurrentChartEditor()
{
    if(!this->currentIsChart2D()) return NULL;
    else return chart_editor_map[this->getDock(current_index)];
}

int MultiView::getCurrentViewIndex()
{
    return current_index;
}

int MultiView::getNumberOfViews()
{
    return dock_index_map.size();
}

void MultiView::setCurrentLogScale(bool state)
{
    this->getCurrentRenderEditor()->setLogState(state);
}

void MultiView::update()
{
    emit chartUpdated();
}

void MultiView::resetCurrentView()
{
    if(this->currentIsRender3D())
    {
        this->getCurrentRender3D()->reset_view();
        this->getCurrentRender3D()->update();
    }
    else if(this->currentIsChart2D())
    {
        this->getCurrentChart2D()->reset_view();
    }
    else return;
}

std::size_t MultiView::addTable(QString const& name, Table table)
{
//    qDebug() << "inserting table with index: " << table_index;
//    qDebug() << "pre table size: " << tables.size();
    std::pair<Tables::iterator,bool> result = tables.insert(MultiView::Tables::value_type(table_index, TableEntry(name, table)));
//    qDebug() << "new table added? " << result.second;
//    qDebug() << "post table size: " << tables.size();
    return table_index++;
}

void MultiView::removeTable(std::size_t id)
{
    tables.erase(id);
    emit tableRemoved(id);
}

void MultiView::takeScreenshotOfActiveView(QString const& filename)
{
    vtkRenderWindow* renderwindow = NULL;
    if(this->currentIsChart2D())
        renderwindow = getCurrentChart2D()->GetRenderWindow();
    else if(this->currentIsRender3D())
        renderwindow = getCurrentRender3D()->GetRenderWindow();

    if(!renderwindow)
    {
        QMessageBox::warning(NULL, QString("Error"), QString("The screenshot could not be recorded as there is no suitable active render window"));
        return;
    }

    vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter =
      vtkSmartPointer<vtkWindowToImageFilter>::New();
    renderwindow->OffScreenRenderingOn(); // for taking the screen, we need to turn this on
    windowToImageFilter->SetInput(renderwindow);
    windowToImageFilter->SetInputBufferTypeToRGBA();
    windowToImageFilter->Update();

    vtkSmartPointer<vtkPNGWriter> writer =
      vtkSmartPointer<vtkPNGWriter>::New();
    writer->SetFileName(filename.toStdString().c_str());
    writer->SetInputConnection(windowToImageFilter->GetOutputPort());
    writer->Write();

    renderwindow->OffScreenRenderingOff(); // and now for normal rendering, turn it off again!
}
