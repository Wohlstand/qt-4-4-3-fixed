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
#include <QtNetwork>

#include "dialog.h"

#if !defined(Q_OS_WINCE)
static const int TotalBytes = 50 * 1024 * 1024;
#else
static const int TotalBytes = 5 * 1024 * 1024;
#endif
static const int PayloadSize = 65536;

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    clientProgressBar = new QProgressBar;
    clientStatusLabel = new QLabel(tr("Client ready"));
    serverProgressBar = new QProgressBar;
    serverStatusLabel = new QLabel(tr("Server ready"));

    startButton = new QPushButton(tr("&Start"));
    quitButton = new QPushButton(tr("&Quit"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(startButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    connect(startButton, SIGNAL(clicked()), this, SLOT(start()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(&tcpServer, SIGNAL(newConnection()),
            this, SLOT(acceptConnection()));
    connect(&tcpClient, SIGNAL(connected()), this, SLOT(startTransfer()));
    connect(&tcpClient, SIGNAL(bytesWritten(qint64)),
            this, SLOT(updateClientProgress(qint64)));
    connect(&tcpClient, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(clientProgressBar);
    mainLayout->addWidget(clientStatusLabel);
    mainLayout->addWidget(serverProgressBar);
    mainLayout->addWidget(serverStatusLabel);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("Loopback"));
}

void Dialog::start()
{
    startButton->setEnabled(false);

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif

    bytesWritten = 0;
    bytesReceived = 0;

    while (!tcpServer.isListening() && !tcpServer.listen()) {
        QMessageBox::StandardButton ret = QMessageBox::critical(this,
                                        tr("Loopback"),
                                        tr("Unable to start the test: %1.")
					.arg(tcpServer.errorString()),
                                        QMessageBox::Retry
					| QMessageBox::Cancel);
        if (ret == QMessageBox::Cancel)
            return;
    }

    serverStatusLabel->setText(tr("Listening"));
    clientStatusLabel->setText(tr("Connecting"));
    tcpClient.connectToHost(QHostAddress::LocalHost, tcpServer.serverPort());
}

void Dialog::acceptConnection()
{
    tcpServerConnection = tcpServer.nextPendingConnection();
    connect(tcpServerConnection, SIGNAL(readyRead()),
            this, SLOT(updateServerProgress()));
    connect(tcpServerConnection, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));

    serverStatusLabel->setText(tr("Accepted connection"));
    tcpServer.close();
}

void Dialog::startTransfer()
{
    bytesToWrite = TotalBytes - (int)tcpClient.write(QByteArray(PayloadSize, '@'));
    clientStatusLabel->setText(tr("Connected"));
}

void Dialog::updateServerProgress()
{
    bytesReceived += (int)tcpServerConnection->bytesAvailable();
    tcpServerConnection->readAll();

    serverProgressBar->setMaximum(TotalBytes);
    serverProgressBar->setValue(bytesReceived);
    serverStatusLabel->setText(tr("Received %1MB")
                               .arg(bytesReceived / (1024 * 1024)));

    if (bytesReceived == TotalBytes) {
        tcpServerConnection->close();
        startButton->setEnabled(true);
#ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
#endif
    }
}

void Dialog::updateClientProgress(qint64 numBytes)
{
    bytesWritten += (int)numBytes;
    if (bytesToWrite > 0)
        bytesToWrite -= (int)tcpClient.write(QByteArray(qMin(bytesToWrite, PayloadSize), '@'));

    clientProgressBar->setMaximum(TotalBytes);
    clientProgressBar->setValue(bytesWritten);
    clientStatusLabel->setText(tr("Sent %1MB")
                               .arg(bytesWritten / (1024 * 1024)));
}

void Dialog::displayError(QAbstractSocket::SocketError socketError)
{
    if (socketError == QTcpSocket::RemoteHostClosedError)
        return;

    QMessageBox::information(this, tr("Network error"),
                             tr("The following error occurred: %1.")
                             .arg(tcpClient.errorString()));

    tcpClient.close();
    tcpServer.close();
    clientProgressBar->reset();
    serverProgressBar->reset();
    clientStatusLabel->setText(tr("Client ready"));
    serverStatusLabel->setText(tr("Server ready"));
    startButton->setEnabled(true);
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
}
