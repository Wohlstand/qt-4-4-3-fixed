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

#include "renderarea.h"

//! [0]
RenderArea::RenderArea(QWidget *parent)
    : QWidget(parent)
{
    QFont newFont = font();
    newFont.setPixelSize(12);
    setFont(newFont);

    QFontMetrics fontMetrics(newFont);
    xBoundingRect = fontMetrics.boundingRect(tr("x"));
    yBoundingRect = fontMetrics.boundingRect(tr("y"));
}
//! [0]

//! [1]
void RenderArea::setOperations(const QList<Operation> &operations)
{
    this->operations = operations;
    update();
}
//! [1]

//! [2]
void RenderArea::setShape(const QPainterPath &shape)
{
    this->shape = shape;
    update();
}
//! [2]

//! [3]
QSize RenderArea::minimumSizeHint() const
{
    return QSize(182, 182);
}
//! [3]

//! [4]
QSize RenderArea::sizeHint() const
{
    return QSize(232, 232);
}
//! [4]

//! [5]
void RenderArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(event->rect(), QBrush(Qt::white));

    painter.translate(66, 66);
//! [5]

//! [6]
    painter.save();
    transformPainter(painter);
    drawShape(painter);
    painter.restore();
//! [6]

//! [7]
    drawOutline(painter);
//! [7]

//! [8]
    transformPainter(painter);
    drawCoordinates(painter);
}
//! [8]

//! [9]
void RenderArea::drawCoordinates(QPainter &painter)
{
    painter.setPen(Qt::red);

    painter.drawLine(0, 0, 50, 0);
    painter.drawLine(48, -2, 50, 0);
    painter.drawLine(48, 2, 50, 0);
    painter.drawText(60 - xBoundingRect.width() / 2,
                     0 + xBoundingRect.height() / 2, tr("x"));

    painter.drawLine(0, 0, 0, 50);
    painter.drawLine(-2, 48, 0, 50);
    painter.drawLine(2, 48, 0, 50);
    painter.drawText(0 - yBoundingRect.width() / 2,
                     60 + yBoundingRect.height() / 2, tr("y"));
}
//! [9]

//! [10]
void RenderArea::drawOutline(QPainter &painter)
{
    painter.setPen(Qt::darkGreen);
    painter.setPen(Qt::DashLine);
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(0, 0, 100, 100);
}
//! [10]

//! [11]
void RenderArea::drawShape(QPainter &painter)
{
    painter.fillPath(shape, Qt::blue);
}
//! [11]

//! [12]
void RenderArea::transformPainter(QPainter &painter)
{
    for (int i = 0; i < operations.size(); ++i) {
        switch (operations[i]) {
        case Translate:
            painter.translate(50, 50);
            break;
        case Scale:
            painter.scale(0.75, 0.75);
            break;
        case Rotate:
            painter.rotate(60);
            break;
        case NoTransformation:
        default:
            ;
        }
    }
}
//! [12]
