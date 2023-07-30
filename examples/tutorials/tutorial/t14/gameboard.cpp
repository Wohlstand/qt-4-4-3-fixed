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
#include <QShortcut>
#include <QVBoxLayout>
#include <QWidget>

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
    QFrame *cannonBox = new QFrame;
//! [0] //! [1]
    cannonBox->setFrameStyle(QFrame::WinPanel | QFrame::Sunken);
//! [1]

    cannonField = new CannonField;

    connect(angle, SIGNAL(valueChanged(int)),
            cannonField, SLOT(setAngle(int)));
    connect(cannonField, SIGNAL(angleChanged(int)),
            angle, SLOT(setValue(int)));

    connect(force, SIGNAL(valueChanged(int)),
            cannonField, SLOT(setForce(int)));
    connect(cannonField, SIGNAL(forceChanged(int)),
            force, SLOT(setValue(int)));

    connect(cannonField, SIGNAL(hit()),
            this, SLOT(hit()));
    connect(cannonField, SIGNAL(missed()),
            this, SLOT(missed()));

    QPushButton *shoot = new QPushButton(tr("&Shoot"));
    shoot->setFont(QFont("Times", 18, QFont::Bold));

    connect(shoot, SIGNAL(clicked()),
            this, SLOT(fire()));
    connect(cannonField, SIGNAL(canShoot(bool)),
            shoot, SLOT(setEnabled(bool)));

    QPushButton *restart = new QPushButton(tr("&New Game"));
    restart->setFont(QFont("Times", 18, QFont::Bold));

    connect(restart, SIGNAL(clicked()), this, SLOT(newGame()));

    hits = new QLCDNumber(2);
    hits->setSegmentStyle(QLCDNumber::Filled);

    shotsLeft = new QLCDNumber(2);
    shotsLeft->setSegmentStyle(QLCDNumber::Filled);

    QLabel *hitsLabel = new QLabel(tr("HITS"));
    QLabel *shotsLeftLabel = new QLabel(tr("SHOTS LEFT"));

//! [2]
    (void) new QShortcut(Qt::Key_Enter, this, SLOT(fire()));
//! [2] //! [3]
    (void) new QShortcut(Qt::Key_Return, this, SLOT(fire()));
//! [3] //! [4]
    (void) new QShortcut(Qt::CTRL + Qt::Key_Q, this, SLOT(close()));
//! [4]

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(shoot);
    topLayout->addWidget(hits);
    topLayout->addWidget(hitsLabel);
    topLayout->addWidget(shotsLeft);
    topLayout->addWidget(shotsLeftLabel);
    topLayout->addStretch(1);
    topLayout->addWidget(restart);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addWidget(angle);
    leftLayout->addWidget(force);

//! [5]
    QVBoxLayout *cannonLayout = new QVBoxLayout;
//! [5] //! [6]
    cannonLayout->addWidget(cannonField);
    cannonBox->setLayout(cannonLayout);

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(quit, 0, 0);
    gridLayout->addLayout(topLayout, 0, 1);
    gridLayout->addLayout(leftLayout, 1, 0);
    gridLayout->addWidget(cannonBox, 1, 1, 2, 1);
    gridLayout->setColumnStretch(1, 10);
    setLayout(gridLayout);
//! [6]

    angle->setValue(60);
    force->setValue(25);
    angle->setFocus();

    newGame();
}

void GameBoard::fire()
{
    if (cannonField->gameOver() || cannonField->isShooting())
        return;
    shotsLeft->display(shotsLeft->intValue() - 1);
    cannonField->shoot();
}

void GameBoard::hit()
{
    hits->display(hits->intValue() + 1);
    if (shotsLeft->intValue() == 0)
        cannonField->setGameOver();
    else
        cannonField->newTarget();
}

void GameBoard::missed()
{
    if (shotsLeft->intValue() == 0)
        cannonField->setGameOver();
}

void GameBoard::newGame()
{
    shotsLeft->display(15);
    hits->display(0);
    cannonField->restartGame();
    cannonField->newTarget();
}
