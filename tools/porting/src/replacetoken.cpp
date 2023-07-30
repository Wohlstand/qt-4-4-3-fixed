/****************************************************************************
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
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

#include "replacetoken.h"
#include "tokenreplacements.h"
#include <QByteArray>

QT_BEGIN_NAMESPACE

/*
    Add an entry to the tokenRuleLookup map for each token replacement rule.
*/
ReplaceToken::ReplaceToken(const QList<TokenReplacement*> &tokenReplacementRules)
:tokenReplacementRules(tokenReplacementRules)
{
    foreach (TokenReplacement* rep, tokenReplacementRules) {
        QByteArray key = rep->getReplaceKey();
        if(!key.isEmpty()) {
            tokenRuleLookup.insert(key, rep);
        }
    }
}

TextReplacements ReplaceToken::getTokenTextReplacements(const TokenEngine::TokenContainer &container)
{
    TextReplacements textReplacements;

    int t=0;
    const int numTokens = container.count();
    while(t < numTokens) {
        QByteArray tokenText = container.text(t);
        bool changed = false;

        if(isPreprocessorDirective(tokenText)) {
            foreach(TokenReplacement *tokenReplacementRule, tokenReplacementRules) {
                if(!changed)
                    changed = tokenReplacementRule->doReplace(container, t, textReplacements);
                if(changed)
                    break;
            }
        } else if (isInterestingToken(tokenText.trimmed())) {
            foreach (TokenReplacement* value, tokenRuleLookup.values(tokenText)) {
                changed = value->doReplace(container, t, textReplacements);
                if(changed) {
                    goto end;
                }
            }
        }
    end:
        ++t;
    }
    return textReplacements;
}

bool ReplaceToken::isInterestingToken(const QByteArray &text)
{
    return !(text.isEmpty() || text==";" || text=="(" || text==")" || text=="{" || text=="}" || text=="="
            || text=="+=" || text=="-=" || text=="if" || text=="then" || text=="else"
    );
}

bool ReplaceToken::isPreprocessorDirective(const QByteArray &token)
{
    return (token[0]=='#');
}

QT_END_NAMESPACE
