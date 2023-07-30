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

#include "certificateinfo.h"
#include "sslclient.h"
#include "ui_sslclient.h"
#include "ui_sslerrors.h"

#include <QtGui/QScrollBar>
#include <QtGui/QStyle>
#include <QtGui/QToolButton>
#include <QtNetwork/QSslCipher>

SslClient::SslClient(QWidget *parent)
    : QWidget(parent), socket(0), padLock(0)
{
    form = new Ui_Form;
    form->setupUi(this);
    form->hostNameEdit->setSelection(0, form->hostNameEdit->text().size());
    form->sessionOutput->setHtml(tr("&lt;not connected&gt;"));

    connect(form->hostNameEdit, SIGNAL(textChanged(QString)),
            this, SLOT(updateEnabledState()));
    connect(form->connectButton, SIGNAL(clicked()),
            this, SLOT(secureConnect()));
    connect(form->sendButton, SIGNAL(clicked()),
            this, SLOT(sendData()));
}

SslClient::~SslClient()
{
    delete form;
}

void SslClient::updateEnabledState()
{
    bool unconnected = !socket || socket->state() == QAbstractSocket::UnconnectedState;

    form->hostNameEdit->setReadOnly(!unconnected);
    form->hostNameEdit->setFocusPolicy(unconnected ? Qt::StrongFocus : Qt::NoFocus);

    form->hostNameLabel->setEnabled(unconnected);
    form->portBox->setEnabled(unconnected);
    form->portLabel->setEnabled(unconnected);
    form->connectButton->setEnabled(unconnected && !form->hostNameEdit->text().isEmpty());

    bool connected = socket && socket->state() == QAbstractSocket::ConnectedState;
    form->sessionBox->setEnabled(connected);
    form->sessionOutput->setEnabled(connected);
    form->sessionInput->setEnabled(connected);
    form->sessionInputLabel->setEnabled(connected);
    form->sendButton->setEnabled(connected);
}

void SslClient::secureConnect()
{
    if (!socket) {
        socket = new QSslSocket(this);
        connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
                this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));
        connect(socket, SIGNAL(encrypted()),
                this, SLOT(socketEncrypted()));
        connect(socket, SIGNAL(sslErrors(QList<QSslError>)),
                this, SLOT(sslErrors(QList<QSslError>)));
        connect(socket, SIGNAL(readyRead()),
                this, SLOT(socketReadyRead()));
    }

    socket->connectToHostEncrypted(form->hostNameEdit->text(), form->portBox->value());
    updateEnabledState();
}

void SslClient::socketStateChanged(QAbstractSocket::SocketState state)
{
    updateEnabledState();
    if (state == QAbstractSocket::UnconnectedState) {
        form->hostNameEdit->setPalette(QPalette());
        form->hostNameEdit->setFocus();
        form->cipherLabel->setText(tr("<none>"));
        if (padLock)
            padLock->hide();
        socket->deleteLater();
        socket = 0;
    }
}

void SslClient::socketEncrypted()
{
    form->sessionOutput->clear();
    form->sessionInput->setFocus();

    QPalette palette;
    palette.setColor(QPalette::Base, QColor(255, 255, 192));
    form->hostNameEdit->setPalette(palette);

    QSslCipher ciph = socket->sessionCipher();
    QString cipher = QString("%1, %2 (%3/%4)").arg(ciph.authenticationMethod())
                     .arg(ciph.name()).arg(ciph.usedBits()).arg(ciph.supportedBits());;
    form->cipherLabel->setText(cipher);

    if (!padLock) {
        padLock = new QToolButton;
        padLock->setIcon(QIcon(":/encrypted.png"));
        padLock->setCursor(Qt::ArrowCursor);
        padLock->setToolTip(tr("Display encryption details."));

        int extent = form->hostNameEdit->height() - 2;
        padLock->resize(extent, extent);
        padLock->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Ignored);

        QHBoxLayout *layout = new QHBoxLayout(form->hostNameEdit);
        layout->setMargin(form->hostNameEdit->style()->pixelMetric(QStyle::PM_DefaultFrameWidth));
        layout->setSpacing(0);
        layout->addStretch();
        layout->addWidget(padLock);

        form->hostNameEdit->setLayout(layout);

        connect(padLock, SIGNAL(clicked()),
                this, SLOT(displayCertificateInfo()));
    } else {
        padLock->show();
    }
}

void SslClient::socketReadyRead()
{
    appendString(QString::fromUtf8(socket->readAll()));
}

void SslClient::sendData()
{
    QString input = form->sessionInput->text();
    appendString(input + "\n");
    socket->write(input.toUtf8() + "\r\n");
    form->sessionInput->clear();
}

void SslClient::sslErrors(const QList<QSslError> &errors)
{
    QDialog errorDialog(this);
    Ui_SslErrors ui;
    ui.setupUi(&errorDialog);
    connect(ui.certificateChainButton, SIGNAL(clicked()),
            this, SLOT(displayCertificateInfo()));

    foreach (const QSslError &error, errors)
        ui.sslErrorList->addItem(error.errorString());

    if (errorDialog.exec() == QDialog::Accepted)
        socket->ignoreSslErrors();
}

void SslClient::displayCertificateInfo()
{
    CertificateInfo *info = new CertificateInfo(this);
    info->setCertificateChain(socket->peerCertificateChain());
    info->exec();
    info->deleteLater();
}

void SslClient::appendString(const QString &line)
{
    QTextCursor cursor(form->sessionOutput->textCursor());
    cursor.movePosition(QTextCursor::End);
    cursor.insertText(line);
    form->sessionOutput->verticalScrollBar()->setValue(form->sessionOutput->verticalScrollBar()->maximum());
}
