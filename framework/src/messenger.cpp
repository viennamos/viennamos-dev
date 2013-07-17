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

#include "messenger.h"

#include <QDebug>

Messenger::Messenger(QWidget *parent) :
    QPlainTextEdit(parent)
{
    setReadOnly(true);
    setStyleSheet("background-color: lightgray");
    setBackgroundVisible(true);
}

Messenger::~Messenger()
{
}

void Messenger::append(QString const& msg)
{
    appendPlainText(msg);
}

QPlainTextEdit* Messenger::getPlainTextEditWidget()
{
    return this;
}

void Messenger::claimStream(std::ostream & os)
{
    // make sure to only add streams, which haven't been already added
    if(!stream_unifier[&os])
    {
        DebugStreamsPtr aga(new QDebugStream(os, this));
        debug_streams.push_back(aga);
        stream_unifier[&os] = true;
    }
}

void Messenger::claimDefaultStreams()
{
    this->claimStream(std::cout);
    this->claimStream(std::cerr);
}


void Messenger::releaseStreams()
{
    debug_streams.clear();
    stream_unifier.clear();
}

