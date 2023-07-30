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

MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent)
{
    QLabel *textLabel = new QLabel(tr("Data:"), this);
    dataLabel = new QLabel(this);
    dataLabel->setFixedSize(200, 200);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(textLabel);
    layout->addWidget(dataLabel);

    setAcceptDrops(true);
    setWindowTitle(tr("Drop Events"));
}

//! [0]
void MyWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasText() || event->mimeData()->hasImage())
        event->acceptProposedAction();
}
//! [0]

//! [1]
void MyWidget::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasText())
        dataLabel->setText(event->mimeData()->text());
    else if (event->mimeData()->hasImage()) {
        QVariant imageData = event->mimeData()->imageData();
        dataLabel->setPixmap(qvariant_cast<QPixmap>(imageData));
    }
    event->acceptProposedAction();
}
//! [1]

//! [2]
void MyWidget::mousePressEvent(QMouseEvent *event)
{
//! [2]
    QString text = dataLabel->text();
    QPixmap iconPixmap(32, 32);
    iconPixmap.fill(qRgba(255, 0, 0, 127));
    QImage image(100, 100, QImage::Format_RGB32);
    image.fill(qRgb(0, 0, 255));

//! [3]
    if (event->button() == Qt::LeftButton) {

        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;

        mimeData->setText(text);
        mimeData->setImageData(image);
        drag->setMimeData(mimeData);
        drag->setPixmap(iconPixmap);

        Qt::DropAction dropAction = drag->exec();
//! [3]
        // ...
//! [4]
        event->accept();
    }
//! [4]
    else if (event->button() == Qt::MidButton) {

        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;

        mimeData->setImageData(image);
        drag->setMimeData(mimeData);
        drag->setPixmap(iconPixmap);

        Qt::DropAction dropAction = drag->exec();
        // ...
        event->accept();
    }
//! [5]
}
//! [5]
