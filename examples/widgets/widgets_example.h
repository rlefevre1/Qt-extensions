#ifndef WIDGETS_EXAMPLE_H
#define WIDGETS_EXAMPLE_H

#include <QMainWindow>
#include <QProgressBar>

class Header : public QWidget
{
    Q_OBJECT

    public:
        Header(const QString & f1, const QString & f2, const QString & f3, QWidget * parent = nullptr);
};
class Body : public QWidget
{
    Q_OBJECT

    protected:
        QProgressBar * p_bar;

    public:
        Body(int val, QWidget * parent = nullptr);

    public slots:
        void switch_value();
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow();
};

#endif // WIDGETS_EXAMPLE_H
