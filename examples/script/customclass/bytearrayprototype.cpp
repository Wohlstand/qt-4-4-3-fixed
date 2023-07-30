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

#include "bytearrayprototype.h"
#include <QtScript/QScriptEngine>

Q_DECLARE_METATYPE(QByteArray*)

ByteArrayPrototype::ByteArrayPrototype(QObject *parent)
    : QObject(parent)
{
}

ByteArrayPrototype::~ByteArrayPrototype()
{
}

//! [0]
QByteArray *ByteArrayPrototype::thisByteArray() const
{
    return qscriptvalue_cast<QByteArray*>(thisObject().data());
}
//! [0]

void ByteArrayPrototype::chop(int n)
{
    thisByteArray()->chop(n);
}

bool ByteArrayPrototype::equals(const QByteArray &other)
{
    return *thisByteArray() == other;
}

QByteArray ByteArrayPrototype::left(int len) const
{
    return thisByteArray()->left(len);
}

//! [1]
QByteArray ByteArrayPrototype::mid(int pos, int len) const
{
    return thisByteArray()->mid(pos, len);
}

QScriptValue ByteArrayPrototype::remove(int pos, int len)
{
    thisByteArray()->remove(pos, len);
    return thisObject();
}
//! [1]

QByteArray ByteArrayPrototype::right(int len) const
{
    return thisByteArray()->right(len);
}

QByteArray ByteArrayPrototype::simplified() const
{
    return thisByteArray()->simplified();
}

QByteArray ByteArrayPrototype::toBase64() const
{
    return thisByteArray()->toBase64();
}

QByteArray ByteArrayPrototype::toLower() const
{
    return thisByteArray()->toLower();
}

QByteArray ByteArrayPrototype::toUpper() const
{
    return thisByteArray()->toUpper();
}

QByteArray ByteArrayPrototype::trimmed() const
{
    return thisByteArray()->trimmed();
}

void ByteArrayPrototype::truncate(int pos)
{
    thisByteArray()->truncate(pos);
}

QString ByteArrayPrototype::toLatin1String() const
{
    return QString::fromLatin1(*thisByteArray());
}

//! [2]
QScriptValue ByteArrayPrototype::valueOf() const
{
    return thisObject().data();
}
//! [2]
