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

#ifndef PEERWIRECLIENT_H
#define PEERWIRECLIENT_H

#include <QBitArray>
#include <QList>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
class QHostAddress;
class QTimerEvent;
QT_END_NAMESPACE
class TorrentPeer;

struct TorrentBlock
{
    inline TorrentBlock(int p, int o, int l)
        : pieceIndex(p), offset(o), length(l)
    {
    }
    inline bool operator==(const TorrentBlock &other) const
    {
        return pieceIndex == other.pieceIndex
                && offset == other.offset
                && length == other.length;
    }
    
    int pieceIndex;
    int offset;
    int length;
};

class PeerWireClient : public QTcpSocket
{
    Q_OBJECT

public:
    enum PeerWireStateFlag {
        ChokingPeer = 0x1,
        InterestedInPeer = 0x2,
        ChokedByPeer = 0x4,
        PeerIsInterested = 0x8
    };
    Q_DECLARE_FLAGS(PeerWireState, PeerWireStateFlag)

    PeerWireClient(const QByteArray &peerId, QObject *parent = 0);
    void initialize(const QByteArray &infoHash, int pieceCount);

    void setPeer(TorrentPeer *peer);
    TorrentPeer *peer() const;

    // State
    inline PeerWireState peerWireState() const { return pwState; }
    QBitArray availablePieces() const;
    QList<TorrentBlock> incomingBlocks() const;

    // Protocol
    void chokePeer();
    void unchokePeer();
    void sendInterested();
    void sendKeepAlive();
    void sendNotInterested();
    void sendPieceNotification(int piece);
    void sendPieceList(const QBitArray &bitField);
    void requestBlock(int piece, int offset, int length);
    void cancelRequest(int piece, int offset, int length);
    void sendBlock(int piece, int offset, const QByteArray &data);

    // Rate control
    qint64 writeToSocket(qint64 bytes);
    qint64 readFromSocket(qint64 bytes);
    qint64 downloadSpeed() const;
    qint64 uploadSpeed() const;

    bool canTransferMore() const;
    qint64 bytesAvailable() const { return incomingBuffer.size() + QTcpSocket::bytesAvailable(); }
    qint64 socketBytesAvailable() const { return socket.bytesAvailable(); }
    qint64 socketBytesToWrite() const { return socket.bytesToWrite(); }

    void setReadBufferSize(int size);

signals:
    void infoHashReceived(const QByteArray &infoHash);
    void readyToTransfer();

    void choked();
    void unchoked();
    void interested();
    void notInterested();

    void piecesAvailable(const QBitArray &pieces);
    void blockRequested(int pieceIndex, int begin, int length);
    void blockReceived(int pieceIndex, int begin, const QByteArray &data);

    void bytesReceived(qint64 size);

protected slots:
    void connectToHostImplementation(const QString &hostName,
                                     quint16 port, OpenMode openMode = ReadWrite);
    void diconnectFromHostImplementation();

protected:
    void timerEvent(QTimerEvent *event);

    qint64 readData(char *data, qint64 maxlen);
    qint64 readLineData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);

private slots:
    void sendHandShake();
    void processIncomingData();
    void socketStateChanged(QAbstractSocket::SocketState state);

private:
    // Data waiting to be read/written
    QByteArray incomingBuffer;
    QByteArray outgoingBuffer;

    struct BlockInfo {
        int pieceIndex;
        int offset;
        int length;
        QByteArray block;
    };
    QList<BlockInfo> pendingBlocks;
    int pendingBlockSizes;
    QList<TorrentBlock> incoming;

    enum PacketType {
        ChokePacket = 0,
        UnchokePacket = 1,
        InterestedPacket = 2,
        NotInterestedPacket = 3,
        HavePacket = 4,
        BitFieldPacket = 5,
        RequestPacket = 6,
        PiecePacket = 7,
        CancelPacket = 8
    };

    // State
    PeerWireState pwState;
    bool receivedHandShake;
    bool gotPeerId;
    bool sentHandShake;
    int nextPacketLength;

    // Upload/download speed records
    qint64 uploadSpeedData[8];
    qint64 downloadSpeedData[8];
    int transferSpeedTimer;

    // Timeout handling
    int timeoutTimer;
    int pendingRequestTimer;
    bool invalidateTimeout;
    int keepAliveTimer;

    // Checksum, peer ID and set of available pieces
    QByteArray infoHash;
    QByteArray peerIdString;
    QBitArray peerPieces;
    TorrentPeer *torrentPeer;

    QTcpSocket socket;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(PeerWireClient::PeerWireState)

#endif
