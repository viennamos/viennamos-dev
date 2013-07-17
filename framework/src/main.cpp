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

#include "mainwindow.h"
#include <QApplication>
#include <QPixmap>
#include <QImage>
#include <QSplashScreen>
#include <QDebug>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication viennamos_app(argc, argv);

    MainWindow window;
    window.show();


    return viennamos_app.exec();

//    QApplication viennamos_app(argc, argv);

//    QImage img(":/pictures/resources/splashimage/viennamos_splashimage.png");
//    QPixmap pixmap = QPixmap::fromImage(img);
//    QSplashScreen splash(pixmap);
//    splash.show();
//    viennamos_app.processEvents();
//    MainWindow window;
//    //window.show();


//    QTimer::singleShot(3000, &splash, SLOT(close()));
//    QTimer::singleShot(3000, &window, SLOT(show()));

//    splash.finish(&window);

//    return viennamos_app.exec();
}
