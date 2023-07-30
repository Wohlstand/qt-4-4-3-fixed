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
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>

#include "model.h"
#include "window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Selected Items in a Table Model");

    model = new TableModel(8, 4, this);

    table = new QTableView(this);
    table->setModel(model);

    QMenu *actionMenu = new QMenu(tr("&Actions"), this);
    QAction *fillAction = actionMenu->addAction(tr("&Fill Selection"));
    QAction *clearAction = actionMenu->addAction(tr("&Clear Selection"));
    QAction *selectAllAction = actionMenu->addAction(tr("&Select All"));
    menuBar()->addMenu(actionMenu);

    connect(fillAction, SIGNAL(triggered()), this, SLOT(fillSelection()));
    connect(clearAction, SIGNAL(triggered()), this, SLOT(clearSelection()));
    connect(selectAllAction, SIGNAL(triggered()), this, SLOT(selectAll()));

    selectionModel = table->selectionModel();

    statusBar();
    setCentralWidget(table);
}

void MainWindow::fillSelection()
{
//! [0]
    QModelIndexList indexes = selectionModel->selectedIndexes();
    QModelIndex index;

    foreach(index, indexes) {
        QString text = QString("(%1,%2)").arg(index.row()).arg(index.column());
        model->setData(index, text);
    }
//! [0]
}

void MainWindow::clearSelection()
{
    QModelIndexList indexes = selectionModel->selectedIndexes();
    QModelIndex index;

    foreach(index, indexes)
        model->setData(index, "");
}

void MainWindow::selectAll()
{
//! [1]
    QModelIndex parent = QModelIndex();
//! [1] //! [2]
    QModelIndex topLeft = model->index(0, 0, parent);
    QModelIndex bottomRight = model->index(model->rowCount(parent)-1,
        model->columnCount(parent)-1, parent);
//! [2]

//! [3]
    QItemSelection selection(topLeft, bottomRight);
    selectionModel->select(selection, QItemSelectionModel::Select);
//! [3]
}
