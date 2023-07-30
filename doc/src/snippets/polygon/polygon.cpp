/****************************************************************************
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the documentation of the Qt Toolkit.
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

int main()
{
    {
        // STREAM
//! [0]
        QPolygon polygon;
        polygon << QPoint(10, 20) << QPoint(20, 30);
//! [0]
    }

    {
        // STREAMF
//! [1]
        QPolygonF polygon;
        polygon << QPointF(10.4, 20.5) << QPointF(20.2, 30.2);
//! [1]
    }

    {
        // SETPOINTS
//! [2]
        static const int points[] = { 10, 20, 30, 40 };
        QPolygon polygon;
        polygon.setPoints(2, points);
//! [2]
    }

    {
        // SETPOINTS2
//! [3]
        QPolygon polygon;
        polygon.setPoints(2, 10, 20, 30, 40);
//! [3]
    }

    {
        // PUTPOINTS
//! [4]
        QPolygon polygon(1);
        polygon[0] = QPoint(4, 5);
        polygon.putPoints(1, 2, 6,7, 8,9);
//! [4]
    }

    {
        // PUTPOINTS2
//! [5]
        QPolygon polygon(3);
        polygon.putPoints(0, 3, 4,5, 0,0, 8,9);
        polygon.putPoints(1, 1, 6,7);
//! [5]
    }

    {
        // PUTPOINTS3
//! [6]
        QPolygon polygon1;
        polygon1.putPoints(0, 3, 1,2, 0,0, 5,6);
        // polygon1 is now the three-point polygon(1,2, 0,0, 5,6);

        QPolygon polygon2;
        polygon2.putPoints(0, 3, 4,4, 5,5, 6,6);
        // polygon2 is now (4,4, 5,5, 6,6);

        polygon1.putPoints(2, 3, polygon2);
        // polygon1 is now the five-point polygon(1,2, 0,0, 4,4, 5,5, 6,6);
//! [6]
    }
    return 0;
}
