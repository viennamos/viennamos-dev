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

#include "charteditor.h"
#include "ui_charteditor.h"

#include <iterator>

#include <QDebug>
#include <QMessageBox>

#include <vtkPlotPoints.h>

ChartEditor::ChartEditor(Chart2D* chart_view, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChartEditor),
    chart_view(chart_view)//, multi_view(multi_view)
{
    ui->setupUi(this);
    this->setWindowTitle("Chart Editor");

    colorDialogLine       = new QColorDialog(this);


    // prepare the comboboxes
    ui->comboBoxLineStyle->addItem("None");
    ui->comboBoxLineStyle->addItem("Solid");
    ui->comboBoxLineStyle->addItem("Dash");
    ui->comboBoxLineStyle->addItem("Dot");
    ui->comboBoxLineStyle->addItem("Dash Dot");
    ui->comboBoxLineStyle->addItem("Dash Dot Dot");

    // setup and map the marker types
    // note that the indices of the map and the insertion indices
    // have to be the same
    ui->comboBoxMarkerStyle->insertItem(0, "None");
    ui->comboBoxMarkerStyle->insertItem(1, "Cross");
    ui->comboBoxMarkerStyle->insertItem(2, "Plus");
    ui->comboBoxMarkerStyle->insertItem(3, "Square");
    ui->comboBoxMarkerStyle->insertItem(4, "Circle");
    ui->comboBoxMarkerStyle->insertItem(5, "Diamond");

    ui->doubleSpinBoxThickness->setMinimum(1.0);

    // default colors
    default_line_color.setRgbF(0.0, 0.0, 0.0);

    default_line_width   = 2.0;
    default_line_style   = 1;
    default_marker_style = 0;
    default_marker_size  = 10.0;

    color_view_border_style = "border: 1px solid black";

    ui->tableWidgetTableList->verticalHeader()->setVisible(false);
    ui->tableWidgetTableList->setColumnCount(2);
    ui->tableWidgetTableList->setSelectionBehavior(QAbstractItemView::SelectRows);
    QStringList header;
    header << "ID" << "Table";
    ui->tableWidgetTableList->setHorizontalHeaderLabels(header);

    this->reset();
}

ChartEditor::~ChartEditor()
{
    delete ui;
}

void ChartEditor::connect_signals()
{
    QObject::connect(ui->toolButtonLineColor,       SIGNAL(clicked()), colorDialogLine,        SLOT(show()));
    QObject::connect(colorDialogLine,       SIGNAL(currentColorChanged(QColor)), this, SLOT(updateLineColor(QColor)));
    QObject::connect(ui->pushButtonApply, SIGNAL(clicked()), this, SLOT(apply()));
    QObject::connect(ui->pushButtonReset, SIGNAL(clicked()), this, SLOT(reset()));
    QObject::connect(ui->pushButtonClose, SIGNAL(clicked()), this, SLOT(close()));
    QObject::connect(ui->radioButtonUseArrayIndices, SIGNAL(clicked()), this, SLOT(toggleUseArrayIndices()));
    QObject::connect(ui->radioButtonUseQuantity,     SIGNAL(clicked()), this, SLOT(toggleUseQuantity()));
    QObject::connect(ui->lineEditLabel, SIGNAL(editingFinished()), this, SLOT(processLabel()));
    QObject::connect(ui->doubleSpinBoxThickness, SIGNAL(valueChanged(double)), this, SLOT(processLineThickness()));
    QObject::connect(ui->comboBoxLineStyle, SIGNAL(currentIndexChanged(int)), this, SLOT(processLineStyle()));
    QObject::connect(ui->comboBoxMarkerStyle, SIGNAL(currentIndexChanged(int)), this, SLOT(processMarkerStyle()));
    QObject::connect(ui->doubleSpinBoxMarkerSize, SIGNAL(valueChanged(double)), this, SLOT(processMarkerSize()));
    QObject::connect(ui->checkBoxAutoColor, SIGNAL(toggled(bool)), this, SLOT(toggleLineColor(bool)));
    QObject::connect(ui->checkBoxEnableAll, SIGNAL(clicked(bool)), this, SLOT(toggleEnableAll(bool)));
    QObject::connect(ui->comboBoxQuantities, SIGNAL(currentIndexChanged(int)), this, SLOT(processXQuantity(int)));
    QObject::connect(ui->listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(updateLineProperties(int)));
    QObject::connect(ui->listWidget, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(processCheckState(QListWidgetItem*)));
    QObject::connect(ui->tableWidgetTableList, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(showTableParameters(int, int, int, int)));
    QObject::connect(ui->listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(checkQuantitySelection()));
}

