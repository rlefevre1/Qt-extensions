/*
 * QT CORE EXTENSIONS
 *
 * Copyright (c) 2019, Raphaël Lefèvre
 *
 * Author: Raphaël Lefèvre
*/

#include "qt_core_extensions.hpp"

#include <QTime>
#include <QCoreApplication>

namespace qt_ext
{
    void delay_ms(int ms)
    {
        QTime die_time = QTime::currentTime().addMSecs(ms);
        while(QTime::currentTime() < die_time)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
    }
    void delay_s(int s)
    {
        QTime die_time = QTime::currentTime().addSecs(s);
        while(QTime::currentTime() < die_time)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}
