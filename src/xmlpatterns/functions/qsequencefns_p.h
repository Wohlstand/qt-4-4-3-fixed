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

#ifndef Patternist_SequenceFNs_H
#define Patternist_SequenceFNs_H

#include "qatomiccomparator_p.h"
#include "qcomparisonplatform_p.h"
#include "qliteral_p.h"
#include "qfunctioncall_p.h"

/**
 * @file
 * @short Contains classes implementing the functions found in
 * <a href="http://www.w3.org/TR/xpath-functions/#general-seq-funcs">XQuery 1.0 and
 * XPath 2.0 Functions and Operators, 15.1 General Functions and Operators on Sequences</a>.
 *
 * @todo document that some functions have both eval funcs implented.
 *
 * @ingroup Patternist_functions
 */

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

namespace QPatternist
{
    /**
     * @short Implements the function <tt>fn:boolean()</tt>.
     *
     * @ingroup Patternist_functions
     * @author Frans Englich <fenglich@trolltech.com>
     */
    class BooleanFN : public FunctionCall
    {
    public:
        virtual bool evaluateEBV(const DynamicContext::Ptr &context) const;

        /**
         * If @p reqType is CommonSequenceTypes::EBV, the type check of
         * the operand is returned. Hence, this removes redundant calls
         * to <tt>fn:boolean()</tt>.
         */
        virtual Expression::Ptr typeCheck(const StaticContext::Ptr &context,
                                          const SequenceType::Ptr &reqType);
    };

    /**
     * @short Implements the function <tt>fn:index-of()</tt>.
     *
     * @ingroup Patternist_functions
     * @author Frans Englich <fenglich@trolltech.com>
     */
    class IndexOfFN : public FunctionCall,
                      public ComparisonPlatform<IndexOfFN, false>
    {
    public:
        inline IndexOfFN() : ComparisonPlatform<IndexOfFN, false>()
        {
        }

        virtual Item::Iterator::Ptr evaluateSequence(const DynamicContext::Ptr &context) const;
        virtual Expression::Ptr typeCheck(const StaticContext::Ptr &context,
                                          const SequenceType::Ptr &reqType);

        inline AtomicComparator::Operator operatorID() const
        {
            return AtomicComparator::OperatorEqual;
        }
    };

    /**
     * @short Implements the functions <tt>fn:exists()</tt> and <tt>fn:empty()</tt>.
     *
     * Existence is a template value class. Appropriate implementations are achieved
     * by instantiating it with either IDExistsFN or IDEmptyFN.
     *
     * @ingroup Patternist_functions
     * @author Frans Englich <fenglich@trolltech.com>
     */
    template<const Expression::ID Id>
    class Existence : public FunctionCall
    {
    public:
        virtual bool evaluateEBV(const DynamicContext::Ptr &context) const
        {
            if(Id == IDExistsFN)
                return !m_operands.first()->evaluateSequence(context)->isEmpty();
            else
                return m_operands.first()->evaluateSequence(context)->isEmpty();
        }

        /**
         * Attempts to rewrite to @c false or @c true by looking at the static
         * cardinality of its operand.
         */
        virtual Expression::Ptr compress(const StaticContext::Ptr &context)
        {
            Q_ASSERT(Id == IDExistsFN || Id == IDEmptyFN);
            const Expression::Ptr me(FunctionCall::compress(context));

            if(me.data() != this)
                return me;

            const Cardinality myCard((Id == IDExistsFN) ? Cardinality::oneOrMore() : Cardinality::empty());

            const Cardinality card(m_operands.first()->staticType()->cardinality());
            if(myCard.isMatch(card))
            { /* Since the dynamic type always is narrower than the static type or equal, and that the
                 static type is in scope, it means we will always be true. */
                return wrapLiteral(CommonValues::BooleanTrue, context, this);
            }
            else
            {
                /* Is it even possible to hit? */
                if(myCard.canMatch(card))
                {
                    return me;
                }
                else
                { /* We can never hit. */
                    return wrapLiteral(CommonValues::BooleanFalse, context, this);
                }
            }
        }
    };

    /**
     * @short Implements the function <tt>fn:distinct-values()</tt>.
     *
     * @ingroup Patternist_functions
     * @author Frans Englich <fenglich@trolltech.com>
     */
    class DistinctValuesFN : public FunctionCall,
                             public ComparisonPlatform<IndexOfFN, false>
    {
    public:
        inline DistinctValuesFN() : ComparisonPlatform<IndexOfFN, false>()
        {
        }

        virtual Item::Iterator::Ptr evaluateSequence(const DynamicContext::Ptr &context) const;
        /**
         * Performs necessary type checks, but also implements the optimization
         * of rewriting to its operand if the operand's cardinality is zero-or-one
         * or exactly-one.
         */
        virtual Expression::Ptr typeCheck(const StaticContext::Ptr &context,
                                          const SequenceType::Ptr &reqType);
        /**
         * @returns a type whose item type is the type of the first operand, and
         * a cardinality which is non-empty if the first operand's type is non-empty
         * and allows exactly-one. The latter is needed for operands which has the
         * cardinality 2+, since distinct-values possibly removes items from the
         * source sequence.
         */
        virtual SequenceType::Ptr staticType() const;

    protected:
        inline AtomicComparator::Operator operatorID() const
        {
            return AtomicComparator::OperatorEqual;
        }
    };