void ChartEditor::disconnect_signals()
{
    QObject::disconnect(ui->toolButtonLineColor);
    QObject::disconnect(colorDialogLine);
    QObject::disconnect(ui->pushButtonApply);
    QObject::disconnect(ui->pushButtonReset);
    QObject::disconnect(ui->pushButtonClose);
    QObject::disconnect(ui->radioButtonUseQuantity);
    QObject::disconnect(ui->lineEditLabel);
    QObject::disconnect(ui->doubleSpinBoxThickness);
    QObject::disconnect(ui->comboBoxLineStyle);
    QObject::disconnect(ui->comboBoxMarkerStyle);
    QObject::disconnect(ui->doubleSpinBoxMarkerSize);
    QObject::disconnect(ui->checkBoxAutoColor);
    QObject::disconnect(ui->checkBoxEnableAll);
    QObject::disconnect(ui->comboBoxQuantities);
    QObject::disconnect(ui->listWidget);
    QObject::disconnect(ui->listWidget);
    QObject::disconnect(ui->tableWidgetTableList);
}


// this function is called, when the editor is opened and presented to
// the user
void ChartEditor::showEvent(QShowEvent *)
{
    if(ui->listWidget->count() == 0)
        this->update();
}

/** @brief This function is called when a ViennaMOS module stored
 *  new data in the central data structures */
