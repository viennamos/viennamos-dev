#ifndef MESSENGER_H
#define MESSENGER_H

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

#include <QPlainTextEdit>

#include <iostream>
#include <streambuf>
#include <string>
#include <vector>

#include "qdebugstream.h"

#include "boost/shared_ptr.hpp"

class Messenger : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit Messenger(QWidget *parent = 0);
    ~Messenger();
    void append(QString const& msg);
    void claimStream(std::ostream & os);
    void claimDefaultStreams();
    void releaseStreams();
    QPlainTextEdit* getPlainTextEditWidget();


private:
    typedef boost::shared_ptr<QDebugStream> DebugStreamsPtr;
    typedef std::vector<DebugStreamsPtr>    DebugStreams;

    DebugStreams    debug_streams;
    std::map<std::ostream*, bool>    stream_unifier;
};

#endif // MESSENGER_H
