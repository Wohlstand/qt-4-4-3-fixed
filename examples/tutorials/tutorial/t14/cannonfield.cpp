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

#include <QDateTime>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>

#include <math.h>
#include <stdlib.h>

#include "cannonfield.h"

CannonField::CannonField(QWidget *parent)
    : QWidget(parent)
{
    currentAngle = 45;
    currentForce = 0;
    timerCount = 0;
    autoShootTimer = new QTimer(this);
    connect(autoShootTimer, SIGNAL(timeout()), this, SLOT(moveShot()));
    shootAngle = 0;
    shootForce = 0;
    target = QPoint(0, 0);
    gameEnded = false;
//! [0]
    barrelPressed = false;
//! [0]
    setPalette(QPalette(QColor(250, 250, 200)));
    setAutoFillBackground(true);
    newTarget();
}

void CannonField::setAngle(int angle)
{
    if (angle < 5)
        angle = 5;
    if (angle > 70)
        angle = 70;
    if (currentAngle == angle)
        return;
    currentAngle = angle;
    update(cannonRect());
    emit angleChanged(currentAngle);
}

void CannonField::setForce(int force)
{
    if (force < 0)
        force = 0;
    if (currentForce == force)
        return;
    currentForce = force;
    emit forceChanged(currentForce);
}

void CannonField::shoot()
{
    if (isShooting())
        return;
    timerCount = 0;
    shootAngle = currentAngle;
    shootForce = currentForce;
    autoShootTimer->start(5);
    emit canShoot(false);
}

void CannonField::newTarget()
{
    static bool firstTime = true;

    if (firstTime) {
        firstTime = false;
        QTime midnight(0, 0, 0);
        qsrand(midnight.secsTo(QTime::currentTime()));
    }
    target = QPoint(200 + qrand() % 190, 10 + qrand() % 255);
    update();
}

void CannonField::setGameOver()
{
    if (gameEnded)
        return;
    if (isShooting())
        autoShootTimer->stop();
    gameEnded = true;
    update();
}

void CannonField::restartGame()
{
    if (isShooting())
        autoShootTimer->stop();
    gameEnded = false;
    update();
    emit canShoot(true);
}

void CannonField::moveShot()
{
    QRegion region = shotRect();
    ++timerCount;

    QRect shotR = shotRect();

    if (shotR.intersects(targetRect())) {
        autoShootTimer->stop();
        emit hit();
        emit canShoot(true);
//! [1]
    } else if (shotR.x() > width() || shotR.y() > height()
               || shotR.intersects(barrierRect())) {
//! [1]
        autoShootTimer->stop();
        emit missed();
        emit canShoot(true);
    } else {
        region = region.unite(shotR);
    }
    update(region);
}

//! [2]
void CannonField::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;
    if (barrelHit(event->pos()))
        barrelPressed = true;
}
//! [2]

//! [3]
void CannonField::mouseMoveEvent(QMouseEvent *event)
{
    if (!barrelPressed)
        return;
    QPoint pos = event->pos();
    if (pos.x() <= 0)
        pos.setX(1);
    if (pos.y() >= height())
        pos.setY(height() - 1);
    double rad = atan(((double)rect().bottom() - pos.y()) / pos.x());
    setAngle(qRound(rad * 180 / 3.14159265));
//! [3] //! [4]
}
//! [4]

//! [5]
void CannonField::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        barrelPressed = false;
}
//! [5]

void CannonField::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);

    if (gameEnded) {
        painter.setPen(Qt::black);
        painter.setFont(QFont("Courier", 48, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter, tr("Game Over"));
    }
    paintCannon(painter);
//! [6]
    paintBarrier(painter);
//! [6]
    if (isShooting())
        paintShot(painter);
    if (!gameEnded)
        paintTarget(painter);
}

void CannonField::paintShot(QPainter &painter)
{
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    painter.drawRect(shotRect());
}

void CannonField::paintTarget(QPainter &painter)
{
    painter.setPen(Qt::black);
    painter.setBrush(Qt::red);
    painter.drawRect(targetRect());
}

//! [7]
void CannonField::paintBarrier(QPainter &painter)
{
    painter.setPen(Qt::black);
    painter.setBrush(Qt::yellow);
    painter.drawRect(barrierRect());
}
//! [7]

const QRect barrelRect(30, -5, 20, 10);

void CannonField::paintCannon(QPainter &painter)
{
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::blue);

    painter.save();
    painter.translate(0, height());
    painter.drawPie(QRect(-35, -35, 70, 70), 0, 90 * 16);
    painter.rotate(-currentAngle);
    painter.drawRect(barrelRect);
    painter.restore();
}

QRect CannonField::cannonRect() const
{
    QRect result(0, 0, 50, 50);
    result.moveBottomLeft(rect().bottomLeft());
    return result;
}

QRect CannonField::shotRect() const
{
    const double gravity = 4;

    double time = timerCount / 20.0;
    double velocity = shootForce;
    double radians = shootAngle * 3.14159265 / 180;

    double velx = velocity * cos(radians);
    double vely = velocity * sin(radians);
    double x0 = (barrelRect.right() + 5) * cos(radians);
    double y0 = (barrelRect.right() + 5) * sin(radians);
    double x = x0 + velx * time;
    double y = y0 + vely * time - 0.5 * gravity * time * time;

    QRect result(0, 0, 6, 6);
    result.moveCenter(QPoint(qRound(x), height() - 1 - qRound(y)));
    return result;
}

QRect CannonField::targetRect() const
{
    QRect result(0, 0, 20, 10);
    result.moveCenter(QPoint(target.x(), height() - 1 - target.y()));
    return result;
}

//! [8]
QRect CannonField::barrierRect() const
{
    return QRect(145, height() - 100, 15, 99);
}
//! [8]

//! [9]
bool CannonField::barrelHit(const QPoint &pos) const
{
    QMatrix matrix;
    matrix.translate(0, height());
    matrix.rotate(-currentAngle);
    matrix = matrix.inverted();
    return barrelRect.contains(matrix.map(pos));
}
//! [9]

bool CannonField::isShooting() const
{
    return autoShootTimer->isActive();
}

QSize CannonField::sizeHint() const
{
    return QSize(400, 300);
}