void ChartEditor::update()
{
//    qDebug() << "update:begin";

    typedef std::map<std::size_t, TableEntry> Tables;
    Tables new_tables = chart_view->getTables();

//    qDebug() << "new table size " << new_tables.size() << " vs old table size " << previous_tables.size();

    typedef std::vector<std::size_t>   Indices;
    Indices new_table_indices;

    // if the previous tables container is empty, e.g., in the case
    // of the first execution of this update function, populate the indices
    // with the indices of the central table set
    //
    if(previous_tables.empty())
    {
        for(Tables::iterator iter = new_tables.begin(); iter != new_tables.end(); iter++)
            new_table_indices.push_back(iter->first);
    }
    // find out which of the tables in the actual table set are new ones
    // compared to the previously processed tables.
    //
    else
    {
        for(Tables::iterator new_iter = new_tables.begin(); new_iter != new_tables.end(); new_iter++)
        {
            typedef Tables::iterator TableIterator;
            TableIterator detector = previous_tables.end();
            for (TableIterator old_iter = previous_tables.begin(); old_iter != previous_tables.end(); ++old_iter )
                if (old_iter->second == new_iter->second)
                    detector = old_iter;

            // the current 'new' table is not available in the old table set
            if(detector == previous_tables.end())
            {
                new_table_indices.push_back(new_iter->first);
            }
        }
//        for(std::size_t ti = 0; ti < new_tables.size(); ti++)
//        {
//            typedef std::map<std::size_t, TableEntry>::iterator TableIterator;
//            TableIterator iter = previous_tables.end();
//            for (TableIterator it = previous_tables.begin(); it != previous_tables.end(); ++it )
//                if (it->second == new_tables.at(ti))
//                    iter = it;

//            // the current 'new' table is not available in the old table set
//            if(iter == previous_tables.end())
//            {
//                new_table_indices.push_back(ti);
//            }
//        }
    }

    // deal with each new table stored in the central table set
    // for each table indexed in the 'new_table_indices' container,
    // create a new table entry in the ui and also add a new entry in the
    // table_parameters data structure
    //
    ui->tableWidgetTableList->blockSignals(true);
    for(Indices::iterator iter = new_table_indices.begin();
        iter != new_table_indices.end(); iter++)
    {
        std::size_t ti = *iter;

//        qDebug() << "update: adding table to ui - id " << ti << " name " << chart_view->getTableName(ti);

        // add the table at the end ..
        //
        int row = ui->tableWidgetTableList->rowCount();
        ui->tableWidgetTableList->insertRow(row);

        // here we store the table id (being the access index in the new_tables map container)
        // in the first cell of the row. we need this unique global id later on,
        // to stay sane when we will remove tables ..
        //
        QTableWidgetItem *index_item = new QTableWidgetItem(QString::number(ti));
        index_item->setFlags(index_item->flags() ^ Qt::ItemIsEditable); // make the item not editable
        ui->tableWidgetTableList->setItem(row, 0, index_item);

        QTableWidgetItem *name_item = new QTableWidgetItem(chart_view->getTableName(ti));
        name_item->setFlags(name_item->flags() ^ Qt::ItemIsEditable); // make the item not editable
        ui->tableWidgetTableList->setItem(row, 1, name_item);


        Chart2D::StringList quantities = chart_view->listQuantities(ti);

        //table_parameters.push_back(ChartTableParameters());
        table_parameters.insert(TableParameters::value_type(ti, ChartTableParameters()));
        std::size_t quan_cnt = 0;
        foreach(QString key, quantities) {
            table_parameters.at(ti).chart_entries.push_back(
                        ChartEntry(
                            quan_cnt++,
                            key,
                            key,
                            default_line_color,
                            true,
                            default_line_width,
                            default_line_style,
                            default_marker_style,
                            default_marker_size,
                            true
                            )
                        );
        }
    }

    ui->tableWidgetTableList->resizeColumnsToContents();

    // store current table set for the next round ..
    //
    previous_tables = new_tables;

    // by default, select the first table and show the respective quantities
    //
    ui->tableWidgetTableList->blockSignals(false);
    ui->tableWidgetTableList->setCurrentCell(0,0); // emits a signal -> showTableParameters()
//    qDebug() << "update:end";
}





void ChartEditor::showTableParameters(int table_row, int, int, int)
{
//    qDebug() << "showTableParameters:begin";

    if((table_row >= 0) && (table_row < ui->tableWidgetTableList->rowCount()))
    {
        //ChartTableParameters::ChartEntries& paras = table_parameters[table_row].chart_entries;

        // from the currently selected row, retrieve the table id by accessing the
        // data in the first cell.
        //
        int table_id = ui->tableWidgetTableList->item(table_row, 0)->text().toInt();

//        qDebug() << "showTableParameters: table_id " << table_id;

        Chart2D::StringList quantities = chart_view->listQuantities(table_id);

//        qDebug() << "quantities size: " << quantities.size();

        // [JW] make sure that the combobox doesn't fire signals,
        // as it would overwrite data on the table_parameters datastructure via the
        // processXQuantity function ..
        //
        ui->comboBoxQuantities->blockSignals(true);
        ui->comboBoxQuantities->clear();
        ui->comboBoxQuantities->addItems(quantities);
        ui->comboBoxQuantities->setCurrentIndex(table_parameters.at(table_id).x_quantity_index);
        ui->comboBoxQuantities->blockSignals(false);

        ui->listWidget->clear();

//        qDebug() << "populating the list .." ;
        for(std::size_t qi = 0; qi < table_parameters.at(table_id).chart_entries.size(); qi++)
        {
            QListWidgetItem *item = new QListWidgetItem(table_parameters.at(table_id).chart_entries[qi].label, ui->listWidget);

            if(table_parameters.at(table_id).chart_entries[qi].enabled)
                item->setCheckState(Qt::Checked);
            else
                item->setCheckState(Qt::Unchecked);

            ui->listWidget->addItem(item);
        }

        if(table_parameters.at(table_id).useIndexForXSeries)
        {
//            qDebug() << "setting the radios!";
            ui->radioButtonUseArrayIndices->setChecked(true);   // emits signal
            ui->radioButtonUseQuantity->setChecked(false);     // emits signal
        }
        else
        {
            ui->radioButtonUseQuantity->setChecked(true);      // emits signal
            ui->radioButtonUseArrayIndices->setChecked(false);   // emits signal
        }

        this->toggleUseArrayIndices();
        this->toggleUseQuantity();

        ui->checkBoxEnableAll->blockSignals(true);
        ui->checkBoxEnableAll->setChecked(table_parameters.at(table_id).enableAll);
        ui->checkBoxEnableAll->blockSignals(false);
    }
//    qDebug() << "check table size: " << chart_view->tableSize();
//    qDebug() << "showTableParameters:end";
}

