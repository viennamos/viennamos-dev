#ifndef STREAMEMITTER_H
#define STREAMEMITTER_H

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

#include <iostream>
#include <streambuf>
#include <string>

#include <QObject>

class StreamEmitter : public QObject, public std::basic_streambuf<char>
{
    Q_OBJECT

public:
    StreamEmitter(std::ostream &stream);
    ~StreamEmitter();

protected:
    virtual int_type overflow(int_type v);
    virtual std::streamsize xsputn(const char *p, std::streamsize n);

signals:
    void message(QString const& msg);

private:
    std::ostream   &m_stream;
    std::streambuf *m_old_buf;
    std::string     m_string;
};

#endif // STREAMEMITTER_H
