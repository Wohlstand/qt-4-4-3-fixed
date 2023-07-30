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

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(textLabel);
    layout->addWidget(textBrowser);
    layout->addWidget(mimeTypeLabel);
    layout->addWidget(mimeTypeCombo);
/*
    ...
    setAcceptDrops(true);
*/
    setAcceptDrops(true);
    setWindowTitle(tr("Drop Actions"));
}

void Window::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/plain"))
        event->acceptProposedAction();
}

void Window::dropEvent(QDropEvent *event)
{
    QMenu actionMenu(this);
    QAction *copyAction = 0;
    QAction *moveAction = 0;
    QAction *linkAction = 0;
    QAction *ignoreAction = 0;
    if (event->possibleActions() & Qt::CopyAction)
        copyAction = actionMenu.addAction(tr("Copy"));
    if (event->possibleActions() & Qt::MoveAction)
        moveAction = actionMenu.addAction(tr("Move"));
    if (event->possibleActions() & Qt::LinkAction)
        linkAction = actionMenu.addAction(tr("Link"));
    if (event->possibleActions() & Qt::IgnoreAction)
        ignoreAction = actionMenu.addAction(tr("Ignore"));

    QAction *result = actionMenu.exec(QCursor::pos());

    if (copyAction && result == copyAction)
        event->setDropAction(Qt::CopyAction);
    else if (moveAction && result == moveAction)
        event->setDropAction(Qt::MoveAction);
    else if (linkAction && result == linkAction)
        event->setDropAction(Qt::LinkAction);
    else {
        event->setDropAction(Qt::IgnoreAction);
        return;
    }

    textBrowser->setPlainText(event->mimeData()->text());
    mimeTypeCombo->clear();
    mimeTypeCombo->addItems(event->mimeData()->formats());
}
