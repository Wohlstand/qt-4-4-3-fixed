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

    QMenu *itemsMenu = new QMenu(tr("&Items"));

    insertAction = itemsMenu->addAction(tr("&Insert Item"));
    removeAction = itemsMenu->addAction(tr("&Remove Item"));
    removeAction->setEnabled(false);
    itemsMenu->addSeparator();
    ascendingAction = itemsMenu->addAction(tr("Sort in &Ascending Order"));
    descendingAction = itemsMenu->addAction(tr("Sort in &Descending Order"));
    autoSortAction = itemsMenu->addAction(tr("&Automatically Sort Items"));
    autoSortAction->setCheckable(true);
    itemsMenu->addSeparator();
    QAction *findItemsAction = itemsMenu->addAction(tr("&Find Items"));
    findItemsAction->setShortcut(tr("Ctrl+F"));

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(itemsMenu);

/*  For convenient quoting:
    QTreeWidget *treeWidget = new QTreeWidget(this);
*/
    treeWidget = new QTreeWidget(this);
    treeWidget->setColumnCount(2);
    QStringList headers;
    headers << tr("Subject") << tr("Default");
    treeWidget->setHeaderLabels(headers);

    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));
    connect(ascendingAction, SIGNAL(triggered()), this, SLOT(sortAscending()));
    connect(autoSortAction, SIGNAL(triggered()), this, SLOT(updateSortItems()));
    connect(descendingAction, SIGNAL(triggered()), this, SLOT(sortDescending()));
    connect(findItemsAction, SIGNAL(triggered()), this, SLOT(findItems()));
    connect(insertAction, SIGNAL(triggered()), this, SLOT(insertItem()));
    connect(removeAction, SIGNAL(triggered()), this, SLOT(removeItem()));
    connect(treeWidget,
            SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)),
            this, SLOT(updateMenus(QTreeWidgetItem *)));

    setupTreeItems();
    updateMenus(treeWidget->currentItem());

    setCentralWidget(treeWidget);
    setWindowTitle(tr("Tree Widget"));
}

void MainWindow::setupTreeItems()
{
    QTreeWidgetItem *planets = new QTreeWidgetItem(treeWidget);
    planets->setText(0, tr("Planets"));
    (new QTreeWidgetItem(planets))->setText(0, tr("Mercury"));
    (new QTreeWidgetItem(planets))->setText(0, tr("Venus"));

    QTreeWidgetItem *earthItem = new QTreeWidgetItem(planets);
    earthItem->setText(0, tr("Earth"));
    earthItem->setText(1, tr("Yes"));

    (new QTreeWidgetItem(planets))->setText(0, tr("Mars"));
    (new QTreeWidgetItem(planets))->setText(0, tr("Jupiter"));
    (new QTreeWidgetItem(planets))->setText(0, tr("Saturn"));
    (new QTreeWidgetItem(planets))->setText(0, tr("Uranus"));
    (new QTreeWidgetItem(planets))->setText(0, tr("Neptune"));
    (new QTreeWidgetItem(planets))->setText(0, tr("Pluto"));
}

void MainWindow::findItems()
{
    QString itemText = QInputDialog::getText(this, tr("Find Items"),
        tr("Text to find:"));

    if (itemText.isEmpty())
        return;

//! [0]
    QTreeWidgetItemIterator it(treeWidget);
    while (*it) {
        if ((*it)->text(0) == itemText)
            (*it)->setSelected(true);
        ++it;
    }
//! [0]
}

void MainWindow::insertItem()
{
    QTreeWidgetItem *currentItem = treeWidget->currentItem();

    if (!currentItem)
        return;

    QString itemText = QInputDialog::getText(this, tr("Insert Item"),
        tr("Input text for the new item:"));

    if (itemText.isEmpty())
        return;

    QTreeWidgetItem *parent = currentItem->parent();
    QTreeWidgetItem *newItem;
    if (parent)
        newItem = new QTreeWidgetItem(parent, treeWidget->currentItem());
    else
        newItem = new QTreeWidgetItem(treeWidget, treeWidget->currentItem());

    newItem->setText(0, itemText);
}

void MainWindow::removeItem()
{
    QTreeWidgetItem *currentItem = treeWidget->currentItem();

    if (!currentItem)
        return;

    QTreeWidgetItem *parent = currentItem->parent();
    int index;

    if (parent) {
        index = parent->indexOfChild(treeWidget->currentItem());
        delete parent->takeChild(index);
    } else {
        index = treeWidget->indexOfTopLevelItem(treeWidget->currentItem());
        delete treeWidget->takeTopLevelItem(index);
    }
}

void MainWindow::sortAscending()
{
    treeWidget->sortItems(0, Qt::AscendingOrder);
}

void MainWindow::sortDescending()
{
    treeWidget->sortItems(0, Qt::DescendingOrder);
}

void MainWindow::updateMenus(QTreeWidgetItem *current)
{
    insertAction->setEnabled(current != 0);
    removeAction->setEnabled(current != 0);
}

void MainWindow::updateSortItems()
{
    ascendingAction->setEnabled(!autoSortAction->isChecked());
    descendingAction->setEnabled(!autoSortAction->isChecked());

    treeWidget->setSortingEnabled(autoSortAction->isChecked());
}
