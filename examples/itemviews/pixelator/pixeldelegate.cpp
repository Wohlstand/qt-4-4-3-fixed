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

#include "pixeldelegate.h"

//! [0]
PixelDelegate::PixelDelegate(QObject *parent)
    : QAbstractItemDelegate(parent)
{
    pixelSize = 12;
}
//! [0]

//! [1]
void PixelDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
{
//! [2]
    if (option.state & QStyle::State_Selected)
        painter->fillRect(option.rect, option.palette.highlight());
//! [1]

//! [3]
    int size = qMin(option.rect.width(), option.rect.height());
//! [3] //! [4]
    int brightness = index.model()->data(index, Qt::DisplayRole).toInt();
    double radius = (size/2.0) - (brightness/255.0 * size/2.0);
    if (radius == 0.0)
        return;
//! [4]

//! [5]
    painter->save();
//! [5] //! [6]
    painter->setRenderHint(QPainter::Antialiasing, true);
//! [6] //! [7]
    painter->setPen(Qt::NoPen);
//! [7] //! [8]
    if (option.state & QStyle::State_Selected)
//! [8] //! [9]
        painter->setBrush(option.palette.highlightedText());
    else
//! [2]
        painter->setBrush(QBrush(Qt::black));
//! [9]

//! [10]
    painter->drawEllipse(QRectF(option.rect.x() + option.rect.width()/2 - radius,
                                option.rect.y() + option.rect.height()/2 - radius,
                                2*radius, 2*radius));
    painter->restore();
}
//! [10]

//! [11]
QSize PixelDelegate::sizeHint(const QStyleOptionViewItem & /* option */,
                              const QModelIndex & /* index */) const
{
    return QSize(pixelSize, pixelSize);
}
//! [11]

//! [12]
void PixelDelegate::setPixelSize(int size)
{
    pixelSize = size;
}
//! [12]
