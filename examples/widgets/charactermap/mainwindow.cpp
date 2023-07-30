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

#include "characterwidget.h"
#include "mainwindow.h"

//! [0]
MainWindow::MainWindow()
{
    QWidget *centralWidget = new QWidget;

    QLabel *fontLabel = new QLabel(tr("Font:"));
    fontCombo = new QFontComboBox;
    QLabel *sizeLabel = new QLabel(tr("Size:"));
    sizeCombo = new QComboBox;
    QLabel *styleLabel = new QLabel(tr("Style:"));
    styleCombo = new QComboBox;
    QLabel *fontMergingLabel = new QLabel(tr("Automatic Font Merging:"));
    fontMerging = new QCheckBox;
    fontMerging->setChecked(true);

    scrollArea = new QScrollArea;
    characterWidget = new CharacterWidget;
    scrollArea->setWidget(characterWidget);
//! [0]

//! [1]
    findStyles(fontCombo->currentFont());
//! [1]
    findSizes(fontCombo->currentFont());

//! [2]
    lineEdit = new QLineEdit;
    QPushButton *clipboardButton = new QPushButton(tr("&To clipboard"));
//! [2]

//! [3]
    clipboard = QApplication::clipboard();
//! [3]

//! [4]
    connect(fontCombo, SIGNAL(currentFontChanged(const QFont &)),
            this, SLOT(findStyles(const QFont &)));
    connect(fontCombo, SIGNAL(currentFontChanged(const QFont &)),
            this, SLOT(findSizes(const QFont &)));
    connect(fontCombo, SIGNAL(currentFontChanged(const QFont &)),
            characterWidget, SLOT(updateFont(const QFont &)));
    connect(sizeCombo, SIGNAL(currentIndexChanged(const QString &)),
            characterWidget, SLOT(updateSize(const QString &)));
    connect(styleCombo, SIGNAL(currentIndexChanged(const QString &)),
            characterWidget, SLOT(updateStyle(const QString &)));
//! [4] //! [5]
    connect(characterWidget, SIGNAL(characterSelected(const QString &)),
            this, SLOT(insertCharacter(const QString &)));
    connect(clipboardButton, SIGNAL(clicked()), this, SLOT(updateClipboard()));
//! [5]
    connect(fontMerging, SIGNAL(toggled(bool)), characterWidget, SLOT(updateFontMerging(bool)));

//! [6]
    QHBoxLayout *controlsLayout = new QHBoxLayout;
    controlsLayout->addWidget(fontLabel);
    controlsLayout->addWidget(fontCombo, 1);
    controlsLayout->addWidget(sizeLabel);
    controlsLayout->addWidget(sizeCombo, 1);
    controlsLayout->addWidget(styleLabel);
    controlsLayout->addWidget(styleCombo, 1);
    controlsLayout->addWidget(fontMergingLabel);
    controlsLayout->addWidget(fontMerging, 1);
    controlsLayout->addStretch(1);

    QHBoxLayout *lineLayout = new QHBoxLayout;
    lineLayout->addWidget(lineEdit, 1);
    lineLayout->addSpacing(12);
    lineLayout->addWidget(clipboardButton);

    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addLayout(controlsLayout);
    centralLayout->addWidget(scrollArea, 1);
    centralLayout->addSpacing(4);
    centralLayout->addLayout(lineLayout);
    centralWidget->setLayout(centralLayout);

    setCentralWidget(centralWidget);
    setWindowTitle(tr("Character Map"));
}
//! [6]

//! [7]
void MainWindow::findStyles(const QFont &font)
{
    QFontDatabase fontDatabase;
    QString currentItem = styleCombo->currentText();
    styleCombo->clear();
//! [7]

//! [8]
    QString style;
    foreach (style, fontDatabase.styles(font.family()))
        styleCombo->addItem(style);

    int styleIndex = styleCombo->findText(currentItem);

    if (styleIndex == -1)
        styleCombo->setCurrentIndex(0);
    else
        styleCombo->setCurrentIndex(styleIndex);
}
//! [8]

void MainWindow::findSizes(const QFont &font)
{
    QFontDatabase fontDatabase;
    QString currentSize = sizeCombo->currentText();
    sizeCombo->blockSignals(true);
    sizeCombo->clear();

    int size;
    if(fontDatabase.isSmoothlyScalable(font.family(), fontDatabase.styleString(font))) {
        foreach(size, QFontDatabase::standardSizes()) {
            sizeCombo->addItem(QVariant(size).toString());
            sizeCombo->setEditable(true);
        }

    } else {
        foreach(size, fontDatabase.smoothSizes(font.family(), fontDatabase.styleString(font))) {
            sizeCombo->addItem(QVariant(size).toString());
            sizeCombo->setEditable(false);
        }
    }

    sizeCombo->blockSignals(false);

    int sizeIndex = sizeCombo->findText(currentSize);

    if(sizeIndex == -1)
        sizeCombo->setCurrentIndex(qMax(0, sizeCombo->count() / 3));
    else
        sizeCombo->setCurrentIndex(sizeIndex);
}

//! [9]
void MainWindow::insertCharacter(const QString &character)
{
    lineEdit->insert(character);
}
//! [9]

//! [10]
void MainWindow::updateClipboard()
{
//! [11]
    clipboard->setText(lineEdit->text(), QClipboard::Clipboard);
//! [11]
    clipboard->setText(lineEdit->text(), QClipboard::Selection);
}
//! [10]
