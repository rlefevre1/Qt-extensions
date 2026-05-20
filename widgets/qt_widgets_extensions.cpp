/* 
 * QT WIDGETS EXTENSIONS
 *
 * Copyright (c) 2019, Raphaël Lefèvre
 *
 * Author: Raphaël Lefèvre
*/

#include "qt_widgets_extensions.hpp"

#include <QTime>
#include <QCoreApplication>
#include <QVBoxLayout>
#include <QPainter>
#include <QPaintEvent>

namespace qt_ext
{
    // ========== ========== ========== ========== ==========
    QExpandableWidget::QExpandableWidget(const QString & title, BEHAVIOUR animation, QWidget * parent) : QWidget(parent),
                      toggle(nullptr), header(nullptr), body(nullptr), expand_height(0), animated(animation), toggleAnimation(nullptr), duration(200)
    {
        init_toggle(title);
    }

    QExpandableWidget::~QExpandableWidget()
    {
        delete toggle;
        delete header;
        delete body;
        delete toggleAnimation;
    }

    void QExpandableWidget::setTitle(const QString & title)
    {
        toggle->setText(title);
    }

    bool QExpandableWidget::setContents(QWidget * b, STRETCHING header_stretching)
    {
        return setContents(b, nullptr, header_stretching);
    }

    bool QExpandableWidget::setContents(QWidget * b, QWidget * h, STRETCHING header_stretching)
    {
        if(body == nullptr && b != nullptr) // If no existing body and one is given
        {
            header = h;
            body = b;

            QVBoxLayout * outer_layout = new QVBoxLayout;
            QHBoxLayout * inner_layout = new QHBoxLayout;

            if(header_stretching == TO_RIGHT || header_stretching == TO_CENTER)
                inner_layout->addStretch();

            inner_layout->addWidget(toggle);

            if(header != nullptr)
                inner_layout->addWidget(header);

            if(header_stretching == TO_LEFT || header_stretching == TO_CENTER)
                inner_layout->addStretch();

            outer_layout->addLayout(inner_layout);
            outer_layout->addWidget(body);
            outer_layout->addStretch();

            expand_height = body->sizeHint().height();
            body->setMaximumHeight(0); // collapse

            this->setLayout(outer_layout);

            if(animated)
                init_animation();

            return true;
        }
        return false;
    }

    void QExpandableWidget::clearContents()
    {
        if(animated)
        {
            delete toggleAnimation;
            toggleAnimation = nullptr;
        }

        QString title = toggle->text();
        delete this->layout();

        init_toggle(title);

        if(header != nullptr)
        {
            delete header;
            header = nullptr;
        }
        if(body != nullptr)
        {
            delete body;
            body = nullptr;
        }
    }

    bool QExpandableWidget::isAnimated() const
    {
        return animated;
    }

    unsigned int QExpandableWidget::getDuration() const
    {
        return duration;
    }

    QWidget * QExpandableWidget::getHeader()
    {
        return header;
    }

    QWidget * QExpandableWidget::getBody()
    {
        return body;
    }

    QToolButton * QExpandableWidget::getToggle()
    {
        return toggle;
    }

    void QExpandableWidget::init_toggle(const QString & title)
    {
        if(toggle == nullptr)
        {
            toggle = new QToolButton(this);
            toggle->setStyleSheet("QToolButton { border: none; }");
            toggle->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            toggle->setArrowType(Qt::ArrowType::RightArrow);
            toggle->setText(title);
            toggle->setCheckable(true);
            toggle->setChecked(false);
            connect(toggle, &QToolButton::toggled, this, &QExpandableWidget::expand_collapse);
        }
    }

    void QExpandableWidget::init_animation()
    {
        if(toggleAnimation == nullptr)
        {
            toggleAnimation = new QPropertyAnimation(body, "maximumHeight", this);
            toggleAnimation->setDuration(static_cast<int>(duration));
        }
    }

    void QExpandableWidget::expand_collapse(bool checked)
    {
        if(checked)
        {
            toggle->setArrowType(Qt::ArrowType::DownArrow);
            if(!animated)
                body->setMaximumHeight(expand_height);
            else
            {
                toggleAnimation->setStartValue(0);
                toggleAnimation->setEndValue(expand_height);
                toggleAnimation->start();
            }
            emit heightChanged(expand_height);
        }
        else
        {
            toggle->setArrowType(Qt::ArrowType::RightArrow);
            if(!animated)
                body->setMaximumHeight(0);
            else
            {
                toggleAnimation->setStartValue(expand_height);
                toggleAnimation->setEndValue(0);
                toggleAnimation->start();
            }
            emit heightChanged(-expand_height);
        }
    }
    // ========== ========== ========== ========== ==========

