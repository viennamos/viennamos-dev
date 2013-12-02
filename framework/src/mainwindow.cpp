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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDockWidget>
#include <QDebug>
#include <QPushButton>
#include <QLabel>
#include <QTime>
#include <QMainWindow>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setup_mainwindow();
    create_widgets();
    setup_signals();
}

MainWindow::~MainWindow()
{
    delete multi_view;
    delete database;
    delete module_manager;
    delete ui;
}

void MainWindow::setup_mainwindow()
{
    setWindowTitle("ViennaMOS");
    resize(1400, 1024);
    setWindowIcon(QIcon("://resources/icons/viennamos.png"));

    // we have to explicitly state that there is no central widget,
    // as Qt expects one ..
    // as we are only working with dockwidgets, and don't
    // have a 'static central' central widget which is always there ..
    this->setCentralWidget(NULL);
}

void MainWindow::create_widgets()
{
    database = new DataBase;
    module_manager = new ModuleManager(avail_modules, this);
    material_manager = new MaterialManager(this);
    multi_view = new MultiView(this);
    screenshot = new Screenshot(multi_view, this);

    active_modules = new QListWidget(this);
    active_modules->setSortingEnabled(false);
//    active_modules->setMaximumWidth(this->width()/4);
//    active_modules->setMaximumHeight(this->height()/4);

    dockModules = new QDockWidget(tr("Active Modules"), this);
    dockModules->setAllowedAreas(Qt::LeftDockWidgetArea);
    dockModules->setWidget(active_modules);
    addDockWidget(Qt::LeftDockWidgetArea, dockModules);

    dockModuleConfig = new QDockWidget(tr("Module Configuration"), this);
    dockModuleConfig->setAllowedAreas(Qt::LeftDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, dockModuleConfig);

    dockOutput = new QDockWidget(tr("Output"), this);
    dockOutput->setAllowedAreas(Qt::LeftDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, dockOutput);

    output = new Messenger(this);
    dockOutput->setWidget(output);

    meshreps << viennamos::key::surface << viennamos::key::surfaceEdges << viennamos::key::points << viennamos::key::wireframe;

    comboBoxMeshRepresentations = new QComboBox(this);
    comboBoxMeshRepresentations->setToolTip("Switch mesh representations");
    comboBoxMeshRepresentations->addItems(meshreps);
    comboBoxMeshRepresentations->setCurrentIndex(0);
    ui->toolBarView->addWidget(comboBoxMeshRepresentations);

    comboBoxFieldViz = new QComboBox(this);
    comboBoxFieldViz->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    comboBoxFieldViz->setToolTip("Visualize quantity fields");
    ui->toolBarView->addWidget(comboBoxFieldViz);
    reset_comboBoxFieldViz();

    pushButtonResetView = new QPushButton(QIcon("://resources/icons/pqResetCamera32.png"), "", this);
    pushButtonResetView->setToolTip("Reset View");
    ui->toolBarView->addWidget(pushButtonResetView);
    QObject::connect(pushButtonResetView, SIGNAL(clicked(bool)), multi_view, SLOT(resetCurrentView()));

    pushButtonToggleColorBar = new QPushButton(QIcon("://resources/icons/pqScalarBar32.png"), "", this);
    pushButtonToggleColorBar->setToolTip("Toggle Color Bar");
    ui->toolBarView->addWidget(pushButtonToggleColorBar);
    QObject::connect(pushButtonToggleColorBar, SIGNAL(clicked(bool)), this, SLOT(toggle_color_legend()));

    pushButtonRenderEditor = new QPushButton(QIcon("://resources/icons/pqEditColor24.png"), "", this);
    pushButtonRenderEditor->setToolTip("Show Render Editor");
    ui->toolBarView->addWidget(pushButtonRenderEditor);
    QObject::connect(pushButtonRenderEditor, SIGNAL(clicked(bool)), this, SLOT(showRenderEditor()));

    pushButtonToggleAxes = new QPushButton(QIcon("://resources/icons/orientation_axes.png"), "", this);
    pushButtonToggleAxes->setToolTip("Toggle Orientation Axes");
    ui->toolBarView->addWidget(pushButtonToggleAxes);
    QObject::connect(pushButtonToggleAxes, SIGNAL(clicked(bool)), this, SLOT(toggle_axes()));

    pushButtonToggleCoordinates = new QPushButton(QIcon("://resources/icons/coordinates.png"), "", this);
    pushButtonToggleCoordinates->setToolTip("Toggle Coordinates");
    ui->toolBarView->addWidget(pushButtonToggleCoordinates);
    QObject::connect(pushButtonToggleCoordinates, SIGNAL(clicked(bool)), this, SLOT(toggle_coordinates()));

    pushButtonToggle2D3D = new QPushButton(QIcon("://resources/icons/2d3d.png"), "", this);
    pushButtonToggle2D3D->setToolTip("Toggle Coordinates");
    ui->toolBarView->addWidget(pushButtonToggle2D3D);
    QObject::connect(pushButtonToggle2D3D, SIGNAL(clicked(bool)), this, SLOT(toggle_2d3d()));

    pushButtonChartEditor = new QPushButton(QIcon("://resources/icons/chart_editor.png"), "", this);
    pushButtonChartEditor->setToolTip("Show Chart Editor");
    ui->toolBarView->addWidget(pushButtonChartEditor);
    QObject::connect(pushButtonChartEditor, SIGNAL(clicked(bool)), this, SLOT(showChartEditor()));

    // ----------
    // [JW] for now, we deactivate the VCR option as we first need an approach
    // for storing quantity sequences via the ViennaData-based device storage
    //
    //setup_vcr();


    ui->actionActive_Modules->setChecked(true);
    ui->actionModule_Configuration->setChecked(true);
    ui->actionOutput->setChecked(true);

    this->addNewView(); // create an empty view for the user to work with right away

    license = new License(this);

    ui->statusBar->showMessage("Ready");
}

