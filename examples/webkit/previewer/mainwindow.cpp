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
#include <QtWebKit>
#include "mainwindow.h"

//! [0]
MainWindow::MainWindow()
{
    createActions();
    createMenus();
    centralWidget = new Previewer(this);
    setCentralWidget(centralWidget);

    connect(centralWidget->webView, SIGNAL(loadFinished(bool)),
        this, SLOT(updateTextEdit()));
    setStartupText();
}
//! [0]

//! [1]
void MainWindow::createActions()
{
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcut(tr("Ctrl+O"));
    openAct->setStatusTip(tr("Open an existing HTML file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    openUrlAct = new QAction(tr("&Open URL..."), this);
    openUrlAct->setShortcut(tr("Ctrl+U"));
    openUrlAct->setStatusTip(tr("Open a URL"));
    connect(openUrlAct, SIGNAL(triggered()), this, SLOT(openUrl()));
//! [1]

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcut(tr("Ctrl+S"));
    saveAct->setStatusTip(tr("Save the HTML file to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setStatusTip(tr("Exit the application"));
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

//! [2]
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(openUrlAct);
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}
//! [2]

//! [3]
void MainWindow::about()
{
    QMessageBox::about(this, tr("About Application"),
        tr("The <b>Application</b> example demonstrates how to "
           "view HTML documents using a QWebView."));
}
//! [3]

//! [4]
void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()) {
        // read from file
        QFile file(fileName);

        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            return;
        }

        QTextStream out(&file);
        QString output = out.readAll();

        // display contents
        centralWidget->plainTextEdit->setPlainText(output);
    }
}
//! [4]

//! [5]
void MainWindow::openUrl()
{
    bool ok;
    QString url = QInputDialog::getText(this, tr("Enter a URL"),
                  tr("URL:"), QLineEdit::Normal, "http://", &ok);

    if (ok && !url.isEmpty()) {
        centralWidget->webView->setUrl(url);
    }
}
//! [5]

//! [6]
void MainWindow::save()
{
    QString content = centralWidget->plainTextEdit->toPlainText();
    QString fileName = QFileDialog::getSaveFileName(this);

    if (!fileName.isEmpty()) {
        // save to file
        QFile file(fileName);

        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            return;
        }

        QTextStream in(&file);
        in << content;
    }
}
//! [6]

//! [7]
void MainWindow::updateTextEdit()
{
    QWebFrame *mainFrame = centralWidget->webView->page()->mainFrame();
    QString frameText = mainFrame->toHtml();
    centralWidget->plainTextEdit->setPlainText(frameText);
}
//! [7]

//! [8]
void MainWindow::setStartupText()
{
    QString string = "<html><body><h1>HTML Previewer</h1>"
                     " <p>This example shows you how to use QWebView to"
                     " preview HTML data written in a QPlainTextEdit.</p>"
                     " </body></html>";
    centralWidget->webView->setHtml(string);
}
//! [8]
