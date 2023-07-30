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

#ifndef RPPTREEEVALUATOR_H
#define RPPTREEEVALUATOR_H

#include "tokenengine.h"
#include "rpp.h"
#include "rpptreewalker.h"
#include <QObject>
#include <QList>
#include <QHash>
#include <QSet>

QT_BEGIN_NAMESPACE

namespace Rpp {

class DefineMap : public QHash<QByteArray, const DefineDirective *>
{

};

class RppTreeEvaluator: public QObject, public RppTreeWalker
{
Q_OBJECT
public:
    RppTreeEvaluator();
    ~RppTreeEvaluator();
    TokenEngine::TokenSectionSequence evaluate(const Source *source,
                                              DefineMap *activedefinitions);
    enum IncludeType {QuoteInclude, AngleBracketInclude};
signals:
    void includeCallback(Rpp::Source *&includee,
                         const Rpp::Source *includer,
                         const QString &filename,
                         Rpp::RppTreeEvaluator::IncludeType includeType);
protected:
    void evaluateIncludeDirective(const IncludeDirective *directive);
    void evaluateDefineDirective(const DefineDirective *directive);
    void evaluateUndefDirective(const UndefDirective *directive);
    void evaluateIfSection(const IfSection *ifSection);
    void evaluateText(const Text *text);
    bool evaluateCondition(const ConditionalDirective *conditionalDirective);
    int evaluateExpression(Expression *expression);

    TokenEngine::TokenContainer evaluateMacro(TokenEngine::TokenContainer tokenContainer, int &identiferTokenIndex);
    TokenEngine::TokenContainer evaluateMacroInternal(QSet<QByteArray> skip, TokenEngine::TokenContainer tokenContainer);
    TokenEngine::TokenContainer cloneTokenList(const TokenEngine::TokenList &list);
    Source *getParentSource(const Item *item) const;
    IncludeType includeTypeFromDirective(
                    const IncludeDirective *includeDirective) const;
private:
    QVector<TokenEngine::TokenSection> m_tokenSections;
    DefineMap *m_activeDefinitions;
    TokenEngine::TokenSection *newlineSection;
};

class MacroFunctionParser
{
public:
    MacroFunctionParser(const TokenEngine::TokenContainer &tokenContainer, int startToken);
    bool isValid();
    int tokenCount();
    int argumentCount();
    TokenEngine::TokenSection argument(int argumentIndex);
private:
    const TokenEngine::TokenContainer &m_tokenContainer;
    const int m_startToken;
    int m_numTokens;
    bool m_valid;
    QVector<TokenEngine::TokenSection> m_arguments;
};

}//namespace Rpp

QT_END_NAMESPACE

#endif
