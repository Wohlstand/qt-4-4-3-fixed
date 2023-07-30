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

#ifndef CANNONFIELD_H
#define CANNONFIELD_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QTimer;
QT_END_NAMESPACE

class CannonField : public QWidget
{
    Q_OBJECT

public:
    CannonField(QWidget *parent = 0);

    int angle() const { return currentAngle; }
    int force() const { return currentForce; }
//! [0]
    bool gameOver() const { return gameEnded; }
//! [0]
    bool isShooting() const;

public slots:
    void setAngle(int angle);
    void setForce(int force);
    void shoot();
    void newTarget();
//! [1]
    void setGameOver();
    void restartGame();
//! [1]

private slots:
    void moveShot();

signals:
    void hit();
    void missed();
    void angleChanged(int newAngle);
    void forceChanged(int newForce);
//! [2]
    void canShoot(bool can);
//! [2]

protected:
    void paintEvent(QPaintEvent *event);

private:
    void paintShot(QPainter &painter);
    void paintTarget(QPainter &painter);
    void paintCannon(QPainter &painter);
    QRect cannonRect() const;
    QRect shotRect() const;
    QRect targetRect() const;

    int currentAngle;
    int currentForce;

    int timerCount;
    QTimer *autoShootTimer;
    float shootAngle;
    float shootForce;

    QPoint target;

//! [3]
    bool gameEnded;
//! [3]
};

#endif
