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

#include "guideline.h"
#include <cmath>

GuideLine::GuideLine(const QLineF &line, Guide *follows) : Guide(follows)
{
    this->line = line;
}

GuideLine::GuideLine(const QPointF &end, Guide *follows) : Guide(follows)
{
    if (follows)
        this->line = QLineF(prevGuide->endPos(), end);
    else
        this->line = QLineF(QPointF(0, 0), end);
}

float GuideLine::length()
{
    return line.length();
}

QPointF GuideLine::startPos()
{
    return QPointF(this->line.p1().x() * scaleX, this->line.p1().y() * scaleY);
}

QPointF GuideLine::endPos()
{
    return QPointF(this->line.p2().x() * scaleX, this->line.p2().y() * scaleY);
}

void GuideLine::guide(DemoItem *item, float moveSpeed)
{
    float frame = item->guideFrame - this->startLength;
    float endX = (this->line.p1().x() + (frame * this->line.dx() / this->length())) * scaleX;
    float endY = (this->line.p1().y() + (frame * this->line.dy() / this->length())) * scaleY;
    QPointF pos(endX, endY);
    this->move(item, pos, moveSpeed);
}

