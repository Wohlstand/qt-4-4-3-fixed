/****************************************************************************
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the Qt3Support module of the Qt Toolkit.
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

#ifndef Q3SYNTAXHIGHLIGHTER_P_H
#define Q3SYNTAXHIGHLIGHTER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#ifndef QT_NO_SYNTAXHIGHLIGHTER
#include "q3syntaxhighlighter.h"
#include "private/q3richtext_p.h"

QT_BEGIN_NAMESPACE

class Q3SyntaxHighlighterPrivate
{
public:
    Q3SyntaxHighlighterPrivate() :
        currentParagraph(-1)
        {}

    int currentParagraph;
};

class Q3SyntaxHighlighterInternal : public Q3TextPreProcessor
{
public:
    Q3SyntaxHighlighterInternal(Q3SyntaxHighlighter *h) : highlighter(h) {}
    void process(Q3TextDocument *doc, Q3TextParagraph *p, int, bool invalidate) {
        if (p->prev() && p->prev()->endState() == -1)
            process(doc, p->prev(), 0, false);

        highlighter->para = p;
        QString text = p->string()->toString();
        int endState = p->prev() ? p->prev()->endState() : -2;
        int oldEndState = p->endState();
        highlighter->d->currentParagraph = p->paragId();
        p->setEndState(highlighter->highlightParagraph(text, endState));
        highlighter->d->currentParagraph = -1;
        highlighter->para = 0;

        p->setFirstPreProcess(false);
        Q3TextParagraph *op = p;
        p = p->next();
        if ((!!oldEndState || !!op->endState()) && oldEndState != op->endState() &&
             invalidate && p && !p->firstPreProcess() && p->endState() != -1) {
            while (p) {
                if (p->endState() == -1)
                    return;
                p->setEndState(-1);
                p = p->next();
            }
        }
    }
    Q3TextFormat *format(int) { return 0; }

private:
    Q3SyntaxHighlighter *highlighter;

    friend class Q3TextEdit;
};

#endif // QT_NO_SYNTAXHIGHLIGHTER

QT_END_NAMESPACE

#endif // Q3SYNTAXHIGHLIGHTER_P_H
