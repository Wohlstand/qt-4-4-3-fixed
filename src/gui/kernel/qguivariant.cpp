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

#include "qvariant.h"

#include "qbitmap.h"
#include "qbrush.h"
#include "qcolor.h"
#include "qcursor.h"
#include "qdatastream.h"
#include "qdebug.h"
#include "qfont.h"
#include "qicon.h"
#include "qimage.h"
#include "qkeysequence.h"
#include "qtransform.h"
#include "qmatrix.h"
#include "qpalette.h"
#include "qpen.h"
#include "qpixmap.h"
#include "qpolygon.h"
#include "qregion.h"
#include "qsizepolicy.h"
#include "qtextformat.h"

#include "private/qvariant_p.h"

QT_BEGIN_NAMESPACE

#ifdef QT3_SUPPORT
extern QDataStream &qt_stream_out_qcolorgroup(QDataStream &s, const QColorGroup &g);
extern QDataStream &qt_stream_in_qcolorgroup(QDataStream &s, QColorGroup &g);
#endif

Q_CORE_EXPORT const QVariant::Handler *qcoreVariantHandler();

static void construct(QVariant::Private *x, const void *copy)
{
    switch (x->type) {
    case QVariant::Bitmap:
        v_construct<QBitmap>(x, copy);
        break;
    case QVariant::Region:
        v_construct<QRegion>(x, copy);
        break;
    case QVariant::Polygon:
        v_construct<QPolygon>(x, copy);
        break;
    case QVariant::Font:
        v_construct<QFont>(x, copy);
        break;
    case QVariant::Pixmap:
        v_construct<QPixmap>(x, copy);
        break;
    case QVariant::Image:
        v_construct<QImage>(x, copy);
        break;
    case QVariant::Brush:
        v_construct<QBrush>(x, copy);
        break;
    case QVariant::Color:
        v_construct<QColor>(x, copy);
        break;
    case QVariant::Palette:
        v_construct<QPalette>(x, copy);
        break;
#ifdef QT3_SUPPORT
    case QVariant::ColorGroup:
        v_construct<QColorGroup>(x, copy);
        break;
#endif
#ifndef QT_NO_ICON
    case QVariant::Icon:
        v_construct<QIcon>(x, copy);
        break;
#endif
    case QVariant::Matrix:
        v_construct<QMatrix>(x, copy);
        break;
    case QVariant::Transform:
        v_construct<QTransform>(x, copy);
        break;
    case QVariant::TextFormat:
        v_construct<QTextFormat>(x, copy);
        break;
    case QVariant::TextLength:
        v_construct<QTextLength>(x, copy);
        break;
#ifndef QT_NO_SHORTCUT
    case QVariant::KeySequence:
        v_construct<QKeySequence>(x, copy);
        break;
#endif
    case QVariant::Pen:
        v_construct<QPen>(x, copy);
        break;
    case QVariant::SizePolicy:
        v_construct<QSizePolicy>(x, copy);
        break;
#ifndef QT_NO_CURSOR
    case QVariant::Cursor:
        v_construct<QCursor>(x, copy);
        break;
#endif
    case 62: {
        // small 'trick' to let a QVariant(Qt::blue) create a variant
        // of type QColor
        x->type = QVariant::Color;
        QColor color(*reinterpret_cast<const Qt::GlobalColor *>(copy));
        v_construct<QColor>(x, &color);
        break;
    }
    default:
        qcoreVariantHandler()->construct(x, copy);
        return;
    }
    x->is_null = !copy;
}

static void clear(QVariant::Private *d)
{
    switch (d->type) {
    case QVariant::Bitmap:
        v_clear<QBitmap>(d);
        break;
    case QVariant::Cursor:
        v_clear<QCursor>(d);
        break;
    case QVariant::Region:
        v_clear<QRegion>(d);
        break;
    case QVariant::Polygon:
        v_clear<QPolygon>(d);
        break;
    case QVariant::Font:
        v_clear<QFont>(d);
        break;
    case QVariant::Pixmap:
        v_clear<QPixmap>(d);
        break;
    case QVariant::Image:
        v_clear<QImage>(d);
        break;
    case QVariant::Brush:
        v_clear<QBrush>(d);
        break;
    case QVariant::Color:
        v_clear<QColor>(d);
        break;
    case QVariant::Palette:
        v_clear<QPalette>(d);
        break;
#ifdef QT3_SUPPORT
    case QVariant::ColorGroup:
        v_clear<QColorGroup>(d);
        break;
#endif
#ifndef QT_NO_ICON
    case QVariant::Icon:
        v_clear<QIcon>(d);
        break;
#endif
    case QVariant::Matrix:
        v_clear<QMatrix>(d);
        break;
    case QVariant::Transform:
        v_clear<QTransform>(d);
        break;
    case QVariant::TextFormat:
        v_clear<QTextFormat>(d);
        break;
    case QVariant::TextLength:
        v_clear<QTextLength>(d);
        break;
    case QVariant::SizePolicy:
        v_clear<QSizePolicy>(d);
        break;
#ifndef QT_NO_SHORTCUT
    case QVariant::KeySequence:
        v_clear<QKeySequence>(d);
        break;
#endif
    case QVariant::Pen:
        v_clear<QPen>(d);
        break;
    default:
        qcoreVariantHandler()->clear(d);
        return;
    }

    d->type = QVariant::Invalid;
    d->is_null = true;
    d->is_shared = false;
}


