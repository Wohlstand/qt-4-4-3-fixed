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

#ifndef GUIDE_H
#define GUIDE_H

#include "demoitem.h"

class Guide
{
public:
    Guide(Guide *follows = 0);
    virtual ~Guide();

    virtual void guide(DemoItem *item, float moveSpeed) = 0;
    void move(DemoItem *item, QPointF &dest, float moveSpeed);
    virtual QPointF startPos(){ return QPointF(0, 0); };
    virtual QPointF endPos(){ return QPointF(0, 0); };
    virtual float length(){ return 1; };
    float lengthAll();

    void setScale(float scaleX, float scaleY, bool all = true);
    void setFence(const QRectF &fence, bool all = true);

    int startLength;
    Guide *nextGuide;
    Guide *firstGuide;
    Guide *prevGuide;
    float scaleX;
    float scaleY;
    QRectF fence;
};

#endif // GUIDE_H