void MainWindow::setup_vcr()
{
  QPushButton* pushButtonFirst = new QPushButton(QIcon("://resources/icons/pqVcrFirst32.png"), "", this);
  pushButtonFirst->setToolTip("Show first frame");
  ui->toolBarVCR->addWidget(pushButtonFirst);
  QObject::connect(pushButtonFirst, SIGNAL(clicked(bool)), this, SLOT(show_first()));

  QPushButton* pushButtonBack = new QPushButton(QIcon("://resources/icons/pqVcrBack32.png"), "", this);
  pushButtonBack->setToolTip("Show previous frame");
  ui->toolBarVCR->addWidget(pushButtonBack);
  QObject::connect(pushButtonBack, SIGNAL(clicked(bool)), this, SLOT(show_back()));

  QPushButton* pushButtonPlay = new QPushButton(QIcon("://resources/icons/pqVcrPlay32.png"), "", this);
  pushButtonPlay->setToolTip("Play all frames");
  ui->toolBarVCR->addWidget(pushButtonPlay);
  QObject::connect(pushButtonPlay, SIGNAL(clicked(bool)), this, SLOT(show_play()));

  QPushButton* pushButtonForward = new QPushButton(QIcon("://resources/icons/pqVcrForward32.png"), "", this);
  pushButtonForward->setToolTip("Show next frame");
  ui->toolBarVCR->addWidget(pushButtonForward);
  QObject::connect(pushButtonForward, SIGNAL(clicked(bool)), this, SLOT(show_forward()));

  QPushButton* pushButtonLast = new QPushButton(QIcon("://resources/icons/pqVcrLast32.png"), "", this);
  pushButtonLast->setToolTip("Show last frame");
  ui->toolBarVCR->addWidget(pushButtonLast);
  QObject::connect(pushButtonLast, SIGNAL(clicked(bool)), this, SLOT(show_last()));


  spinBoxVCRDelay = new QSpinBox(this);
  spinBoxVCRDelay->setRange(0, std::numeric_limits<int>::max());
  spinBoxVCRDelay->setValue(500);
  spinBoxVCRDelay->setSuffix("ms");
  spinBoxVCRDelay->setToolTip("Set the delay in milliseconds between frames");
  ui->toolBarVCR->addWidget(spinBoxVCRDelay);
}

