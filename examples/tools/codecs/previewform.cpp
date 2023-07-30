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

#include "previewform.h"

PreviewForm::PreviewForm(QWidget *parent)
    : QDialog(parent)
{
    encodingComboBox = new QComboBox;

    encodingLabel = new QLabel(tr("&Encoding:"));
    encodingLabel->setBuddy(encodingComboBox);

    textEdit = new QTextEdit;
    textEdit->setLineWrapMode(QTextEdit::NoWrap);
    textEdit->setReadOnly(true);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                    | QDialogButtonBox::Cancel);

    connect(encodingComboBox, SIGNAL(activated(int)),
            this, SLOT(updateTextEdit()));
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(encodingLabel, 0, 0);
    mainLayout->addWidget(encodingComboBox, 0, 1);
    mainLayout->addWidget(textEdit, 1, 0, 1, 2);
    mainLayout->addWidget(buttonBox, 2, 0, 1, 2);
    setLayout(mainLayout);

    setWindowTitle(tr("Choose Encoding"));
    resize(400, 300);
}

void PreviewForm::setCodecList(const QList<QTextCodec *> &list)
{
    encodingComboBox->clear();
    foreach (QTextCodec *codec, list)
        encodingComboBox->addItem(codec->name(), codec->mibEnum());
}

void PreviewForm::setEncodedData(const QByteArray &data)
{
    encodedData = data;
    updateTextEdit();
}

void PreviewForm::updateTextEdit()
{
    int mib = encodingComboBox->itemData(
                      encodingComboBox->currentIndex()).toInt();
    QTextCodec *codec = QTextCodec::codecForMib(mib);

    QTextStream in(&encodedData);
    in.setAutoDetectUnicode(false);
    in.setCodec(codec);
    decodedStr = in.readAll();

    textEdit->setPlainText(decodedStr);
}
