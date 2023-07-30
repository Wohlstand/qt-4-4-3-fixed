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

#include <stdio.h>

#include <QtCore/QCoreApplication>
#include <QtCore/QFile>
#include <QtCore/QDebug>
#include <QtCore/QProcess>
#include <QtDBus/QtDBus>

#include "ping-common.h"
#include "complexping.h"

void Ping::start(const QString &name, const QString &oldValue, const QString &newValue)
{
    Q_UNUSED(oldValue);

    if (name != SERVICE_NAME || newValue.isEmpty())
        return;

    // open stdin for reading
    qstdin.open(stdin, QIODevice::ReadOnly);

    // find our remote
    iface = new QDBusInterface(SERVICE_NAME, "/", "com.trolltech.QtDBus.ComplexPong.Pong",
                               QDBusConnection::sessionBus(), this);
    if (!iface->isValid()) {
        fprintf(stderr, "%s\n",
                qPrintable(QDBusConnection::sessionBus().lastError().message()));
        QCoreApplication::instance()->quit();
    }

    connect(iface, SIGNAL(aboutToQuit()), QCoreApplication::instance(), SLOT(quit()));

    while (true) {
        printf("Ask your question: ");

        QString line = QString::fromLocal8Bit(qstdin.readLine()).trimmed();
        if (line.isEmpty()) {
            iface->call("quit");
            return;
        } else if (line == "value") {
            QVariant reply = iface->property("value");
            if (!reply.isNull())
                printf("value = %s\n", qPrintable(reply.toString()));
        } else if (line.startsWith("value=")) {
            iface->setProperty("value", line.mid(6));            
        } else {
            QDBusReply<QDBusVariant> reply = iface->call("query", line);
            if (reply.isValid())
                printf("Reply was: %s\n", qPrintable(reply.value().variant().toString()));
        }

        if (iface->lastError().isValid())
            fprintf(stderr, "Call failed: %s\n", qPrintable(iface->lastError().message()));
    }
}    

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    if (!QDBusConnection::sessionBus().isConnected()) {
        fprintf(stderr, "Cannot connect to the D-BUS session bus.\n"
                "To start it, run:\n"
                "\teval `dbus-launch --auto-syntax`\n");
        return 1;
    }

    Ping ping;
    ping.connect(QDBusConnection::sessionBus().interface(),
                 SIGNAL(serviceOwnerChanged(QString,QString,QString)),
                 SLOT(start(QString,QString,QString)));

    QProcess pong;
    pong.start("./complexpong");

    app.exec();
}
