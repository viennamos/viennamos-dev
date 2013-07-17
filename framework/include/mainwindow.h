#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

#include <QMainWindow>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QSplitter>
#include <QDockWidget>
#include <QVTKWidget.h>
#include <QGridLayout>
#include <QListWidget>
#include <QWidget>
#include <QComboBox>
#include <QTextEdit>
#include <QSplitter>
#include <QElapsedTimer>
#include <QSpinBox>

#include "messenger.h"
#include "common.hpp"
#include "device.hpp"
#include "database.hpp"
#include "module_manager.h"
#include "materialmanager.h"
//#include "quantity_set.hpp"
#include "multiview.h"
#include "screenshot.h"
#include "license.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    typedef std::map< QString, std::map< QString, std::size_t > >   ModuleQuanIndex;
    typedef std::map< int, int >                                    IndexMap;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionModule_Manager_triggered();
    void loadActiveModules(QList<QString>& new_modules);
    void unloadActiveModules(QList<QString>& new_modules);
    void show_module_widget(QListWidgetItem * item);
    void on_actionExit_triggered();
    void apply_module_end(QString const& module);
    void apply_module_end_error(QString const& module);
    void apply_module_begin(QString const& module);
    void change_mesh_visualization(QString id);
    void on_actionActive_Modules_triggered();
    void on_actionModule_Configuration_triggered();
    void update_dockModules_view_action();
    void update_dockModuleConfig_view_action();
    void change_quantity_visualization(int idx);
    void toggle_color_legend();
    void toggle_axes();
    void toggle_coordinates();
    void toggle_2d3d();
    void showRenderEditor();
    void showChartEditor();
    void show_first();
    void show_back();
    void show_play();
    void show_forward();
    void show_last();
    void on_actionRemove_Current_View_triggered();
    void current_view_changed();
    void on_actionSplit_Current_View_Horizontal_triggered();
    void on_actionSplit_Current_View_Vertical_triggered();
    void on_actionLicense_triggered();
    void on_actionMaterial_Manager_triggered();
    void initializeCurrent3DRenderView();
    void initializeCurrent2DChartView();


    void on_actionOutput_triggered();

    void on_actionTake_Screenshot_triggered();

private:
    void setup_mainwindow();
    void create_widgets();
    void setup_vcr();
    void setup_signals();
    void evaluate_module_states();
    void reset_comboBoxFieldViz();
    void repopulate_quantity_selection(QuantitySet const& quantityset);
    double get_current_module_timer_seconds();
    void delay(int mseconds);
    void addNewView();
    QDockWidget* createNewViewDock();

    void enableChartControls(bool state);
    void enableRenderControls(bool state);
    void enableViewControls(bool state);


    Ui::MainWindow*                 ui;
    ModuleManager*                  module_manager;
    MaterialManager*                material_manager;
    ModulesMap                      avail_modules;
    DataBase*                       database;
    QVTKWidget*                     qvtkwidget;
    QDockWidget*                    dockModules;
    QDockWidget*                    dockModuleConfig;
    QDockWidget*                    dockOutput;
    QListWidget*                    active_modules;
    QComboBox*                      comboBoxMeshRepresentations;
    QComboBox*                      comboBoxFieldViz;
    QSplitter*                      splitter;
    QSpinBox*                       spinBoxVCRDelay;
    MultiView*                      multi_view;
    QElapsedTimer                   module_timer;
    ModuleQuanIndex                 module_quan_index;
    IndexMap                        meshrep_views;
    IndexMap                        quan_views;
    QStringList                     meshreps;
    Messenger*                      output;
    Screenshot*                     screenshot;
    License*                        license;


    QPushButton* pushButtonResetView;
    QPushButton* pushButtonToggleColorBar;
    QPushButton* pushButtonToggleAxes;
    QPushButton* pushButtonToggleCoordinates;
    QPushButton* pushButtonToggle2D3D;
    QPushButton* pushButtonRenderEditor;
    QPushButton* pushButtonChartEditor;
};


#endif // MAINWINDOW_H
