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

#include <QtGui>

#include "circlewidget.h"
#include "window.h"

//! [0]
Window::Window()
{
    aliasedLabel = createLabel(tr("Aliased"));
    antialiasedLabel = createLabel(tr("Antialiased"));
    intLabel = createLabel(tr("Int"));
    floatLabel = createLabel(tr("Float"));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(aliasedLabel, 0, 1);
    layout->addWidget(antialiasedLabel, 0, 2);
    layout->addWidget(intLabel, 1, 0);
    layout->addWidget(floatLabel, 2, 0);
//! [0]

//! [1]
    QTimer *timer = new QTimer(this);

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            circleWidgets[i][j] = new CircleWidget;
            circleWidgets[i][j]->setAntialiased(j != 0);
            circleWidgets[i][j]->setFloatBased(i != 0);

            connect(timer, SIGNAL(timeout()),
                    circleWidgets[i][j], SLOT(nextAnimationFrame()));

            layout->addWidget(circleWidgets[i][j], i + 1, j + 1);
        }
    }
//! [1] //! [2]
    timer->start(100);
    setLayout(layout);

    setWindowTitle(tr("Concentric Circles"));
}
//! [2]

//! [3]
QLabel *Window::createLabel(const QString &text)
{
    QLabel *label = new QLabel(text);
    label->setAlignment(Qt::AlignCenter);
    label->setMargin(2);
    label->setFrameStyle(QFrame::Box | QFrame::Sunken);
    return label;
}
//! [3]
