#include "widgets_example.h"
#include "../../widgets/qt_widgets_extensions.hpp"

#include <QApplication>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QTextEdit>

Header::Header(const QString & f1, const QString & f2, const QString & f3, QWidget * parent) : QWidget(parent)
{
    QGridLayout * lay = new QGridLayout;
    lay->setMargin(0);
    lay->setSpacing(0);
    lay->setContentsMargins(0, 0, 0, 0);

    QLabel * f1_lab = new QLabel(f1, this);
    f1_lab->setAlignment(Qt::AlignCenter);
    QLabel * f2_lab = new QLabel(f2, this);
    f2_lab->setAlignment(Qt::AlignCenter);
    QLabel * f3_lab = new QLabel(f3, this);
    f3_lab->setAlignment(Qt::AlignCenter);

    lay->addWidget(f1_lab, 0, 0);
    lay->addWidget(f2_lab, 0, 1);
    lay->addWidget(f3_lab, 0, 2);

    this->setLayout(lay);
}
Body::Body(int val, QWidget * parent) : QWidget(parent)
{
    QHBoxLayout * lay = new QHBoxLayout;

    QPushButton * button = new QPushButton("switch", this);

    p_bar = new QProgressBar(this);
    p_bar->setRange(0, 100);
    p_bar->setValue(val);

    lay->addWidget(button);
    lay->addWidget(p_bar, 1);

    this->setLayout(lay);

    connect(button, &QPushButton::clicked, this, &Body::switch_value);
}
void Body::switch_value()
{
    p_bar->setValue(100 - p_bar->value());
}

MainWindow::MainWindow()
{
    setWindowTitle("Qt widgets extensions example");
    resize(1280, 720);

    QWidget * wid = new QWidget;
    QVBoxLayout * lay = new QVBoxLayout;

    qt_ext::QListWidgetView * lwv = new qt_ext::QListWidgetView(this);
    lwv->setVerticalScrollMode(qt_ext::QListWidgetView::ScrollPerPixel);

    qt_ext::QExpandableWidget * ew1 = new qt_ext::QExpandableWidget("First", qt_ext::QExpandableWidget::DIRECT, lwv);
    qt_ext::QExpandableWidget * ew2 = new qt_ext::QExpandableWidget("Second", qt_ext::QExpandableWidget::DIRECT, lwv);
    qt_ext::QExpandableWidget * ew3 = new qt_ext::QExpandableWidget("Third", qt_ext::QExpandableWidget::DIRECT, lwv);

    Header * h1 = new Header("<Header>", "Item", "1", ew1);
    Header * h2 = new Header("<Header>", "Item", "2", ew2);
    Header * h3 = new Header("<Header>", "Item", "3", ew3);

    Body * b1 = new Body(0, ew1);
    Body * b2 = new Body(20, ew2);
    Body * b3 = new Body(40, ew3);

    ew1->setContents(b1, h1, qt_ext::QExpandableWidget::NONE);
    ew2->setContents(b2, h2, qt_ext::QExpandableWidget::NONE);
    ew3->setContents(b3, h3, qt_ext::QExpandableWidget::NONE);

    lwv->addWidget(ew1);
    lwv->addWidget(ew2);
    lwv->addWidget(ew3);

    qt_ext::QExpandableWidget * ew = new qt_ext::QExpandableWidget("Hello world", qt_ext::QExpandableWidget::ANIMATED, this);
    QTextEdit * te = new QTextEdit(ew);
    ew->setContents(te);

    lay->addWidget(lwv);
    lay->addWidget(ew);
    lay->addStretch();

    wid->setLayout(lay);
    this->setCentralWidget(wid);

    lwv->setStyleSheet("QListWidget {background-color: rgb(64, 64, 64);}");
    this->setStyleSheet("QMainWindow {background-color: rgb(32, 32, 32);}");
    ew->getToggle()->setStyleSheet("QToolButton {border: none; color: white;}");
}

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);

    MainWindow mw;
    mw.show();

    return app.exec();
}
