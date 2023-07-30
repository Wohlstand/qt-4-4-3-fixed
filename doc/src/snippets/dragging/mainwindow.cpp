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

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QFrame *centralFrame = new QFrame(this);

    QLabel *nameLabel = new QLabel(tr("Comment:"), centralFrame);
    commentEdit = new QTextEdit(centralFrame);
    QLabel *dragLabel = new QLabel(tr("<p>Drag the icon to a filer "
                                      "window or the desktop background:</p>"),
                                      centralFrame);
    iconLabel = new QLabel(centralFrame);
    iconPixmap.load(":/images/file.png");
    iconLabel->setPixmap(iconPixmap);

    QGridLayout *grid = new QGridLayout(centralFrame);
    grid->addWidget(nameLabel, 0, 0);
    grid->addWidget(commentEdit, 1, 0, 1, 2);
    grid->addWidget(dragLabel, 2, 0);
    grid->addWidget(iconLabel, 2, 1);

    statusBar();
    setCentralWidget(centralFrame);
    setWindowTitle(tr("Dragging"));
}

//! [0]
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton
        && iconLabel->geometry().contains(event->pos())) {

//! [1]
        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;

        mimeData->setText(commentEdit->toPlainText());
        drag->setMimeData(mimeData);
//! [1]
        drag->setPixmap(iconPixmap);

        Qt::DropAction dropAction = drag->exec();
//! [0]

        QString actionText;
        switch (dropAction) {
            case Qt::CopyAction:
                actionText = tr("The text was copied.");
                break;
            case Qt::MoveAction:
                actionText = tr("The text was moved.");
                break;
            case Qt::LinkAction:
                actionText = tr("The text was linked.");
                break;
            case Qt::IgnoreAction:
                actionText = tr("The drag was ignored.");
                break;
            default:
                actionText = tr("Unknown action.");
                break;
        }
        statusBar()->showMessage(actionText);
//! [2]
    }
}
//! [2]
