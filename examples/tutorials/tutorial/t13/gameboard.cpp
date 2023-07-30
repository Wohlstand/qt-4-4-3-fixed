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

#include <QApplication>
#include <QFont>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLCDNumber>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "cannonfield.h"
#include "gameboard.h"
#include "lcdrange.h"

GameBoard::GameBoard(QWidget *parent)
    : QWidget(parent)
{
    QPushButton *quit = new QPushButton(tr("&Quit"));
    quit->setFont(QFont("Times", 18, QFont::Bold));

    connect(quit, SIGNAL(clicked()), qApp, SLOT(quit()));

    LCDRange *angle = new LCDRange(tr("ANGLE"));
    angle->setRange(5, 70);

    LCDRange *force = new LCDRange(tr("FORCE"));
    force->setRange(10, 50);

//! [0]
    cannonField = new CannonField;
//! [0]

    connect(angle, SIGNAL(valueChanged(int)),
            cannonField, SLOT(setAngle(int)));
    connect(cannonField, SIGNAL(angleChanged(int)),
            angle, SLOT(setValue(int)));

    connect(force, SIGNAL(valueChanged(int)),
            cannonField, SLOT(setForce(int)));
    connect(cannonField, SIGNAL(forceChanged(int)),
            force, SLOT(setValue(int)));

//! [1]
    connect(cannonField, SIGNAL(hit()),
//! [1] //! [2]
            this, SLOT(hit()));
//! [2] //! [3]
    connect(cannonField, SIGNAL(missed()),
//! [3] //! [4]
            this, SLOT(missed()));
//! [4]

    QPushButton *shoot = new QPushButton(tr("&Shoot"));
    shoot->setFont(QFont("Times", 18, QFont::Bold));

//! [5]
    connect(shoot, SIGNAL(clicked()),
//! [5] //! [6]
            this, SLOT(fire()));
//! [6] //! [7]
    connect(cannonField, SIGNAL(canShoot(bool)),
//! [7] //! [8]
            shoot, SLOT(setEnabled(bool)));
//! [8]

//! [9]
    QPushButton *restart = new QPushButton(tr("&New Game"));
    restart->setFont(QFont("Times", 18, QFont::Bold));

    connect(restart, SIGNAL(clicked()), this, SLOT(newGame()));
//! [9] //! [10]

    hits = new QLCDNumber(2);
    hits->setSegmentStyle(QLCDNumber::Filled);

    shotsLeft = new QLCDNumber(2);
    shotsLeft->setSegmentStyle(QLCDNumber::Filled);

    QLabel *hitsLabel = new QLabel(tr("HITS"));
    QLabel *shotsLeftLabel = new QLabel(tr("SHOTS LEFT"));
//! [10]

//! [11]
    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(shoot);
    topLayout->addWidget(hits);
    topLayout->addWidget(hitsLabel);
    topLayout->addWidget(shotsLeft);
    topLayout->addWidget(shotsLeftLabel);
    topLayout->addStretch(1);
//! [11] //! [12]
    topLayout->addWidget(restart);
//! [12]

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addWidget(angle);
    leftLayout->addWidget(force);

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(quit, 0, 0);
    gridLayout->addLayout(topLayout, 0, 1);
    gridLayout->addLayout(leftLayout, 1, 0);
    gridLayout->addWidget(cannonField, 1, 1, 2, 1);
    gridLayout->setColumnStretch(1, 10);
    setLayout(gridLayout);

    angle->setValue(60);
    force->setValue(25);
    angle->setFocus();

//! [13]
    newGame();
//! [13]
}

//! [14]
void GameBoard::fire()
{
    if (cannonField->gameOver() || cannonField->isShooting())
        return;
    shotsLeft->display(shotsLeft->intValue() - 1);
    cannonField->shoot();
}
//! [14]

//! [15]
void GameBoard::hit()
{
    hits->display(hits->intValue() + 1);
    if (shotsLeft->intValue() == 0)
        cannonField->setGameOver();
    else
        cannonField->newTarget();
}
//! [15]

//! [16]
void GameBoard::missed()
{
    if (shotsLeft->intValue() == 0)
        cannonField->setGameOver();
}
//! [16]

//! [17]
void GameBoard::newGame()
{
    shotsLeft->display(15);
    hits->display(0);
    cannonField->restartGame();
    cannonField->newTarget();
}
//! [17]
