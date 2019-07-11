/*
 * QT WIDGETS EXTENSIONS
 *
 * Copyright (c) 2019, Raphaël Lefèvre
 *
 * Author: Raphaël Lefèvre
*/

#ifndef QT_WIDGETS_EXTENSIONS_H
#define QT_WIDGETS_EXTENSIONS_H

#include <QListWidget>
#include <QToolButton>
#include <QPropertyAnimation>

namespace qt_ext
{
    // ---------- ---------- ---------- ---------- ----------
    class QExpandableWidget : public QWidget
    {
        Q_OBJECT

        public:
            enum BEHAVIOUR : bool {DIRECT=false, ANIMATED=true};
            enum STRETCHING {NONE, TO_LEFT, TO_CENTER, TO_RIGHT};

        protected:
            QToolButton * toggle;
            QWidget * header;
            QWidget * body;
            int expand_height;

            const bool animated;
            QPropertyAnimation * toggleAnimation;
            const unsigned int duration;

        public:
            QExpandableWidget(const QString & title, BEHAVIOUR animation, QWidget * parent = nullptr);
            virtual ~QExpandableWidget();
            void setTitle(const QString & title);
            void setContents(QWidget * b, STRETCHING header_stretching = TO_LEFT);
            void setContents(QWidget * b, QWidget * h, STRETCHING header_stretching = TO_LEFT);
            void clearContents();
            bool isAnimated() const;
            unsigned int getDuration() const;
            QWidget * getHeader();
            QWidget * getBody();
            QToolButton * getToggle();

        private:
            void init_toggle(const QString & title);
            void init_animation();

        private slots:
            void expand_collapse(bool checked);

        signals:
            void heightChanged(int delta);
    };
    // ---------- ---------- ---------- ---------- ----------

    // ---------- ---------- ---------- ---------- ----------
    class QListWidgetView : public QListWidget
    {
        Q_OBJECT

        public:
            QListWidgetView(QWidget * parent = nullptr);
            void addWidget(QWidget * w);
            void addWidget(QExpandableWidget * ew);

        private slots:
            void itemHeightChanged(int delta);
    };
    // ---------- ---------- ---------- ---------- ----------
}

#endif // QT_WIDGETS_EXTENSIONS_H
