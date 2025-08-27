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
            bool setContents(QWidget * b, STRETCHING header_stretching = TO_LEFT);
            bool setContents(QWidget * b, QWidget * h, STRETCHING header_stretching = TO_LEFT);
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
    class QListWidget : public ::QListWidget
    {
        Q_OBJECT

        public:
            QListWidget(QWidget * parent = nullptr);
            void addWidget(QWidget * w);
            void addWidget(QExpandableWidget * ew);

        private slots:
            void itemHeightChanged(int delta);
    };
    // ---------- ---------- ---------- ---------- ----------

    // ---------- ---------- ---------- ---------- ----------
    class QMultiSegmentBar : public QFrame
    {
        Q_OBJECT

        public:
            enum DISPLAY_MODE {RAW, PERCENTAGES, WEIGHTS};

            using QFrame::QFrame;

            bool addSegment(const QString & label, unsigned int weight, const QBrush & background, const QColor & foreground = Qt::black);
            bool removeSegment(const QString & label);
            void clear();
            bool moveWeight(const QString & label_from, const QString & label_to, unsigned int value = 1);
            void setDisplayMode(DISPLAY_MODE disp_mode);

        protected:
            void paintEvent(QPaintEvent * pe) override;

        private:
            struct segment
            {
                QString label;
                unsigned int weight;
                QBrush background;
                QColor foreground;
            };

            QList<segment> segments;
            unsigned int total_weight = 0;
            DISPLAY_MODE display_mode = RAW;
    };
    // ---------- ---------- ---------- ---------- ----------
}

#endif // QT_WIDGETS_EXTENSIONS_H
