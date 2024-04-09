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
#include <QEventLoop>
#include <QTimer>

namespace qt_ext
{
    void delay_ms(unsigned int ms)
    {
        QTime die_time = QTime::currentTime().addMSecs(ms);
        while(QTime::currentTime() < die_time)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
    }
    void delay_s(unsigned int s)
    {
        QTime die_time = QTime::currentTime().addSecs(s);
        while(QTime::currentTime() < die_time)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    void wait_ms(unsigned int ms)
    {
        QEventLoop loop;
        QTimer::singleShot(ms, &loop, &QEventLoop::quit);
        loop.exec();
    }
    void wait_s(unsigned int s)
    {
        QEventLoop loop;
        QTimer::singleShot(s * 1000, &loop, &QEventLoop::quit);
        loop.exec();
    }
}
