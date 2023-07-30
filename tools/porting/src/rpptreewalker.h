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

#ifndef RPPTREEWALKER_H
#define RPPTREEWALKER_H

#include "rpp.h"

QT_BEGIN_NAMESPACE

namespace Rpp {

class RppTreeWalker
{
public:
    virtual ~RppTreeWalker(){};
    virtual void evaluateItem(const Item *item);
    virtual void evaluateItemComposite(const ItemComposite *itemComposite);
    virtual void evaluateSource(const Source *source);
    virtual void evaluateDirective(const Directive *directive);
    virtual void evaluateIfSection(const IfSection *ifSection);
    virtual void evaluateConditionalDirective(const ConditionalDirective *conditionalDirective);

    virtual void evaluateText(const Text *textLine) {Q_UNUSED(textLine);}
    virtual void evaluateEmptyDirective(const EmptyDirective *directive) {Q_UNUSED(directive);}
    virtual void evaluateErrorDirective(const ErrorDirective *directive) {Q_UNUSED(directive);}
    virtual void evaluatePragmaDirective(const PragmaDirective *directive) {Q_UNUSED(directive);}
    virtual void evaluateIncludeDirective(const IncludeDirective *directive) {Q_UNUSED(directive);}
    virtual void evaluateDefineDirective(const DefineDirective *directive) {Q_UNUSED(directive);}
    virtual void evaluateUndefDirective(const UndefDirective *directive) {Q_UNUSED(directive);}
    virtual void evaluateLineDirective(const LineDirective *directive) {Q_UNUSED(directive);}
    virtual void evaluateNonDirective(const NonDirective *directive) {Q_UNUSED(directive);}

    virtual void evaluateIfdefDirective(const IfdefDirective *directive);
    virtual void evaluateIfndefDirective(const IfndefDirective *directive);
    virtual void evaluateIfDirective(const IfDirective *directive);
    virtual void evaluateElifDirective(const ElifDirective *directive);
    virtual void evaluateElseDirective(const ElseDirective *directive);

    virtual void evaluateEndifDirective(const EndifDirective *directive) {Q_UNUSED(directive);}
};

}

QT_END_NAMESPACE

#endif
