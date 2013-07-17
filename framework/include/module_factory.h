#ifndef MODULE_FACTORY_H
#define MODULE_FACTORY_H

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

#include "database.hpp"


#include "module_interface.h"



class ModuleFactory
{
public:
    virtual ~ModuleFactory() {}
    virtual ModuleInterface* create() = 0;
};

#define ModuleFactory_iid "ViennaMOS.ModuleFactory"
Q_DECLARE_INTERFACE(ModuleFactory, ModuleFactory_iid)

#endif // MODULE_FACTORY_H