void MainWindow::setup_signals()
{
    QObject::connect(module_manager,              SIGNAL(loadModules(QList<QString>&)),        this, SLOT(loadActiveModules(QList<QString>&)));
    QObject::connect(module_manager,              SIGNAL(unloadModules(QList<QString>&)),      this, SLOT(unloadActiveModules(QList<QString>&)));
    QObject::connect(active_modules,              SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(show_module_widget(QListWidgetItem*)));
    QObject::connect(comboBoxMeshRepresentations, SIGNAL(activated(QString)),                  this, SLOT(change_mesh_visualization(QString)));
    QObject::connect(dockModules,                 SIGNAL(visibilityChanged(bool)),             this, SLOT(update_dockModules_view_action()));
    QObject::connect(dockModuleConfig,            SIGNAL(visibilityChanged(bool)),             this, SLOT(update_dockModuleConfig_view_action()));
    QObject::connect(multi_view,                  SIGNAL(currentViewChanged()),                this, SLOT(current_view_changed()));
    QObject::connect(multi_view,                  SIGNAL(added3DRenderView()),                 this, SLOT(initializeCurrent3DRenderView()));
    QObject::connect(multi_view,                  SIGNAL(added2DChartView()),                  this, SLOT(initializeCurrent2DChartView()));
    QObject::connect(comboBoxFieldViz,            SIGNAL(currentIndexChanged(int)),            this, SLOT(change_quantity_visualization(int)));
}

void MainWindow::on_actionModule_Manager_triggered()
{
    module_manager->show();
}

void MainWindow::loadActiveModules(QList<QString>& modules)
{
    QStringList ready_modules;
    QStringList unready_modules;
    foreach(QString module, modules) {

        if(!avail_modules.value(module)) module_manager->recreate_module(module);
        avail_modules[module]->setup(database, multi_view, output, material_manager);

        if(avail_modules[module]->is_ready()) ready_modules.append(module);
        else                                  unready_modules.append(module);

        QObject::connect(avail_modules[module], SIGNAL(module_end(QString)),
                         this, SLOT(apply_module_end(QString)));
        QObject::connect(avail_modules[module], SIGNAL(module_begin(QString)),
                         this, SLOT(apply_module_begin(QString)));
        QObject::connect(avail_modules[module], SIGNAL(module_end_error(QString)),
                         this, SLOT(apply_module_end_error(QString)));
    }

    foreach(QString module, ready_modules) {
        QListWidgetItem* item = new QListWidgetItem(active_modules);
        item->setText(module);
        item->setIcon(QIcon("://resources/icons/module.png"));
        item->setFont(QFont("Arial", 12));
        active_modules->addItem(item);
    }
    foreach(QString module, unready_modules) {
        QListWidgetItem* item = new QListWidgetItem(active_modules);
        item->setText(module);
        item->setIcon(QIcon("://resources/icons/module.png"));
        item->setFont(QFont("Arial", 12));
        item->setFlags(Qt::NoItemFlags);  // deactivate this one
        active_modules->addItem(item);
    }

    // check if modules have unresolved dependencies at the moment - deactivate them in the active module list
    //
    evaluate_module_states();
}

void MainWindow::unloadActiveModules(QList<QString>& modules)
{
    foreach(QString module, modules) {

        // first, order the module to 'reset' itself -> release memory
        //
        avail_modules[module]->reset();

        // now remove it from the 'active modules' dock-listwidget
        //
        QList<QListWidgetItem *> result = active_modules->findItems(module, Qt::MatchExactly);
        int row = active_modules->row(result.first());
        QListWidgetItem * item = active_modules->takeItem(row);
        delete item;
    }
}