static bool isNull(const QVariant::Private *d)
{
    switch(d->type) {
    case QVariant::Bitmap:
        return v_cast<QBitmap>(d)->isNull();
    case QVariant::Region:
        return v_cast<QRegion>(d)->isEmpty();
    case QVariant::Polygon:
        return v_cast<QPolygon>(d)->isEmpty();
    case QVariant::Pixmap:
        return v_cast<QPixmap>(d)->isNull();
    case QVariant::Image:
        return v_cast<QImage>(d)->isNull();
#ifndef QT_NO_ICON
    case QVariant::Icon:
        return v_cast<QIcon>(d)->isNull();
#endif
    case QVariant::Matrix:
    case QVariant::TextFormat:
    case QVariant::TextLength:
    case QVariant::Cursor:
    case QVariant::StringList:
    case QVariant::Font:
    case QVariant::Brush:
    case QVariant::Color:
    case QVariant::Palette:
#ifdef QT3_SUPPORT
    case QVariant::ColorGroup:
#endif
    case QVariant::SizePolicy:
#ifndef QT_NO_SHORTCUT
    case QVariant::KeySequence:
#endif
    case QVariant::Pen:
        break;
    default:
        return qcoreVariantHandler()->isNull(d);
    }
    return d->is_null;
}

static bool compare(const QVariant::Private *a, const QVariant::Private *b)
{
    Q_ASSERT(a->type == b->type);
    switch(a->type) {
    case QVariant::Cursor:
#ifndef QT_NO_CURSOR
        return v_cast<QCursor>(a)->shape() == v_cast<QCursor>(b)->shape();
#endif
    case QVariant::Bitmap:
        return v_cast<QBitmap>(a)->cacheKey()
            == v_cast<QBitmap>(b)->cacheKey();
    case QVariant::Polygon:
        return *v_cast<QPolygon>(a) == *v_cast<QPolygon>(b);
    case QVariant::Region:
        return *v_cast<QRegion>(a) == *v_cast<QRegion>(b);
    case QVariant::Font:
        return *v_cast<QFont>(a) == *v_cast<QFont>(b);
    case QVariant::Pixmap:
        return v_cast<QPixmap>(a)->cacheKey() == v_cast<QPixmap>(b)->cacheKey();
    case QVariant::Image:
        return *v_cast<QImage>(a) == *v_cast<QImage>(b);
    case QVariant::Brush:
        return *v_cast<QBrush>(a) == *v_cast<QBrush>(b);
    case QVariant::Color:
        return *v_cast<QColor>(a) == *v_cast<QColor>(b);
    case QVariant::Palette:
        return *v_cast<QPalette>(a) == *v_cast<QPalette>(b);
#ifdef QT3_SUPPORT
    case QVariant::ColorGroup:
        return *v_cast<QColorGroup>(a) == *v_cast<QColorGroup>(b);
#endif
#ifndef QT_NO_ICON
    case QVariant::Icon:
        /* QIcon::operator==() cannot be reasonably implemented for QIcon,
         * so we always return false. */
        return false;
#endif
    case QVariant::Matrix:
        return *v_cast<QMatrix>(a) == *v_cast<QMatrix>(b);
    case QVariant::Transform:
        return *v_cast<QTransform>(a) == *v_cast<QTransform>(b);
    case QVariant::TextFormat:
        return *v_cast<QTextFormat>(a) == *v_cast<QTextFormat>(b);
    case QVariant::TextLength:
        return *v_cast<QTextLength>(a) == *v_cast<QTextLength>(b);
    case QVariant::SizePolicy:
        return *v_cast<QSizePolicy>(a) == *v_cast<QSizePolicy>(b);
#ifndef QT_NO_SHORTCUT
    case QVariant::KeySequence:
        return *v_cast<QKeySequence>(a) == *v_cast<QKeySequence>(b);
#endif
    case QVariant::Pen:
        return *v_cast<QPen>(a) == *v_cast<QPen>(b);
    default:
        break;
    }
    return qcoreVariantHandler()->compare(a, b);
}