QString ChartEditor::getStyleStringFromQColor(QColor color)
{
    QString style = "rgb(%1, %2, %3)";
    return style.arg(color.red()).arg(color.green()).arg(color.blue());
}

// function allows to reset the parameters of the editor to a defined, specific
// setting
void ChartEditor::reset()
{
    this->disconnect_signals();


//    qDebug() << "reset:begin";
    ui->pushButtonLineColorViewer->setFlat(true);
    ui->pushButtonLineColorViewer->autoFillBackground();
    QString style = "background-color: " + getStyleStringFromQColor(default_line_color) +" ;";
    style += color_view_border_style;
    ui->pushButtonLineColorViewer->setStyleSheet(style);
    ui->radioButtonUseArrayIndices->setChecked(true);
    ui->doubleSpinBoxThickness->setValue(default_line_width);
    ui->comboBoxLineStyle->setCurrentIndex(default_line_style);
    ui->comboBoxMarkerStyle->setCurrentIndex(default_marker_style);
    ui->doubleSpinBoxMarkerSize->setValue(default_marker_size);
    this->setEnableYAxisProperties(false);
    ui->checkBoxAutoColor->setChecked(true);
    this->toggleLineColor(ui->checkBoxAutoColor->isChecked());
    ui->spinBoxAxisFontSize->setValue(20);
    ui->spinBoxTitleFontSize->setValue(14);
    ui->lineEditXLabel->setText("X-Axis");
    ui->lineEditYLabel->setText("Y-Axis");
    ui->lineEditTitleLabel->setText("");
    ui->checkBoxXLog->setChecked(false);
    ui->checkBoxYLog->setChecked(false);
    ui->checkBoxShowLegend->setChecked(true);
    ui->checkBoxShowGrid->setChecked(true);
    ui->checkBoxShowLabels->setChecked(true);
    ui->checkBoxEnableAll->setChecked(true);
//    qDebug() << "reset:end";

    if(ui->listWidget->count() > 0)
        toggleEnableAll(ui->checkBoxEnableAll->isChecked());

    this->connect_signals();
}

