/****************************************************************************
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
** Copyright (C) 2001-2004 Roberto Raggi
**
** This file is part of the qt3to4 porting application of the Qt Toolkit.
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

#ifndef TOKENSTREAMADAPTER_H
#define TOKENSTREAMADAPTER_H

#include "tokenengine.h"
#include "tokens.h"

#include <QVector>

QT_BEGIN_NAMESPACE

namespace TokenStreamAdapter {
struct TokenStream
{
    TokenStream(TokenEngine::TokenSectionSequence translationUnit, QVector<Type> tokenKindList)
    :m_translationUnit(translationUnit),
     m_tokenKindList(tokenKindList),
     m_cursor(0),
     m_numTokens(tokenKindList.count())
    {
        Q_ASSERT(translationUnit.count() == m_numTokens);

        // Copy out the container and containerIndex for each token so we can have
        // constant time random access to it.
        TokenEngine::TokenSectionSequenceIterator it(translationUnit);
        while(it.nextToken()) {
            m_tokenContainers.append(it.tokenContainer());
            m_containerIndices.append(it.containerIndex());
        }
    }

    bool isHidden(int index) const
    {
       if(index >= m_numTokens)
            return false;
        QT_PREPEND_NAMESPACE(Type) type = m_tokenKindList.at(index);
        return (type == Token_whitespaces || type == 10 /*newline*/ ||
                type == Token_comment || type == Token_preproc );
    }

    inline int lookAhead(int n = 0) const
    {
        if(m_cursor + n >= m_numTokens)
            return 0;
        return m_tokenKindList.at(m_cursor + n);
    }

    inline int currentToken() const
    { return lookAhead(); }

    inline QByteArray currentTokenText() const
    {
        return tokenText(m_cursor);
    }

    inline TokenEngine::TokenContainer tokenContainer(int index = 0) const
    {
        if (index < m_numTokens)
            return m_tokenContainers.at(index);
        else
            return TokenEngine::TokenContainer();
    }

    inline int containerIndex(int index = 0) const
    {
        if (index < m_numTokens)
            return m_containerIndices.at(index);
        else
            return -1;
    }

    inline QByteArray tokenText(int index = 0) const
    {
        if (index <  m_numTokens) {
            const TokenEngine::TokenContainer container = tokenContainer(index);
            const int cIndex = containerIndex(index);
            return container.text(cIndex);
        } else {
            return QByteArray();
        }
    }

    inline void rewind(int index)
    { m_cursor = index; }

    inline int cursor() const
    { return m_cursor; }

    inline void nextToken()
    { ++m_cursor; }

    inline bool tokenAtEnd()
    { return m_cursor >= m_numTokens; }

    TokenEngine::TokenSectionSequence tokenSections() const
    { return m_translationUnit;  }

private:
   TokenEngine::TokenSectionSequence m_translationUnit;
   QVector<Type> m_tokenKindList;
   QList<TokenEngine::TokenContainer> m_tokenContainers;
   QList<int> m_containerIndices;
   int m_cursor;
   int m_numTokens;
};

} //namespace TokenStreamAdapter

QT_END_NAMESPACE

#endif
