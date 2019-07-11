#include "../../core/qt_core_extensions.hpp"

#include <QDebug>

int main()
{
    int delay(5);
    qInfo() << "Waiting 5 seconds...";
    qt_ext::delay_s(delay);
    qInfo() << "Done !";

    return 0;
}
