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

#include <QApplication>
#include <QBuffer>
#include <QPalette>

static void main_snippet()
{
//! [0]
    QBuffer buffer;
    char ch;

    buffer.open(QBuffer::ReadWrite);
    buffer.write("Qt rocks!");
    buffer.seek(0);
    buffer.getChar(&ch);  // ch == 'Q'
    buffer.getChar(&ch);  // ch == 't'
    buffer.getChar(&ch);  // ch == ' '
    buffer.getChar(&ch);  // ch == 'r'
//! [0]
}

static void write_datastream_snippets()
{
//! [1]
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);

    QDataStream out(&buffer);
    out << QApplication::palette();
//! [1]
}

static void read_datastream_snippets()
{
    QByteArray byteArray;

//! [2]
    QPalette palette;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::ReadOnly);

    QDataStream in(&buffer);
    in >> palette;
//! [2]
}

static void bytearray_ptr_ctor_snippet()
{
//! [3]
    QByteArray byteArray("abc");
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    buffer.seek(3);
    buffer.write("def", 3);
    buffer.close();
    // byteArray == "abcdef"
//! [3]
}

static void setBuffer_snippet()
{
//! [4]
    QByteArray byteArray("abc");
    QBuffer buffer;
    buffer.setBuffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    buffer.seek(3);
    buffer.write("def", 3);
    buffer.close();
    // byteArray == "abcdef"
//! [4]
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    main_snippet();
    bytearray_ptr_ctor_snippet();
    write_datastream_snippets();
    read_datastream_snippets();
    setBuffer_snippet();
    return 0;
}
