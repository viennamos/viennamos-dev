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

#include "rendereditor.h"
#include "ui_rendereditor.h"

#include <QCheckBox>
#include <QPushButton>
#include <QGridLayout>
#include <QColor>
#include <QLabel>
#include <QKeyEvent>
#include <QDebug>

RenderEditor::RenderEditor(Render3D* render_view, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::RenderEditor),
    render_view(render_view)
{
    ui->setupUi(this);
    setWindowTitle("Render Editor");

    colorDialogBackground = new QColorDialog(this);
    colorDialogSolid      = new QColorDialog(this);
    colorDialogEdges      = new QColorDialog(this);
    colorDialogBarTitle   = new QColorDialog(this);
    colorDialogBarLabels  = new QColorDialog(this);
    colorDialogAxes       = new QColorDialog(this);

    QObject::connect(ui->pushButtonBackgroundColor, SIGNAL(clicked()),          colorDialogBackground,  SLOT(show()));
    QObject::connect(ui->pushButtonSolidColor,      SIGNAL(clicked()),          colorDialogSolid,       SLOT(show()));
    QObject::connect(ui->pushButtonEdgeColor,       SIGNAL(clicked()),          colorDialogEdges,       SLOT(show()));
    QObject::connect(ui->pushButtonTitleColor,      SIGNAL(clicked()),          colorDialogBarTitle,    SLOT(show()));
    QObject::connect(ui->pushButtonLabelsColor,     SIGNAL(clicked()),          colorDialogBarLabels,   SLOT(show()));
    QObject::connect(ui->pushButtonColorAxes,       SIGNAL(clicked()),          colorDialogAxes,        SLOT(show()));

    QObject::connect(ui->pushButtonApply,           SIGNAL(clicked()),          this,                   SLOT(apply()));
    QObject::connect(ui->pushButtonReset,           SIGNAL(clicked()),          this,                   SLOT(reset_dialog()));
    QObject::connect(ui->pushButtonClose,           SIGNAL(clicked()),          this,                   SLOT(close()));


    font_map["Arial"] = VTK_ARIAL;
    font_map["Times"] = VTK_TIMES;
    font_map["Courier"] = VTK_COURIER;

    for(FontMap::iterator iter = font_map.begin(); iter != font_map.end(); iter++)
    {
        ui->comboBoxTitleFonts->addItem (QString::fromStdString(iter->first));
        ui->comboBoxLabelsFonts->addItem(QString::fromStdString(iter->first));
    }
    ui->spinBoxLabels->setMinimum(2);

    ui->comboBoxFlyMode->addItem("StaticEdges");
    ui->comboBoxFlyMode->addItem("StaticTriad");
    ui->comboBoxFlyMode->addItem("OuterEdges");
    ui->comboBoxFlyMode->addItem("ClosestTriad");
    ui->comboBoxFlyMode->addItem("FurthestTriad");

    current_log_state = false;

    ui->tableWidgetSegments->verticalHeader()->setVisible(false);
    ui->tableWidgetSegments->setColumnCount(3);

    QStringList header;
    header << "Index" << "Opacity" << "Color";
    ui->tableWidgetSegments->setHorizontalHeaderLabels(header);

    reset();
}

RenderEditor::~RenderEditor()
{
    delete ui;
}

void RenderEditor::reset_dialog()
{
    if(QMessageBox::question(this, tr("Reset Render Editor"), tr("Are you sure you want to reset the render editor?"),
                             QMessageBox::Yes, QMessageBox::No, QMessageBox::NoButton) == QMessageBox::Yes)
    {
        this->reset();
    }
}

void RenderEditor::showEvent(QShowEvent *)
{
    // get the current color bar title
    ui->lineEditTitle->setText(render_view->get_colorbar()->GetTitle());
    current_bar_title = ui->lineEditTitle->text(); // backup for reset ..
}

/** @brief This function is called when a ViennaMOS module stored
 *  new data in the central data structures */
