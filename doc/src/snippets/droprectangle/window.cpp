/****************************************************************************
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the documentation of the Qt Toolkit.
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

#include "window.h"

Window::Window(QWidget *parent)
    : QWidget(parent)
{
    QLabel *textLabel = new QLabel(tr("Data:"), this);
    textBrowser = new QTextBrowser(this);

    QLabel *mimeTypeLabel = new QLabel(tr("MIME types:"), this);
    mimeTypeCombo = new QComboBox(this);

    dropFrame = new QFrame(this);
    dropFrame->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    QLabel *dropLabel = new QLabel(tr("Drop items here"), dropFrame);
    dropLabel->setAlignment(Qt::AlignHCenter);

    QVBoxLayout *dropFrameLayout = new QVBoxLayout(dropFrame);
    dropFrameLayout->addWidget(dropLabel);

    QHBoxLayout *dropLayout = new QHBoxLayout;
    dropLayout->addStretch(0);
    dropLayout->addWidget(dropFrame);
    dropLayout->addStretch(0);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(textLabel);
    mainLayout->addWidget(textBrowser);
    mainLayout->addWidget(mimeTypeLabel);
    mainLayout->addWidget(mimeTypeCombo);
    mainLayout->addSpacing(32);
    mainLayout->addLayout(dropLayout);

    setAcceptDrops(true);
    setWindowTitle(tr("Drop Rectangle"));
}

//! [0]
void Window::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("text/plain")
        && event->answerRect().intersects(dropFrame->geometry()))

        event->acceptProposedAction();
}
//! [0]

void Window::dropEvent(QDropEvent *event)
{
    textBrowser->setPlainText(event->mimeData()->text());
    mimeTypeCombo->clear();
    mimeTypeCombo->addItems(event->mimeData()->formats());

    event->acceptProposedAction();
}
