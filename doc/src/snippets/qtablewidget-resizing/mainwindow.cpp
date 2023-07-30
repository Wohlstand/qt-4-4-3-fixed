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

#include <QtGui>

#include "mainwindow.h"

MainWindow::MainWindow()
{
    QMenu *fileMenu = new QMenu(tr("&File"));

    QAction *quitAction = fileMenu->addAction(tr("E&xit"));
    quitAction->setShortcut(tr("Ctrl+Q"));

    QMenu *tableMenu = new QMenu(tr("&Table"));

    QAction *tableWidthAction = tableMenu->addAction(tr("Change Table &Width"));
    QAction *tableHeightAction = tableMenu->addAction(tr("Change Table &Height"));

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(tableMenu);

//! [0]
    tableWidget = new QTableWidget(this);
//! [0]
    tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));
    connect(tableWidthAction, SIGNAL(triggered()), this, SLOT(changeWidth()));
    connect(tableHeightAction, SIGNAL(triggered()), this, SLOT(changeHeight()));

    setupTableItems();

    setCentralWidget(tableWidget);
    setWindowTitle(tr("Table Widget Resizing"));
}

void MainWindow::setupTableItems()
{
//! [1]
    tableWidget->setRowCount(10);
    tableWidget->setColumnCount(5);
//! [1]

    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        for (int column = 0; column < tableWidget->columnCount(); ++column) {
//! [2]
    QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg(
        (row+1)*(column+1)));
    tableWidget->setItem(row, column, newItem);
//! [2]
        }
    }
}

void MainWindow::changeWidth()
{
    bool ok;

    int newWidth = QInputDialog::getInteger(this, tr("Change table width"),
        tr("Input the number of columns required (1-20):"),
        tableWidget->columnCount(), 1, 20, 1, &ok);

    if (ok)
        tableWidget->setColumnCount(newWidth);
}

void MainWindow::changeHeight()
{
    bool ok;

    int newHeight = QInputDialog::getInteger(this, tr("Change table height"),
        tr("Input the number of rows required (1-20):"),
        tableWidget->rowCount(), 1, 20, 1, &ok);

    if (ok)
        tableWidget->setRowCount(newHeight);
}
