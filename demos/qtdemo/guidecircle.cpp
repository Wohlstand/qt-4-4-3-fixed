/****************************************************************************
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the demonstration applications of the Qt Toolkit.
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

#include "guidecircle.h"

static float PI2 = 2*3.1415f;

GuideCircle::GuideCircle(const QRectF &rect, float startAngle, float span, DIRECTION dir, Guide *follows) : Guide(follows)
{
    this->radiusX = rect.width() / 2.0;
    this->radiusY = rect.height() / 2.0;
    this->posX = rect.topLeft().x();
    this->posY = rect.topLeft().y();
    this->spanRad = span * PI2 / -360.0;
    if (dir == CCW){
        this->startAngleRad = startAngle * PI2 / -360.0;
        this->endAngleRad = startAngleRad + spanRad;
        this->stepAngleRad = this->spanRad / this->length();
    }
    else{
        this->startAngleRad = spanRad + (startAngle * PI2 / -360.0);
        this->endAngleRad = startAngle * PI2 / -360.0;
        this->stepAngleRad = -this->spanRad / this->length();
    }
}

float GuideCircle::length()
{
    return qAbs(this->radiusX * spanRad);
}

QPointF GuideCircle::startPos()
{
    return QPointF((posX + radiusX + radiusX * cos(startAngleRad)) * scaleX,
                   (posY + radiusY + radiusY * sin(startAngleRad)) * scaleY);
}

QPointF GuideCircle::endPos()
{
    return QPointF((posX + radiusX + radiusX * cos(endAngleRad)) * scaleX,
                   (posY + radiusY + radiusY * sin(endAngleRad)) * scaleY);
}

void GuideCircle::guide(DemoItem *item, float moveSpeed)
{
    float frame = item->guideFrame - this->startLength;
    QPointF end((posX + radiusX + radiusX * cos(startAngleRad + (frame * stepAngleRad))) * scaleX,
                (posY + radiusY + radiusY * sin(startAngleRad + (frame * stepAngleRad))) * scaleY);
    this->move(item, end, moveSpeed);
}
