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

#include "draglabel.h"

//! [0]
DragLabel::DragLabel(const QString &text, QWidget *parent)
    : QLabel(parent)
{
    QFontMetrics metric(font());
    QSize size = metric.size(Qt::TextSingleLine, text);

    QImage image(size.width() + 12, size.height() + 12,
                 QImage::Format_ARGB32_Premultiplied);
    image.fill(qRgba(0, 0, 0, 0));

    QFont font;
    font.setStyleStrategy(QFont::ForceOutline);
//! [0]

//! [1]
    QPainter painter;
    painter.begin(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::white);
    painter.drawRoundedRect(QRectF(0.5, 0.5, image.width()-1, image.height()-1),
                            25, 25, Qt::RelativeSize);

    painter.setFont(font);
    painter.setBrush(Qt::black);
    painter.drawText(QRect(QPoint(6, 6), size), Qt::AlignCenter, text);
    painter.end();
//! [1]

//! [2]
    setPixmap(QPixmap::fromImage(image));
    labelText = text;
}
//! [2]

//! [3]
void DragLabel::mousePressEvent(QMouseEvent *event)
//! [3]
{
//! [4]
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << labelText << QPoint(event->pos() - rect().topLeft());
//! [4]

//! [5]
    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-fridgemagnet", itemData);
    mimeData->setText(labelText);
//! [5]

//! [6]
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(event->pos() - rect().topLeft());
    drag->setPixmap(*pixmap());

    hide();
//! [6]

//! [7]
    if (drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction) == Qt::MoveAction)
        close();
    else
        show();
}
//! [7]
