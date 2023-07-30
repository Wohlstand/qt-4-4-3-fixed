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
    QTextCharFormat colorFormat = plainFormat;
    colorFormat.setForeground(Qt::red);

    cursor.insertText(tr("Text can be displayed in a variety of "
                                  "different character "
                                  "formats. "), plainFormat);
    cursor.insertText(tr("We can emphasize text by making it "));
    cursor.insertText(tr("italic, give it a different color "));
    cursor.insertText(tr("to the default text color, underline it, "));
    cursor.insertText(tr("and use many other effects."));

    QString searchString = tr("text");

    QTextDocument *document = editor->document();
//! [0]
    QTextCursor newCursor(document);

    while (!newCursor.isNull() && !newCursor.atEnd()) {
        newCursor = document->find(searchString, newCursor);

        if (!newCursor.isNull()) {
            newCursor.movePosition(QTextCursor::WordRight,
                                   QTextCursor::KeepAnchor);

            newCursor.mergeCharFormat(colorFormat);
        }
//! [0] //! [1]
    }
//! [1]

    editor->setWindowTitle(tr("Text Document Find"));
    editor->resize(320, 480);
    editor->show();
    return app.exec();
}
