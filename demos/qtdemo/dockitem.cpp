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

#include "dockitem.h"
#include "colors.h"

DockItem::DockItem(ORIENTATION orien, qreal x, qreal y, qreal width, qreal length, QGraphicsScene *scene, QGraphicsItem *parent)
    : DemoItem(scene, parent)
{
    this->orientation = orien;
    this->width = width;
    this->length = length;
    this->setPos(x, y);
    this->setZValue(40);
    this->setupPixmap();
}

void DockItem::setupPixmap()
{
    this->pixmap = new QPixmap(int(this->boundingRect().width()), int(this->boundingRect().height()));
    this->pixmap->fill(QColor(0, 0, 0, 0));
    QPainter painter(this->pixmap);
    // create brush:
    QColor background = Colors::sceneBg1;
    QLinearGradient brush(0, 0, 0, this->boundingRect().height());
    brush.setSpread(QGradient::PadSpread);

    if (this->orientation == DOWN){
        brush.setColorAt(0.0, background);
        brush.setColorAt(0.2, background);
        background.setAlpha(0);
        brush.setColorAt(1.0, background);
    }
    else
        if (this->orientation == UP){
        brush.setColorAt(1.0, background);
        brush.setColorAt(0.8, background);
        background.setAlpha(0);
        brush.setColorAt(0.0, background);
    }
    else
        qWarning("DockItem doesn't support the orientation given!");

    painter.fillRect(0, 0, int(this->boundingRect().width()), int(this->boundingRect().height()), brush);

}

DockItem::~DockItem()
{
    delete this->pixmap;
}

QRectF DockItem::boundingRect() const
{
    if (this->orientation == UP || this->orientation == DOWN)
        return QRectF(0, 0, this->length, this->width);
    else
        return QRectF(0, 0, this->width, this->length);
}

void DockItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->drawPixmap(0, 0, *this->pixmap);
}