void MainWindow::show_module_widget(QListWidgetItem * item)
{
    // update the widget before retrieving the widget
//    avail_modules[item->text()]->update();
    dockModuleConfig->setWidget(avail_modules[item->text()]->get_widget());
}

void MainWindow::apply_module_begin(const QString &module)
{
    module_timer.start();
    ui->statusBar->showMessage("running .." );
    //this->setCursor(Qt::WaitCursor);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    output->append("");
    output->append("# ------------------------------------------------------------------------------------------");
    output->append("# [BEGIN] \""+module+"\"");
    output->append("# ------------------------------------------------------------------------------------------");
    output->append("");
    //output->claimDefaultStreams();
}

/**
 * @brief Slot: Is called when the module finished it's execution normally
 * @param module The name key string of the module
 */
void MainWindow::apply_module_end(QString const& module)
{
    // reactivate the mouse curser
    //this->setCursor(Qt::ArrowCursor);
    QApplication::restoreOverrideCursor();

    // update the 'mainwindow state' indicators
    ui->statusBar->showMessage("ready");
    //output->releaseStreams();
    output->append("");
    output->append("# ------------------------------------------------------------------------------------------");
    output->append("# [END] \""+module+"\" exec-time: "+QString::number(get_current_module_timer_seconds())+" s");
    output->append("# ------------------------------------------------------------------------------------------");


    // update the multiview: let the views know, that the data might have changed
    multi_view->update();


    // see if we can now switch a previously deactivated module on
    // (as the required data may now be available ..)
    evaluate_module_states();

    reset_comboBoxFieldViz();

    // if the module prepared some quantity field data, populate the combobox and
    // visualize the default result
    //
    QString current_module = active_modules->item(active_modules->currentRow())->text();
    QuantitySet& quantity_set = avail_modules.value(current_module)->get_quantity_set();

    if(quantity_set.size() > 0)
    {
      // store the number of default entries, so we have an index
      // pointing to the first module quantity at the subsequent step
      //
      int first_module_quantity_index = comboBoxFieldViz->count();

      // update the quantity selection combobox with this module's generated quantities
      repopulate_quantity_selection(quantity_set);

      // at this point,comboboxfieldviz cotains the default two entries
      // - being the 'solid' and 'segment index' key - plus at least one
      // quantity entry from the module.
      // Therefore, we use the element count of the comboBoxFieldViz object
      // before we populated it with the module quantities.
      // This way, we make sure that we visualize the first result quantity of the module
      //
      comboBoxFieldViz->setCurrentIndex(first_module_quantity_index); // emits 'currentIndexChanged'
    }
    else // there is no quantity, so do a plain 'solid' render
    {
      comboBoxFieldViz->setCurrentIndex(0); // emits 'currentIndexChanged'
    }

//    // record the current quantity for later restore of the quantity selection in the combobox due to view-switching
//    //
//    quan_views[ multi_view->getCurrentViewIndex() ] = comboBoxFieldViz->currentIndex();
}

/**
 * @brief Slot: Is called in case a module aborted prematurely
 * @param module The name key string of the module
 */
void MainWindow::apply_module_end_error(QString const& module)
{
    ui->statusBar->showMessage("ready" );
    //this->setCursor(Qt::ArrowCursor);
    QApplication::restoreOverrideCursor();

    output->releaseStreams();
    output->append("");
    output->append("# ------------------------------------------------------------------------------------------");
    output->append("# [ERROR] \""+module+"\" exec-time: "+QString::number(get_current_module_timer_seconds())+" s");
    output->append("# ------------------------------------------------------------------------------------------");
}


//void MainWindow::update_current_results()
//{

//}

/**
 * @brief Deletes the content of the quantity fields combobox and repopulate it with the default 'solid color' option
 */
