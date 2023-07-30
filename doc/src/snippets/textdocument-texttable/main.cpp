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

int main(int argc, char * argv[])
{
    int rows = 6;
    int columns = 2;

    QApplication app(argc, argv);
    QTextEdit *textEdit = new QTextEdit;
    QTextCursor cursor(textEdit->textCursor());
    cursor.movePosition(QTextCursor::Start);

    QTextTableFormat tableFormat;
    tableFormat.setAlignment(Qt::AlignHCenter);
    tableFormat.setCellPadding(2);
    tableFormat.setCellSpacing(2);
    QTextTable *table = cursor.insertTable(rows, columns);
    table->setFormat(tableFormat);
    
    QTextCharFormat boldFormat;
    boldFormat.setFontWeight(QFont::Bold);

    QTextBlockFormat centerFormat;
    centerFormat.setAlignment(Qt::AlignHCenter);
    cursor.mergeBlockFormat(centerFormat);

    cursor = table->cellAt(0, 0).firstCursorPosition();
    cursor.insertText(("Details"), boldFormat);

    cursor = table->cellAt(1, 0).firstCursorPosition();
    cursor.insertText("Alan");

    cursor = table->cellAt(1, 1).firstCursorPosition();
    cursor.insertText("5, Pickety Street");

//! [0]
    table->mergeCells(0, 0, 1, 2);
//! [0] //! [1]
    table->splitCell(0, 0, 1, 1);
//! [1]

    textEdit->show();
    return app.exec();
}