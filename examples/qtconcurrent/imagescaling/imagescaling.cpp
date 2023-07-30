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
#include "imagescaling.h"
#include "math.h"

#ifndef QT_NO_CONCURRENT

const int imageSize = 100;

QImage scale(const QString &imageFileName)
{
    QImage image(imageFileName);
    return image.scaled(QSize(imageSize, imageSize), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

Images::Images(QWidget *parent) 
: QWidget(parent)
{
    setWindowTitle(tr("Image loading and scaling example"));
    resize(800, 600);

    imageScaling = new QFutureWatcher<QImage>(this);
    connect(imageScaling, SIGNAL(resultReadyAt(int)), SLOT(showImage(int)));
    connect(imageScaling, SIGNAL(finished()), SLOT(finished()));

    openButton = new QPushButton(tr("Open Images"));
    connect(openButton, SIGNAL(clicked()), SLOT(open()));

    cancelButton = new QPushButton(tr("Cancel"));
    cancelButton->setEnabled(false);
    connect(cancelButton, SIGNAL(clicked()), imageScaling, SLOT(cancel()));
   
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(openButton);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addStretch();

    imagesLayout = new QGridLayout();

    mainLayout = new QVBoxLayout();
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(imagesLayout);
    mainLayout->addStretch();
    setLayout(mainLayout);
}

Images::~Images()
{
    imageScaling->cancel();
    imageScaling->waitForFinished();
}

void Images::open()
{
    // Cancel and wait if we are already loading images.
    if (imageScaling->isRunning()) {
        imageScaling->cancel();
        imageScaling->waitForFinished();
    }

    // Show a file open dialog at QDesktopServices::PicturesLocation.
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Select Images"), 
                            QDesktopServices::storageLocation(QDesktopServices::PicturesLocation),
                            "*.jpg *.png");

    if (files.count() == 0)
        return;

    // Do a simple layout.
    qDeleteAll(labels);
    labels.clear();

    int dim = sqrt(qreal(files.count())) + 1;
    for (int i = 0; i < dim; ++i) {
        for (int j = 0; j < dim; ++j) {
            QLabel *imageLabel = new QLabel;
            imageLabel->setFixedSize(imageSize,imageSize);
            imagesLayout->addWidget(imageLabel,i,j);
            labels.append(imageLabel);
        }
    }

    // Use mapped to run the thread safe scale function on the files.
    imageScaling->setFuture(QtConcurrent::mapped(files, scale));

    openButton->setEnabled(false);
    cancelButton->setEnabled(true);
}

void Images::showImage(int num)
{
    labels[num]->setPixmap(QPixmap::fromImage(imageScaling->resultAt(num)));
}

void Images::finished()
{
    openButton->setEnabled(true);
    cancelButton->setEnabled(false);
}

#endif // QT_NO_CONCURRENT

