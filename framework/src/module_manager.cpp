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

#include "module_manager.h"
#include "ui_module_manager.h"

#include <QDir>
#include <QPluginLoader>
#include <QMessageBox>
#include <QDebug>

//RenderEditor::RenderEditor(Visualizer* central_viz, QWidget *parent) :
//    QDialog(parent),
//    ui(new Ui::RenderEditor),
//    viz(central_viz)
//{
//    ui->setupUi(this);
//    setWindowTitle("Render Editor");

ModuleManager::ModuleManager(ModulesMap& avail_modules, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModuleManager),
    avail_modules(avail_modules)
{
    ui->setupUi(this);
    setWindowTitle("Module Manager");
    //module_path   = "../../modules/build";
    module_path = ".";
    //module_prefix = "viennamos_module_";
    module_prefix = "";
    key_active = "active";
    key_inactive = "inactive";
    load_modules();

    // for now, deactivate this function
    ui->pushButtonReload->hide();

    ui->treeWidget->setFont(QFont("Arial", 12));

    QObject::connect(ui->treeWidget, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(on_pushButtonActivate_clicked()));
}

ModuleManager::~ModuleManager()
{
    delete ui;
}


void ModuleManager::load_modules()
{
    QDir pluginsDir(module_path);

    if(!pluginsDir.exists())
    {
        QMessageBox::critical(this, QString("Error"), QString("Module plugin path is not valid!"));
        return;
    }

    QStringList dso_filter;

#if defined Q_OS_LINUX || defined Q_WS_LINUX
    dso_filter << module_prefix << "*.so";
#elif defined Q_OS_MAC || defined Q_WS_MAC
    dso_filter << module_prefix << "*.dylib";
#elif defined Q_OS_WIN || defined Q_WS_WIN
    dso_filter << module_prefix << "*.dll";
#endif

    // Load the GUI plugins
    //
    foreach (QString fileName, pluginsDir.entryList(dso_filter, QDir::Files)) {

        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));

        QObject* object = NULL;
        object = pluginLoader.instance();

        if(pluginLoader.isLoaded() && object)
        {
            ModuleFactory* factory = qobject_cast<ModuleFactory *>(object);

            ModuleInterface* module = factory->create();

            if(!avail_modules.contains(module->name()))
            {
                avail_modules[module->name()] = module;
                factories[module->name()] = factory;
            }
            else
            {
                QMessageBox::warning(this, QString("Error"), QString("Multiple modules sharing the name ")+module->name()+QString(" detected"));
            }
        }
        else
        {
//            qDebug() << "plugin: " << fileName << " could not be loaded ..";
            qDebug() << "plugin: " << fileName << " could not be loaded (" << pluginLoader.errorString() << ").";
        }
    }

    ui->treeWidget->setSelectionMode(QAbstractItemView::MultiSelection);

    // setup treewidget
    //
    QTreeWidgetItem* headerItem = new QTreeWidgetItem;
    headerItem->setText(0,QString("Plugin"));
    headerItem->setText(1,QString("Description"));
    headerItem->setText(2,QString("Version"));
    headerItem->setText(3,QString("State"));
    ui->treeWidget->setHeaderItem(headerItem); // tree takes ownership of item
    ui->treeWidget->header()->resizeSection(0, 200);
    ui->treeWidget->header()->resizeSection(1, 400);
    ui->treeWidget->header()->resizeSection(2, 100);
    ui->treeWidget->header()->resizeSection(3, 100);


    int i = 0;
    foreach(QString name, avail_modules.keys()) {
        module_tree_index[name] = i++;
        QTreeWidgetItem* item = new QTreeWidgetItem;
        item->setText(0, name);
        item->setText(1, avail_modules.value(name)->description());
        item->setText(2, avail_modules.value(name)->version());
        item->setText(3, key_inactive);
        ui->treeWidget->addTopLevelItem(item); // tree takes ownership of item
    }

    // set plugin size label
    //
    ui->labelPluginSize->setText(QString::number(avail_modules.size())+" Module(s) available");

}


void ModuleManager::delete_module(QString const& name)
{
    if(avail_modules.contains(name))
    {
        delete avail_modules.value(name); // delete the module widget
        avail_modules[name] = NULL;

        QTreeWidgetItem* item = ui->treeWidget->topLevelItem(module_tree_index[name]);
        item->setText(3, key_inactive);
    }
}

void ModuleManager::recreate_module(const QString& name)
{
    avail_modules[name] = factories[name]->create();
}

void ModuleManager::on_pushButtonClose_clicked()
{
    close();
}

void ModuleManager::on_pushButtonReload_clicked()
{
    if(avail_modules.empty())
        load_modules();
    else
    {
        foreach(QString module, avail_modules.keys())
        {
            delete avail_modules.value(module);
        }
        avail_modules.clear();

        ui->treeWidget->clear();
        load_modules();
    }
}

void ModuleManager::on_pushButtonDeactivate_clicked()
{
    QList<QTreeWidgetItem*> selected_modules = ui->treeWidget->selectedItems();
    QList<QString> module_names;
    foreach (QTreeWidgetItem* item, selected_modules) {
        if(item->text(3) == key_active)
        {
            module_names.append(item->text(0));
            ui->treeWidget->topLevelItem(ui->treeWidget->indexOfTopLevelItem(item))->setText(3, key_inactive);
        }
    }
    emit unloadModules(module_names);
}

void ModuleManager::on_pushButtonActivate_clicked()
{
    QList<QTreeWidgetItem*> selected_modules = ui->treeWidget->selectedItems();
    QList<QString> module_names;
    foreach (QTreeWidgetItem* item, selected_modules) {
        if(item->text(3) == key_inactive)
        {
            module_names.append(item->text(0));
            ui->treeWidget->topLevelItem(ui->treeWidget->indexOfTopLevelItem(item))->setText(3, key_active);
        }
    }
    emit loadModules(module_names);
}
