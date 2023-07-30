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

#include "bencodeparser.h"
#include "connectionmanager.h"
#include "torrentclient.h"
#include "torrentserver.h"
#include "trackerclient.h"

#include <QtCore>

TrackerClient::TrackerClient(TorrentClient *downloader, QObject *parent)
    : QObject(parent), torrentDownloader(downloader)
{
    length = 0;
    requestInterval = 5 * 60;
    requestIntervalTimer = -1;
    firstTrackerRequest = true;
    lastTrackerRequest = false;
    firstSeeding = true;

    connect(&http, SIGNAL(done(bool)), this, SLOT(httpRequestDone(bool)));
}

void TrackerClient::start(const MetaInfo &info)
{
    metaInfo = info;
    QTimer::singleShot(0, this, SLOT(fetchPeerList()));

    if (metaInfo.fileForm() == MetaInfo::SingleFileForm) {
        length = metaInfo.singleFile().length;
    } else {
        QList<MetaInfoMultiFile> files = metaInfo.multiFiles();
        for (int i = 0; i < files.size(); ++i)
            length += files.at(i).length;
    }
}

void TrackerClient::startSeeding()
{
    firstSeeding = true;
    fetchPeerList();
}

void TrackerClient::stop()
{
    lastTrackerRequest = true;
    http.abort();
    fetchPeerList();
}

void TrackerClient::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == requestIntervalTimer) {
        if (http.state() == QHttp::Unconnected)
            fetchPeerList();
    } else {
        QObject::timerEvent(event);
    }
}

void TrackerClient::fetchPeerList()
{
    // Prepare connection details
    QString fullUrl = metaInfo.announceUrl();
    QUrl url(fullUrl);
    QString passkey = "?";
    if (fullUrl.contains("?passkey")) {
        passkey = metaInfo.announceUrl().mid(fullUrl.indexOf("?passkey"), -1);
        passkey += "&";
    }

    // Percent encode the hash
    QByteArray infoHash = torrentDownloader->infoHash();
    QString encodedSum;
    for (int i = 0; i < infoHash.size(); ++i) {
        encodedSum += '%';
        encodedSum += QString::number(infoHash[i], 16).right(2).rightJustified(2, '0');
    }

    bool seeding = (torrentDownloader->state() == TorrentClient::Seeding);
    QByteArray query;
    query += url.path().toLatin1();
    query += passkey;
    query += "info_hash=" + encodedSum;
    query += "&peer_id=" + ConnectionManager::instance()->clientId();
    query += "&port=" + QByteArray::number(TorrentServer::instance()->serverPort());
    query += "&compact=1";
    query += "&uploaded=" + QByteArray::number(torrentDownloader->uploadedBytes());

    if (!firstSeeding) {
        query += "&downloaded=0";
        query += "&left=0";
    } else {
        query += "&downloaded=" + QByteArray::number(
            torrentDownloader->downloadedBytes());
        int left = qMax<int>(0, metaInfo.totalSize() - torrentDownloader->downloadedBytes());
        query += "&left=" + QByteArray::number(seeding ? 0 : left);
    }

    if (seeding && firstSeeding) {
        query += "&event=completed";
        firstSeeding = false;
    } else if (firstTrackerRequest) {
        firstTrackerRequest = false;
        query += "&event=started";
    } else if(lastTrackerRequest) {
        query += "&event=stopped";
    }

    if (!trackerId.isEmpty())
        query += "&trackerid=" + trackerId;

    http.setHost(url.host(), url.port() == -1 ? 80 : url.port());
    if (!url.userName().isEmpty())
        http.setUser(url.userName(), url.password());
    http.get(query);
}

void TrackerClient::httpRequestDone(bool error)
{
    if (lastTrackerRequest) {
        emit stopped();
        return;
    }

    if (error) {
        emit connectionError(http.error());
        return;
    }

    QByteArray response = http.readAll();
    http.abort();

    BencodeParser parser;
    if (!parser.parse(response)) {
        qWarning("Error parsing bencode response from tracker: %s",
                 qPrintable(parser.errorString()));
        http.abort();
        return;
    }

    QMap<QByteArray, QVariant> dict = parser.dictionary();

    if (dict.contains("failure reason")) {
        // no other items are present
        emit failure(QString::fromUtf8(dict.value("failure reason").toByteArray()));
        return;
    }

    if (dict.contains("warning message")) {
        // continue processing
        emit warning(QString::fromUtf8(dict.value("warning message").toByteArray()));
    }

    if (dict.contains("tracker id")) {
        // store it
        trackerId = dict.value("tracker id").toByteArray();
    }

    if (dict.contains("interval")) {
        // Mandatory item
        if (requestIntervalTimer != -1)
            killTimer(requestIntervalTimer);
        requestIntervalTimer = startTimer(dict.value("interval").toInt() * 1000);
    }

    if (dict.contains("peers")) {
        // store it
        peers.clear();
        QVariant peerEntry = dict.value("peers");
        if (peerEntry.type() == QVariant::List) {
            QList<QVariant> peerTmp = peerEntry.toList();
            for (int i = 0; i < peerTmp.size(); ++i) {
                TorrentPeer tmp;
                QMap<QByteArray, QVariant> peer = qVariantValue<QMap<QByteArray, QVariant> >(peerTmp.at(i));
                tmp.id = QString::fromUtf8(peer.value("peer id").toByteArray());
                tmp.address.setAddress(QString::fromUtf8(peer.value("ip").toByteArray()));
                tmp.port = peer.value("port").toInt();
                peers << tmp;
            }
        } else {
            QByteArray peerTmp = peerEntry.toByteArray();
            for (int i = 0; i < peerTmp.size(); i += 6) {
                TorrentPeer tmp;
                uchar *data = (uchar *)peerTmp.constData() + i;
                tmp.port = (int(data[4]) << 8) + data[5];
                uint ipAddress = 0;
                ipAddress += uint(data[0]) << 24;
                ipAddress += uint(data[1]) << 16;
                ipAddress += uint(data[2]) << 8;
                ipAddress += uint(data[3]);
                tmp.address.setAddress(ipAddress);
                peers << tmp;
            }
        }
        emit peerListUpdated(peers);
    }
}
