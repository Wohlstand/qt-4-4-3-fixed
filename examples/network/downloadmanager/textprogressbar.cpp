/****************************************************************************
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the example classes of the Qt Toolkit.
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

#include "textprogressbar.h"
#include <QByteArray>
#include <stdio.h>

TextProgressBar::TextProgressBar()
    : value(0), maximum(-1), iteration(0)
{
}

void TextProgressBar::clear()
{
    printf("\n");
    fflush(stdout);

    iteration = 0;
    value = 0;
    maximum = -1;
}

void TextProgressBar::update()
{
    ++iteration;

    if (maximum > 0) {
        // we know the maximum
        // draw a progress bar
        int percent = value * 100 / maximum;
        int hashes = percent / 2;

        QByteArray progressbar(hashes, '#');
        if (percent % 2)
            progressbar += '>';

        printf("\r[%-50s] %3d%% %s     ",
               progressbar.constData(),
               percent,
               qPrintable(message));
    } else {
        // we don't know the maximum, so we can't draw a progress bar
        int center = (iteration % 48) + 1; // 50 spaces, minus 2
        QByteArray before(qMax(center - 2, 0), ' ');
        QByteArray after(qMin(center + 2, 50), ' ');

        printf("\r[%s###%s]      %s      ",
               before.constData(), after.constData(), qPrintable(message));
    }
}

void TextProgressBar::setMessage(const QString &m)
{
    message = m;
}

void TextProgressBar::setStatus(qint64 val, qint64 max)
{
    value = val;
    maximum = max;
}