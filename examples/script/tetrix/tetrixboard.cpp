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

#include "tetrixboard.h"

#include <QtGui>

Q_DECLARE_METATYPE(QPainter*)

TetrixBoard::TetrixBoard(QWidget *parent)
    : QFrame(parent)
{
    timer = new QTimer(this);
    qMetaTypeId<QPainter*>();
}

void TetrixBoard::setNextPieceLabel(QWidget *label)
{
    nextPieceLabel = qobject_cast<QLabel*>(label);
}

QObject *TetrixBoard::getTimer()
{
    return timer;
}

QSize TetrixBoard::minimumSizeHint() const
{
    return QSize(BoardWidth * 5 + frameWidth() * 2,
                 BoardHeight * 5 + frameWidth() * 2);
}

void TetrixBoard::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    QPainter painter(this);
    QRect rect = contentsRect();
    int boardTop = rect.bottom() - BoardHeight*squareHeight();
    painter.translate(rect.left(), boardTop);

    emit paintRequested(&painter);
}

void TetrixBoard::keyPressEvent(QKeyEvent *event)
{
    emit keyPressed(event->key());
}

void TetrixBoard::showNextPiece(int width, int height)
{
    if (!nextPieceLabel)
        return;

    QPixmap pixmap(width * squareWidth(), height * squareHeight());
    QPainter painter(&pixmap);
    painter.fillRect(pixmap.rect(), nextPieceLabel->palette().background());

    emit paintNextPieceRequested(&painter);

    nextPieceLabel->setPixmap(pixmap);
}

void TetrixBoard::drawPauseScreen(QPainter *painter)
{
    painter->drawText(contentsRect(), Qt::AlignCenter, tr("Pause"));
}

void TetrixBoard::drawSquare(QPainter *painter, int x, int y, int shape)
{
    static const QRgb colorTable[8] = {
        0x000000, 0xCC6666, 0x66CC66, 0x6666CC,
        0xCCCC66, 0xCC66CC, 0x66CCCC, 0xDAAA00
    };

    x = x*squareWidth();
    y = y*squareHeight();

    QColor color = colorTable[shape];
    painter->fillRect(x + 1, y + 1, squareWidth() - 2, squareHeight() - 2,
                      color);

    painter->setPen(color.light());
    painter->drawLine(x, y + squareHeight() - 1, x, y);
    painter->drawLine(x, y, x + squareWidth() - 1, y);

    painter->setPen(color.dark());
    painter->drawLine(x + 1, y + squareHeight() - 1,
                      x + squareWidth() - 1, y + squareHeight() - 1);
    painter->drawLine(x + squareWidth() - 1, y + squareHeight() - 1,
                      x + squareWidth() - 1, y + 1);
}
