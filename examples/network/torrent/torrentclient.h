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

#ifndef TORRENTCLIENT_H
#define TORRENTCLIENT_H

#include <QBitArray>
#include <QHostAddress>
#include <QList>

class MetaInfo;
class PeerWireClient;
class TorrentClientPrivate;
class TorrentPeer;
class TorrentPiece;
QT_BEGIN_NAMESPACE
class QTimerEvent;
QT_END_NAMESPACE

class TorrentPeer {
public:
    QHostAddress address;
    quint16 port;
    QString id;
    bool interesting;
    bool seed;
    uint lastVisited;
    uint connectStart;
    uint connectTime;
    QBitArray pieces;
    int numCompletedPieces;

    inline bool operator==(const TorrentPeer &other)
    {
        return port == other.port
            && address == other.address
            && id == other.id;
    }
};

class TorrentClient : public QObject
{
    Q_OBJECT

public:
    enum State {
        Idle,
        Paused,
        Stopping,
        Preparing,
        Searching,
        Connecting,
        WarmingUp,
        Downloading,
        Endgame,
        Seeding
    };
    enum Error {
        UnknownError,
        TorrentParseError,
        InvalidTrackerError,
        FileError,
        ServerError
    };

    TorrentClient(QObject *parent = 0);
    ~TorrentClient();

    bool setTorrent(const QString &fileName);
    bool setTorrent(const QByteArray &torrentData);
    MetaInfo metaInfo() const;

    void setMaxConnections(int connections);
    int maxConnections() const;

    void setDestinationFolder(const QString &directory);
    QString destinationFolder() const;

    void setDumpedState(const QByteArray &dumpedState);
    QByteArray dumpedState() const;

    // Progress and stats for download feedback.
    qint64 progress() const;
    void setDownloadedBytes(qint64 bytes);
    qint64 downloadedBytes() const;
    void setUploadedBytes(qint64 bytes);
    qint64 uploadedBytes() const;
    int connectedPeerCount() const;
    int seedCount() const;
   
    // Accessors for the tracker
    QByteArray peerId() const;
    QByteArray infoHash() const;
    quint16 serverPort() const;

    // State and error.
    State state() const;
    QString stateString() const;
    Error error() const;
    QString errorString() const;

signals:
    void stateChanged(TorrentClient::State state);
    void error(TorrentClient::Error error);

    void downloadCompleted();
    void peerInfoUpdated();
    
    void dataSent(int uploadedBytes);
    void dataReceived(int downloadedBytes);
    void progressUpdated(int percentProgress);
    void downloadRateUpdated(int bytesPerSecond);
    void uploadRateUpdated(int bytesPerSecond);

    void stopped();

public slots:
    void start();
    void stop();
    void setPaused(bool paused);
    void setupIncomingConnection(PeerWireClient *client);

protected slots:
    void timerEvent(QTimerEvent *event);

private slots:
    // File management
    void sendToPeer(int readId, int pieceIndex, int begin, const QByteArray &data);
    void fullVerificationDone();
    void pieceVerified(int pieceIndex, bool ok);
    void handleFileError();

    // Connection handling
    void connectToPeers();
    QList<TorrentPeer *> weighedFreePeers() const;
    void setupOutgoingConnection();
    void initializeConnection(PeerWireClient *client);
    void removeClient();
    void peerPiecesAvailable(const QBitArray &pieces);
    void peerRequestsBlock(int pieceIndex, int begin, int length);
    void blockReceived(int pieceIndex, int begin, const QByteArray &data);
    void peerWireBytesWritten(qint64 bytes);
    void peerWireBytesReceived(qint64 bytes);
    int blocksLeftForPiece(const TorrentPiece *piece) const;

    // Scheduling
    void scheduleUploads();
    void scheduleDownloads();
    void schedulePieceForClient(PeerWireClient *client);
    void requestMore(PeerWireClient *client);
    int requestBlocks(PeerWireClient *client, TorrentPiece *piece, int maxBlocks);
    void peerChoked();
    void peerUnchoked();

    // Tracker handling
    void addToPeerList(const QList<TorrentPeer> &peerList);
    void trackerStopped();

    // Progress
    void updateProgress(int progress = -1);

private:
    TorrentClientPrivate *d;
    friend class TorrentClientPrivate;
};

#endif