    /**
     * @short Implements the function <tt>fn:insert-before()</tt>.
     *
     * @todo docs, explain why evaluateSequence and evaluateSingleton is implemented
     *
     * @ingroup Patternist_functions
     * @author Frans Englich <fenglich@trolltech.com>
     */
    class InsertBeforeFN : public FunctionCall
    {
    public:
        virtual Item::Iterator::Ptr evaluateSequence(const DynamicContext::Ptr &context) const;
        virtual Item evaluateSingleton(const DynamicContext::Ptr &context) const;

        /**
         * Implements the static enferences rules. The function's static item type
         * is the union type of the first and third argument, and the cardinality is
         * the cardinalities of the two operands added together. For example,
         * insert-before((1, "str"), 1, xs:double(0)) has the static type xs:anyAtomicType+.
         *
         * @see <a href="http://www.w3.org/TR/xquery-semantics/#sec_fn_insert_before">XQuery 1.0
         * and XPath 2.0 Formal Semantics, 7.2.15 The fn:insert-before function</a>
         */
        virtual SequenceType::Ptr staticType() const;
    };

    /**
     * @short Implements the function <tt>fn:remove()</tt>.
     *
     * @ingroup Patternist_functions
     * @author Frans Englich <fenglich@trolltech.com>
     */
    class RemoveFN : public FunctionCall
    {
    public:
        virtual Item::Iterator::Ptr evaluateSequence(const DynamicContext::Ptr &context) const;
        virtual Item evaluateSingleton(const DynamicContext::Ptr &context) const;

        /**
         * Implements the static enferences rules, "Since one item may be removed
         * from the sequence, the resulting type is made optional:"
         *
         * <tt>statEnv |-  (FN-URI,"remove")(Type, Type1) : prime(Type) * quantifier(Type)?</tt>
         *
         * However, because Patternist's type system is more fine grained than Formal Semantics,
         * the sequence isn't made optional. Instead its minimum length is reduced with one.
         *
         * @see <a href="http://www.w3.org/TR/xquery-semantics/#sec_fn_remove">XQuery 1.0
         * and XPath 2.0 Formal Semantics, 7.2.11 The fn:remove function</a>
         */
        virtual SequenceType::Ptr staticType() const;
    };

    /**
     * @short Implements the function <tt>fn:reverse()</tt>.
     *
     * @ingroup Patternist_functions
     * @author Frans Englich <fenglich@trolltech.com>
     */
    class ReverseFN : public FunctionCall
    {
    public:

        virtual Item::Iterator::Ptr evaluateSequence(const DynamicContext::Ptr &context) const;
        virtual Expression::Ptr typeCheck(const StaticContext::Ptr &context,
                                          const SequenceType::Ptr &reqType);

        /**
         * Formally speaking, the type inference is:
         *
@verbatim
statEnv |-  (FN-URI,"reverse")(Type) : prime(Type) * quantifier(Type)
@endverbatim
         *
         * @see <a href="http://www.w3.org/TR/xquery-semantics/#sec_fn_reverse">XQuery 1.0
         * and XPath 2.0 Formal Semantics, 7.2.12 The fn:reverse function</a>
         * @returns the static type of the function's first argument.
         */
        virtual SequenceType::Ptr staticType() const;
    };

    /**
     * @short Implements the function <tt>fn:subsequence()</tt>.
     *
     * @ingroup Patternist_functions
     * @author Frans Englich <fenglich@trolltech.com>
     * @todo Type inference can be made stronger for this function
     */
    class SubsequenceFN : public FunctionCall
    {
    public:
        virtual Item::Iterator::Ptr evaluateSequence(const DynamicContext::Ptr &context) const;
        virtual Item evaluateSingleton(const DynamicContext::Ptr &context) const;

        /**
         * This function implements rewrites the SubsequenceFN instance into an
         * empty sequence if its third argument, the sequence length argument, is
         * evaluated and is effectively equal or less than zero.
         */
        virtual Expression::Ptr compress(const StaticContext::Ptr &context);

        /**
         * Partially implements the static type inference rules.
         *
         * @see <a href="http://www.w3.org/TR/xquery-semantics/#sec_fn_subsequence">XQuery 1.0
         * and XPath 2.0 Formal Semantics, 7.2.13 The fn:subsequence function</a>
         */
        virtual SequenceType::Ptr staticType() const;
    };
}

QT_END_NAMESPACE

QT_END_HEADER

#endif
