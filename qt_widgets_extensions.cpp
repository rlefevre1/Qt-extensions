/* 
 * QT WIDGETS EXTENSIONS
 *
 * Copyright (c) 2019, Raphaël Lefèvre
 *
 * Author: Raphaël Lefèvre
 * Repository: https://github.com/rlefevre1/Qt-extensions
*/

#include "qt_widgets_extensions.h"

#include <QTime>
#include <QCoreApplication>
#include <QVBoxLayout>

#include <QDebug>

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
    void QExpandableWidget::setContent(QWidget * b, STRETCHING header_stretching)
    {
        setContent(b, nullptr, header_stretching);
    }
    void QExpandableWidget::setContent(QWidget * b, QWidget * h, STRETCHING header_stretching)
    {
        if(body != nullptr)
            qWarning() << "qt_ext::QExpandableWidget::setContent : Already existing content -> nothing to be done";
        else if(b == nullptr)
            qWarning() << "qt_ext::QExpandableWidget::setContent : No body given -> nothing to be done";
        else
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
        }
    }
    void QExpandableWidget::clearContent()
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
            emit heightChanged(expand_height);
            toggle->setArrowType(Qt::ArrowType::DownArrow);
            if(!animated)
                body->setMaximumHeight(expand_height);
            else
            {
                toggleAnimation->setStartValue(0);
                toggleAnimation->setEndValue(expand_height);
                toggleAnimation->start();
            }
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
    QListWidgetView::QListWidgetView(QWidget * parent) : QListWidget(parent)
    {}
    void QListWidgetView::addWidget(QWidget * w)
    {
        QListWidgetItem * item = new QListWidgetItem(this);
        this->addItem(item);
        item->setSizeHint(w->sizeHint());
        this->setItemWidget(item, w);
    }
    void QListWidgetView::addWidget(QExpandableWidget * ew)
    {
        QListWidgetItem * item = new QListWidgetItem(this);
        this->addItem(item);
        item->setSizeHint(ew->sizeHint());
        this->setItemWidget(item, ew);

        connect(ew, &QExpandableWidget::heightChanged, this, &QListWidgetView::itemHeightChanged);
    }
    void QListWidgetView::itemHeightChanged(int delta)
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
}