// function is called when the 'apply' button is pressed
void ChartEditor::apply()
{
//    qDebug() << "apply:begin";
    chart_view->clear();
    chart_view->showLegend(ui->checkBoxShowLegend->isChecked());
    chart_view->showLabels(ui->checkBoxShowLabels->isChecked());
    chart_view->showGrid(ui->checkBoxShowGrid->isChecked());
    chart_view->setTitle(ui->lineEditTitleLabel->text());
    chart_view->setTitleFontSize(ui->spinBoxTitleFontSize->value());
    chart_view->setXAxisLabel(ui->lineEditXLabel->text());
    chart_view->setYAxisLabel(ui->lineEditYLabel->text());
    chart_view->setXAxisLogScale(ui->checkBoxXLog->isChecked());
    chart_view->setYAxisLogScale(ui->checkBoxYLog->isChecked());
    chart_view->setAxisLabelFontSize(ui->spinBoxAxisFontSize->value());

    //for(std::size_t ti = 0; ti < table_parameters.size(); ti++)
    for(TableParameters::iterator tit = table_parameters.begin();
        tit != table_parameters.end(); tit++)
    {
       // chart_view->getTables()[ti].table->Dump();

        for(std::size_t qi = 0; qi < (tit->second).chart_entries.size(); qi++)
        {
            if((tit->second).chart_entries[qi].enabled)
            {
//                qDebug() << "apply::adding plot: " << table_parameters[ti].x_quantity_index
//                                                   << table_parameters[ti].useIndexForXSeries
//                                                   << ti;
                chart_view->addPlot((tit->second).x_quantity_index,
                                    (tit->second).chart_entries[qi],
                                    (tit->second).useIndexForXSeries,
                                    tit->first); // TODO make this more efficient ..
            }
        }
    }

    chart_view->update();
//    qDebug() << "apply:end";
}

void ChartEditor::toggleUseArrayIndices()
{
//    qDebug() << "toggleUseArrayIndices";
    if(ui->radioButtonUseArrayIndices->isChecked())
    {
//        qDebug() << "disabling combo";
        ui->comboBoxQuantities->setEnabled(false);
        if(!table_parameters.empty())
        {
//            qDebug() << "toggleUseArrayIndices::using array indices for table " << ui->tableWidgetTableList->currentRow();
            table_parameters.at(getCurrentTableID()).useIndexForXSeries = true;
        }
    }
}

void ChartEditor::toggleUseQuantity()
{
//    qDebug() << "toggleUseQuantity";
    if(ui->radioButtonUseQuantity->isChecked())
    {
//        qDebug() << "enabling combo";
        ui->comboBoxQuantities->setEnabled(true);
        if(!table_parameters.empty())
        {
//            qDebug() << "toggleUseQuantity::using quantity for x for table " << ui->tableWidgetTableList->currentRow();
            table_parameters.at(getCurrentTableID()).useIndexForXSeries = false;
        }
    }

}

void ChartEditor::processLabel()
{
    int current_table = getCurrentTableID();
    int current_row = ui->listWidget->currentRow();
    if(current_row < 0) return;
    if(std::size_t(current_row) < table_parameters.at(current_table).chart_entries.size())
    {
        if(chart_view->tableHasColumn(current_table, ui->lineEditLabel->text()))
        {
            QMessageBox::critical(this, "Error", "The label is already taken, please use a unique label string.");
            ui->lineEditLabel->setText(table_parameters.at(current_table).chart_entries[ui->listWidget->currentRow()].label);
            return;
        }
        else
            table_parameters.at(current_table).chart_entries[ui->listWidget->currentRow()].label = ui->lineEditLabel->text();
    }
}

void ChartEditor::processLineThickness()
{
    int current_table = getCurrentTableID();
    int current_row = ui->listWidget->currentRow();
    if((current_row >= 0) && (std::size_t(current_row) < table_parameters.at(current_table).chart_entries.size()))
        table_parameters.at(current_table).chart_entries[ui->listWidget->currentRow()].thickness = ui->doubleSpinBoxThickness->value();
}

void ChartEditor::processLineStyle()
{
    int current_table = getCurrentTableID();
    int current_row = ui->listWidget->currentRow();
    if((current_row >= 0) && (std::size_t(current_row) < table_parameters.at(current_table).chart_entries.size()))
        table_parameters.at(current_table).chart_entries[ui->listWidget->currentRow()].line_style = ui->comboBoxLineStyle->currentIndex();
}

