/****************************************************************************
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the plugins of the Qt Toolkit.
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

#include <qtextcodecplugin.h>
#include <qtextcodec.h>
#include <qstringlist.h>

#include "qeuckrcodec.h"

QT_BEGIN_NAMESPACE

#ifndef QT_NO_TEXTCODECPLUGIN

class KRTextCodecs : public QTextCodecPlugin
{
public:
    KRTextCodecs() {}

    QList<QByteArray> names() const;
    QList<QByteArray> aliases() const;
    QList<int> mibEnums() const;

    QTextCodec *createForMib(int);
    QTextCodec *createForName(const QByteArray &);
};

QList<QByteArray> KRTextCodecs::names() const
{
    QList<QByteArray> list;
    list += QEucKrCodec::_name();
#ifdef Q_WS_X11
    list += QFontKsc5601Codec::_name();
#endif
    list += QCP949Codec::_name();
    return list;
}

QList<QByteArray> KRTextCodecs::aliases() const
{
    QList<QByteArray> list;
    list += QEucKrCodec::_aliases();
#ifdef Q_WS_X11
    list += QFontKsc5601Codec::_aliases();
#endif
    list += QCP949Codec::_aliases();
    return list;
}

QList<int> KRTextCodecs::mibEnums() const
{
    QList<int> list;
    list += QEucKrCodec::_mibEnum();
#ifdef Q_WS_X11
    list += QFontKsc5601Codec::_mibEnum();
#endif
    list += QCP949Codec::_mibEnum();
    return list;
}

QTextCodec *KRTextCodecs::createForMib(int mib)
{
    if (mib == QEucKrCodec::_mibEnum())
        return new QEucKrCodec;
#ifdef Q_WS_X11
    if (mib == QFontKsc5601Codec::_mibEnum())
        return new QFontKsc5601Codec;
#endif
    if (mib == QCP949Codec::_mibEnum())
        return new QCP949Codec;
    return 0;
}


QTextCodec *KRTextCodecs::createForName(const QByteArray &name)
{
    if (name == QEucKrCodec::_name() || QEucKrCodec::_aliases().contains(name))
        return new QEucKrCodec;
#ifdef Q_WS_X11
    if (name == QFontKsc5601Codec::_name() || QFontKsc5601Codec::_aliases().contains(name))
        return new QFontKsc5601Codec;
#endif
    if (name == QCP949Codec::_name() || QCP949Codec::_aliases().contains(name))
        return new QCP949Codec;
    return 0;
}


Q_EXPORT_STATIC_PLUGIN(KRTextCodecs);
Q_EXPORT_PLUGIN2(qkrcodecs, KRTextCodecs);

#endif // QT_NO_TEXTCODECPLUGIN

QT_END_NAMESPACE
