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

#include "stardelegate.h"
#include "stareditor.h"
#include "starrating.h"

//! [0]
void StarDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{
    if (qVariantCanConvert<StarRating>(index.data())) {
        StarRating starRating = qVariantValue<StarRating>(index.data());

        if (option.state & QStyle::State_Selected)
            painter->fillRect(option.rect, option.palette.highlight());

        starRating.paint(painter, option.rect, option.palette,
                         StarRating::ReadOnly);
    } else {
        QItemDelegate::paint(painter, option, index);
    }
//! [0]
}

//! [1]
QSize StarDelegate::sizeHint(const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    if (qVariantCanConvert<StarRating>(index.data())) {
        StarRating starRating = qVariantValue<StarRating>(index.data());
        return starRating.sizeHint();
    } else {
        return QItemDelegate::sizeHint(option, index);
    }
}
//! [1]

//! [2]
QWidget *StarDelegate::createEditor(QWidget *parent,
                                    const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const

{
    if (qVariantCanConvert<StarRating>(index.data())) {
        StarEditor *editor = new StarEditor(parent);
        connect(editor, SIGNAL(editingFinished()),
                this, SLOT(commitAndCloseEditor()));
        return editor;
    } else {
        return QItemDelegate::createEditor(parent, option, index);
    }
}
//! [2]

//! [3]
void StarDelegate::setEditorData(QWidget *editor,
                                 const QModelIndex &index) const
{
    if (qVariantCanConvert<StarRating>(index.data())) {
        StarRating starRating = qVariantValue<StarRating>(index.data());
        StarEditor *starEditor = qobject_cast<StarEditor *>(editor);
        starEditor->setStarRating(starRating);
    } else {
        QItemDelegate::setEditorData(editor, index);
    }
}
//! [3]

//! [4]
void StarDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                const QModelIndex &index) const
{
    if (qVariantCanConvert<StarRating>(index.data())) {
        StarEditor *starEditor = qobject_cast<StarEditor *>(editor);
        model->setData(index, qVariantFromValue(starEditor->starRating()));
    } else {
        QItemDelegate::setModelData(editor, model, index);
    }
}
//! [4]

//! [5]
void StarDelegate::commitAndCloseEditor()
{
    StarEditor *editor = qobject_cast<StarEditor *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}
//! [5]