void ChartEditor::processMarkerStyle()
{
    int current_table = getCurrentTableID();
    int current_row = ui->listWidget->currentRow();
    if((current_row >= 0) && (std::size_t(current_row) < table_parameters.at(current_table).chart_entries.size()))
        table_parameters.at(current_table).chart_entries[ui->listWidget->currentRow()].marker_style = ui->comboBoxMarkerStyle->currentIndex();
}

void ChartEditor::processMarkerSize()
{
    int current_table = getCurrentTableID();
    int current_row = ui->listWidget->currentRow();
    if((current_row >= 0) && (std::size_t(current_row) < table_parameters.at(current_table).chart_entries.size()))
        table_parameters.at(current_table).chart_entries[ui->listWidget->currentRow()].marker_size = ui->doubleSpinBoxMarkerSize->value();
}

void ChartEditor::updateLineProperties(int row)
{
    int current_table = getCurrentTableID();
    if((row >= 0) && (std::size_t(row) < table_parameters.at(current_table).chart_entries.size()))
    {
        this->setEnableYAxisProperties(true);
        ui->lineEditLabel->setText(table_parameters.at(current_table).chart_entries[row].label);
        this->updateLineColor(table_parameters.at(current_table).chart_entries[row].color_rgb);
        ui->doubleSpinBoxThickness->setValue(table_parameters.at(current_table).chart_entries[row].thickness);
        ui->comboBoxLineStyle->setCurrentIndex(table_parameters.at(current_table).chart_entries[row].line_style);
        ui->comboBoxMarkerStyle->setCurrentIndex(table_parameters.at(current_table).chart_entries[row].marker_style);
        ui->checkBoxAutoColor->setChecked(table_parameters.at(current_table).chart_entries[row].auto_color);
        ui->doubleSpinBoxMarkerSize->setValue(table_parameters.at(current_table).chart_entries[row].marker_size);
        this->toggleLineColor(ui->checkBoxAutoColor->isChecked());
    }
}

void ChartEditor::setEnableYAxisProperties(bool state)
{
    ui->lineEditLabel->setEnabled(state);
    ui->doubleSpinBoxThickness->setEnabled(state);
    ui->comboBoxLineStyle->setEnabled(state);
    ui->comboBoxMarkerStyle->setEnabled(state);
    ui->checkBoxAutoColor->setEnabled(state);
    ui->pushButtonLineColorViewer->setEnabled(state);
    ui->doubleSpinBoxMarkerSize->setEnabled(state);
}

void ChartEditor::toggleLineColor(bool state)
{
    ui->toolButtonLineColor->setEnabled(!state);
    ui->pushButtonLineColorViewer->setEnabled(!state);

    if(state)
    {
        ui->pushButtonLineColorViewer->setFlat(true);
        ui->pushButtonLineColorViewer->autoFillBackground();
        QString style;
        style += color_view_border_style;
        ui->pushButtonLineColorViewer->setStyleSheet(style);
    }
    else
    {
        if(!table_parameters.empty())
        {
            int current_table = getCurrentTableID();
            int current_row = ui->listWidget->currentRow();
            if((current_row >= 0) && (std::size_t(current_row) < table_parameters.at(current_table).chart_entries.size()))
            {
                ui->pushButtonLineColorViewer->setFlat(true);
                ui->pushButtonLineColorViewer->autoFillBackground();
                QString style = "background-color: " + getStyleStringFromQColor(table_parameters.at(current_table).chart_entries[current_row].color_rgb) +" ;";
                style += color_view_border_style;
                ui->pushButtonLineColorViewer->setStyleSheet(style);
            }
        }
    }
    if(!table_parameters.empty())
    {
        int current_table = getCurrentTableID();
        int current_row = ui->listWidget->currentRow();
        if((current_row >= 0) && (std::size_t(current_row) < table_parameters.at(current_table).chart_entries.size()))
            table_parameters.at(current_table).chart_entries[ui->listWidget->currentRow()].auto_color = state;
    }
}