static bool convert(const QVariant::Private *d, QVariant::Type t,
                 void *result, bool *ok)
{
    switch (t) {
    case QVariant::ByteArray:
        if (d->type == QVariant::Color) {
            *static_cast<QByteArray *>(result) = v_cast<QColor>(d)->name().toLatin1();
            return true;
        }
        break;
    case QVariant::String: {
        QString *str = static_cast<QString *>(result);
        switch (d->type) {
#ifndef QT_NO_SHORTCUT
        case QVariant::KeySequence:
            *str = QString(*v_cast<QKeySequence>(d));
            return true;
#endif
        case QVariant::Font:
            *str = v_cast<QFont>(d)->toString();
            return true;
        case QVariant::Color:
            *str = v_cast<QColor>(d)->name();
            return true;
        default:
            break;
        }
        break;
    }
    case QVariant::Pixmap:
        if (d->type == QVariant::Image) {
            *static_cast<QPixmap *>(result) = QPixmap::fromImage(*v_cast<QImage>(d));
            return true;
        } else if (d->type == QVariant::Bitmap) {
            *static_cast<QPixmap *>(result) = *v_cast<QBitmap>(d);
            return true;
        } else if (d->type == QVariant::Brush) {
            if (v_cast<QBrush>(d)->style() == Qt::TexturePattern) {
                *static_cast<QPixmap *>(result) = v_cast<QBrush>(d)->texture();
                return true;
            }
        }
        break;
    case QVariant::Image:
        if (d->type == QVariant::Pixmap) {
            *static_cast<QImage *>(result) = v_cast<QPixmap>(d)->toImage();
            return true;
        } else if (d->type == QVariant::Bitmap) {
            *static_cast<QImage *>(result) = v_cast<QBitmap>(d)->toImage();
            return true;
        }
        break;
    case QVariant::Bitmap:
        if (d->type == QVariant::Pixmap) {
            *static_cast<QBitmap *>(result) = *v_cast<QPixmap>(d);
            return true;
        } else if (d->type == QVariant::Image) {
            *static_cast<QBitmap *>(result) = QBitmap::fromImage(*v_cast<QImage>(d));
            return true;
        }
        break;
#ifndef QT_NO_SHORTCUT
    case QVariant::Int:
        if (d->type == QVariant::KeySequence) {
            *static_cast<int *>(result) = (int)(*(v_cast<QKeySequence>(d)));
            return true;
        }
        break;
#endif
    case QVariant::Font:
        if (d->type == QVariant::String) {
            QFont *f = static_cast<QFont *>(result);
            f->fromString(*v_cast<QString>(d));
            return true;
        }
        break;
    case QVariant::Color:
        if (d->type == QVariant::String) {
            static_cast<QColor *>(result)->setNamedColor(*v_cast<QString>(d));
            return static_cast<QColor *>(result)->isValid();
        } else if (d->type == QVariant::ByteArray) {
            static_cast<QColor *>(result)->setNamedColor(QString::fromLatin1(
                                *v_cast<QByteArray>(d)));
            return true;
        } else if (d->type == QVariant::Brush) {
            if (v_cast<QBrush>(d)->style() == Qt::SolidPattern) {
                *static_cast<QColor *>(result) = v_cast<QBrush>(d)->color();
                return true;
            }
        }
        break;
    case QVariant::Brush:
        if (d->type == QVariant::Color) {
            *static_cast<QBrush *>(result) = QBrush(*v_cast<QColor>(d));
            return true;
        } else if (d->type == QVariant::Pixmap) {
            *static_cast<QBrush *>(result) = QBrush(*v_cast<QPixmap>(d));
            return true;
        }
        break;
#ifndef QT_NO_SHORTCUT
    case QVariant::KeySequence: {
        QKeySequence *seq = static_cast<QKeySequence *>(result);
        switch (d->type) {
        case QVariant::String:
            *seq = QKeySequence(*v_cast<QString>(d));
            return true;
        case QVariant::Int:
            *seq = QKeySequence(d->data.i);
            return true;
        default:
            break;
        }
    }
#endif
    default:
        break;
    }
    return qcoreVariantHandler()->convert(d, t, result, ok);
}

