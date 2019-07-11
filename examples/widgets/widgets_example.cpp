#include "widgets_example.h"
#include "../../widgets/qt_widgets_extensions.hpp"

#include <QApplication>

MainWindow::MainWindow()
{
    setWindowTitle("Qt widgets extensions example");
    resize(1280, 720);

    // TODO: write an example
}

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);

    MainWindow mw;
    mw.show();

    return app.exec();
}
