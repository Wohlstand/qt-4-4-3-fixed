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
#include "letteritem.h"
#include "colors.h"

LetterItem::LetterItem(char letter, QGraphicsScene *scene, QGraphicsItem *parent) : DemoItem(scene, parent), letter(letter)
{
    useSharedImage(QString(__FILE__) + letter);
}

LetterItem::~LetterItem()
{
}

QImage *LetterItem::createImage(const QMatrix &matrix) const
{
    QRect scaledRect = matrix.mapRect(QRect(0, 0, 25, 25));
    QImage *image = new QImage(scaledRect.width(), scaledRect.height(), QImage::Format_ARGB32_Premultiplied);
    image->fill(0);
    QPainter painter(image);
    painter.scale(matrix.m11(), matrix.m22());
    painter.setRenderHints(QPainter::TextAntialiasing | QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setPen(Qt::NoPen);
    if (Colors::useEightBitPalette){
        painter.setBrush(QColor(102, 175, 54));
        painter.drawEllipse(0, 0, 25, 25);
        painter.setFont(Colors::tickerFont());
        painter.setPen(QColor(255, 255, 255));
        painter.drawText(10, 15, QString(this->letter));
    }
    else {
        QLinearGradient brush(0, 0, 0, 25);
        brush.setSpread(QLinearGradient::PadSpread);
        brush.setColorAt(0.0, QColor(102, 175, 54, 200));
        brush.setColorAt(1.0, QColor(102, 175, 54, 60));
        painter.setBrush(brush);
        painter.drawEllipse(0, 0, 25, 25);
        painter.setFont(Colors::tickerFont());
        painter.setPen(QColor(255, 255, 255, 255));
        painter.drawText(10, 15, QString(this->letter));
    }
    return image;
}


