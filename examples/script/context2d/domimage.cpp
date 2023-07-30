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

#include "domimage.h"

#include <QVariant>

#include <qscriptcontext.h>

QScriptValue DomImage::s_self;

DomImage::DomImage()
{
}


int DomImage::width() const
{
    return m_image.width();
}


int DomImage::height() const
{
    return m_image.height();
}


QString DomImage::src() const
{
    return m_src;
}

void DomImage::setSrc(const QString &src)
{
    m_src = src;
    m_image = QImage(m_src);
}


QString DomImage::name() const
{
    return m_src;
}

static QScriptValue Image(QScriptContext *context, QScriptEngine *env)
{
    QScriptValue val = context->thisObject();
    DomImage *image = new DomImage();
    QScriptValue klass = env->newVariant(qVariantFromValue(image));
    klass.setPrototype(DomImage::s_self);
    return klass;
}


static QScriptValue width(QScriptContext *context, QScriptEngine *env)
{
    QScriptValue val = context->thisObject();

    DomImage *image = qvariant_cast<DomImage*> (val.toVariant());
    if (image)
        return QScriptValue(env, image->width());

    return QScriptValue(env, 0);
}


static QScriptValue height(QScriptContext *context, QScriptEngine *env)
{
    QScriptValue val = context->thisObject();

    DomImage *image = qvariant_cast<DomImage*> (val.toVariant());
    if (image)
        return QScriptValue(env, image->height());

    return QScriptValue(env, 0);
}


static QScriptValue setSrc(QScriptContext *context, QScriptEngine *env)
{
    QScriptValue val = context->thisObject();
    QString src  = context->argument(0).toString();

    DomImage *image = qvariant_cast<DomImage*> (val.toVariant());
    if (image)
        image->setSrc(src);

    return env->undefinedValue();
}


static QScriptValue name(QScriptContext *context, QScriptEngine *env)
{
    QScriptValue val = context->thisObject();

    DomImage *image = qvariant_cast<DomImage*> (val.toVariant());
    if (image)
        return QScriptValue(env, image->name());

    return QScriptValue(env, QString());
}


void DomImage::setup(QScriptEngine *e)
{
    qRegisterMetaType<DomImage>();

    e->globalObject().setProperty("Image",
                                  e->newFunction(::Image, 0));

    s_self = e->newObject();
    s_self.setProperty("setSrc", e->newFunction(&::setSrc, 1));
    s_self.setProperty("width", e->newFunction(&::width));
    s_self.setProperty("height", e->newFunction(&::height));
    s_self.setProperty("name", e->newFunction(&::name));

    e->setDefaultPrototype(qMetaTypeId<DomImage>(), s_self);
}
