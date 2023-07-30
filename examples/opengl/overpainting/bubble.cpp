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

#include <QtGui>

#include "bubble.h"

Bubble::Bubble(const QPointF &position, qreal radius, const QPointF &velocity)
    : position(position), vel(velocity), radius(radius)
{
    innerColor = randomColor();
    outerColor = randomColor();
    updateBrush();
}

void Bubble::updateBrush()
{
    QRadialGradient gradient(QPointF(radius, radius), radius,
                             QPointF(radius*0.5, radius*0.5));

    gradient.setColorAt(0, QColor(255, 255, 255, 255));
    gradient.setColorAt(0.25, innerColor);
    gradient.setColorAt(1, outerColor);
    brush = QBrush(gradient);
}

void Bubble::drawBubble(QPainter *painter)
{
    painter->save();
    painter->translate(position.x() - radius, position.y() - radius);
    painter->setBrush(brush);
    painter->drawEllipse(0, 0, int(2*radius), int(2*radius));
    painter->restore();
}

QColor Bubble::randomColor()
{
    int red = int(205 + 50.0*qrand()/(RAND_MAX+1.0));
    int green = int(205 + 50.0*qrand()/(RAND_MAX+1.0));
    int blue = int(205 + 50.0*qrand()/(RAND_MAX+1.0));
    int alpha = int(91 + 100.0*qrand()/(RAND_MAX+1.0));

    return QColor(red, green, blue, alpha);
}

void Bubble::move(const QRect &bbox)
{
    position += vel;
    qreal leftOverflow = position.x() - radius - bbox.left();
    qreal rightOverflow = position.x() + radius - bbox.right();
    qreal topOverflow = position.y() - radius - bbox.top();
    qreal bottomOverflow = position.y() + radius - bbox.bottom();
    
    if (leftOverflow < 0.0) {
        position.setX(position.x() - 2 * leftOverflow);
        vel.setX(-vel.x());
    } else if (rightOverflow > 0.0) {
        position.setX(position.x() - 2 * rightOverflow);
        vel.setX(-vel.x());
    }

    if (topOverflow < 0.0) {
        position.setY(position.y() - 2 * topOverflow);
        vel.setY(-vel.y());
    } else if (bottomOverflow > 0.0) {
        position.setY(position.y() - 2 * bottomOverflow);
        vel.setY(-vel.y());
    }
}

QRectF Bubble::rect()
{
    return QRectF(position.x() - radius, position.y() - radius,
                  2 * radius, 2 * radius);
}