void ChartEditor::updateLineColor(QColor new_color)
{
    int current_table = getCurrentTableID();
    ui->pushButtonLineColorViewer->setFlat(true);
    ui->pushButtonLineColorViewer->autoFillBackground();
    QString style = "background-color: " + getStyleStringFromQColor(new_color) +" ;";
    style += color_view_border_style;
    ui->pushButtonLineColorViewer->setStyleSheet(style);

    int row = ui->listWidget->currentRow();
    if((row >= 0) && (std::size_t(row) < table_parameters.at(current_table).chart_entries.size()))
    {
        table_parameters.at(current_table).chart_entries[row].color_rgb = new_color;
    }
}

void ChartEditor::processXQuantity(int index)
{
   table_parameters.at(getCurrentTableID()).x_quantity_index = index;
}

void ChartEditor::processCheckState(QListWidgetItem* item)
{
    int current_table = getCurrentTableID();
    int item_row = ui->listWidget->row(item);

    if(item->checkState() == Qt::Checked)
        table_parameters.at(current_table).chart_entries[item_row].enabled = true;
    else
        table_parameters.at(current_table).chart_entries[item_row].enabled = false;
}

void ChartEditor::toggleEnableAll(bool state)
{
    if(ui->tableWidgetTableList->rowCount() > 0)
    {
        int current_table = getCurrentTableID();
        table_parameters.at(current_table).enableAll = state;

        //qDebug() << "current table " << current_table << " state " << state << " chart entries " << table_parameters.at(current_table).chart_entries.size();

        for(std::size_t qi = 0; qi < table_parameters.at(current_table).chart_entries.size(); qi++)
        {
            //qDebug() << "   quan " << qi << " state " << state;
            table_parameters.at(current_table).chart_entries[qi].enabled = state;
        }
        this->showTableParameters(ui->tableWidgetTableList->currentRow());
    }
}

void ChartEditor::on_pushButtonAdopt_clicked()
{
    if(ui->tableWidgetTableList->rowCount() == 0) return;

    this->setEnabled(false);

    std::size_t current_table = getCurrentTableID();

    // traverse all quantities of the currently selected table
    for(std::size_t qi = 0; qi < table_parameters.at(current_table).chart_entries.size(); qi++)
    {
        // extract for each quantity the key and the state: enabled/disabled
        bool    reference_table_state = table_parameters.at(current_table).enableAll;
        QString reference_key    = table_parameters.at(current_table).chart_entries[qi].key;
        bool    reference_state = table_parameters.at(current_table).chart_entries[qi].enabled;

        // for each quantity, we traverse the set of available tables ..
        for(TableParameters::iterator tpiter = table_parameters.begin();
            tpiter != table_parameters.end(); tpiter++)
        {
            // .. and skip processing if it's the 'currently selected one'
            if(tpiter->first == current_table) continue;

            // adopt the table 'enable all' checkbox state
            tpiter->second.enableAll = reference_table_state;

            // traverse for each other table the set of quantities
            for(std::size_t qi2 = 0; qi2 < tpiter->second.chart_entries.size(); qi2++)
            {
                // check if a quentity with the same key is present ..
                if(tpiter->second.chart_entries[qi2].key == reference_key )
                {
                    //qDebug() << "adopting table " << ti << " quantity-id " << qi2 << " to " << reference_state;
                    // and adopt the enable-state
                    tpiter->second.chart_entries[qi2].enabled = reference_state;
                }
            }
        }


//        // for each quantity, we traverse the set of available tables ..
//        for(std::size_t ti = 0; ti < table_parameters.size(); ti++)
//        {
//            // .. and skip processing if it's the 'currently selected one'
//            if(ti == current_table) continue;

//            // adopt the table 'enable all' checkbox state
//            table_parameters.at(ti).enableAll = reference_table_state;

//            // traverse for each other table the set of quantities
//            for(std::size_t qi2 = 0; qi2 < table_parameters.at(ti).chart_entries.size(); qi2++)
//            {
//                // check if a quentity with the same key is present ..
//                if(table_parameters.at(ti).chart_entries[qi2].key == reference_key )
//                {
//                    //qDebug() << "adopting table " << ti << " quantity-id " << qi2 << " to " << reference_state;
//                    // and adopt the enable-state
//                    table_parameters.at(ti).chart_entries[qi2].enabled = reference_state;
//                }
//            }
//        }
    }

    this->setEnabled(true);
}


