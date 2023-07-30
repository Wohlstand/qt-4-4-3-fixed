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

#include "screenshot.h"

//! [0]
Screenshot::Screenshot()
{
    screenshotLabel = new QLabel;
    screenshotLabel->setSizePolicy(QSizePolicy::Expanding,
                                   QSizePolicy::Expanding);
    screenshotLabel->setAlignment(Qt::AlignCenter);
    screenshotLabel->setMinimumSize(240, 160);

    createOptionsGroupBox();
    createButtonsLayout();

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(screenshotLabel);
    mainLayout->addWidget(optionsGroupBox);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);

    shootScreen();
    delaySpinBox->setValue(5);

    setWindowTitle(tr("Screenshot"));
    resize(300, 200);
}
//! [0]

//! [1]
void Screenshot::resizeEvent(QResizeEvent * /* event */)
{
    QSize scaledSize = originalPixmap.size();
    scaledSize.scale(screenshotLabel->size(), Qt::KeepAspectRatio);
    if (!screenshotLabel->pixmap()
            || scaledSize != screenshotLabel->pixmap()->size())
        updateScreenshotLabel();
}
//! [1]

//! [2]
void Screenshot::newScreenshot()
{
    if (hideThisWindowCheckBox->isChecked())
        hide();
    newScreenshotButton->setDisabled(true);

    QTimer::singleShot(delaySpinBox->value() * 1000, this, SLOT(shootScreen()));
}
//! [2]

//! [3]
void Screenshot::saveScreenshot()
{
    QString format = "png";
    QString initialPath = QDir::currentPath() + tr("/untitled.") + format;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                               initialPath,
                               tr("%1 Files (*.%2);;All Files (*)")
                               .arg(format.toUpper())
                               .arg(format));
    if (!fileName.isEmpty())
        originalPixmap.save(fileName, format.toAscii());
}
//! [3]

//! [4]
void Screenshot::shootScreen()
{
    if (delaySpinBox->value() != 0)
        qApp->beep();
//! [4]
    originalPixmap = QPixmap(); // clear image for low memory situations
                                // on embedded devices.
//! [5]
    originalPixmap = QPixmap::grabWindow(QApplication::desktop()->winId());
    updateScreenshotLabel();

    newScreenshotButton->setDisabled(false);
    if (hideThisWindowCheckBox->isChecked())
        show();
}
//! [5]

//! [6]
void Screenshot::updateCheckBox()
{
    if (delaySpinBox->value() == 0)
        hideThisWindowCheckBox->setDisabled(true);
    else
        hideThisWindowCheckBox->setDisabled(false);
}
//! [6]

//! [7]
void Screenshot::createOptionsGroupBox()
{
    optionsGroupBox = new QGroupBox(tr("Options"));

    delaySpinBox = new QSpinBox;
    delaySpinBox->setSuffix(tr(" s"));
    delaySpinBox->setMaximum(60);
    connect(delaySpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateCheckBox()));

    delaySpinBoxLabel = new QLabel(tr("Screenshot Delay:"));

    hideThisWindowCheckBox = new QCheckBox(tr("Hide This Window"));

    optionsGroupBoxLayout = new QGridLayout;
    optionsGroupBoxLayout->addWidget(delaySpinBoxLabel, 0, 0);
    optionsGroupBoxLayout->addWidget(delaySpinBox, 0, 1);
    optionsGroupBoxLayout->addWidget(hideThisWindowCheckBox, 1, 0, 1, 2);
    optionsGroupBox->setLayout(optionsGroupBoxLayout);
}
//! [7]

//! [8]
void Screenshot::createButtonsLayout()
{
    newScreenshotButton = createButton(tr("New Screenshot"),
                                       this, SLOT(newScreenshot()));

    saveScreenshotButton = createButton(tr("Save Screenshot"),
                                        this, SLOT(saveScreenshot()));

    quitScreenshotButton = createButton(tr("Quit"), this, SLOT(close()));

    buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(newScreenshotButton);
    buttonsLayout->addWidget(saveScreenshotButton);
    buttonsLayout->addWidget(quitScreenshotButton);
}
//! [8]

//! [9]
QPushButton *Screenshot::createButton(const QString &text, QWidget *receiver,
                                      const char *member)
{
    QPushButton *button = new QPushButton(text);
    button->connect(button, SIGNAL(clicked()), receiver, member);
    return button;
}
//! [9]

//! [10]
void Screenshot::updateScreenshotLabel()
{
    screenshotLabel->setPixmap(originalPixmap.scaled(screenshotLabel->size(),
                                                     Qt::KeepAspectRatio,
                                                     Qt::SmoothTransformation));
}
//! [10]
