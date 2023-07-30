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

QString tr(const char *text)
{
    return QApplication::translate(text, text);
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

//! [0]
    QTextEdit *editor = new QTextEdit();
    QTextCursor cursor(editor->textCursor());
//! [0]
    cursor.movePosition(QTextCursor::Start); 

    QTextBlockFormat blockFormat = cursor.blockFormat();
    blockFormat.setTopMargin(4);
    blockFormat.setLeftMargin(4);
    blockFormat.setRightMargin(4);
    blockFormat.setBottomMargin(4);

    cursor.setBlockFormat(blockFormat);
    cursor.insertText(tr("This contains plain text inside a "
                         "text block with margins to keep it separate "
                         "from other parts of the document."));

    cursor.insertBlock();

//! [1]
    QTextBlockFormat backgroundFormat = blockFormat;
    backgroundFormat.setBackground(QColor("lightGray"));

    cursor.setBlockFormat(backgroundFormat);
//! [1]
    cursor.insertText(tr("The background color of a text block can be "
                         "changed to highlight text."));

    cursor.insertBlock();

    QTextBlockFormat rightAlignedFormat = blockFormat;
    rightAlignedFormat.setAlignment(Qt::AlignRight);

    cursor.setBlockFormat(rightAlignedFormat);
    cursor.insertText(tr("The alignment of the text within a block is "
                         "controlled by the alignment properties of "
                         "the block itself. This text block is "
                         "right-aligned."));

    cursor.insertBlock();

    QTextBlockFormat paragraphFormat = blockFormat;
    paragraphFormat.setAlignment(Qt::AlignJustify);
    paragraphFormat.setTextIndent(32);

    cursor.setBlockFormat(paragraphFormat);
    cursor.insertText(tr("Text can be formatted so that the first "
                         "line in a paragraph has its own margin. "
                         "This makes the text more readable."));

    cursor.insertBlock();

    QTextBlockFormat reverseFormat = blockFormat;
    reverseFormat.setAlignment(Qt::AlignJustify);
    reverseFormat.setTextIndent(32);

    cursor.setBlockFormat(reverseFormat);
    cursor.insertText(tr("The direction of the text can be reversed. "
                         "This is useful for right-to-left "
                         "languages."));

    editor->setWindowTitle(tr("Text Block Formats"));
    editor->resize(480, 480);
    editor->show();
    
    return app.exec();
}
