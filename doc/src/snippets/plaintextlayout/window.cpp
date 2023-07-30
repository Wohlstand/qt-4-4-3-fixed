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
#include <math.h>

#include "window.h"

Window::Window(QWidget *parent)
    : QWidget(parent)
{
    text = QString("Support for text rendering and layout in Qt 4 has been "
                   "redesigned around a system that allows textual content to "
                   "be represented in a more flexible way than was possible "
                   "with Qt 3. Qt 4 also provides a more convenient "
                   "programming interface for editing documents. These "
                   "improvements are made available through a reimplementation "
                   "of the existing text rendering engine, and the "
                   "introduction of several new classes. "
                   "See the relevant module overview for a detailed discussion "
                   "of this framework. The following sections provide a brief "
                   "overview of the main concepts behind Scribe.");

    setWindowTitle(tr("Plain Text Layout"));
}

void Window::paintEvent(QPaintEvent *event)
{
//! [0]
    QTextLayout textLayout(text, font);
    qreal margin = 10;
    qreal radius = qMin(width()/2.0, height()/2.0) - margin;
    QFontMetrics fm(font);

    qreal lineHeight = fm.height();
    qreal y = 0;

    textLayout.beginLayout();

    while (1) {
        // create a new line 
        QTextLine line = textLayout.createLine();
        if (!line.isValid())
            break;

        qreal x1 = qMax(0.0, pow(pow(radius,2)-pow(radius-y,2), 0.5));
        qreal x2 = qMax(0.0, pow(pow(radius,2)-pow(radius-(y+lineHeight),2), 0.5));
        qreal x = qMax(x1, x2) + margin;
        qreal lineWidth = (width() - margin) - x;

        line.setLineWidth(lineWidth);
        line.setPosition(QPointF(x, margin+y));
        y += line.height();
    }

    textLayout.endLayout();

    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Qt::white);
    painter.setBrush(QBrush(Qt::black));
    painter.setPen(QPen(Qt::black));
    textLayout.draw(&painter, QPoint(0,0));

    painter.setBrush(QBrush(QColor("#a6ce39")));
    painter.setPen(QPen(Qt::black));
    painter.drawEllipse(QRectF(-radius, margin, 2*radius, 2*radius));
    painter.end();
//! [0]
}