#if !defined(QT_NO_DEBUG_STREAM) && !defined(Q_BROKEN_DEBUG_STREAM)
static void streamDebug(QDebug dbg, const QVariant &v)
{
    switch(v.type()) {
    case QVariant::Cursor:
#ifndef QT_NO_CURSOR
//        dbg.nospace() << qvariant_cast<QCursor>(v); //FIXME
#endif
        break;
    case QVariant::Bitmap:
//        dbg.nospace() << qvariant_cast<QBitmap>(v); //FIXME
        break;
    case QVariant::Polygon:
        dbg.nospace() << qvariant_cast<QPolygon>(v);
        break;
    case QVariant::Region:
        dbg.nospace() << qvariant_cast<QRegion>(v);
        break;
    case QVariant::Font:
//        dbg.nospace() << qvariant_cast<QFont>(v);  //FIXME
        break;
    case QVariant::Matrix:
        dbg.nospace() << qvariant_cast<QMatrix>(v);
        break;
    case QVariant::Transform:
        dbg.nospace() << qvariant_cast<QTransform>(v);
        break;
    case QVariant::Pixmap:
//        dbg.nospace() << qvariant_cast<QPixmap>(v); //FIXME
        break;
    case QVariant::Image:
//        dbg.nospace() << qvariant_cast<QImage>(v); //FIXME
        break;
    case QVariant::Brush:
        dbg.nospace() << qvariant_cast<QBrush>(v);
        break;
    case QVariant::Color:
        dbg.nospace() << qvariant_cast<QColor>(v);
        break;
    case QVariant::Palette:
//        dbg.nospace() << qvariant_cast<QPalette>(v); //FIXME
        break;
#ifndef QT_NO_ICON
    case QVariant::Icon:
//        dbg.nospace() << qvariant_cast<QIcon>(v); // FIXME
        break;
#endif
    case QVariant::SizePolicy:
//        dbg.nospace() << qvariant_cast<QSizePolicy>(v); //FIXME
        break;
#ifndef QT_NO_SHORTCUT
    case QVariant::KeySequence:
        dbg.nospace() << qvariant_cast<QKeySequence>(v);
        break;
#endif
    case QVariant::Pen:
        dbg.nospace() << qvariant_cast<QPen>(v);
        break;
    default:
        qcoreVariantHandler()->debugStream(dbg, v);
        break;
    }
}
#endif

const QVariant::Handler qt_gui_variant_handler = {
    construct,
    clear,
    isNull,
#ifndef QT_NO_DATASTREAM
    0,
    0,
#endif
    compare,
    convert,
    0,
#if !defined(QT_NO_DEBUG_STREAM) && !defined(Q_BROKEN_DEBUG_STREAM)
    streamDebug
#else
    0
#endif
};

struct QMetaTypeGuiHelper
{
    QMetaType::Constructor constr;
    QMetaType::Destructor destr;
#ifndef QT_NO_DATASTREAM
    QMetaType::SaveOperator saveOp;
    QMetaType::LoadOperator loadOp;
#endif
};

#ifdef QT_MAKEDLL
#  define Q_VARIANT_DECL_IMPORT Q_DECL_IMPORT
#else
#  define Q_VARIANT_DECL_IMPORT
#endif
extern Q_VARIANT_DECL_IMPORT const QMetaTypeGuiHelper *qMetaTypeGuiHelper;