void MainWindow::reset_comboBoxFieldViz()
{
  comboBoxFieldViz->blockSignals(true);
  comboBoxFieldViz->clear();
  comboBoxFieldViz->addItem(QIcon("://resources/icons/pqSolidColor16.png"), viennamos::key::solid_color);
  comboBoxFieldViz->addItem(QIcon("://resources/icons/cell_data.png"), viennamos::key::segment_index);
  comboBoxFieldViz->blockSignals(false);
}

void MainWindow::repopulate_quantity_selection(QuantitySet const& quantityset)
{
    //qDebug() << "repopulating the quantity selection combobox ..";

  foreach(Quantity quantity, quantityset) {

    // we store the actual ViennaMOS::Quantity within the QVariant value of the
    // combobox item. This way we have access to the quantity at a later point
    // when we retrieve an item again.
    //
    QVariant var;
    var.setValue(quantity);

    if(quantity.cell_level == viennamos::VERTEX)
    {
        comboBoxFieldViz->addItem(QIcon("://resources/icons/vertex_data.png"), QString::fromStdString(quantity.name), var);
    }
    else if(quantity.cell_level == viennamos::CELL)
    {
        comboBoxFieldViz->addItem(QIcon("://resources/icons/cell_data.png"), QString::fromStdString(quantity.name), var);
    }
    else
    {
        QMessageBox::warning(this, QString("Error"), QString("Only vertex and cell quantity fields are supported"));
    }
  }

  // make sure the combobox is large enough to accomodate the recently added quantity names and icons
  //
  comboBoxFieldViz->adjustSize();
}

/**
 * @brief Shut down the application
 */
void MainWindow::on_actionExit_triggered()
{
    qApp->exit();
}


void MainWindow::change_mesh_visualization(QString id)
{
    int current_view_index = multi_view->getCurrentViewIndex();
    Render3D* current_render = multi_view->getCurrentRender3D();
    if(current_render)
    {
        if(id == viennamos::key::surfaceEdges)
        {
            meshrep_views[current_view_index] = meshreps.indexOf(viennamos::key::surfaceEdges);
            current_render->switch_to_surface_with_edges_representation();
        }
        else
        if(id == viennamos::key::points)
        {
            meshrep_views[current_view_index] = meshreps.indexOf(viennamos::key::points);
            current_render->switch_to_point_representation();
        }
        else
        if(id == viennamos::key::surface)
        {
            meshrep_views[current_view_index] = meshreps.indexOf(viennamos::key::surface);
            current_render->switch_to_surface_representation();
        }
        else
        if(id == viennamos::key::wireframe)
        {
            meshrep_views[current_view_index] = meshreps.indexOf(viennamos::key::wireframe);
            current_render->switch_to_wireframe_representation();
        }
        else return;
    }
}

void MainWindow::evaluate_module_states()
{
    for(int row = 0; row < active_modules->count(); row++)
    {
        // proceed if the item is already active ..
        // if(active_modules->item(row)->flags() == (Qt::ItemIsEnabled | Qt::ItemIsSelectable) ) continue;

        QString current_module = active_modules->item(row)->text();

        if(avail_modules.value(current_module)->is_ready())
        {
            //qDebug() << "Module " << current_module << " is ready .. ";

            // activate the module in the 'active modules' list
            active_modules->item(row)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

            // trigger a module update: as the required input dependencies are resolved,
            // the module might want to analyse it prior to presenting the widget to the framework
            // the next time it is configured
            //
            avail_modules.value(current_module)->update();
        }
        else
        {
            //qDebug() << "Module " << current_module << " is not ready .. ";

            // deactivate the module in the 'active modules' list
            active_modules->item(row)->setFlags(Qt::NoItemFlags);
        }

    }
}

void MainWindow::on_actionActive_Modules_triggered()
{
    if(ui->actionActive_Modules->isChecked()) dockModules->show();
    else dockModules->hide();
}

void MainWindow::on_actionModule_Configuration_triggered()
{
    if(ui->actionModule_Configuration->isChecked()) dockModuleConfig->show();
    else dockModuleConfig->hide();
}