    // ========== ========== ========== ========== ==========
    QListWidget::QListWidget(QWidget * parent) : ::QListWidget(parent)
    {}

    void QListWidget::addWidget(QWidget * w)
    {
        QListWidgetItem * item = new QListWidgetItem(this);
        this->addItem(item);
        item->setSizeHint(w->sizeHint());
        this->setItemWidget(item, w);
    }

    void QListWidget::addWidget(QExpandableWidget * ew)
    {
        addWidget(qobject_cast<QWidget*>(ew));
        connect(ew, &QExpandableWidget::heightChanged, this, &QListWidget::itemHeightChanged);
    }

    void QListWidget::itemHeightChanged(int delta)
    {
        QExpandableWidget * ew = qobject_cast<QExpandableWidget*>(sender());
        if(ew != nullptr)
        {
            bool found(false);
            int i = 0;
            while(!found && i < count())
            {
                if(itemWidget(item(i)) == ew)
                    found = true;
                else
                    ++i;
            }
            if(ew->isAnimated() && (delta < 0))
            {
                QTime dieTime = QTime::currentTime().addMSecs(static_cast<int>(ew->getDuration()));
                while(QTime::currentTime() < dieTime)
                    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
            }
            item(i)->setSizeHint(QSize(ew->sizeHint().width(), item(i)->sizeHint().height()+delta));
        }
    }
    // ========== ========== ========== ========== ==========

    // ========== ========== ========== ========== ==========
    bool QMultiSegmentBar::addSegment(const QString & label, unsigned int weight, const QBrush & background, const QColor & foreground)
    {
        bool valid = true;
        for(const segment & s : segments)
        {
            if(s.label == label)
            {
                valid = false;
                break;
            }
        }

        if(valid)
        {
            segments.append({label, weight, background, foreground});
            total_weight += weight;
            update();
        }

        return valid;
    }

    bool QMultiSegmentBar::removeSegment(const QString & label)
    {
        int idx = -1;
        unsigned int weight {};
        for(int i = 0; i < segments.size(); ++i)
        {
            if(segments[i].label == label)
            {
                idx = i;
                weight = segments[i].weight;
                break;
            }
        }

        if(idx >= 0)
        {
            segments.removeAt(idx);
            total_weight -= weight;
            update();
        }

        return (idx >= 0);
    }

    void QMultiSegmentBar::clear()
    {
        segments.clear();
        update();
    }

    bool QMultiSegmentBar::moveWeight(const QString & label_from, const QString & label_to, unsigned int value)
    {
        segment * seg_from = nullptr;
        segment * seg_to = nullptr;

        unsigned int flag = 0;
        for(auto it = segments.begin(); flag < 2 && it != segments.end(); ++it)
        {
            if(it->label == label_from)
            {
                seg_from = &(*it);
                ++flag;
            }

            if(it->label == label_to)
            {
                seg_to = &(*it);
                ++flag;
            }
        }

        if(flag == 2 && seg_from->weight >= value)
        {
            seg_from->weight -= value;
            seg_to->weight += value;
            update();

            return true;
        }
        else
            return false;
    }

    void QMultiSegmentBar::setDisplayMode(DISPLAY_MODE disp_mode)
    {
        display_mode = disp_mode;
        update();
    }

    void QMultiSegmentBar::paintEvent(QPaintEvent * pe)
    {
        if(total_weight)
        {
            QPainter painter(this);
    
            QRect seg_area {0, 0, 0, pe->rect().height()};
            for(const segment & s : segments)
            {
                // Define the segment with
                seg_area.setWidth(s.weight * pe->rect().width() / total_weight);
    
                // Paint
                QString txt = s.label;
    
                if(display_mode == PERCENTAGES)
                    txt.append(" (" + QString::number(qRound(s.weight * 100.0 / total_weight)) + "%)");
    
                else if(display_mode == WEIGHTS)
                    txt.append(" (" + QString::number(s.weight) + ')');
    
                painter.fillRect(seg_area, s.background);
                painter.setPen(s.foreground);
                painter.drawText(seg_area, Qt::AlignCenter, txt);
    
                // Update starting point for next segment
                seg_area.setX(seg_area.x() + seg_area.width());
            }
        }
    }
    // ========== ========== ========== ========== ==========
}