void ChartEditor::on_pushButtonRemoveTables_clicked()
{
    std::vector<int>    table_indices;
    QList<QTableWidgetItem*>  selected_items = ui->tableWidgetTableList->selectedItems();

    foreach(QTableWidgetItem* item, selected_items)
    {
        if(item->column() == 0)
        {
           int table_id = item->text().toInt();

           //Chart2D::Tables::iterator table_itr = chart_view->getTables().begin();
           //std::advance(table_itr, table_index);
           //chart_view->getTables().erase(table_id);
            chart_view->removeTable(table_id);
           //multi_view->removeTable(table_id);

           // here we really want to store the row
           // as we'll need this info for the subsequent step of
           // removing the rows
           //
           table_indices.push_back(item->row());

           //qDebug() << "removing table id " << table_id << " in row " << item->row();

           table_parameters.erase(table_id);
           //table_parameters.erase(table_parameters.begin()+table_index);
        }
    }

//    typedef std::map<std::size_t, TableEntry> Tables;
//    Tables tables = chart_view->getTables();
//    qDebug() << "available tables: ";
//    for(Tables::iterator iter = tables.begin(); iter != tables.end(); iter++)
//    {
//        qDebug() << "  " << iter->first;
//    }


//    qDebug() << "available table parameters: ";
//    for(TableParameters::iterator iter = table_parameters.begin(); iter != table_parameters.end(); iter++)
//    {
//        qDebug() << "  " << iter->first;
//    }

    previous_tables = chart_view->getTables();

    ui->tableWidgetTableList->blockSignals(true);

    // when removing a row from the table, we are messing with
    // the table indices. to be sure that we avoid that, and that
    // the index stored in the table_indices still points to a valid
    // entry in the table widget even after several removes,
    // we first sort the indices in ascending order and process the list
    // backwards.
    //
    std::sort(table_indices.begin(), table_indices.end());
    for(std::vector<int>::reverse_iterator iter = table_indices.rbegin();
        iter != table_indices.rend(); iter++)
    {
        ui->tableWidgetTableList->removeRow(*iter);
    }

//    qDebug() << "available ui tables: ";
//    for(int i = 0; i < ui->tableWidgetTableList->rowCount(); i++)
//    {
//        QTableWidgetItem* item = ui->tableWidgetTableList->item(i, 0);
//        qDebug() << "  " << item->row() << " id " << item->text();
//    }


    ui->tableWidgetTableList->update();

    if(ui->tableWidgetTableList->rowCount() > 0)
        ui->tableWidgetTableList->setCurrentCell(0,0);
    else
    {
        ui->comboBoxQuantities->blockSignals(true);
        ui->comboBoxQuantities->clear();
        ui->comboBoxQuantities->blockSignals(false);

        ui->listWidget->blockSignals(true);
        ui->listWidget->clear();
        ui->listWidget->blockSignals(false);
    }
    ui->tableWidgetTableList->blockSignals(false);
}

int ChartEditor::getCurrentTableID()
{
    return ui->tableWidgetTableList->item(ui->tableWidgetTableList->currentRow(), 0)->text().toInt();
}

void ChartEditor::checkQuantitySelection()
{
    setEnableYAxisProperties(!ui->listWidget->selectedItems().empty());
}