void RenderEditor::update()
{
    // get the segment colors for coloring the semgent table
    //
    std::vector<Render3D::RGB> segment_rgbs;
    render_view->extract_segment_colors(segment_rgbs);


    // there might be a new grid available, so
    // re-populate the segment table in the segment tab
    //
    ui->tableWidgetSegments->clearContents();
    ui->tableWidgetSegments->setRowCount(render_view->segment_size());
    for(int i = 0; i < ui->tableWidgetSegments->rowCount(); i++)
    {
        // the first item in the row provides the 'check' mechanism
        // and the segment index
        //
        QTableWidgetItem* id = new QTableWidgetItem(QString::number(i));
        id->setFlags(id->flags() ^ Qt::ItemIsEditable); // make the table not editable
        id->setCheckState(Qt::Checked);
        ui->tableWidgetSegments->setItem(i, 0, id);

        // the second item in the row provides a spinbox in
        // 0.0 <= x <= 1.0 for the opacity
        //
        QTableWidgetItem* opacity = new QTableWidgetItem;
        ui->tableWidgetSegments->setItem(i, 1, opacity);
        QDoubleSpinBox* spin = new QDoubleSpinBox;
        spin->setMinimum(0.0);
        spin->setMaximum(1.0);
        spin->setValue(1.0);
        ui->tableWidgetSegments->setCellWidget(i, 1, spin);

        // now color the third table cell in the row according
        // to the color produced by the segment indices in the render view
        //
        QTableWidgetItem* color = new QTableWidgetItem;
        color->setBackground(QBrush(QColor::fromRgbF(segment_rgbs[i][0], segment_rgbs[i][1], segment_rgbs[i][2])));
        color->setFlags(Qt::NoItemFlags|Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        ui->tableWidgetSegments->setItem(i, 2, color);
    }
}

void RenderEditor::reset()
{
    background_color[0] = 0.328125;
    background_color[1] = 0.347656;
    background_color[2] = 0.425781;

    solid_color[0] = 0.0;
    solid_color[1] = 0.0;
    solid_color[2] = 1.0;

    edge_color[0] = 0.0;
    edge_color[1] = 0.0;
    edge_color[2] = 0.0;

    ui->lineEditTitle->setText(current_bar_title);

    ui->spinBoxLabels->setValue(5);
    ui->spinBoxTitleSize->setValue(10);
    ui->spinBoxLabelSize->setValue(10);

    ui->doubleSpinBoxAspectRatio->setValue(20.0);

    title_color[0] = 1.0;
    title_color[1] = 1.0;
    title_color[2] = 1.0;

    labels_color[0] = 1.0;
    labels_color[1] = 1.0;
    labels_color[2] = 1.0;

    axes_color[0] = 1.0;
    axes_color[1] = 1.0;
    axes_color[2] = 1.0;

    colorDialogBackground->setCurrentColor(QColor::fromRgbF(background_color[0], background_color[1], background_color[2]));
    colorDialogSolid->setCurrentColor     (QColor::fromRgbF(solid_color[0], solid_color[1], solid_color[2]));
    colorDialogEdges->setCurrentColor     (QColor::fromRgbF(edge_color[0], edge_color[1], edge_color[2]));
    colorDialogBarTitle->setCurrentColor  (QColor::fromRgbF(title_color[0], title_color[1], title_color[2]));
    colorDialogBarLabels->setCurrentColor (QColor::fromRgbF(labels_color[0], labels_color[1], labels_color[2]));
    colorDialogAxes->setCurrentColor      (QColor::fromRgbF(axes_color[0], axes_color[1], axes_color[2]));

    ui->comboBoxTitleFonts->setCurrentIndex(0); // make 'arial' default ..
    ui->comboBoxLabelsFonts->setCurrentIndex(0); // make 'arial' default ..

    ui->checkBoxTitleShadow->setChecked(false);
    ui->checkBoxLabelsShadow->setChecked(false);
    ui->checkBoxUseLog->setChecked(current_log_state);

    ui->checkBoxShowXAxis->setChecked(true);
    ui->checkBoxShowYAxis->setChecked(true);
    ui->checkBoxShowZAxis->setChecked(true);
    ui->checkBoxAxesLines->setChecked(false);
    ui->checkBoxAxesInnerLines->setChecked(false);
    ui->checkBoxAxesLabels->setChecked(true);
    ui->checkBoxAxesTicks->setChecked(true);
    ui->checkBoxAxesMinorTicks->setChecked(false);
    ui->lineEditLabelFormat->setText("%6.1f");
    ui->doubleSpinBoxCornerOffset->setValue(0.0);
    ui->lineEditUnit->setText("");
    ui->comboBoxFlyMode->setCurrentIndex(1);



}

void RenderEditor::apply()
{
    // process background color selection
    QColor bgcolor = colorDialogBackground->currentColor();
    if(bgcolor.isValid())
        bgcolor.getRgbF(&background_color[0],&background_color[1],&background_color[2]);
    else {
        bgcolor = colorDialogBackground->selectedColor();
        bgcolor.getRgbF(&background_color[0],&background_color[1],&background_color[2]);
    }
    render_view->set_background_color(background_color);

    // process solid color selection
    QColor solidcolor = colorDialogSolid->currentColor();
    if(solidcolor.isValid())
        solidcolor.getRgbF(&solid_color[0],&solid_color[1],&solid_color[2]);
    else {
        solidcolor = colorDialogSolid->selectedColor();
        solidcolor.getRgbF(&solid_color[0],&solid_color[1],&solid_color[2]);
    }
    render_view->set_solid_color(solid_color);

    // process edge color selection
    QColor edgecolor = colorDialogEdges->currentColor();
    if(edgecolor.isValid())
        edgecolor.getRgbF(&edge_color[0],&edge_color[1],&edge_color[2]);
    else {
        edgecolor = colorDialogEdges->selectedColor();
        edgecolor.getRgbF(&edge_color[0],&edge_color[1],&edge_color[2]);
    }
    render_view->set_edge_color(edge_color);

    // process log state checkbox
    if(ui->checkBoxUseLog->checkState() == Qt::Unchecked)
        render_view->set_log(false);
    else if(ui->checkBoxUseLog->checkState() == Qt::Checked)
        render_view->set_log(true);
    else render_view->set_log(false);

    // set the general parameters of the color bar
    render_view->get_colorbar()->SetAspectRatio( ui->doubleSpinBoxAspectRatio->value() );
    render_view->get_colorbar()->SetNumberOfLabels( ui->spinBoxLabels->value() );

    // now set the color bar title related stuff
    render_view->get_colorbar()->SetTitle( ui->lineEditTitle->text().toStdString().c_str() );
    render_view->get_colorbar()->GetTitleTextProperty()->SetFontFamily(font_map.at(ui->comboBoxTitleFonts->currentText().toStdString()));
    render_view->get_colorbar()->GetTitleTextProperty()->SetFontSize( ui->spinBoxTitleSize->value() );

    QColor titlecolor = colorDialogBarTitle->currentColor();
    if(titlecolor.isValid())
        titlecolor.getRgbF(&title_color[0],&title_color[1],&title_color[2]);
    else {
        titlecolor = colorDialogBarTitle->selectedColor();
        titlecolor.getRgbF(&title_color[0],&title_color[1],&title_color[2]);
    }
    render_view->get_colorbar()->GetTitleTextProperty()->SetColor(title_color[0], title_color[1], title_color[2]);

    if(ui->checkBoxTitleShadow->checkState() == Qt::Unchecked)
        render_view->get_colorbar()->GetTitleTextProperty()->ShadowOff();
    else if(ui->checkBoxTitleShadow->checkState() == Qt::Checked)
        render_view->get_colorbar()->GetTitleTextProperty()->ShadowOn();
    else render_view->get_colorbar()->GetTitleTextProperty()->ShadowOff();

    // now set the color bar labels related stuff
    render_view->get_colorbar()->GetLabelTextProperty()->SetFontFamily(font_map.at(ui->comboBoxLabelsFonts->currentText().toStdString()));
    render_view->get_colorbar()->GetLabelTextProperty()->SetFontSize( ui->spinBoxLabelSize->value() );

    QColor labelscolor = colorDialogBarLabels->currentColor();
    if(labelscolor.isValid())
        labelscolor.getRgbF(&labels_color[0],&labels_color[1],&labels_color[2]);
    else {
        labelscolor = colorDialogBarLabels->selectedColor();
        labelscolor.getRgbF(&labels_color[0],&labels_color[1],&labels_color[2]);
    }
    render_view->get_colorbar()->GetLabelTextProperty()->SetColor(labels_color[0], labels_color[1], labels_color[2]);

    if(ui->checkBoxLabelsShadow->checkState() == Qt::Unchecked)
        render_view->get_colorbar()->GetLabelTextProperty()->ShadowOff();
    else if(ui->checkBoxLabelsShadow->checkState() == Qt::Checked)
        render_view->get_colorbar()->GetLabelTextProperty()->ShadowOn();
    else render_view->get_colorbar()->GetLabelTextProperty()->ShadowOff();

    // processing the axes parameters
    //
    bool show_axis_x = false;
    if(ui->checkBoxShowXAxis->checkState() == Qt::Unchecked)
    {
        show_axis_x = false;
        render_view->get_cubeAxes()->XAxisVisibilityOff();
    }
    if(ui->checkBoxShowXAxis->checkState() == Qt::Checked)
    {
        show_axis_x = true;
        render_view->get_cubeAxes()->XAxisVisibilityOn();
    }

    bool show_axis_y = false;
    if(ui->checkBoxShowYAxis->checkState() == Qt::Unchecked)
    {
        show_axis_y = false;
        render_view->get_cubeAxes()->YAxisVisibilityOff();
    }
    if(ui->checkBoxShowYAxis->checkState() == Qt::Checked)
    {
        show_axis_y = true;
        render_view->get_cubeAxes()->YAxisVisibilityOn();
    }

    bool show_axis_z = false;
    if(ui->checkBoxShowZAxis->checkState() == Qt::Unchecked)
    {
        show_axis_z = false;
        render_view->get_cubeAxes()->ZAxisVisibilityOff();
    }
    if(ui->checkBoxShowZAxis->checkState() == Qt::Checked)
    {
        show_axis_z = true;
        render_view->get_cubeAxes()->ZAxisVisibilityOn();
    }

    if(ui->checkBoxAxesLines->checkState() == Qt::Unchecked)
    {
        if(show_axis_x) render_view->get_cubeAxes()->DrawXGridlinesOff();
        if(show_axis_y) render_view->get_cubeAxes()->DrawYGridlinesOff();
        if(show_axis_z) render_view->get_cubeAxes()->DrawZGridlinesOff();
    }
    else if(ui->checkBoxAxesLines->checkState() == Qt::Checked)
    {
        if(show_axis_x) render_view->get_cubeAxes()->DrawXGridlinesOn();
        if(show_axis_y) render_view->get_cubeAxes()->DrawYGridlinesOn();
        if(show_axis_z) render_view->get_cubeAxes()->DrawZGridlinesOn();
    }

    if(ui->checkBoxAxesInnerLines->checkState() == Qt::Unchecked)
    {
#if VTK_MAJOR_VERSION > 5 || (VTK_MAJOR_VERSION == 5 && VTK_MINOR_VERSION > 8)
        if(show_axis_x) render_view->get_cubeAxes()->DrawXInnerGridlinesOff();
        if(show_axis_y) render_view->get_cubeAxes()->DrawYInnerGridlinesOff();
        if(show_axis_z) render_view->get_cubeAxes()->DrawZInnerGridlinesOff();
#endif
    }
    else if(ui->checkBoxAxesInnerLines->checkState() == Qt::Checked)
    {
#if VTK_MAJOR_VERSION > 5 || (VTK_MAJOR_VERSION == 5 && VTK_MINOR_VERSION > 8)
        if(show_axis_x) render_view->get_cubeAxes()->DrawXInnerGridlinesOn();
        if(show_axis_y) render_view->get_cubeAxes()->DrawYInnerGridlinesOn();
        if(show_axis_z) render_view->get_cubeAxes()->DrawZInnerGridlinesOn();
#endif
    }

    if(ui->checkBoxAxesLabels->checkState() == Qt::Unchecked)
    {
        if(show_axis_x) render_view->get_cubeAxes()->XAxisLabelVisibilityOff();
        if(show_axis_y) render_view->get_cubeAxes()->YAxisLabelVisibilityOff();
        if(show_axis_z) render_view->get_cubeAxes()->ZAxisLabelVisibilityOff();
    }
    else if(ui->checkBoxAxesLabels->checkState() == Qt::Checked)
    {
        if(show_axis_x) render_view->get_cubeAxes()->XAxisLabelVisibilityOn();
        if(show_axis_y) render_view->get_cubeAxes()->YAxisLabelVisibilityOn();
        if(show_axis_z) render_view->get_cubeAxes()->ZAxisLabelVisibilityOn();
    }

    if(ui->checkBoxAxesTicks->checkState() == Qt::Unchecked)
    {
        if(show_axis_x) render_view->get_cubeAxes()->XAxisTickVisibilityOff();
        if(show_axis_y) render_view->get_cubeAxes()->YAxisTickVisibilityOff();
        if(show_axis_z) render_view->get_cubeAxes()->ZAxisTickVisibilityOff();
    }
    else if(ui->checkBoxAxesTicks->checkState() == Qt::Checked)
    {
        if(show_axis_x) render_view->get_cubeAxes()->XAxisTickVisibilityOn();
        if(show_axis_y) render_view->get_cubeAxes()->YAxisTickVisibilityOn();
        if(show_axis_z) render_view->get_cubeAxes()->ZAxisTickVisibilityOn();
    }

    if(ui->checkBoxAxesMinorTicks->checkState() == Qt::Unchecked)
    {
        if(show_axis_x) render_view->get_cubeAxes()->XAxisMinorTickVisibilityOff();
        if(show_axis_y) render_view->get_cubeAxes()->YAxisMinorTickVisibilityOff();
        if(show_axis_z) render_view->get_cubeAxes()->ZAxisMinorTickVisibilityOff();
    }
    else if(ui->checkBoxAxesMinorTicks->checkState() == Qt::Checked)
    {
        if(show_axis_x) render_view->get_cubeAxes()->XAxisMinorTickVisibilityOn();
        if(show_axis_y) render_view->get_cubeAxes()->YAxisMinorTickVisibilityOn();
        if(show_axis_z) render_view->get_cubeAxes()->ZAxisMinorTickVisibilityOn();
    }

    render_view->get_cubeAxes()->SetXLabelFormat( ui->lineEditLabelFormat->text().toStdString().c_str() );
    render_view->get_cubeAxes()->SetYLabelFormat( ui->lineEditLabelFormat->text().toStdString().c_str() );
    render_view->get_cubeAxes()->SetZLabelFormat( ui->lineEditLabelFormat->text().toStdString().c_str() );

    render_view->get_cubeAxes()->SetCornerOffset(ui->doubleSpinBoxCornerOffset->value());
    render_view->get_cubeAxes()->SetXUnits(ui->lineEditUnit->text().toStdString().c_str());
    render_view->get_cubeAxes()->SetYUnits(ui->lineEditUnit->text().toStdString().c_str());
    render_view->get_cubeAxes()->SetZUnits(ui->lineEditUnit->text().toStdString().c_str());


    QString flymode = ui->comboBoxFlyMode->currentText();
    if(flymode == "StaticEdges")   render_view->get_cubeAxes()->SetFlyModeToStaticEdges();
    if(flymode == "StaticTriad")   render_view->get_cubeAxes()->SetFlyModeToStaticTriad();
    if(flymode == "OuterEdges")    render_view->get_cubeAxes()->SetFlyModeToOuterEdges();
    if(flymode == "ClosestTriad")  render_view->get_cubeAxes()->SetFlyModeToClosestTriad();
    if(flymode == "FurthestTriad") render_view->get_cubeAxes()->SetFlyModeToFurthestTriad();

    // process edge color selection
    QColor axescolor = colorDialogAxes->currentColor();
    if(axescolor.isValid())
        axescolor.getRgbF(&axes_color[0],&axes_color[1],&axes_color[2]);
    else {
        axescolor = colorDialogAxes->selectedColor();
        axescolor.getRgbF(&axes_color[0],&axes_color[1],&axes_color[2]);
    }
    // color the axes
#if VTK_MAJOR_VERSION > 5 || (VTK_MAJOR_VERSION == 5 && VTK_MINOR_VERSION > 8)
    render_view->get_cubeAxes()->GetTitleTextProperty(0)->SetColor(axes_color);
    render_view->get_cubeAxes()->GetTitleTextProperty(1)->SetColor(axes_color);
    render_view->get_cubeAxes()->GetTitleTextProperty(2)->SetColor(axes_color);

    render_view->get_cubeAxes()->GetLabelTextProperty(0)->SetColor(axes_color);
    render_view->get_cubeAxes()->GetLabelTextProperty(1)->SetColor(axes_color);
    render_view->get_cubeAxes()->GetLabelTextProperty(2)->SetColor(axes_color);

    render_view->get_cubeAxes()->GetXAxesLinesProperty()->SetColor(axes_color);
    render_view->get_cubeAxes()->GetYAxesLinesProperty()->SetColor(axes_color);
    render_view->get_cubeAxes()->GetZAxesLinesProperty()->SetColor(axes_color);

    render_view->get_cubeAxes()->GetXAxesGridlinesProperty()->SetColor(axes_color);
    render_view->get_cubeAxes()->GetYAxesGridlinesProperty()->SetColor(axes_color);
    render_view->get_cubeAxes()->GetZAxesGridlinesProperty()->SetColor(axes_color);

    render_view->get_cubeAxes()->GetXAxesInnerGridlinesProperty()->SetColor(axes_color);
    render_view->get_cubeAxes()->GetYAxesInnerGridlinesProperty()->SetColor(axes_color);
    render_view->get_cubeAxes()->GetZAxesInnerGridlinesProperty()->SetColor(axes_color);

    render_view->get_cubeAxes()->GetXAxesGridpolysProperty()->SetColor(axes_color);
    render_view->get_cubeAxes()->GetYAxesGridpolysProperty()->SetColor(axes_color);
    render_view->get_cubeAxes()->GetZAxesGridpolysProperty()->SetColor(axes_color);
#endif

    // perform the segment-specific visualization
    //
    for(int i = 0; i < ui->tableWidgetSegments->rowCount(); i++)
    {
        QTableWidgetItem *id = ui->tableWidgetSegments->item(i, 0);
        if(id->checkState() == Qt::Checked)
        {
            render_view->set_segment_opacity(i,
                qobject_cast<QDoubleSpinBox*>(ui->tableWidgetSegments->cellWidget(i, 1))->value()
            );
        }
        // deactivate this particular segment
        else
        {
            // .. by setting the opacity to 0
            render_view->set_segment_opacity(i, 0.0);
        }
    }


    // finally, update the visualizer with the new parameters
    render_view->update_render();
}

void RenderEditor::setLogState(bool state)
{
    ui->checkBoxUseLog->setChecked(state);
    render_view->set_log(state);
    current_log_state = state;
}
