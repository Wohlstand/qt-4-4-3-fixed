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

/*
    treeitem.cpp

    A container for items of data supplied by the simple tree model.
*/

#include <QStringList>

#include "treeitem.h"

//! [0]
TreeItem::TreeItem(const QList<QVariant> &data, TreeItem *parent)
{
    parentItem = parent;
    itemData = data;
}
//! [0]

//! [1]
TreeItem::~TreeItem()
{
    qDeleteAll(childItems);
}
//! [1]

//! [2]
void TreeItem::appendChild(TreeItem *item)
{
    childItems.append(item);
}
//! [2]

//! [3]
TreeItem *TreeItem::child(int row)
{
    return childItems.value(row);
}
//! [3]

//! [4]
int TreeItem::childCount() const
{
    return childItems.count();
}
//! [4]

//! [5]
int TreeItem::columnCount() const
{
    return itemData.count();
}
//! [5]

//! [6]
QVariant TreeItem::data(int column) const
{
    return itemData.value(column);
}
//! [6]

//! [7]
TreeItem *TreeItem::parent()
{
    return parentItem;
}
//! [7]

//! [8]
int TreeItem::row() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}
//! [8]
