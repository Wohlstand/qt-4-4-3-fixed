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

#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "tokenengine.h"
#include <QVector>
#include <QByteArray>

QT_BEGIN_NAMESPACE

class Tokenizer
{
public:
    Tokenizer();
    ~Tokenizer();
    typedef void (Tokenizer::*scan_fun_ptr)(int *kind);
    QVector<TokenEngine::Token> tokenize(QByteArray text);
private:
    bool nextToken(TokenEngine::Token &tok);

    void scanChar(int *kind);
    void scanUnicodeChar(int *kind);
    void scanNewline(int *kind);
    void scanWhiteSpaces(int *kind);
    void scanCharLiteral(int *kind);
    void scanStringLiteral(int *kind);
    void scanNumberLiteral(int *kind);
    void scanIdentifier(int *kind);
    void scanComment(int *kind);
    void scanPreprocessor(int *kind);
    void scanOperator(int *kind);

    void setupScanTable();
private:
    static bool s_initialized;
    static scan_fun_ptr s_scan_table[];
    static int s_attr_table[];

    const char *m_buffer;
    int m_ptr;

    QVector<TokenEngine::Token> m_tokens;
};

QT_END_NAMESPACE

#endif
