/****************************************************************************
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the QtXMLPatterns module of the Qt Toolkit.
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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#ifndef Patternist_BuiltinTypes_H
#define Patternist_BuiltinTypes_H

#include "qanynodetype_p.h"
#include "qanysimpletype_p.h"
#include "qanytype_p.h"
#include "qbuiltinatomictype_p.h"
#include "qitemtype_p.h"
#include "qnumerictype_p.h"
#include "quntyped_p.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

namespace QPatternist
{

    /**
     * @short Provides access to singleton instances of ItemType and SchemaType sub-classes.
     *
     * @ingroup Patternist_types
     * @author Frans Englich <fenglich@trolltech.com>
     */
    class Q_AUTOTEST_EXPORT BuiltinTypes
    {
    public:
        static const SchemaType::Ptr        xsAnyType;
        static const SchemaType::Ptr        xsAnySimpleType;
        static const SchemaType::Ptr        xsUntyped;

        static const AtomicType::Ptr        xsAnyAtomicType;
        static const AtomicType::Ptr        xsUntypedAtomic;
        static const AtomicType::Ptr        xsDateTime;
        static const AtomicType::Ptr        xsDate;
        static const AtomicType::Ptr        xsTime;
        static const AtomicType::Ptr        xsDuration;
        static const AtomicType::Ptr        xsYearMonthDuration;
        static const AtomicType::Ptr        xsDayTimeDuration;

        /**
         * An artificial type for implementation purposes
         * that represents the XPath type @c numeric.
         */
        static const AtomicType::Ptr        numeric;
        static const AtomicType::Ptr        xsFloat;
        static const AtomicType::Ptr        xsDouble;
        static const AtomicType::Ptr        xsInteger;
        static const AtomicType::Ptr        xsDecimal;
        static const AtomicType::Ptr        xsNonPositiveInteger;
        static const AtomicType::Ptr        xsNegativeInteger;
        static const AtomicType::Ptr        xsLong;
        static const AtomicType::Ptr        xsInt;
        static const AtomicType::Ptr        xsShort;
        static const AtomicType::Ptr        xsByte;
        static const AtomicType::Ptr        xsNonNegativeInteger;
        static const AtomicType::Ptr        xsUnsignedLong;
        static const AtomicType::Ptr        xsUnsignedInt;
        static const AtomicType::Ptr        xsUnsignedShort;
        static const AtomicType::Ptr        xsUnsignedByte;
        static const AtomicType::Ptr        xsPositiveInteger;


        static const AtomicType::Ptr        xsGYearMonth;
        static const AtomicType::Ptr        xsGYear;
        static const AtomicType::Ptr        xsGMonthDay;
        static const AtomicType::Ptr        xsGDay;
        static const AtomicType::Ptr        xsGMonth;

        static const AtomicType::Ptr        xsBoolean;

        static const AtomicType::Ptr        xsBase64Binary;
        static const AtomicType::Ptr        xsHexBinary;
        static const AtomicType::Ptr        xsAnyURI;
        static const AtomicType::Ptr        xsQName;
        static const AtomicType::Ptr        xsString;
        static const AtomicType::Ptr        xsNormalizedString;
        static const AtomicType::Ptr        xsToken;
        static const AtomicType::Ptr        xsLanguage;
        static const AtomicType::Ptr        xsNMTOKEN;
        static const AtomicType::Ptr        xsName;
        static const AtomicType::Ptr        xsNCName;
        static const AtomicType::Ptr        xsID;
        static const AtomicType::Ptr        xsIDREF;
        static const AtomicType::Ptr        xsENTITY;

        static const AtomicType::Ptr        xsNOTATION;
        static const ItemType::Ptr          item;

        static const AnyNodeType::Ptr       node;
        static const ItemType::Ptr          attribute;
        static const ItemType::Ptr          comment;
        static const ItemType::Ptr          document;
        static const ItemType::Ptr          element;
        static const ItemType::Ptr          pi;
        static const ItemType::Ptr          text;

    private:
        /**
         * The constructor is protected because this class is not meant to be instantiated,
         * but should only be used via its static const members.
         */
        BuiltinTypes();
        Q_DISABLE_COPY(BuiltinTypes)
    };
}

QT_END_NAMESPACE

QT_END_HEADER

#endif

