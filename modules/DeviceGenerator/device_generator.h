#ifndef DEVICE_GENERATOR_H
#define DEVICE_GENERATOR_H

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

#include <QtPlugin>

#include "device_generator_global.h"
#include "device_generator_module.h"
#include "module_factory.h"

class DEVICEGENERATORSHARED_EXPORT DeviceGenerator : public QObject, public ModuleFactory
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID ModuleFactory_iid FILE "device_generator.json")
#endif
    Q_INTERFACES(ModuleFactory)

public:
    DeviceGenerator();
    ModuleInterface* create();
};

#endif // DEVICE_GENERATOR_H
