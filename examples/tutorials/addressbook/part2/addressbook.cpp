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
#include "addressbook.h"

AddressBook::AddressBook(QWidget *parent)
    : QWidget(parent)
{
    QLabel *nameLabel = new QLabel(tr("Name:"));
    nameLine = new QLineEdit;
//! [setting readonly 1]
    nameLine->setReadOnly(true);
//! [setting readonly 1]
    QLabel *addressLabel = new QLabel(tr("Address:"));
    addressText = new QTextEdit;
//! [setting readonly 2]    
    addressText->setReadOnly(true);
//! [setting readonly 2]
   
//! [pushbutton declaration]
    addButton = new QPushButton(tr("&Add"));
    addButton->show();
    submitButton = new QPushButton(tr("&Submit"));
    submitButton->hide();
    cancelButton = new QPushButton(tr("&Cancel"));
    cancelButton->hide();
//! [pushbutton declaration]
//! [connecting signals and slots]
    connect(addButton, SIGNAL(clicked()), this, SLOT(addContact()));
    connect(submitButton, SIGNAL(clicked()), this, SLOT(submitContact()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
//! [connecting signals and slots]
//! [vertical layout]
    QVBoxLayout *buttonLayout1 = new QVBoxLayout;
    buttonLayout1->addWidget(addButton, Qt::AlignTop);
    buttonLayout1->addWidget(submitButton);
    buttonLayout1->addWidget(cancelButton);
    buttonLayout1->addStretch();
//! [vertical layout]
//! [grid layout]
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(nameLabel, 0, 0);
    mainLayout->addWidget(nameLine, 0, 1);
    mainLayout->addWidget(addressLabel, 1, 0, Qt::AlignTop);
    mainLayout->addWidget(addressText, 1, 1);
    mainLayout->addLayout(buttonLayout1, 1, 2);
//! [grid layout]
    setLayout(mainLayout);
    setWindowTitle(tr("Simple Address Book"));
}
//! [addContact]
void AddressBook::addContact()
{
    oldName = nameLine->text();
    oldAddress = addressText->toPlainText();

    nameLine->clear();
    addressText->clear();
    
    nameLine->setReadOnly(false);
    nameLine->setFocus(Qt::OtherFocusReason);
    addressText->setReadOnly(false);

    addButton->setEnabled(false);
    submitButton->show();
    cancelButton->show();
}
//! [addContact]

//! [submitContact part1]
void AddressBook::submitContact()
{
    QString name = nameLine->text();
    QString address = addressText->toPlainText();
    
    if (name == "" || address == "") {
        QMessageBox::information(this, tr("Empty Field"),
            tr("Please enter a name and address."));
        return;
    }
//! [submitContact part1]
//! [submitContact part2]
    if (!contacts.contains(name)) {
        contacts.insert(name, address);
        QMessageBox::information(this, tr("Add Successful"),
            tr("\"%1\" has been added to your address book.").arg(name));
    } else {
        QMessageBox::information(this, tr("Add Unsuccessful"),
            tr("Sorry, \"%1\" is already in your address book.").arg(name));
        return;
    }
//! [submitContact part2]
//! [submitContact part3]
    if (contacts.isEmpty()) {
        nameLine->clear();
        addressText->clear();
    }

    nameLine->setReadOnly(true);
    addressText->setReadOnly(true);
    addButton->setEnabled(true);
    submitButton->hide();
    cancelButton->hide();
}
//! [submitContact part3]
//! [cancel]
void AddressBook::cancel()
{
    nameLine->setText(oldName);
    nameLine->setReadOnly(true);

    addressText->setText(oldAddress);
    addressText->setReadOnly(true);

    addButton->setEnabled(true);
    submitButton->hide();
    cancelButton->hide();    
}
//! [cancel]
