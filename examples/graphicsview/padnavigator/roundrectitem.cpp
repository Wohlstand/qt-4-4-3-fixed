/****************************************************************************
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the example classes of the Qt Toolkit.
**
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License versions 2.0 or 3.0 as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file.  Please review the following information
** to ensure GNU General Public Licensing requirements will be met:
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.  In addition, as a special
** exception, Nokia gives you certain additional rights. These rights
** are described in the Nokia Qt GPL Exception version 1.3, included in
** the file GPL_EXCEPTION.txt in this package.
**
** Qt for Windows(R) Licensees
** As a special exception, Nokia, as the sole copyright holder for Qt
** Designer, grants users of the Qt/Eclipse Integration plug-in the
** right for the Qt/Eclipse Integration to link to functionality
** provided by Qt Designer and its related libraries.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
****************************************************************************/

#include "roundrectitem.h"

#include <QtGui/QtGui>

RoundRectItem::RoundRectItem(const QRectF &rect, const QBrush &brush, QWidget *embeddedWidget)
    : QGraphicsRectItem(rect),
      brush(brush),
      timeLine(75),
      lastVal(0),
      opa(1),
      proxyWidget(0)
{
    connect(&timeLine, SIGNAL(valueChanged(qreal)),
            this, SLOT(updateValue(qreal)));
    
    if (embeddedWidget) {
        proxyWidget = new QGraphicsProxyWidget(this);
        proxyWidget->setFocusPolicy(Qt::StrongFocus);
        proxyWidget->setWidget(embeddedWidget);
        proxyWidget->setGeometry(boundingRect().adjusted(25, 25, -25, -25));
    }
}

void RoundRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QTransform x = painter->worldTransform();

    QLineF unit = x.map(QLineF(0, 0, 1, 1));
    if (unit.p1().x() > unit.p2().x() || unit.p1().y() > unit.p2().y()) {
        if (proxyWidget && proxyWidget->isVisible()) {
            proxyWidget->hide();
            proxyWidget->setGeometry(rect());
        }
        return;
    }

    if (proxyWidget && !proxyWidget->isVisible()) {
        proxyWidget->show();
        proxyWidget->setFocus();
    }
    if (proxyWidget && proxyWidget->pos() != QPoint())
        proxyWidget->setGeometry(boundingRect().adjusted(25, 25, -25, -25));

    painter->setOpacity(opacity());
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(0, 0, 0, 64));
    painter->drawRoundRect(rect().translated(2, 2));

    if (!proxyWidget) {
        QLinearGradient gradient(rect().topLeft(), rect().bottomRight());
        const QColor col = brush.color();
        gradient.setColorAt(0, col);
        gradient.setColorAt(1, col.dark(int(200 + lastVal * 50)));
        painter->setBrush(gradient);
    } else {
        painter->setBrush(brush);
    }

    painter->setPen(QPen(Qt::black, 1));
    painter->drawRoundRect(rect());
    if (!pix.isNull()) {
        painter->scale(1.95, 1.95);
        painter->drawPixmap(-pix.width() / 2, -pix.height() / 2, pix);;
    }
}

QRectF RoundRectItem::boundingRect() const
{
    qreal penW = 0.5;
    qreal shadowW = 2.0;
    return rect().adjusted(-penW, -penW, penW + shadowW, penW + shadowW);
}

void RoundRectItem::setPixmap(const QPixmap &pixmap)
{
    pix = pixmap;
    if (scene() && isVisible())
        update();
}

qreal RoundRectItem::opacity() const
{
    RoundRectItem *parent = parentItem() ? (RoundRectItem *)parentItem() : 0;
    return opa + (parent ? parent->opacity() : 0);
}

void RoundRectItem::setOpacity(qreal opacity)
{
    opa = opacity;
    update();
}

void RoundRectItem::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat() || event->key() != Qt::Key_Return
        || (timeLine.state() == QTimeLine::Running && timeLine.direction() == QTimeLine::Forward)) {
        QGraphicsRectItem::keyPressEvent(event);
        return;
    }

    timeLine.stop();
    timeLine.setDirection(QTimeLine::Forward);
    timeLine.start();
    emit activated();
}

void RoundRectItem::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() != Qt::Key_Return) {
        QGraphicsRectItem::keyReleaseEvent(event);
        return;
    }
    timeLine.stop();
    timeLine.setDirection(QTimeLine::Backward);
    timeLine.start();
}

void RoundRectItem::updateValue(qreal value)
{
    lastVal = value;
    if (!proxyWidget)
        setTransform(QTransform().scale(1 - value / 10.0, 1 - value / 10.0));
}
