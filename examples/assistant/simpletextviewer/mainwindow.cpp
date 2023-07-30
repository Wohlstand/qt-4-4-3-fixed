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

#include "mainwindow.h"
#include "findfiledialog.h"

//! [0]
MainWindow::MainWindow()
{
    textViewer = new QTextEdit;
    textViewer->setReadOnly(true);
    QFile file("documentation/intro.html");
    if (file.open(QIODevice::ReadOnly))
        textViewer->setHtml(file.readAll());

    setCentralWidget(textViewer);

    createActions();
    createMenus();

    initializeAssistant();

    setWindowTitle(tr("Simple Text Viewer"));
    resize(750, 400);
}
//! [0]

//! [1]
void MainWindow::closeEvent(QCloseEvent *)
{
    if (assistantClient)
        assistantClient->closeAssistant();
}
//! [1]

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Simple Text Viewer"),
                         tr("This example demonstrates how to use\n" \
                            "Qt Assistant as help system for your\n" \
                            "own application."));
}

//! [2]
void MainWindow::assistant()
{
    assistantClient->showPage(QLibraryInfo::location(QLibraryInfo::ExamplesPath) +
                                              QDir::separator() +
                                              "assistant/simpletextviewer/documentation/index.html");
}
//! [2]

//! [3]
void MainWindow::open()
{
    FindFileDialog dialog(textViewer, assistantClient);
    dialog.exec();
}
//! [3]

void MainWindow::createActions()
{
    assistantAct = new QAction(tr("Help Contents"), this);
    assistantAct->setShortcut(tr("F1"));
    connect(assistantAct, SIGNAL(triggered()), this, SLOT(assistant()));

    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcut(tr("Ctrl+O"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    clearAct = new QAction(tr("&Clear"), this);
    clearAct->setShortcut(tr("Ctrl+C"));
    connect(clearAct, SIGNAL(triggered()), textViewer, SLOT(clear()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus()
{
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addAction(clearAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(assistantAct);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);


    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(helpMenu);
}

//! [4]
void MainWindow::initializeAssistant()
{
    assistantClient = new QAssistantClient(QLibraryInfo::location(QLibraryInfo::BinariesPath), this);

    QStringList arguments;
    arguments << "-profile" << QString("documentation") + QDir::separator() + QString("simpletextviewer.adp");
    assistantClient->setArguments(arguments);
}
//! [4]