void MainWindow::update_dockModules_view_action()
{
    if(dockModules->isVisible()) ui->actionActive_Modules->setChecked(true);
    else ui->actionActive_Modules->setChecked(false);
}

void MainWindow::update_dockModuleConfig_view_action()
{
    if(dockModuleConfig->isVisible()) ui->actionModule_Configuration->setChecked(true);
    else ui->actionModule_Configuration->setChecked(false);
}

void MainWindow::change_quantity_visualization(int idx)
{
//  qDebug() << "change_quantity_visualization: " << idx;
  if(idx < 0) return;

  if(comboBoxFieldViz->count() < 1) return;

  // store the current selection in the quantity field combobox
  // for this particular render view, so if the user
  // at a later point switches between views, the last
  // quantity is displayed ..
  //
  quan_views[ multi_view->getCurrentViewIndex() ] = idx;
  //qDebug() << "view " << multi_view->getCurrentViewIndex() << " idx " << idx;

  if(idx == 0)
  {
//      qDebug() << "  showing current grid" ;
      multi_view->show_current_grid();
  }
  else
  if(idx == 1)
  {
//      qDebug() << "  showing current grid segment ids" ;
      multi_view->show_current_grid_segments();
  }
  else
  {
//      qDebug() << "  showing quantity" ;
    Quantity quantity = comboBoxFieldViz->itemData(idx).value<Quantity>();

    // get the currently selected active module ..
    QListWidgetItem* item = active_modules->currentItem();

    // make sure that there is actually one .. otherwise there is a segfault
    if(!item) return;

    QString module = item->text();

  //  qDebug() << "quantity viz: quantity" ;

    // forward the quantity id to the module's renderer
    // the module will render the quantity accordingly
    // by default, we show the first result of a sequence
    // that's why we pass a '0' here to the plugin's render method
    avail_modules[module]->render(quantity, 0);

    // set the current index of the current module's quantity to 0
    //module_quan_index[module][id] = 0;

  }
}

void MainWindow::toggle_color_legend()
{
    Render3D* current_render = multi_view->getCurrentRender3D();
    if(current_render)
    {
        multi_view->getCurrentRender3D()->toggle_color_legend();
    }
}

double MainWindow::get_current_module_timer_seconds()
{
    return double(module_timer.elapsed()) / 1000.0; // get seconds
}

void MainWindow::toggle_axes()
{
    Render3D* current_render = multi_view->getCurrentRender3D();
    if(current_render)
    {
        multi_view->getCurrentRender3D()->toggle_axes();
    }
}

void MainWindow::toggle_coordinates()
{
    Render3D* current_render = multi_view->getCurrentRender3D();
    if(current_render)
    {
        multi_view->getCurrentRender3D()->toggle_coordinates();
    }
}

void MainWindow::toggle_2d3d()
{
    Render3D* current_render = multi_view->getCurrentRender3D();
    if(current_render)
    {
        multi_view->getCurrentRender3D()->toggle_2d3d();
    }
}

void MainWindow::showRenderEditor()
{
    if(multi_view->currentIsRender3D())
        multi_view->getCurrentRenderEditor()->show();
}

void MainWindow::showChartEditor()
{
    if(multi_view->currentIsChart2D())
        multi_view->getCurrentChartEditor()->show();
}

void MainWindow::show_first()
{
//    if(active_modules->count() == 0)   return;

//    // get the currently selected active module ..
//    //
//    QListWidgetItem* item = active_modules->currentItem();
//    QString module = item->text();

//    // get the currently selected quantity from the combobox ..
//    //
//    QString quantity = comboBoxFieldViz->currentText();

//    if(quantity == key::solid_color)        return;
//    else
//    {
//        // reset the module's quantity index
//        //
//        module_quan_index[module][quantity] = 0;

//        // forward the quantity id to the module's renderer
//        // the module will render the quantity accordingly
//        avail_modules[module]->render(quantity.toStdString(), module_quan_index[module][quantity]);
//    }
}

