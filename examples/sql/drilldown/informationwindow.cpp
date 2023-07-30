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

#include "informationwindow.h"

//! [0]
InformationWindow::InformationWindow(int id, QSqlRelationalTableModel *offices,
                                     QWidget *parent)
    : QDialog(parent)
{
//! [0] //! [1]
    QLabel *locationLabel = new QLabel(tr("Location: "));
    QLabel *countryLabel = new QLabel(tr("Country: "));
    QLabel *descriptionLabel = new QLabel(tr("Description: "));
    QLabel *imageFileLabel = new QLabel(tr("Image file: "));

    createButtons();

    locationText = new QLabel;
    countryText = new QLabel;
    descriptionEditor = new QTextEdit;
//! [1]

//! [2]
    imageFileEditor = new QComboBox;
    imageFileEditor->setModel(offices->relationModel(1));
    imageFileEditor->setModelColumn(offices->relationModel(1)->fieldIndex("file"));
//! [2]

//! [3]
    mapper = new QDataWidgetMapper(this);
    mapper->setModel(offices);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setItemDelegate(new QSqlRelationalDelegate(mapper));
    mapper->addMapping(imageFileEditor, 1);
    mapper->addMapping(locationText, 2);
    mapper->addMapping(countryText, 3);
    mapper->addMapping(descriptionEditor, 4);
    mapper->setCurrentIndex(id);
//! [3]

//! [4]
    connect(descriptionEditor, SIGNAL(textChanged()),
            this, SLOT(enableButtons()));
    connect(imageFileEditor, SIGNAL(currentIndexChanged(int)),
            this, SLOT(enableButtons()));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(locationLabel, 0, 0, Qt::AlignLeft | Qt::AlignTop);
    layout->addWidget(countryLabel, 1, 0, Qt::AlignLeft | Qt::AlignTop);
    layout->addWidget(imageFileLabel, 2, 0, Qt::AlignLeft | Qt::AlignTop);
    layout->addWidget(descriptionLabel, 3, 0, Qt::AlignLeft | Qt::AlignTop);
    layout->addWidget(locationText, 0, 1);
    layout->addWidget(countryText, 1, 1);
    layout->addWidget(imageFileEditor, 2, 1);
    layout->addWidget(descriptionEditor, 3, 1);
    layout->addWidget(buttonBox, 4, 0, 1, 2);
    setLayout(layout);

    locationId = id;
    displayedImage = imageFileEditor->currentText();

    setWindowFlags(Qt::Window);
    enableButtons(false);
    setWindowTitle(tr("Office: %1").arg(locationText->text()));
    resize(320, sizeHint().height());
}
//! [4]

//! [5]
int InformationWindow::id()
{
    return locationId;
}
//! [5]

//! [6]
void InformationWindow::revert()
{
    mapper->revert();
    enableButtons(false);
}
//! [6]

//! [7]
void InformationWindow::submit()
{
    QString newImage(imageFileEditor->currentText());

    if (displayedImage != newImage) {
        displayedImage = newImage;
        emit imageChanged(locationId, newImage);
    }

    mapper->submit();
    mapper->setCurrentIndex(locationId);

    enableButtons(false);
}
//! [7]

//! [8]
void InformationWindow::createButtons()
{
    closeButton = new QPushButton(tr("&Close"));
    revertButton = new QPushButton(tr("&Revert"));
    submitButton = new QPushButton(tr("&Submit"));

    closeButton->setDefault(true);

    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(revertButton, SIGNAL(clicked()), this, SLOT(revert()));
    connect(submitButton, SIGNAL(clicked()), this, SLOT(submit()));
//! [8]

//! [9]
    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(submitButton, QDialogButtonBox::ResetRole);
    buttonBox->addButton(revertButton, QDialogButtonBox::ResetRole);
    buttonBox->addButton(closeButton, QDialogButtonBox::RejectRole);
}
//! [9]

//! [10]
void InformationWindow::enableButtons(bool enable)
{
    revertButton->setEnabled(enable);
    submitButton->setEnabled(enable);
}
//! [10]


