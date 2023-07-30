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
#include "xmlwriter.h"

MainWindow::MainWindow()
{
    QMenu *fileMenu = new QMenu(tr("&File"));

    QAction *saveAction = fileMenu->addAction(tr("&Save..."));
    saveAction->setShortcut(tr("Ctrl+S"));

    QAction *quitAction = fileMenu->addAction(tr("E&xit"));
    quitAction->setShortcut(tr("Ctrl+Q"));

    QMenu *insertMenu = new QMenu(tr("&Insert"));

    QAction *calendarAction = insertMenu->addAction(tr("&Calendar"));
    calendarAction->setShortcut(tr("Ctrl+I"));

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(insertMenu);

//! [0]
    editor = new QTextEdit(this);
//! [0]

    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));
    connect(calendarAction, SIGNAL(triggered()), this, SLOT(insertCalendar()));

    setCentralWidget(editor);
    setWindowTitle(tr("Text Document Writer"));
}

void MainWindow::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save document as:"), "", tr("XML (*.xml)"));

    if (!fileName.isEmpty()) {
        if (writeXml(fileName))
            setWindowTitle(fileName);
        else
            QMessageBox::warning(this, tr("Warning"),
                tr("Failed to save the document."), QMessageBox::Cancel,
                QMessageBox::NoButton);
    }
}

void MainWindow::insertCalendar()
{
//! [1]
    QTextCursor cursor(editor->textCursor());
    cursor.movePosition(QTextCursor::Start); 

    QTextCharFormat format(cursor.charFormat());
    format.setFontFamily("Courier");
    
    QTextCharFormat boldFormat = format;
    boldFormat.setFontWeight(QFont::Bold);

    cursor.insertBlock();
    cursor.insertText(" ", boldFormat);

    QDate date = QDate::currentDate();
    int year = date.year(), month = date.month();

    for (int weekDay = 1; weekDay <= 7; ++weekDay) {
        cursor.insertText(QString("%1 ").arg(QDate::shortDayName(weekDay), 3),
            boldFormat);
    }

    cursor.insertBlock();
    cursor.insertText(" ", format);

    for (int column = 1; column < QDate(year, month, 1).dayOfWeek(); ++column) {
        cursor.insertText("    ", format);
    }

    for (int day = 1; day <= date.daysInMonth(); ++day) {
//! [1] //! [2]
        int weekDay = QDate(year, month, day).dayOfWeek();

        if (QDate(year, month, day) == date)
            cursor.insertText(QString("%1 ").arg(day, 3), boldFormat);
        else
            cursor.insertText(QString("%1 ").arg(day, 3), format);

        if (weekDay == 7) {
            cursor.insertBlock();
            cursor.insertText(" ", format);
        }
//! [2] //! [3]
    }
//! [3]
}

bool MainWindow::writeXml(const QString &fileName)
{
    XmlWriter documentWriter(editor->document());

    QDomDocument *domDocument = documentWriter.toXml();
    QFile file(fileName);

    if (file.open(QFile::WriteOnly)) {
        QTextStream textStream(&file);
        textStream.setCodec(QTextCodec::codecForName("UTF-8"));
        
        textStream << domDocument->toString(1).toUtf8();
        file.close();
        return true;
    }
    else
        return false;
}
