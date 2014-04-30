#ifndef VIENNAMINIMODULE_H
#define VIENNAMINIMODULE_H

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


// ViennaMOS includes
//
#include "module_interface.h"

// Local includes
//
#include "viennaminiform.h"

#include "viennamini/simulator.hpp"


class ViennaMiniModule : public ModuleInterface
{
  Q_OBJECT

public:
  ViennaMiniModule                            ();
  ~ViennaMiniModule                           ();
  virtual QString         name                ();
  virtual QString         description         ();
  virtual QString         version             ();
  virtual void            render              (Quantity& quan, int step);
  virtual bool            is_ready            ();
  virtual void            update              ();
  virtual void            reset               ();
  virtual std::size_t     quantity_sequence_size  (std::string quankey);
  virtual void            execute             ();
  virtual void            preprocess          ();

private slots:
  void loadMeshFile(QString const& filename);

private:
  ViennaMiniForm*     widget;

  viennamini::simulator_handle  vmini_simulator_;
  std::string vmini_device_db_key_;
};

#endif // VIENNAMINIMODULE_H
