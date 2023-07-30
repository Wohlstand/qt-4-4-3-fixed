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

#include "clipwindow.h"

ClipWindow::ClipWindow(QWidget *parent)
    : QMainWindow(parent)
{
    clipboard = QApplication::clipboard();

    QWidget *centralWidget = new QWidget(this);
    QWidget *currentItem = new QWidget(centralWidget);
    QLabel *mimeTypeLabel = new QLabel(tr("MIME types:"), currentItem);
    mimeTypeCombo = new QComboBox(currentItem);
    QLabel *dataLabel = new QLabel(tr("Data:"), currentItem);
    dataInfoLabel = new QLabel("", currentItem);

    previousItems = new QListWidget(centralWidget);

//! [0]
    connect(clipboard, SIGNAL(dataChanged()), this, SLOT(updateClipboard()));
//! [0]
    connect(mimeTypeCombo, SIGNAL(activated(const QString &)),
            this, SLOT(updateData(const QString &)));

    QVBoxLayout *currentLayout = new QVBoxLayout(currentItem);
    currentLayout->addWidget(mimeTypeLabel);
    currentLayout->addWidget(mimeTypeCombo);
    currentLayout->addWidget(dataLabel);
    currentLayout->addWidget(dataInfoLabel);
    currentLayout->addStretch(1);

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->addWidget(currentItem, 1);
    mainLayout->addWidget(previousItems);

    setCentralWidget(centralWidget);
    setWindowTitle(tr("Clipboard"));
}

//! [1]
void ClipWindow::updateClipboard()
{
    QStringList formats = clipboard->mimeData()->formats();
    QByteArray data = clipboard->mimeData()->data(format);
//! [1]

    mimeTypeCombo->clear();
    mimeTypeCombo->insertStringList(formats);

    int size = clipboard->mimeData()->data(formats[0]).size();
    QListWidgetItem *newItem = new QListWidgetItem(previousItems);
    newItem->setText(tr("%1 (%2 bytes)").arg(formats[0]).arg(size));

    updateData(formats[0]);
//! [2]
}
//! [2]

void ClipWindow::updateData(const QString &format)
{
    QByteArray data = clipboard->mimeData()->data(format);
    dataInfoLabel->setText(tr("%1 bytes").arg(data.size()));
}
