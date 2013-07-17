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

#include "stream_emitter.h"

#include <QDebug>

StreamEmitter::StreamEmitter(std::ostream &stream)
    : QObject(NULL), m_stream(stream)
{
    m_old_buf = stream.rdbuf();
    stream.rdbuf(this);
}


StreamEmitter::~StreamEmitter()
{
    m_stream.rdbuf(m_old_buf);
}


StreamEmitter::int_type StreamEmitter::overflow(StreamEmitter::int_type v)
{
    if (v == '\n')
    {
        emit message(QString::fromStdString(m_string));
        m_string.erase(m_string.begin(), m_string.end());
    }
    else
        m_string += v;

    return v;
}

std::streamsize StreamEmitter::xsputn(const char *p, std::streamsize n)
{
    m_string.append(p, p + n);

    std::size_t pos = 0;
    while (pos != std::string::npos)
    {
        pos = m_string.find('\n');
        if (pos != std::string::npos)
        {
            std::string tmp(m_string.begin(), m_string.begin() + pos);

            emit message(QString::fromStdString(tmp));
            m_string.erase(m_string.begin(), m_string.begin() + pos + 1);
        }
    }
    return n;
}

