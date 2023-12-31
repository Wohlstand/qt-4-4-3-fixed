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

#ifndef Patternist_Atomizer_H
#define Patternist_Atomizer_H

#include "qitem_p.h"
#include "qsinglecontainer_p.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

namespace QPatternist
{

    /**
     * @short Performs atomization. Effectively, it is an implementation
     * of the <tt>fn:data()</tt> function.
     *
     * @see <a href="http://www.w3.org/TR/xpath-functions/#func-data">XQuery 1.0 and XPath
     * 2.0 Functions and Operators, 2.4 fn:data</a>
     * @see <a href="http://www.w3.org/TR/xpath20/#id-atomization">XML
     * Path Language (XPath) 2.0, 2.4.2 Atomization</a>
     * @author Frans Englich <fenglich@trolltech.com>
     * @ingroup Patternist_expressions
     */
    class Atomizer : public SingleContainer
    {
    public:
        Atomizer(const Expression::Ptr &operand);

        virtual Item evaluateSingleton(const DynamicContext::Ptr &) const;
        virtual Item::Iterator::Ptr evaluateSequence(const DynamicContext::Ptr &) const;

        virtual SequenceType::Ptr staticType() const;

        virtual SequenceType::List expectedOperandTypes() const;
        virtual const SourceLocationReflection *actualReflection() const;

        /**
         * Makes an early compression, by returning the result of
         * the type checked operand, if the operand has the static type
         * xs:anyAtomicType(no atomization needed).
         */
        virtual Expression::Ptr typeCheck(const StaticContext::Ptr &context,
                                          const SequenceType::Ptr &reqType);

        inline Item::Iterator::Ptr mapToSequence(const Item &item,
                                                 const DynamicContext::Ptr &context) const;
        virtual ExpressionVisitorResult::Ptr accept(const ExpressionVisitor::Ptr &visitor) const;

    private:
        typedef QExplicitlySharedDataPointer<const Atomizer> ConstPtr;
    };
}

QT_END_NAMESPACE

QT_END_HEADER

#endif
