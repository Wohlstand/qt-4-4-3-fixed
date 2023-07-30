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

#include "rpptreewalker.h"

QT_BEGIN_NAMESPACE

namespace Rpp {

void RppTreeWalker::evaluateItem(const Item *item)
{
    if(!item)
        return;
    if (Source *source = item->toSource())
        evaluateSource(source);
    else if (Directive *directive = item->toDirective())
        evaluateDirective(directive);
    else if (IfSection *ifSection = item->toIfSection())
        evaluateIfSection(ifSection);
    else if (Text *text = item->toText())
        evaluateText(text);
}

void RppTreeWalker::evaluateItemComposite(const ItemComposite *itemComposite)
{
    if (!itemComposite)
        return;
    for (int i = 0; i < itemComposite->count(); ++i) {
        evaluateItem(itemComposite->item(i));
    }
}

void RppTreeWalker::evaluateSource(const Source *source)
{
    evaluateItemComposite(source->toItemComposite());
}

void RppTreeWalker::evaluateDirective(const Directive *directive)
{
    if (!directive)
        return;
    if (EmptyDirective *dir = directive->toEmptyDirective())
        evaluateEmptyDirective(dir);
    else if (ErrorDirective *dir = directive->toErrorDirective())
        evaluateErrorDirective(dir);
    else if (PragmaDirective *dir = directive->toPragmaDirective())
        evaluatePragmaDirective(dir);
    else if (IncludeDirective *dir = directive->toIncludeDirective())
        evaluateIncludeDirective(dir);
    else if (DefineDirective *dir = directive->toDefineDirective())
        evaluateDefineDirective(dir);
    else if (UndefDirective *dir = directive->toUndefDirective())
        evaluateUndefDirective(dir);
    else if (LineDirective *dir = directive->toLineDirective())
        evaluateLineDirective(dir);
    else if (NonDirective *dir = directive->toNonDirective())
        evaluateNonDirective(dir);
    else if (NonDirective *dir = directive->toNonDirective())
        evaluateNonDirective(dir);
    else if (ConditionalDirective *dir = directive->toConditionalDirective())
        evaluateConditionalDirective(dir);
}

/*
    This function evaluates all the branches of an IfSection. You should 
    override it if you want to only evaluate the "correct" branch.
*/
void RppTreeWalker::evaluateIfSection(const IfSection *ifSection)
{
    if (!ifSection)
        return;
    evaluateItemComposite(ifSection->toItemComposite());
}

void RppTreeWalker::evaluateConditionalDirective(const ConditionalDirective *conditionalDirective)
{
    if (!conditionalDirective)
        return;
    if (IfdefDirective *dir = conditionalDirective->toIfdefDirective())
         evaluateIfdefDirective(dir);
    else if (IfndefDirective *dir = conditionalDirective->toIfndefDirective())
         evaluateIfndefDirective(dir);
    else if (IfDirective *dir = conditionalDirective->toIfDirective())
         evaluateIfDirective(dir);
    else if (ElifDirective *dir = conditionalDirective->toElifDirective())
         evaluateElifDirective(dir);
    else if (ElseDirective *dir = conditionalDirective->toElseDirective())
         evaluateElseDirective(dir);
}

void RppTreeWalker::evaluateIfdefDirective(const IfdefDirective *directive)
{
   if (!directive) 
       return;
   evaluateItemComposite(directive->toItemComposite());
}

void RppTreeWalker::evaluateIfndefDirective(const IfndefDirective *directive)
{
   if (!directive) 
       return;
   evaluateItemComposite(directive->toItemComposite());
}

void RppTreeWalker::evaluateIfDirective(const IfDirective *directive)
{
   if (!directive) 
       return;
   evaluateItemComposite(directive->toItemComposite());
}

void RppTreeWalker::evaluateElifDirective(const ElifDirective *directive)
{
   if (!directive) 
       return;
   evaluateItemComposite(directive->toItemComposite());
}

void RppTreeWalker::evaluateElseDirective(const ElseDirective *directive)
{
   if (!directive) 
       return;
   evaluateItemComposite(directive->toItemComposite());
}

}

QT_END_NAMESPACE
