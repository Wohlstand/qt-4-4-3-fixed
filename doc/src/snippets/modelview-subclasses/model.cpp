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

/*
  model.cpp

  A simple model that uses a QVector as its data source.
*/

#include "model.h"

/*!
    Returns the number of items in the string list as the number of rows
    in the model.
*/

int LinearModel::rowCount(const QModelIndex &parent) const
{
    Q_USING(parent);

    return values.count();
}

/*
    Returns an appropriate value for the requested data.
    If the view requests an invalid index, an invalid variant is returned.
    If a header is requested then we just return the column or row number,
    depending on the orientation of the header.
    Any valid index that corresponds to a string in the list causes that
    string to be returned.
*/

/*!
    Returns a model index for other component to use when referencing the
    item specified by the given row, column, and type. The parent index
    is ignored.
*/

QModelIndex LinearModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent == QModelIndex() && row >= 0 && row < rowCount()
        && column == 0)
        return createIndex(row, column, 0);
    else
        return QModelIndex();
}

QVariant LinearModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role);

    if (!index.isValid())
        return QVariant();

    return values.at(index.row());
}

/*!
    Returns Qt::ItemIsEditable so that all items in the vector can be edited.
*/

Qt::ItemFlags LinearModel::flags(const QModelIndex &index) const
{
    // all items in the model are editable
    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

/*!
    Changes an item in the string list, but only if the following conditions
    are met:

    * The index supplied is valid.
    * The index corresponds to an item to be shown in a view.
    * The role associated with editing text is specified.

    The dataChanged() signal is emitted if the item is changed.
*/

bool LinearModel::setData(const QModelIndex &index,
                          const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return false;
    values.replace(index.row(), value.toInt());
    emit dataChanged(index, index);
    return true;
}

/*!
    Inserts a number of rows into the model at the specified position.
*/

bool LinearModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    beginInsertRows(parent, position, position + rows - 1);

    values.insert(position, rows, 0);

    endInsertRows();
    return true;
}

/*!
    Removes a number of rows from the model at the specified position.
*/

bool LinearModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    beginRemoveRows(QModelIndex(), position, position+rows-1);

    values.remove(position, rows);

    endRemoveRows();
    return true;
}
