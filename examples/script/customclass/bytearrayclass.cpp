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

#include <QtScript/QScriptClassPropertyIterator>
#include <QtScript/QScriptEngine>
#include "bytearrayclass.h"
#include "bytearrayprototype.h"

#include <stdlib.h>

Q_DECLARE_METATYPE(QByteArray*)
Q_DECLARE_METATYPE(ByteArrayClass*)

class ByteArrayClassPropertyIterator : public QScriptClassPropertyIterator
{
public:
    ByteArrayClassPropertyIterator(const QScriptValue &object);
    ~ByteArrayClassPropertyIterator();

    bool hasNext() const;
    void next();

    bool hasPrevious() const;
    void previous();

    void toFront();
    void toBack();

    QScriptString name() const;
    uint id() const;

private:
    int m_index;
    int m_last;
};

static qint32 toArrayIndex(const QString &str)
{
    QByteArray bytes = str.toUtf8();
    char *eptr;
    quint32 pos = strtoul(bytes.constData(), &eptr, 10);
    if ((eptr == bytes.constData() + bytes.size())
        && (QByteArray::number(pos) == bytes)) {
        return pos;
    }
    return -1;
}

//! [0]
ByteArrayClass::ByteArrayClass(QScriptEngine *engine)
    : QObject(engine), QScriptClass(engine)
{
    qScriptRegisterMetaType<QByteArray>(engine, toScriptValue, fromScriptValue);

    length = engine->toStringHandle(QLatin1String("length"));

    proto = engine->newQObject(new ByteArrayPrototype(this),
                               QScriptEngine::QtOwnership,
                               QScriptEngine::SkipMethodsInEnumeration
                               | QScriptEngine::ExcludeSuperClassMethods
                               | QScriptEngine::ExcludeSuperClassProperties);
    QScriptValue global = engine->globalObject();
    proto.setPrototype(global.property("Object").property("prototype"));

    ctor = engine->newFunction(construct);
    ctor.setData(qScriptValueFromValue(engine, this));
}
//! [0]

ByteArrayClass::~ByteArrayClass()
{
}

//! [3]
QScriptClass::QueryFlags ByteArrayClass::queryProperty(const QScriptValue &object,
                                                       const QScriptString &name,
                                                       QueryFlags flags, uint *id)
{
    QByteArray *ba = qscriptvalue_cast<QByteArray*>(object.data());
    if (!ba)
        return 0;
    if (name == length) {
        return flags;
    } else {
        qint32 pos = toArrayIndex(name);
        if (pos == -1)
            return 0;
        *id = pos;
        if ((flags & HandlesReadAccess) && (pos >= ba->size()))
            flags &= ~HandlesReadAccess;
        return flags;
    }
}
//! [3]

//! [4]
QScriptValue ByteArrayClass::property(const QScriptValue &object,
                                      const QScriptString &name, uint id)
{
    QByteArray *ba = qscriptvalue_cast<QByteArray*>(object.data());
    if (!ba)
        return QScriptValue();
    if (name == length) {
        return QScriptValue(engine(), ba->length());
    } else {
        qint32 pos = id;
        if ((pos < 0) || (pos >= ba->size()))
            return QScriptValue();
        return QScriptValue(engine(), uint(ba->at(pos)) & 255);
    }
    return QScriptValue();
}
//! [4]

//! [5]
void ByteArrayClass::setProperty(QScriptValue &object,
                                 const QScriptString &name,
                                 uint id, const QScriptValue &value)
{
    QByteArray *ba = qscriptvalue_cast<QByteArray*>(object.data());
    if (!ba)
        return;
    if (name == length) {
        ba->resize(value.toInt32());
    } else {
        qint32 pos = id;
        if (pos < 0)
            return;
        if (ba->size() <= pos)
            ba->resize(pos + 1);
        (*ba)[pos] = char(value.toInt32());
    }
}
//! [5]

//! [6]
QScriptValue::PropertyFlags ByteArrayClass::propertyFlags(
    const QScriptValue &/*object*/, const QScriptString &name, uint /*id*/)
{
    if (name == length) {
        return QScriptValue::Undeletable
            | QScriptValue::SkipInEnumeration;
    }
    return QScriptValue::Undeletable;
}
//! [6]

//! [7]
QScriptClassPropertyIterator *ByteArrayClass::newIterator(const QScriptValue &object)
{
    return new ByteArrayClassPropertyIterator(object);
}
//! [7]

QString ByteArrayClass::name() const
{
    return QLatin1String("ByteArray");
}

QScriptValue ByteArrayClass::prototype() const
{
    return proto;
}

QScriptValue ByteArrayClass::constructor()
{
    return ctor;
}

QScriptValue ByteArrayClass::newInstance(int size)
{
    return newInstance(QByteArray(size, /*ch=*/0));
}

//! [1]
QScriptValue ByteArrayClass::newInstance(const QByteArray &ba)
{
    QScriptValue data = engine()->newVariant(qVariantFromValue(ba));
    return engine()->newObject(this, data);
}
//! [1]

//! [2]
QScriptValue ByteArrayClass::construct(QScriptContext *ctx, QScriptEngine *)
{
    ByteArrayClass *cls = qscriptvalue_cast<ByteArrayClass*>(ctx->callee().data());
    if (!cls)
        return QScriptValue();
    int size = ctx->argument(0).toInt32();
    return cls->newInstance(size);
}
//! [2]

QScriptValue ByteArrayClass::toScriptValue(QScriptEngine *eng, const QByteArray &ba)
{
    QScriptValue ctor = eng->globalObject().property("ByteArray");
    ByteArrayClass *cls = qscriptvalue_cast<ByteArrayClass*>(ctor.data());
    if (!cls)
        return eng->newVariant(qVariantFromValue(ba));
    return cls->newInstance(ba);
}

void ByteArrayClass::fromScriptValue(const QScriptValue &obj, QByteArray &ba)
{
    ba = qscriptvalue_cast<QByteArray>(obj.data());
}



ByteArrayClassPropertyIterator::ByteArrayClassPropertyIterator(const QScriptValue &object)
    : QScriptClassPropertyIterator(object)
{
    toFront();
}

ByteArrayClassPropertyIterator::~ByteArrayClassPropertyIterator()
{
}

//! [8]
bool ByteArrayClassPropertyIterator::hasNext() const
{
    QByteArray *ba = qscriptvalue_cast<QByteArray*>(object().data());
    return m_index < ba->size();
}

void ByteArrayClassPropertyIterator::next()
{
    m_last = m_index;
    ++m_index;
}

bool ByteArrayClassPropertyIterator::hasPrevious() const
{
    return (m_index > 0);
}

void ByteArrayClassPropertyIterator::previous()
{
    --m_index;
    m_last = m_index;
}

void ByteArrayClassPropertyIterator::toFront()
{
    m_index = 0;
    m_last = -1;
}

void ByteArrayClassPropertyIterator::toBack()
{
    QByteArray *ba = qscriptvalue_cast<QByteArray*>(object().data());
    m_index = ba->size();
    m_last = -1;
}

QScriptString ByteArrayClassPropertyIterator::name() const
{
    return QScriptString();
}

uint ByteArrayClassPropertyIterator::id() const
{
    return m_last;
}
//! [8]
