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
    QWidget anyPaintDevice;
    {
        // PEN SNIPPET
        QPainter painter;
        QPen pen(Qt::red, 2);                 // red solid line, 2 pixels wide
        painter.begin(&anyPaintDevice);   // paint something
        painter.setPen(pen);              // set the red, wide pen
        painter.drawRect(40,30, 200,100); // draw a rectangle
        painter.setPen(Qt::blue);             // set blue pen, 0 pixel width
        painter.drawLine(40,30, 240,130); // draw a diagonal in rectangle
        painter.end();                    // painting done
    }

    {
        // BRUSH SNIPPET
        QPainter painter;
        QBrush brush(Qt::yellow);           // yellow solid pattern
        painter.begin(&anyPaintDevice);   // paint something
        painter.setBrush(brush);          // set the yellow brush
        painter.setPen(Qt::NoPen);        // do not draw outline
        painter.drawRect(40,30, 200,100); // draw filled rectangle
        painter.setBrush(Qt::NoBrush);    // do not fill
        painter.setPen(Qt::black);            // set black pen, 0 pixel width
        painter.drawRect(10,10, 30,20);   // draw rectangle outline
        painter.end();                    // painting done
    }

    // LINEAR
//! [0]
    QLinearGradient linearGrad(QPointF(100, 100), QPointF(200, 200));
    linearGrad.setColorAt(0, Qt::black);
    linearGrad.setColorAt(1, Qt::white);
//! [0]

    // RADIAL
//! [1]
    QRadialGradient radialGrad(QPointF(100, 100), 100);
    radialGrad.setColorAt(0, Qt::red);
    radialGrad.setColorAt(0.5, Qt::blue);
    radialGrad.setColorAt(1, Qt::green);
//! [1]
}
