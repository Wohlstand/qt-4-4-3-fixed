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

#ifndef Patternist_String_H
#define Patternist_String_H

#include <QUrl>

#include "qitem_p.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

namespace QPatternist
{
    /**
     * @short Implements the value instance of the @c xs:string type.
     *
     *
     * This class was originally called String, and correspondingly the header
     * file was called String.h. However, this broke building on OS X, which
     * looks up file names case insensitively, and therefore found string.h.
     *
     * @author Frans Englich <fenglich@trolltech.com>
     * @ingroup Patternist_xdm
     * @todo Documentation is missing/incomplete
     */
    class Q_AUTOTEST_EXPORT AtomicString : public AtomicValue
    {
    public:
        friend class CommonValues;

        typedef AtomicValue::Ptr Ptr;

        /**
         * Creates an instance representing @p value.
         *
         * @note This function does not remove the string literal escaping allowed in XPath 2.0
         */
        static AtomicString::Ptr fromValue(const QString &value);

        static inline AtomicString::Ptr fromValue(const QUrl &value)
        {
            return fromValue(value.toString());
        }

        /**
         * Get the Effective %Boolean Value of this string. A zero-length
         * string has an effective boolean value of @c false, in all other cases @c true.
         *
         * @returns @c false if the contained string has a zero-length, otherwise @c true.
         */
        virtual bool evaluateEBV(const QExplicitlySharedDataPointer<DynamicContext> &) const;

        /**
         * The string value of a AtomicString instance is the value space.
         */
        virtual QString stringValue() const;

        virtual ItemType::Ptr type() const;

    protected:
        friend class StringComparator;
        friend class CompareFN;
        AtomicString(const QString &value);
        const QString m_value;
    };
}

QT_END_NAMESPACE

QT_END_HEADER

#endif
