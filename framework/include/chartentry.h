#ifndef CHARTENTRY_H
#define CHARTENTRY_H

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

#include <QString>
#include <QColor>
#include <boost/array.hpp>

struct ChartEntry
{
    ChartEntry();
    ChartEntry(int index, QString key, QString label, QColor color_rgb, bool auto_color,
               int thickness, int line_style, int marker_style, float marker_size, bool enabled);

    int                     index;
    QString                 key;
    QString                 label;
    QColor                  color_rgb;
    bool                    auto_color;
    float                   thickness;
    int                     line_style;
    int                     marker_style;
    float                   marker_size;
    bool                    enabled;
};


#endif // CHARTENTRY_H
