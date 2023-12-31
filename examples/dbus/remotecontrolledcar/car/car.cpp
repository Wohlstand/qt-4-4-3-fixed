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

#include "car.h"
#include <QtGui/QtGui>
#include <math.h>

static const double Pi = 3.14159265358979323846264338327950288419717;

QRectF Car::boundingRect() const
{
    return QRectF(-35, -81, 70, 115);
}

Car::Car() : color(Qt::green), wheelsAngle(0), speed(0)
{
    startTimer(1000 / 33);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
}

void Car::accelerate()
{
    if (speed < 10)
        ++speed;
}

void Car::decelerate()
{
    if (speed > -10)
        --speed;
}

void Car::turnLeft()
{
    if (wheelsAngle > -30)
        wheelsAngle -= 5;
}

void Car::turnRight()
{
    if (wheelsAngle < 30)
       wheelsAngle += 5;
}

void Car::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setBrush(Qt::gray);
    painter->drawRect(-20, -58, 40, 2); // front axel
    painter->drawRect(-20, 7, 40, 2); // rear axel

    painter->setBrush(color);
    painter->drawRect(-25, -79, 50, 10); // front wing

    painter->drawEllipse(-25, -48, 50, 20); // side pods
    painter->drawRect(-25, -38, 50, 35); // side pods
    painter->drawRect(-5, 9, 10, 10); // back pod

    painter->drawEllipse(-10, -81, 20, 100); // main body

    painter->drawRect(-17, 19, 34, 15); // rear wing

    painter->setBrush(Qt::black);
    painter->drawPie(-5, -51, 10, 15, 0, 180 * 16);
    painter->drawRect(-5, -44, 10, 10); // cocpit

    painter->save();
    painter->translate(-20, -58);
    painter->rotate(wheelsAngle);
    painter->drawRect(-10, -7, 10, 15); // front left
    painter->restore();

    painter->save();
    painter->translate(20, -58);
    painter->rotate(wheelsAngle);
    painter->drawRect(0, -7, 10, 15); // front left
    painter->restore();

    painter->drawRect(-30, 0, 12, 17); // rear left
    painter->drawRect(19, 0, 12, 17);  // rear right
}

void Car::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    const qreal axelDistance = 54;
    qreal wheelsAngleRads = (wheelsAngle * Pi) / 180;
    qreal turnDistance = ::cos(wheelsAngleRads) * axelDistance * 2;
    qreal turnRateRads = wheelsAngleRads / turnDistance;  // rough estimate
    qreal turnRate = (turnRateRads * 180) / Pi;
    qreal rotation = speed * turnRate;
    
    rotate(rotation);
    translate(0, -speed);
    update();
}