void MainWindow::show_back()
{
//    if(active_modules->count() == 0)   return;

//    // get the currently selected active module ..
//    //
//    QListWidgetItem* item = active_modules->currentItem();
//    QString module = item->text();

//    // get the currently selected quantity from the combobox ..
//    //
//    QString quantity = comboBoxFieldViz->currentText();

//    if(quantity == key::solid_color)        return;
//    else
//    {
//        // decrement the current index - set 0
//        //
//        if(module_quan_index[module][quantity] != 0)
//            module_quan_index[module][quantity]--;

//        // forward the quantity id to the module's renderer
//        // the module will render the quantity accordingly
//        avail_modules[module]->render(quantity.toStdString(), module_quan_index[module][quantity]);
//    }
}

void MainWindow::show_play()
{
//    if(active_modules->count() == 0)   return;

//    // get the currently selected active module ..
//    //
//    QListWidgetItem* item = active_modules->currentItem();
//    QString module = item->text();

//    // get the currently selected quantity from the combobox ..
//    //
//    QString quantity = comboBoxFieldViz->currentText();

//    if(quantity == key::solid_color)        return;
//    else
//    {
//        std::size_t maxi = avail_modules[module]->quantity_sequence_size(quantity.toStdString());
//        for(std::size_t i = module_quan_index[module][quantity]; i < maxi; i++)
//        {
//            module_quan_index[module][quantity] = i;
//            avail_modules[module]->render(quantity.toStdString(), module_quan_index[module][quantity]);
//            if(i < (maxi-1)) delay(spinBoxVCRDelay->value()); // in milliseconds
//        }
//    }
}

void MainWindow::show_forward()
{
//    if(active_modules->count() == 0)   return;

//    // get the currently selected active module ..
//    //
//    QListWidgetItem* item = active_modules->currentItem();
//    QString module = item->text();

//    // get the currently selected quantity from the combobox ..
//    //
//    QString quantity = comboBoxFieldViz->currentText();

//    if(quantity == key::solid_color)        return;
//    else
//    {
//        // increment the current index - watch for the upper bound
//        //
//        if(module_quan_index[module][quantity] != (avail_modules[module]->quantity_sequence_size(quantity.toStdString())-1))
//            module_quan_index[module][quantity]++;

//        // forward the quantity id to the module's renderer
//        // the module will render the quantity accordingly
//        avail_modules[module]->render(quantity.toStdString(), module_quan_index[module][quantity]);
//    }
}

void MainWindow::show_last()
{
//    if(active_modules->count() == 0)   return;

//    // get the currently selected active module ..
//    //
//    QListWidgetItem* item = active_modules->currentItem();
//    QString module = item->text();

//    // get the currently selected quantity from the combobox ..
//    //
//    QString quantity = comboBoxFieldViz->currentText();

//    if(quantity == key::solid_color)        return;
//    else
//    {
//        // set the current index to the last frame
//        //
//        module_quan_index[module][quantity] = avail_modules[module]->quantity_sequence_size(quantity.toStdString())-1;


//        // forward the quantity id to the module's renderer
//        // the module will render the quantity accordingly
//        avail_modules[module]->render(quantity.toStdString(), module_quan_index[module][quantity]);
//    }
}

void MainWindow::delay(int mseconds)
{
    QTime dieTime= QTime::currentTime().addMSecs(mseconds);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, mseconds);
}


void MainWindow::on_actionRemove_Current_View_triggered()
{
    QDockWidget* dock = multi_view->removeCurrentView();
    this->removeDockWidget(dock);
    if(multi_view->getNumberOfViews() == 0)
    {
        this->addNewView();
    }
}

void MainWindow::addNewView()
{
    QDockWidget* dock = this->createNewViewDock();
    this->addDockWidget(Qt::RightDockWidgetArea, dock);
    multi_view->addNewView(dock);
    this->enableViewControls(false);
}

