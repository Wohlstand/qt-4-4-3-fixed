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

#include <QtUiTools>
#include <QtGui>
#include "textfinder.h"

//! [0]
TextFinder::TextFinder(QWidget *parent)
    : QWidget(parent)
{
    QWidget *formWidget = loadUiFile();

//! [1]
    ui_findButton = qFindChild<QPushButton*>(this, "findButton");
    ui_textEdit = qFindChild<QTextEdit*>(this, "textEdit");
    ui_lineEdit = qFindChild<QLineEdit*>(this, "lineEdit");
//! [0] //! [1]

//! [2]
    QMetaObject::connectSlotsByName(this);
//! [2]

//! [3]
    loadTextFile();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(formWidget);
    setLayout(layout);
    setWindowTitle(tr("Text Finder"));

    isFirstTime = true;
}
//! [3]

//! [4]
QWidget* TextFinder::loadUiFile()
{
    QUiLoader loader;

    QFile file(":/forms/textfinder.ui");
    file.open(QFile::ReadOnly);

    QWidget *formWidget = loader.load(&file, this);
    file.close();

    return formWidget;
}
//! [4]

//! [5]
void TextFinder::loadTextFile()
{
    QFile inputFile(":/forms/input.txt");
    inputFile.open(QIODevice::ReadOnly);
    QTextStream in(&inputFile);
    QString line = in.readAll();
    inputFile.close();

    ui_textEdit->append(line);
    ui_textEdit->setUndoRedoEnabled(false);
    ui_textEdit->setUndoRedoEnabled(true);
}
//! [5]

//! [6] //! [7]
void TextFinder::on_findButton_clicked()
{
    QString searchString = ui_lineEdit->text();
    QTextDocument *document = ui_textEdit->document();

    bool found = false;

    if (isFirstTime == false)
        document->undo();

    if (searchString == "") {
        QMessageBox::information(this, tr("Empty Search Field"),
                "The search field is empty. Please enter a word and click Find.");
    } else {

        QTextCursor highlightCursor(document);  
        QTextCursor cursor(document);
        
        cursor.beginEditBlock();
//! [6]

        QTextCharFormat plainFormat(highlightCursor.charFormat());
        QTextCharFormat colorFormat = plainFormat;
        colorFormat.setForeground(Qt::red);

        while (!highlightCursor.isNull() && !highlightCursor.atEnd()) {
            highlightCursor = document->find(searchString, highlightCursor, QTextDocument::FindWholeWords);

            if (!highlightCursor.isNull()) {
                found = true;
                highlightCursor.movePosition(QTextCursor::WordRight,
                                       QTextCursor::KeepAnchor);
                highlightCursor.mergeCharFormat(colorFormat);
            }
        }

//! [8]
        cursor.endEditBlock();
//! [7] //! [9]
        isFirstTime = false;

        if (found == false) {
            QMessageBox::information(this, tr("Word Not Found"),
                "Sorry, the word cannot be found.");
        }    
    }
}
//! [8] //! [9]
