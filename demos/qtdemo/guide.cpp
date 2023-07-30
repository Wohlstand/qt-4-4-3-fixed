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

#include <cmath>
#include "guide.h"
#include "colors.h"

Guide::Guide(Guide *follows)
{
    this->scaleX = 1.0;
    this->scaleY = 1.0;

    if (follows){
        while (follows->nextGuide != follows->firstGuide) // append to end
            follows = follows->nextGuide;

        follows->nextGuide = this;
        this->prevGuide = follows;
        this->firstGuide = follows->firstGuide;
        this->nextGuide = follows->firstGuide;
        this->startLength = int(follows->startLength + follows->length()) + 1;
    }
    else{
        this->prevGuide = this;
        this->firstGuide = this;
        this->nextGuide = this;
        this->startLength = 0;
    }
}

void Guide::setScale(float scaleX, float scaleY, bool all)
{
    this->scaleX = scaleX;
    this->scaleY = scaleY;

    if (all){
        Guide *next = this->nextGuide;
        while(next != this){
            next->scaleX = scaleX;
            next->scaleY = scaleY;
            next = next->nextGuide;
        }
    }
}

void Guide::setFence(const QRectF &fence, bool all)
{
    this->fence = fence;

    if (all){
        Guide *next = this->nextGuide;
        while(next != this){
            next->fence = fence;
            next = next->nextGuide;
        }
    }
}

Guide::~Guide()
{
    if (this != this->nextGuide && this->nextGuide != this->firstGuide)
        delete this->nextGuide;
}

float Guide::lengthAll()
{
    float len = length();
    Guide *next = this->nextGuide;
    while(next != this){
        len += next->length();
        next = next->nextGuide;
    }
    return len;
}

void Guide::move(DemoItem *item, QPointF &dest, float moveSpeed)
{
    QLineF walkLine(item->getGuidedPos(), dest);
    if (moveSpeed >= 0 && walkLine.length() > moveSpeed){
        // The item is too far away from it's destination point.
        // So we choose to move it towards it instead.
        float dx = walkLine.dx();
        float dy = walkLine.dy();

        if (qAbs(dx) > qAbs(dy)){
            // walk along x-axis
            if (dx != 0){
                float d = moveSpeed * dy / qAbs(dx);
                float s = dx > 0 ? moveSpeed : -moveSpeed;
                dest.setX(item->getGuidedPos().x() + s);
                dest.setY(item->getGuidedPos().y() + d);
            }
        }
        else{
            // walk along y-axis
            if (dy != 0){
                float d = moveSpeed * dx / qAbs(dy);
                float s = dy > 0 ? moveSpeed : -moveSpeed;
                dest.setX(item->getGuidedPos().x() + d);
                dest.setY(item->getGuidedPos().y() + s);
            }
        }
    }

    item->setGuidedPos(dest);
}
