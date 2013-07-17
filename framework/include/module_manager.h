#ifndef MODULE_MANAGER_H
#define MODULE_MANAGER_H

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
#include <QDialog>
#include <QMap>

#include "module_factory.h"
#include "module_interface.h"


typedef QMap<QString, ModuleInterface*>     ModulesMap;


//class RenderEditor : public QDialog
//{
//    Q_OBJECT

//public:
//    explicit RenderEditor(Visualizer* central_viz, QWidget *parent = 0);
//    ~RenderEditor();


namespace Ui {
class ModuleManager;
}

class ModuleManager : public QDialog
{
    Q_OBJECT
    
public:
    explicit ModuleManager(ModulesMap& avail_modules, QWidget *parent = 0);
    ~ModuleManager();
    
    void delete_module(QString const& name);
    void recreate_module(QString const& name);

private slots:

    void on_pushButtonClose_clicked();
    void on_pushButtonReload_clicked();
    void on_pushButtonDeactivate_clicked();
    void on_pushButtonActivate_clicked();

signals:
    void loadModules(QList<QString>& module_list);
    void unloadModules(QList<QString>& module_list);

private:

    void load_modules();

    Ui::ModuleManager *ui;
    ModulesMap&        avail_modules;
    QString            module_path;
    QString            module_prefix;
    QString            key_active;
    QString            key_inactive;
    QMap<QString, int> module_tree_index;
    QMap<QString, ModuleFactory*>   factories;
};

#endif // MODULE_MANAGER_H
