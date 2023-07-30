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
  window.cpp

  A minimal subclass of QTableView with slots to allow the selection model
  to be monitored.
*/

#include <QAbstractItemModel>
#include <QItemSelection>
#include <QItemSelectionModel>
#include <QStatusBar>

#include "model.h"
#include "window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Selected items in a table model");

    model = new TableModel(8, 4, this);

    table = new QTableView(this);
    table->setModel(model);

    selectionModel = table->selectionModel();
    connect(selectionModel,
        SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
        this, SLOT(updateSelection(const QItemSelection &, const QItemSelection &)));
    connect(selectionModel,
        SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
        this, SLOT(changeCurrent(const QModelIndex &, const QModelIndex &)));

    statusBar();
    setCentralWidget(table);
}

//! [0]
void MainWindow::updateSelection(const QItemSelection &selected,
    const QItemSelection &deselected)
{
    QModelIndex index;
    QModelIndexList items = selected.indexes();

    foreach (index, items) {
        QString text = QString("(%1,%2)").arg(index.row()).arg(index.column());
        model->setData(index, text);
//! [0] //! [1]
    }
//! [1]

//! [2]
    items = deselected.indexes();

    foreach (index, items)
        model->setData(index, "");
}
//! [2]

//! [3]
void MainWindow::changeCurrent(const QModelIndex &current,
    const QModelIndex &previous)
{
    statusBar()->showMessage(
        tr("Moved from (%1,%2) to (%3,%4)")
            .arg(previous.row()).arg(previous.column())
            .arg(current.row()).arg(current.column()));
}
//! [3]
