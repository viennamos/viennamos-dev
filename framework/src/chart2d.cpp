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

#include "chart2d.h"
#include "multiview.h"

#include <QDebug>
#include <vtkPlotPoints.h>
#include <vtkAxis.h>
#include <vtkTextProperty.h>

Chart2D::Chart2D(MultiView* multiview, Chart2D::Tables& tables, QWidget *parent) :
        QVTKWidget(parent),
        tables(tables),
        multiview(multiview)
{
    mapper = vtkSmartPointer<Mapper>::New();

    actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(actor);

    // QVTKWidget has already created a render window and interactor
    renderWindow = this->GetRenderWindow();
    renderWindow->AddRenderer(renderer);
    interactor = renderWindow->GetInteractor();
    style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    interactor->SetInteractorStyle(style);

    view = vtkSmartPointer<vtkContextView>::New();
    view->SetInteractor(this->GetInteractor());
    this->SetRenderWindow(view->GetRenderWindow());

    // Add multiple line plots, setting the colors etc
    chart = vtkSmartPointer<vtkChartXY>::New();
    view->GetScene()->AddItem(chart);

    // provide an index mapping from the position in the combobox
    // with vtk indices
    vtk_marker[0] = vtkPlotPoints::NONE;
    vtk_marker[1] = vtkPlotPoints::CROSS;
    vtk_marker[2] = vtkPlotPoints::PLUS;
    vtk_marker[3] = vtkPlotPoints::SQUARE;
    vtk_marker[4] = vtkPlotPoints::CIRCLE;
    vtk_marker[5] = vtkPlotPoints::DIAMOND;

    vtk_line[0] = vtkPen::NO_PEN;
    vtk_line[1] = vtkPen::SOLID_LINE;
    vtk_line[2] = vtkPen::DASH_LINE;
    vtk_line[3] = vtkPen::DOT_LINE;
    vtk_line[4] = vtkPen::DASH_DOT_LINE;
    vtk_line[5] = vtkPen::DASH_DOT_DOT_LINE;

    this->reset();
}

void Chart2D::reset()
{
//    renderer->SetBackground(0.0, 0.0, 0.0);
}

void Chart2D::reset_view()
{
    chart->RecalculateBounds();
}

Chart2D::StringList Chart2D::listQuantities(std::size_t table_index)
{
    StringList names;

//    qDebug() << "index: " << table_index << " size: " << tables.size();

    if(tables.find(table_index) != tables.end())
    {
        for(int i = 0; i < tables.at(table_index).table->GetNumberOfColumns(); i++)
        {
            names.append(QString(tables.at(table_index).table->GetColumnName(i)));
        }
    }
    return names;
}

void Chart2D::setBackgroundColor(QColor color)
{
    renderer->SetBackground(color.redF(), color.greenF(), color.blueF());
}

void Chart2D::showLegend(bool state)
{
    chart->SetShowLegend(state);
}

void Chart2D::showGrid(bool state)
{
    chart->GetAxis(vtkAxis::BOTTOM)->SetGridVisible(state);
    chart->GetAxis(vtkAxis::LEFT)->SetGridVisible(state);
}

void Chart2D::showLabels(bool state)
{
    chart->GetAxis(vtkAxis::BOTTOM)->SetLabelsVisible(state);
    chart->GetAxis(vtkAxis::LEFT)->SetLabelsVisible(state);
}

void Chart2D::setXAxisLabel(QString const& label)
{
    chart->GetAxis(vtkAxis::BOTTOM)->SetTitle(label.toStdString().c_str());
}

void Chart2D::setYAxisLabel(QString const& label)
{
    chart->GetAxis(vtkAxis::LEFT)->SetTitle(label.toStdString().c_str());
}

void Chart2D::setXAxisLogScale(bool state)
{
    chart->GetAxis(vtkAxis::BOTTOM)->SetLogScale(state);
}

void Chart2D::setYAxisLogScale(bool state)
{
    chart->GetAxis(vtkAxis::LEFT)->SetLogScale(state);
}

void Chart2D::setTitle(QString const& title)
{
    chart->SetTitle(title.toStdString());
}

void Chart2D::setTitleFontSize(int font_size)
{
    chart->GetTitleProperties()->SetFontSize(font_size);
}

void Chart2D::setAxisLabelFontSize(int font_size)
{
    chart->GetAxis(vtkAxis::BOTTOM)->GetTitleProperties()->SetFontSize(font_size);
    chart->GetAxis(vtkAxis::LEFT)->GetTitleProperties()->SetFontSize(font_size);
}

void Chart2D::clear()
{
    chart->ClearPlots();
}

void Chart2D::addPlot(int x_index, ChartEntry& entry,
                      bool useIndexForXSeries, std::size_t table_index)
{
    // transfer the 'label' string to the 'legend'
    tables.at(table_index).table->GetColumn(entry.index)->SetName(entry.label.toStdString().c_str());

    vtkPlot *line = NULL;
    line = chart->AddPlot(vtkChart::LINE);

//    qDebug() << "addPlot:: table " << table_index << " useindex " << useIndexForXSeries << " xindex " << x_index
//             << " entryidx " << entry.index << " label " << entry.label;


    line->SetUseIndexForXSeries(useIndexForXSeries);
#if VTK_MAJOR_VERSION <= 5
    line->SetInput(tables.at(table_index).table, x_index, entry.index);
#else
    line->SetInputData(tables.at(table_index).table, x_index, entry.index);
#endif
    if(!entry.auto_color)
        line->SetColor(entry.color_rgb.redF(), entry.color_rgb.greenF(), entry.color_rgb.blueF());
    line->SetWidth(entry.thickness);
    line->GetPen()->SetLineType(vtk_line[entry.line_style]);
    vtkPlotPoints::SafeDownCast(line)->SetMarkerStyle(vtk_marker[entry.marker_style]);
    vtkPlotPoints::SafeDownCast(line)->SetMarkerSize(entry.marker_size);
}

void Chart2D::showAllPlots()
{
    emit signalShowAllPlots();
}

std::size_t Chart2D::tableSize()
{
    return tables.size();
}

QString Chart2D::getTableName(std::size_t index)
{
    return tables.at(index).name;
}

Chart2D::Tables& Chart2D::getTables()
{
    return tables;
}

void Chart2D::removeTable(std::size_t index)
{
    multiview->removeTable(index);
}

void Chart2D::update()
{
    chart->Update();
}

bool Chart2D::tableHasColumn(int table_id, QString const& key)
{
    if(tables.at(table_id).table->GetColumnByName(key.toStdString().c_str())) return true;
    else return false;
}
