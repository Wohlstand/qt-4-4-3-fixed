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

#ifndef Patternist_AtomicTypeDispatch_H
#define Patternist_AtomicTypeDispatch_H

#include <QSharedData>


QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

namespace QPatternist
{
    class AnyAtomicType;
    class AnyURIType;
    class Base64BinaryType;
    class BooleanType;
    class DateTimeType;
    class DateType;
    class DayTimeDurationType;
    class DecimalType;
    class DoubleType;
    class DurationType;
    class FloatType;
    class GDayType;
    class GMonthDayType;
    class GMonthType;
    class GYearMonthType;
    class GYearType;
    class HexBinaryType;
    class IntegerType;
    class NOTATIONType;
    class QNameType;
    class SourceLocationReflection;
    class StringType;
    class SchemaTimeType;
    class UntypedAtomicType;
    class YearMonthDurationType;

    enum TypeOfDerivedInteger
    {
        TypeByte,
        TypeInt,
        TypeLong,
        TypeNegativeInteger,
        TypeNonNegativeInteger,
        TypeNonPositiveInteger,
        TypePositiveInteger,
        TypeShort,
        TypeUnsignedByte,
        TypeUnsignedInt,
        TypeUnsignedLong,
        TypeUnsignedShort
    };

    template<TypeOfDerivedInteger DerivedType> class DerivedIntegerType;

    enum TypeOfDerivedString
    {
        TypeString,
        TypeNormalizedString,
        TypeToken,
        TypeLanguage,
        TypeNMTOKEN,
        TypeName,
        TypeNCName,
        TypeID,
        TypeIDREF,
        TypeENTITY
    };

    template<TypeOfDerivedString DerivedType> class DerivedStringType;

    /**
     * @todo Documentation's missing:
     * - Add link to wikipedia's "multiple dispatch" and "visitor" page.
     * - Add link to http://www.eptacom.net/pubblicazioni/pub_eng/mdisp.html
     *
     * @defgroup Patternist_types_dispatch Atomic Type Dispatching
     */

    /**
     * @todo Docs missing
     *
     * @ingroup Patternist_types_dispatch
     * @author Frans Englich <fenglich@trolltech.com>
     */
    class AtomicTypeVisitorResult : public QSharedData
    {
    public:
        typedef QExplicitlySharedDataPointer<AtomicTypeVisitorResult> Ptr;
        AtomicTypeVisitorResult() {}
        virtual ~AtomicTypeVisitorResult() {}
    };

    /**
     * @todo Docs missing
     *
     * @see ParameterizedAtomicTypeVisitor
     * @ingroup Patternist_types_dispatch
     * @author Frans Englich <fenglich@trolltech.com>
     */
    class AtomicTypeVisitor : public QSharedData
    {
    public:
        typedef QExplicitlySharedDataPointer<AtomicTypeVisitor> Ptr;
        virtual ~AtomicTypeVisitor() {}

        virtual AtomicTypeVisitorResult::Ptr visit(const AnyAtomicType *,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const AnyURIType *,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const Base64BinaryType *,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const BooleanType *,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const DateTimeType *,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const DateType *,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const DayTimeDurationType *,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const DecimalType *,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const DoubleType *,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const DurationType *,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const FloatType *,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const GDayType *,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const GMonthDayType *,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const GMonthType *,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const GYearMonthType *,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const GYearType *,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const HexBinaryType *,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const IntegerType *,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const NOTATIONType *,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const QNameType *,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const StringType *,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const SchemaTimeType *,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const UntypedAtomicType *,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const YearMonthDurationType *,
                                                   const SourceLocationReflection *const reflection) const = 0;
    };

    /**
     * @todo Docs missing
     *
     * @see AtomicTypeVisitor
     * @ingroup Patternist_types_dispatch
     * @author Frans Englich <fenglich@trolltech.com>
     */
    class ParameterizedAtomicTypeVisitor : public QSharedData
    {
    public:
        typedef QExplicitlySharedDataPointer<ParameterizedAtomicTypeVisitor> Ptr;
        virtual ~ParameterizedAtomicTypeVisitor() {}

        virtual AtomicTypeVisitorResult::Ptr visit(const AnyAtomicType *, const qint16 param,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const AnyURIType *, const qint16 param,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const Base64BinaryType *, const qint16 param,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const BooleanType *, const qint16 param,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const DateTimeType *, const qint16 param,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const DateType *, const qint16 param,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const DayTimeDurationType *, const qint16 param,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const DecimalType *, const qint16 param,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const DoubleType *, const qint16 param,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const DurationType *, const qint16 param,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const FloatType *, const qint16 param,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const GDayType *, const qint16 param,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const GMonthDayType *, const qint16 param,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const GMonthType *, const qint16 param,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const GYearMonthType *, const qint16 param,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const GYearType *, const qint16 param,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const HexBinaryType *, const qint16 param,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const IntegerType *, const qint16 param,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const NOTATIONType *, const qint16 param,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const QNameType *, const qint16 param,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const StringType *, const qint16 param,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const SchemaTimeType *, const qint16 param,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const UntypedAtomicType *, const qint16 param,
                                                   const SourceLocationReflection *const reflection) const = 0;
        virtual AtomicTypeVisitorResult::Ptr visit(const YearMonthDurationType *, const qint16 param,
                                                   const SourceLocationReflection *const reflection) const = 0;
    };
}

QT_END_NAMESPACE

QT_END_HEADER

#endif
