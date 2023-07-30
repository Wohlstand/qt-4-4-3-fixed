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

    fileMenu->addAction(tr("&Open..."), this, SLOT(openFile()),
                        QKeySequence(tr("Ctrl+O", "File|Open")));

    printAction = fileMenu->addAction(tr("&Print..."), this, SLOT(printFile()));
    printAction->setEnabled(false);

    pdfPrintAction = fileMenu->addAction(tr("Print as P&DF..."), this, SLOT(printPdf()));
    pdfPrintAction->setEnabled(false);

    fileMenu->addAction(tr("E&xit"), this, SLOT(close()),
                        QKeySequence(tr("Ctrl+Q", "File|Exit")));

    menuBar()->addMenu(fileMenu);

    editor = new QTextEdit(this);
    document = new QTextDocument(this);
    editor->setDocument(document);

    connect(editor, SIGNAL(selectionChanged()), this, SLOT(updateMenus()));

    setCentralWidget(editor);
    setWindowTitle(tr("Text Document Writer"));
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open file"), currentFile, "HTML files (*.html);;Text files (*.txt)");
    
    if (!fileName.isEmpty()) {
        QFileInfo info(fileName);
        if (info.completeSuffix() == "html") {
            QFile file(fileName);
            
            if (file.open(QIODevice::ReadOnly)) {
                editor->setHtml(file.readAll());
                file.close();
                currentFile = fileName;
            }
        } else if (info.completeSuffix() == "txt") {
            QFile file(fileName);
            
            if (file.open(QIODevice::ReadOnly)) {
                editor->setPlainText(file.readAll());
                file.close();
                currentFile = fileName;
            }
        }
        printAction->setEnabled(true);
        pdfPrintAction->setEnabled(true);
    }
}

void MainWindow::printFile()
{
//! [0]
    QTextDocument *document = editor->document();
    QPrinter printer;

    QPrintDialog *dlg = new QPrintDialog(&printer, this);
    if (dlg->exec() != QDialog::Accepted)
        return;

    document->print(&printer);
//! [0]
}

void MainWindow::printPdf()
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);

    QPrintDialog *printDialog = new QPrintDialog(&printer, this);
    if (printDialog->exec() == QDialog::Accepted)
        editor->document()->print(&printer);
}