#ifdef QT_NO_DATASTREAM
#  define Q_DECL_METATYPE_HELPER(TYPE) \
     typedef void *(*QConstruct##TYPE)(const TYPE *); \
     static const QConstruct##TYPE qConstruct##TYPE = qMetaTypeConstructHelper<TYPE>; \
     typedef void (*QDestruct##TYPE)(TYPE *); \
     static const QDestruct##TYPE qDestruct##TYPE = qMetaTypeDeleteHelper<TYPE>;
#else
#  define Q_DECL_METATYPE_HELPER(TYPE) \
     typedef void *(*QConstruct##TYPE)(const TYPE *); \
     static const QConstruct##TYPE qConstruct##TYPE = qMetaTypeConstructHelper<TYPE>; \
     typedef void (*QDestruct##TYPE)(TYPE *); \
     static const QDestruct##TYPE qDestruct##TYPE = qMetaTypeDeleteHelper<TYPE>; \
     typedef void (*QSave##TYPE)(QDataStream &, const TYPE *); \
     static const QSave##TYPE qSave##TYPE = qMetaTypeSaveHelper<TYPE>; \
     typedef void (*QLoad##TYPE)(QDataStream &, TYPE *); \
     static const QLoad##TYPE qLoad##TYPE = qMetaTypeLoadHelper<TYPE>;
#endif

#ifdef QT3_SUPPORT
Q_DECL_METATYPE_HELPER(QColorGroup)
#endif
Q_DECL_METATYPE_HELPER(QFont)
Q_DECL_METATYPE_HELPER(QPixmap)
Q_DECL_METATYPE_HELPER(QBrush)
Q_DECL_METATYPE_HELPER(QColor)
Q_DECL_METATYPE_HELPER(QPalette)
#ifndef QT_NO_ICON
Q_DECL_METATYPE_HELPER(QIcon)
#endif
Q_DECL_METATYPE_HELPER(QImage)
Q_DECL_METATYPE_HELPER(QPolygon)
Q_DECL_METATYPE_HELPER(QRegion)
Q_DECL_METATYPE_HELPER(QBitmap)
#ifndef QT_NO_CURSOR
Q_DECL_METATYPE_HELPER(QCursor)
#endif
Q_DECL_METATYPE_HELPER(QSizePolicy)
#ifndef QT_NO_SHORTCUT
Q_DECL_METATYPE_HELPER(QKeySequence)
#endif
Q_DECL_METATYPE_HELPER(QPen)
Q_DECL_METATYPE_HELPER(QTextLength)
Q_DECL_METATYPE_HELPER(QTextFormat)
Q_DECL_METATYPE_HELPER(QMatrix)
Q_DECL_METATYPE_HELPER(QTransform)

#ifdef QT_NO_DATASTREAM
#  define Q_IMPL_METATYPE_HELPER(TYPE) \
     { reinterpret_cast<QMetaType::Constructor>(qConstruct##TYPE), \
       reinterpret_cast<QMetaType::Destructor>(qDestruct##TYPE) }
#else
#  define Q_IMPL_METATYPE_HELPER(TYPE) \
     { reinterpret_cast<QMetaType::Constructor>(qConstruct##TYPE), \
       reinterpret_cast<QMetaType::Destructor>(qDestruct##TYPE), \
       reinterpret_cast<QMetaType::SaveOperator>(qSave##TYPE), \
       reinterpret_cast<QMetaType::LoadOperator>(qLoad##TYPE) \
     }
#endif

static const QMetaTypeGuiHelper qVariantGuiHelper[] = {
#ifdef QT3_SUPPORT
    Q_IMPL_METATYPE_HELPER(QColorGroup),
#else
    {0, 0, 0, 0},
#endif
    Q_IMPL_METATYPE_HELPER(QFont),
    Q_IMPL_METATYPE_HELPER(QPixmap),
    Q_IMPL_METATYPE_HELPER(QBrush),
    Q_IMPL_METATYPE_HELPER(QColor),
    Q_IMPL_METATYPE_HELPER(QPalette),
#ifdef QT_NO_ICON
    {0, 0, 0, 0},
#else
    Q_IMPL_METATYPE_HELPER(QIcon),
#endif
    Q_IMPL_METATYPE_HELPER(QImage),
    Q_IMPL_METATYPE_HELPER(QPolygon),
    Q_IMPL_METATYPE_HELPER(QRegion),
    Q_IMPL_METATYPE_HELPER(QBitmap),
#ifdef QT_NO_CURSOR
    {0, 0, 0, 0},
#else
    Q_IMPL_METATYPE_HELPER(QCursor),
#endif
    Q_IMPL_METATYPE_HELPER(QSizePolicy),
#ifdef QT_NO_SHORTCUT
    {0, 0, 0, 0},
#else
    Q_IMPL_METATYPE_HELPER(QKeySequence),
#endif
    Q_IMPL_METATYPE_HELPER(QPen),
    Q_IMPL_METATYPE_HELPER(QTextLength),
    Q_IMPL_METATYPE_HELPER(QTextFormat),
    Q_IMPL_METATYPE_HELPER(QMatrix),
    Q_IMPL_METATYPE_HELPER(QTransform)
};

static const QVariant::Handler *qt_guivariant_last_handler = 0;
int qRegisterGuiVariant()
{
    qt_guivariant_last_handler = QVariant::handler;
    QVariant::handler = &qt_gui_variant_handler;
    qMetaTypeGuiHelper = qVariantGuiHelper;
    return 1;
}
Q_CONSTRUCTOR_FUNCTION(qRegisterGuiVariant)

int qUnregisterGuiVariant()
{
    QVariant::handler = qt_guivariant_last_handler;
    qMetaTypeGuiHelper = 0;
    return 1;
}
Q_DESTRUCTOR_FUNCTION(qUnregisterGuiVariant)

QT_END_NAMESPACE
