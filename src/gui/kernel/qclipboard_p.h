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

#ifndef QCLIPBOARD_P_H
#define QCLIPBOARD_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "private/qobject_p.h"
#include "QtGui/qmime.h"
#include "QtGui/qclipboard.h"

QT_BEGIN_NAMESPACE

class QClipboardPrivate;

class QMimeDataWrapper : public QMimeSource
{
public:
    QMimeDataWrapper() {}

    const char* format(int n) const;
    QByteArray encodedData(const char*) const;

    mutable QList<QByteArray> formats;
    const QMimeData *data;
};

class QMimeSourceWrapper : public QMimeData
{
public:
    QMimeSourceWrapper(QClipboardPrivate *priv, QClipboard::Mode m);
    ~QMimeSourceWrapper();

    bool hasFormat(const QString &mimetype) const;
    QStringList formats() const;

protected:
    QVariant retrieveData(const QString &mimetype, QVariant::Type) const;
private:
    QClipboardPrivate *d;
    QClipboard::Mode mode;
    QMimeSource *source;
};


class QClipboardPrivate : public QObjectPrivate
{
public:
    QClipboardPrivate() : QObjectPrivate() {
        for (int i = 0; i <= QClipboard::LastMode; ++i) {
            compat_data[i] = 0;
            wrapper[i] = new QMimeDataWrapper();
        }
    }
    ~QClipboardPrivate() {
        for (int i = 0; i <= QClipboard::LastMode; ++i) {
            delete wrapper[i];
            delete compat_data[i];
        }
    }

    mutable QMimeDataWrapper *wrapper[QClipboard::LastMode + 1];
    mutable QMimeSource *compat_data[QClipboard::LastMode + 1];
};

inline QMimeSourceWrapper::QMimeSourceWrapper(QClipboardPrivate *priv, QClipboard::Mode m)
    : QMimeData()
{
    d = priv;
    mode = m;
    source = d->compat_data[mode];
}

inline QMimeSourceWrapper::~QMimeSourceWrapper()
{
    if (d->compat_data[mode] == source)
        d->compat_data[mode] = 0;
    delete source;
}

QT_END_NAMESPACE

#endif // QCLIPBOARD_P_H
