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
#include "model.h"

/*!
    The main window constructor creates and populates a model with values
    from a string list then displays the contents of the model using a
    QListView widget.
*/

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QMenu *fileMenu = new QMenu(tr("&File"));

    QAction *quitAction = fileMenu->addAction(tr("E&xit"));
    quitAction->setShortcut(tr("Ctrl+Q"));

    QMenu *itemsMenu = new QMenu(tr("&Items"));

    insertAction = itemsMenu->addAction(tr("&Insert Item"));
    removeAction = itemsMenu->addAction(tr("&Remove Item"));

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(itemsMenu);

    QStringList numbers;
    numbers << tr("One") << tr("Two") << tr("Three") << tr("Four") << tr("Five")
            << tr("Six") << tr("Seven") << tr("Eight") << tr("Nine") << tr("Ten");

    model = new StringListModel(numbers);
    QListView *view = new QListView(this);
    view->setModel(model);

    selectionModel = view->selectionModel();

    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(insertAction, SIGNAL(triggered()), this, SLOT(insertItem()));
    connect(removeAction, SIGNAL(triggered()), this, SLOT(removeItem()));
    connect(selectionModel,
            SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(updateMenus(const QModelIndex &)));
    
    setCentralWidget(view);
    setWindowTitle("View onto a string list model");
}

void MainWindow::insertItem()
{
    if (!selectionModel->currentIndex().isValid())
        return;

    QString itemText = QInputDialog::getText(this, tr("Insert Item"),
        tr("Input text for the new item:"));

    if (itemText.isNull())
        return;

    int row = selectionModel->currentIndex().row();

    if (model->insertRows(row, 1))
        model->setData(model->index(row, 0), itemText, Qt::EditRole);
}

void MainWindow::removeItem()
{
    if (!selectionModel->currentIndex().isValid())
        return;

    int row = selectionModel->currentIndex().row();

    model->removeRows(row, 1);
}

void MainWindow::updateMenus(const QModelIndex &currentIndex)
{
    insertAction->setEnabled(currentIndex.isValid());
    removeAction->setEnabled(currentIndex.isValid());
}
