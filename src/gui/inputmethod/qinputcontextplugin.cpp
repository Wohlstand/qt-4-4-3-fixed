/****************************************************************************
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the QtGui module of the Qt Toolkit.
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

/****************************************************************************
**
** Implementation of QInputContext class
**
** Copyright (C) 2003-2004 immodule for Qt Project.  All rights reserved.
**
** This file is written to contribute to Nokia Corporation and/or its subsidiary(-ies) under their own
** license. You may use this file under your Qt license. Following
** description is copied from their original file headers. Contact
** immodule-qt@freedesktop.org if any conditions of this licensing are
** not clear to you.
**
****************************************************************************/

#include "qinputcontextplugin.h"

#ifndef QT_NO_IM
#ifndef QT_NO_LIBRARY

QT_BEGIN_NAMESPACE

/*!
    \class QInputContextPlugin
    \brief The QInputContextPlugin class provides an abstract base for custom QInputContext plugins.

    \reentrant
    \ingroup plugins

    The input context plugin is a simple plugin interface that makes it
    easy to create custom input contexts that can be loaded dynamically
    into applications.

    To create an input context plugin you subclass this base class,
    reimplement the pure virtual functions keys(), create(),
    languages(), displayName(), and description(), and export the
    class with the Q_EXPORT_PLUGIN2() macro.

    \legalese
    Copyright (C) 2003-2004 immodule for Qt Project.  All rights reserved.

    This file is written to contribute to Nokia Corporation and/or its subsidiary(-ies) under their own
    license. You may use this file under your Qt license. Following
    description is copied from their original file headers. Contact
    immodule-qt@freedesktop.org if any conditions of this licensing are
    not clear to you.
    \endlegalese

    \sa QInputContext, {How to Create Qt Plugins}
*/

/*!
    \fn QStringList QInputContextPlugin::keys() const

    Returns the list of QInputContext keys this plugin provides.

    These keys are usually the class names of the custom input context
    that are implemented in the plugin. The names are used, for
    example, to identify and specify input methods for the input
    method switching mechanism.  They have to be consistent with
    QInputContext::identifierName(), and may only contain ASCII
    characters.

    \sa create(), displayName(), QInputContext::identifierName()
*/

/*!
    \fn QInputContext* QInputContextPlugin::create( const QString& key )

    Creates and returns a QInputContext object for the input context
    key \a key.  The input context key is usually the class name of
    the required input method.

    \sa keys()
*/

/*!
    \fn QStringList QInputContextPlugin::languages(const QString &key)

    Returns the languages supported by the QInputContext object
    specified by \a key.

    The languages are expressed as language code (e.g. "zh_CN",
    "zh_TW", "zh_HK", "ja", "ko", ...). An input context that supports
    multiple languages can return all supported languages as
    QStringList. The name has to be consistent with
    QInputContext::language().

    This information may be used to optimize user interface.

    \sa keys(), QInputContext::language(), QLocale
*/

/*!
    \fn QString QInputContextPlugin::displayName(const QString &key)

    Returns a user friendly internationalized name of the
    QInputContext object specified by \a key. You can, for example,
    use this name in a menu.

    \sa keys(), QInputContext::identifierName()
*/

/*!
    \fn QString QInputContextPlugin::description(const QString &key)

    Returns an internationalized brief description of the QInputContext
    object specified by \a key. You can, for example, use this
    description in a user interface.

    \sa keys(), displayName()
*/


/*!
    Constructs a input context plugin with the given \a parent. This
    is invoked automatically by the Q_EXPORT_PLUGIN2() macro.
*/
QInputContextPlugin::QInputContextPlugin(QObject *parent)
    :QObject(parent)
{
}

/*!
    Destroys the input context plugin.

    You never have to call this explicitly. Qt destroys a plugin
    automatically when it's no longer used.
*/
QInputContextPlugin::~QInputContextPlugin()
{
}

QT_END_NAMESPACE

#endif // QT_NO_LIBRARY

#endif // QT_NO_IM
