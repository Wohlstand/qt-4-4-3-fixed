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

/****************************************************************************
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of an example program for Qt.
** EDITIONS: NOLIMITS
**
****************************************************************************/

#include <QtGui>

#include "dragdropmodel.h"

DragDropModel::DragDropModel(const QStringList &strings, QObject *parent)
    : TreeModel(strings, parent)
{
}

bool DragDropModel::dropMimeData(const QMimeData *data,
    Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    if (action == Qt::IgnoreAction)
        return true;

    if (!data->hasFormat("text/plain"))
        return false;

    int beginRow;

    if (row != -1)
        beginRow = row;
    else if (parent.isValid())
        beginRow = 0;
    else
        beginRow = rowCount(QModelIndex());

    QByteArray encodedData = data->data("text/plain");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QHash<qint64, QMap<int,QHash<int,QString> > > newItems;

    while (!stream.atEnd()) {
        qint64 id;
        int row;
        int column;
        QString text;
        stream >> id >> row >> column >> text;
        newItems[id][row][column] = text;
    }
    int rows = newItems.count();

    insertRows(beginRow, rows, parent);
    QMap<int,QHash<int,QString> > childItems;
    foreach (childItems, newItems.values()) {
        QHash<int,QString> rowItems;
        foreach (rowItems, childItems.values()) {
            foreach (int column, rowItems.keys()) {
                QModelIndex idx = index(beginRow, column, parent);
                setData(idx, rowItems[column]);
            }
            ++beginRow;
        }
    }

    return true;
}

Qt::ItemFlags DragDropModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = TreeModel::flags(index);

    if (index.isValid())
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
    else
        return Qt::ItemIsDropEnabled | defaultFlags;
}

QMimeData *DragDropModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    foreach (QModelIndex index, indexes) {
        if (index.isValid()) {
            QString text = data(index, Qt::DisplayRole).toString();
            stream << index.internalId() << index.row() << index.column() << text;
        }
    }

    mimeData->setData("text/plain", encodedData);
    return mimeData;
}

QStringList DragDropModel::mimeTypes() const
{
    QStringList types;
    types << "text/plain";
    return types;
}

Qt::DropActions DragDropModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}