void MainWindow::current_view_changed()
{
    comboBoxMeshRepresentations->setCurrentIndex(meshrep_views[multi_view->getCurrentViewIndex()]);
    comboBoxFieldViz->setCurrentIndex(quan_views[multi_view->getCurrentViewIndex()]);

    if(multi_view->currentIsChart2D())
    {
        this->enableChartControls(true);
    }
    else if(multi_view->currentIsRender3D())
    {
        this->enableRenderControls(true);
    }
    else
    {
        this->enableViewControls(false);
    }
}

void MainWindow::enableChartControls(bool state)
{
    pushButtonToggleColorBar->setEnabled(!state);
    pushButtonToggleAxes->setEnabled(!state);
    pushButtonToggleCoordinates->setEnabled(!state);
    pushButtonToggle2D3D->setEnabled(!state);
    pushButtonRenderEditor->setEnabled(!state);
    pushButtonToggleColorBar->setEnabled(!state);
    ui->toolBarVCR->setEnabled(!state);
    comboBoxFieldViz->setEnabled(!state);
    comboBoxMeshRepresentations->setEnabled(!state);

    pushButtonChartEditor->setEnabled(state);
    pushButtonResetView->setEnabled(state);
}

void MainWindow::enableRenderControls(bool state)
{
    pushButtonToggleColorBar->setEnabled(state);
    pushButtonToggleAxes->setEnabled(state);
    pushButtonToggleCoordinates->setEnabled(state);
    pushButtonToggle2D3D->setEnabled(state);
    pushButtonRenderEditor->setEnabled(state);
    pushButtonToggleColorBar->setEnabled(state);
    pushButtonResetView->setEnabled(state);
    ui->toolBarVCR->setEnabled(state);
    comboBoxFieldViz->setEnabled(state);
    comboBoxMeshRepresentations->setEnabled(state);

    pushButtonChartEditor->setEnabled(!state);
}


void MainWindow::enableViewControls(bool state)
{
    pushButtonToggleColorBar->setEnabled(state);
    pushButtonToggleAxes->setEnabled(state);
    pushButtonToggleCoordinates->setEnabled(state);
    pushButtonToggle2D3D->setEnabled(state);
    pushButtonRenderEditor->setEnabled(state);
    pushButtonToggleColorBar->setEnabled(state);
    pushButtonResetView->setEnabled(state);
    ui->toolBarVCR->setEnabled(state);
    comboBoxFieldViz->setEnabled(state);
    comboBoxMeshRepresentations->setEnabled(state);

    pushButtonChartEditor->setEnabled(state);
}

void MainWindow::on_actionSplit_Current_View_Horizontal_triggered()
{
    QDockWidget* dock = this->createNewViewDock();
    this->splitDockWidget(multi_view->getCurrentDock(), dock, Qt::Horizontal);
    multi_view->addNewView(dock);
}

void MainWindow::on_actionSplit_Current_View_Vertical_triggered()
{
    QDockWidget* dock = this->createNewViewDock();
    this->splitDockWidget(multi_view->getCurrentDock(), dock, Qt::Vertical);
    multi_view->addNewView(dock);
}

QDockWidget* MainWindow::createNewViewDock()
{
    QDockWidget* dock = new QDockWidget(this);
    dock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    dock->setFeatures(QDockWidget::DockWidgetMovable);
    dock->setFeatures(QDockWidget::DockWidgetFloatable);
    return dock;
}

void MainWindow::on_actionLicense_triggered()
{
   license->show();
}

void MainWindow::on_actionMaterial_Manager_triggered()
{
    material_manager->show();
}



void MainWindow::on_actionOutput_triggered()
{
    if(ui->actionOutput->isChecked()) dockOutput->show();
    else dockOutput->hide();
}

void MainWindow::on_actionTake_Screenshot_triggered()
{
    screenshot->show();
}

void MainWindow::initializeCurrent3DRenderView()
{
  change_quantity_visualization(0);
  multi_view->resetCurrentView();
}

void MainWindow::initializeCurrent2DChartView()
{

}
