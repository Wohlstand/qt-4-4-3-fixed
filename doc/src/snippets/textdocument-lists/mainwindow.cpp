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

    fileMenu->addAction(tr("E&xit"), this, SLOT(close()),
        QKeySequence(tr("Ctrl+Q", "File|Exit")));

    QMenu *editMenu = new QMenu(tr("&Edit"));

    cutAction = editMenu->addAction(tr("Cu&t"), this, SLOT(cutSelection()),
        QKeySequence(tr("Ctrl+X", "Edit|Cut")));
    copyAction = editMenu->addAction(tr("&Copy"), this, SLOT(copySelection()),
        QKeySequence(tr("Ctrl+C", "Edit|Copy")));
    pasteAction = editMenu->addAction(tr("&Paste"), this,
        SLOT(pasteSelection()), QKeySequence(tr("Ctrl+V", "Edit|Paste")));

    QMenu *selectMenu = new QMenu(tr("&Select"));
    selectMenu->addAction(tr("&Word"), this, SLOT(selectWord()));
    selectMenu->addAction(tr("&Line"), this, SLOT(selectLine()));
    selectMenu->addAction(tr("&Block"), this, SLOT(selectBlock()));
    selectMenu->addAction(tr("&Frame"), this, SLOT(selectFrame()));

    QMenu *insertMenu = new QMenu(tr("&Insert"));

    insertMenu->addAction(tr("&List"), this, SLOT(insertList()),
        QKeySequence(tr("Ctrl+L", "Insert|List")));

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(editMenu);
    menuBar()->addMenu(selectMenu);
    menuBar()->addMenu(insertMenu);

    editor = new QTextEdit(this);
    document = new QTextDocument(this);
    editor->setDocument(document);

    connect(editor, SIGNAL(selectionChanged()), this, SLOT(updateMenus()));

    updateMenus();

    setCentralWidget(editor);
    setWindowTitle(tr("Text Document Writer"));
}

void MainWindow::cutSelection()
{
    QTextCursor cursor = editor->textCursor();
    if (cursor.hasSelection()) {
        selection = cursor.selection();
        cursor.removeSelectedText();
    }
}

void MainWindow::copySelection()
{
    QTextCursor cursor = editor->textCursor();
    if (cursor.hasSelection()) {
        selection = cursor.selection();
        cursor.clearSelection();
    }
}

void MainWindow::pasteSelection()
{
    QTextCursor cursor = editor->textCursor();
    cursor.insertFragment(selection);
}

void MainWindow::selectWord()
{
    QTextCursor cursor = editor->textCursor();
    QTextBlock block = cursor.block();

    cursor.beginEditBlock();
    cursor.movePosition(QTextCursor::StartOfWord);
    cursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
    cursor.endEditBlock();

    editor->setTextCursor(cursor);
}

void MainWindow::selectLine()
{
    QTextCursor cursor = editor->textCursor();
    QTextBlock block = cursor.block();

    cursor.beginEditBlock();
    cursor.movePosition(QTextCursor::StartOfLine);
    cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
    cursor.endEditBlock();

    editor->setTextCursor(cursor);
}

void MainWindow::selectBlock()
{
    QTextCursor cursor = editor->textCursor();
    QTextBlock block = cursor.block();

    cursor.beginEditBlock();
    cursor.movePosition(QTextCursor::StartOfBlock);
    cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
    cursor.endEditBlock();

    editor->setTextCursor(cursor);
}

void MainWindow::selectFrame()
{
    QTextCursor cursor = editor->textCursor();
    QTextFrame *frame = cursor.currentFrame();

    cursor.beginEditBlock();
    cursor.setPosition(frame->firstPosition());
    cursor.setPosition(frame->lastPosition(), QTextCursor::KeepAnchor);
    cursor.endEditBlock();

    editor->setTextCursor(cursor);
}

void MainWindow::insertList()
{
    QTextCursor cursor = editor->textCursor();
    cursor.beginEditBlock();

    QTextList *list = cursor.currentList();
//! [0]
    QTextListFormat listFormat;
    if (list) {
        listFormat = list->format();
        listFormat.setIndent(listFormat.indent() + 1);
    }

    listFormat.setStyle(QTextListFormat::ListDisc);
    cursor.insertList(listFormat);
//! [0]

    cursor.endEditBlock();
}

void MainWindow::updateMenus()
{
    QTextCursor cursor = editor->textCursor();
    cutAction->setEnabled(cursor.hasSelection());
    copyAction->setEnabled(cursor.hasSelection());

    pasteAction->setEnabled(!selection.isEmpty());
}
