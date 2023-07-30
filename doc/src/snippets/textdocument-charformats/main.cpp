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
    QTextEdit *editor = new QTextEdit();

    QTextCursor cursor(editor->textCursor());
    cursor.movePosition(QTextCursor::Start); 

    QTextCharFormat plainFormat(cursor.charFormat());

    QTextCharFormat headingFormat = plainFormat;
    headingFormat.setFontWeight(QFont::Bold);
    headingFormat.setFontPointSize(16);

    QTextCharFormat emphasisFormat = plainFormat;
    emphasisFormat.setFontItalic(true);

    QTextCharFormat qtFormat = plainFormat;
    qtFormat.setForeground(QColor("#990000"));

    QTextCharFormat underlineFormat = plainFormat;
    underlineFormat.setFontUnderline(true);

//! [0]
    cursor.insertText(tr("Character formats"),
                      headingFormat);

    cursor.insertBlock();

    cursor.insertText(tr("Text can be displayed in a variety of "
                                  "different character formats. "), plainFormat);
    cursor.insertText(tr("We can emphasize text by "));
    cursor.insertText(tr("making it italic"), emphasisFormat);
//! [0]
    cursor.insertText(tr(", give it a "), plainFormat);
    cursor.insertText(tr("different color "), qtFormat);
    cursor.insertText(tr("to the default text color, "), plainFormat);
    cursor.insertText(tr("underline it"), underlineFormat);
    cursor.insertText(tr(", and use many other effects."), plainFormat);

    editor->setWindowTitle(tr("Text Document Character Formats"));
    editor->resize(320, 480);
    editor->show();
    return app.exec();
}
