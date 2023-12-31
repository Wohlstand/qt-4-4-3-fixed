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

#ifndef Patternist_RangeIterator_H
#define Patternist_RangeIterator_H

#include "qitem_p.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

namespace QPatternist
{
    /**
     * @short RangeIterator represents a sequence of integers between a
     * start and end value.
     *
     * The RangeIterator contains the evaluation logic for the range expression, <tt>N to M</tt>,
     * and its behavior is therefore consistent with the definition of that XPath expression.
     * Hence, the detailed behavior of the RangeIterator can be found in the XPath 2.0
     * specification.
     *
     * @see <a href="http://www.w3.org/TR/xpath20/\#doc-xpath-RangeExpr">XML Path Language
     * (XPath) 2.0, 3.3 Sequence Expressions, RangeExpr</a>
     * @see RangeExpression
     * @author Frans Englich <fenglich@trolltech.com>
     * @ingroup Patternist_iterators
     * @todo Documentation is missing
     */
    class Q_AUTOTEST_EXPORT RangeIterator : public Item::Iterator
    {
    public:

        /**
         * RangeIterator can iterate in both directions.
         * This enumerator exist for identifying different directions.
         */
        enum Direction
        {
            /**
             * Signifies that the QAbstractXmlForwardIterator operates in a reverse direction, where the
             * first item returned by the next() function is from the beginning of the
             * source sequence.
             */
            Backward = 0,

            /**
             * Signifies the forward direction. Iterators do conceptually operate
             * in the forward direction by default.
             */
            Forward = 1
        };

        /**
         * Creates an QAbstractXmlForwardIterator that returns integer values from consequtive sequence
         * of integers between @p start and @p end, where the the step taken
         * between each integer is 1 with polarity as specified in @p direction.
         *
         * @note @p start must be smaller than @p end, not larger
         * or equal. This is not checked.
         */
        RangeIterator(const xsInteger start,
                      const Direction direction,
                      const xsInteger end);

        virtual Item next();
        virtual Item current() const;
        virtual xsInteger position() const;
        virtual xsInteger count();
        virtual Item::Iterator::Ptr toReversed();
        virtual Item::Iterator::Ptr copy() const;

    private:
        xsInteger m_start;
        xsInteger m_end;
        Item m_current;
        xsInteger m_position;
        xsInteger m_count;
        const Direction m_direction;

        /**
         * We only need to store -1 or 1, so save memory with a bit field.
         */
        const qint8 m_increment : 2;
    };
}

QT_END_NAMESPACE

QT_END_HEADER

#endif
