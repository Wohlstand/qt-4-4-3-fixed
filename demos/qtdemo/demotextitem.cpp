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

#include "demotextitem.h"
#include "colors.h"

DemoTextItem::DemoTextItem(const QString &text, const QFont &font, const QColor &textColor,
                           float textWidth, QGraphicsScene *scene, QGraphicsItem *parent, TYPE type, const QColor &bgColor)
                           : DemoItem(scene, parent)
{
    this->type = type;
    this->text = text;
    this->font = font;
    this->textColor = textColor;
    this->bgColor = bgColor;
    this->textWidth = textWidth;
    this->noSubPixeling = true;
}

void DemoTextItem::setText(const QString &text)
{
    this->text = text;
    this->update();
}

QImage *DemoTextItem::createImage(const QMatrix &matrix) const
{
    if (this->type == DYNAMIC_TEXT)
        return 0;

    float sx = qMin(matrix.m11(), matrix.m22());
    float sy = matrix.m22() < sx ? sx : matrix.m22();

    QGraphicsTextItem textItem(0, 0);
    textItem.setHtml(this->text);
    textItem.setTextWidth(this->textWidth);
    textItem.setFont(this->font);
    textItem.setDefaultTextColor(this->textColor);

    float w = textItem.boundingRect().width();
    float h = textItem.boundingRect().height();
    QImage *image = new QImage(int(w * sx), int(h * sy), QImage::Format_ARGB32_Premultiplied);
    image->fill(QColor(0, 0, 0, 0).rgba());
    QPainter painter(image);
    painter.scale(sx, sy);
    QStyleOptionGraphicsItem style;
    textItem.paint(&painter, &style, 0);
    return image;
}


void DemoTextItem::animationStarted(int)
{
    this->noSubPixeling = false;
}


void DemoTextItem::animationStopped(int)
{
    this->noSubPixeling = true;
}

QRectF DemoTextItem::boundingRect() const

{
    if (this->type == STATIC_TEXT)
        return DemoItem::boundingRect();
    return QRectF(0, 0, 50, 20); // Sorry for using magic number
}


void DemoTextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (this->type == STATIC_TEXT) {
        DemoItem::paint(painter, option, widget);
        return;
    }

    painter->setPen(this->textColor);
    painter->drawText(0, 0, this->text);
}
