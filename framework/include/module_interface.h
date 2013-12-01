#ifndef MODULE_INTERFACE_H
#define MODULE_INTERFACE_H

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

#include <QObject>
#include <QGridLayout>
#include <QStringList>
#include <vector>

#include "database.hpp"
#include "multiview.h"
//#include "quantity_set.hpp"
#include "quantity.h"
#include "messenger.h"
#include "materialmanager.h"
#include "modulecontrol.h"

class ModuleInterface : public QObject
{
    Q_OBJECT

public:
    ModuleInterface (ModuleInterface* child);
    virtual ~ModuleInterface                        ();
    virtual QString         name                    ()                                                  = 0;
    virtual QString         description             ()                                                  = 0;
    virtual QString         version                 ()                                                  = 0;
    virtual void            render                  (Quantity& quan, int step)                     = 0;
    virtual bool            is_ready                ()                                                  = 0;
    virtual void            update                  ()                                                  = 0;
    virtual void            reset                   ()                                                  = 0;
    virtual std::size_t     quantity_sequence_size  (std::string quankey)                               = 0;
    virtual void execute    () = 0;

    void setup(DataBase* central_db,         MultiView* central_multiview,
               Messenger* central_messenger, MaterialManager* central_material_manager);
    QWidget*                    get_widget();
    void                        register_module_widget(QWidget* widget);

    QuantitySet& get_quantity_set();
    void         register_quantity(Quantity const& quan);
    void  clear_quantities();

private slots:
    void on_pushButtonSave_clicked();
    void on_pushButtonLoad_clicked();
    void pre_execute();
    void post_execute();
    void report_error();



public slots:
    virtual void preprocess () = 0;


signals:
    void module_begin     (QString const& name);
    void module_end       (QString const& name);
    //void module_end       (QString const& name, Quantity::TupleContainer const& quantity_tuples, int default_quantity);
    void module_end_error (QString const& name);
    void render_log       (bool state);
    void state_loaded     (QSettings& settings);
    void state_saved      (QSettings& settings);
    void execute_module   ();
    void base_data_ready  ();
    void finished         ();
    void abort            ();

public:
    DataBase*                   database;
    MultiView*                  multiview;
    Messenger*                  messenger;
    MaterialManager*            material_manager;

    QuantitySet                 quantity_set;
    ModuleControl*              module_control_widget;
    ModuleInterface*            child;
};


#endif // MODULE_INTERFACE_H
