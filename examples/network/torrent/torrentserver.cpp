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

#include "connectionmanager.h"
#include "peerwireclient.h"
#include "ratecontroller.h"
#include "torrentclient.h"
#include "torrentserver.h"

Q_GLOBAL_STATIC(TorrentServer, torrentServer)

TorrentServer *TorrentServer::instance()
{
    return torrentServer();
}

void TorrentServer::addClient(TorrentClient *client)
{
    clients << client;
}

void TorrentServer::removeClient(TorrentClient *client)
{
    clients.removeAll(client);
}

void TorrentServer::incomingConnection(int socketDescriptor)
{
    PeerWireClient *client =
  	new PeerWireClient(ConnectionManager::instance()->clientId(), this);

    if (client->setSocketDescriptor(socketDescriptor)) {
        if (ConnectionManager::instance()->canAddConnection() && !clients.isEmpty()) {
            connect(client, SIGNAL(infoHashReceived(const QByteArray &)),
                    this, SLOT(processInfoHash(const QByteArray &)));
            connect(client, SIGNAL(error(QAbstractSocket::SocketError)),
                    this, SLOT(removeClient()));
            RateController::instance()->addSocket(client);
            ConnectionManager::instance()->addConnection(client);
            return;
        }
    }
    client->abort();
    delete client;
}

void TorrentServer::removeClient()
{
    PeerWireClient *peer = qobject_cast<PeerWireClient *>(sender());
    RateController::instance()->removeSocket(peer);
    ConnectionManager::instance()->removeConnection(peer);
    peer->deleteLater();
}

void TorrentServer::processInfoHash(const QByteArray &infoHash)
{
    PeerWireClient *peer = qobject_cast<PeerWireClient *>(sender());
    foreach (TorrentClient *client, clients) {
        if (client->state() >= TorrentClient::Searching && client->infoHash() == infoHash) {
            peer->disconnect(peer, 0, this, 0);
            client->setupIncomingConnection(peer);
            return;
        }
    }
    removeClient();
}
