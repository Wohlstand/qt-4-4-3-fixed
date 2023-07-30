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

#ifndef TOKENREPLACEMENTS_H
#define TOKENREPLACEMENTS_H

#include "tokenengine.h"
#include "textreplacement.h"
#include <QStringList>
#include <QByteArray>

QT_BEGIN_NAMESPACE

void addLogSourceEntry(const QString &text, const TokenEngine::TokenContainer&, const int index);
void addLogWarning(const QString &text);

class TokenReplacement
{
public:

    virtual bool doReplace(const TokenEngine::TokenContainer& ,
                           int /*tokenIndex*/,
                           TextReplacements&){return false;};
    /*
        returns the replace key for this replacement. Every time a token matches the replace key,
        doReplace() will be called for this TokenReplacement.
    */
    virtual QByteArray getReplaceKey(){return QByteArray();};
    virtual ~TokenReplacement(){};
};

/*
    A TokenReplacement that change any token
*/
class GenericTokenReplacement : public TokenReplacement
{
public:
    GenericTokenReplacement(QByteArray oldToken, QByteArray newToken);
    bool doReplace(const TokenEngine::TokenContainer &tokenContainer,
                   int tokenIndex, TextReplacements &textReplacements);
    QByteArray getReplaceKey();
private:
    QByteArray oldToken;
    QByteArray newToken;
};

/*
    A TokenReplacement that changes tokens that specify class names.
    In some cases where the class name token is a part of a qualified name
    it is not correct to rename it. ex:

    QButton::toggleState

    Here it is wrong to rename QButton -> Q3Button, since there is
    a rule that says QButton::ToggleState -> QCheckBox::ToggleState,
    but no rule for Q3Button::ToggleState.
*/
class ClassNameReplacement : public TokenReplacement
{
public:
    ClassNameReplacement(QByteArray oldToken, QByteArray newToken);
    bool doReplace(const TokenEngine::TokenContainer &tokenContainer,
                   int tokenIndex, TextReplacements &textReplacements);
    QByteArray getReplaceKey();
private:
    QByteArray oldToken;
    QByteArray newToken;
};

/*
   Changes scoped tokens:
   AA::BB -> CC::DD
   oldToken corresponds to the AA::BB part, newToken corresponds CC::DD.
   Since this is a token replacement, the AA part of oldToken is typically
   unknown. This means that we might change tokens named BB that does not belong
   to the AA scope. Ast replacemnts will fix this.

*/
class ScopedTokenReplacement : public TokenReplacement
{
public:
    ScopedTokenReplacement(const QByteArray &oldToken, const QByteArray &newToken);
    bool doReplace(const TokenEngine::TokenContainer &tokenContainer,
                   int tokenIndex, TextReplacements &textReplacements);
    QByteArray getReplaceKey();
private:
    QByteArray oldName;
    QByteArray oldScope;
    QByteArray newName;
    QByteArray newScope;
    QByteArray newScopedName;
    bool strictMode;
};

class QualifiedNameParser
{
public:
    QualifiedNameParser(const TokenEngine::TokenContainer &tokenContainer,
                        const int tokenIndex);
    enum Direction { Left=-1, Right=1 };
    bool isPartOfQualifiedName();
    bool isValidIndex(int index);
    bool isQualifier();
    bool isName();
    int peek(Direction direction);
    int move(Direction direction);
private:
    int nextScopeToken(Direction direction);
    int findScopeOperator(Direction direction);
    const TokenEngine::TokenContainer tokenContainer;
    int currentIndex;
};

QT_END_NAMESPACE

#endif
