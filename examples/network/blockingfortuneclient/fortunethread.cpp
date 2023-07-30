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

#include <QtNetwork>

#include "fortunethread.h"

FortuneThread::FortuneThread(QObject *parent)
    : QThread(parent), quit(false)
{
}

//! [0]
FortuneThread::~FortuneThread()
{
    quit = true;
    cond.wakeOne();
    wait();
}
//! [0]

//! [1] //! [2]
void FortuneThread::requestNewFortune(const QString &hostName, quint16 port)
{
//! [1]
    QMutexLocker locker(&mutex);
    this->hostName = hostName;
    this->port = port;
//! [3]
    if (!isRunning())
        start();
    else
        cond.wakeOne();
}
//! [2] //! [3]

//! [4]
void FortuneThread::run()
{
    mutex.lock();
//! [4] //! [5]
    QString serverName = hostName;
    quint16 serverPort = port;
    mutex.unlock();
//! [5]

//! [6]
    while (!quit) {
//! [7]
        const int Timeout = 5 * 1000;

        QTcpSocket socket;
        socket.connectToHost(serverName, serverPort);
//! [6] //! [8]

        if (!socket.waitForConnected(Timeout)) {
            emit error(socket.error(), socket.errorString());
            return;
        }
//! [8] //! [9]

        while (socket.bytesAvailable() < (int)sizeof(quint16)) {
            if (!socket.waitForReadyRead(Timeout)) {
                emit error(socket.error(), socket.errorString());
                return;
            }
//! [9] //! [10]
        }
//! [10] //! [11]

        quint16 blockSize;
        QDataStream in(&socket);
        in.setVersion(QDataStream::Qt_4_0);
        in >> blockSize;
//! [11] //! [12]

        while (socket.bytesAvailable() < blockSize) {
            if (!socket.waitForReadyRead(Timeout)) {
                emit error(socket.error(), socket.errorString());
                return;
            }
//! [12] //! [13]
        }
//! [13] //! [14]

        QMutexLocker locker(&mutex);

        QString fortune;
        in >> fortune;
        emit newFortune(fortune);
//! [7] //! [14] //! [15]

        cond.wait(&mutex);
        serverName = hostName;
        serverPort = port;
    }
//! [15]
}
