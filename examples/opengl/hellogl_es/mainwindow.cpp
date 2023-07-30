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

#include "mainwindow.h"

#include <QApplication>
#include <QMenuBar>
#include <QGroupBox>
#include <QGridLayout>
#include <QSlider>
#include <QLabel>
#include <QTimer>

#include "glwidget.h"

MainWindow::MainWindow()
{
    GLWidget *glwidget = new GLWidget();
    QLabel *label = new QLabel(this);
    QTimer *timer = new QTimer(this);
    QSlider *slider = new QSlider(this);
    slider->setOrientation(Qt::Horizontal);
    
    slider->setRange(0, 100);
    slider->setSliderPosition(50);
    timer->setInterval(10);
    label->setText("A QGlWidget with OpenGl ES");
    label->setAlignment(Qt::AlignHCenter);

    QGroupBox * groupBox = new QGroupBox(this);
    setCentralWidget(groupBox);
    groupBox->setTitle("OpenGL ES Example");

    QGridLayout *layout = new QGridLayout(groupBox);

    layout->addWidget(glwidget,1,0,8,1);
    layout->addWidget(label,9,0,1,1);
    layout->addWidget(slider, 11,0,1,1);

    groupBox->setLayout(layout);

    QMenu *fileMenu = new QMenu("File");
    QMenu *helpMenu = new QMenu("Help");
    QMenu *showMenu = new QMenu("Show");
    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(showMenu);
    menuBar()->addMenu(helpMenu);
    QAction *exit = new QAction("Exit", fileMenu);
    QAction *aboutQt = new QAction("AboutQt", helpMenu);
    QAction *showLogo = new QAction("Show 3D Logo", showMenu);
    QAction *showTexture = new QAction("Show 2D Texture", showMenu);
    QAction *showBubbles = new QAction("Show bubbles", showMenu);
    showBubbles->setCheckable(true);
    showBubbles->setChecked(true);
    fileMenu->addAction(exit);
    helpMenu->addAction(aboutQt);
    showMenu->addAction(showLogo);
    showMenu->addAction(showTexture);
    showMenu->addAction(showBubbles);

    QObject::connect(timer, SIGNAL(timeout()), glwidget, SLOT(updateGL()));
    QObject::connect(exit, SIGNAL(triggered(bool)), this, SLOT(close()));
    QObject::connect(aboutQt, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt()));

    QObject::connect(showLogo, SIGNAL(triggered(bool)), glwidget, SLOT(setLogo()));
    QObject::connect(showTexture, SIGNAL(triggered(bool)), glwidget, SLOT(setTexture()));
    QObject::connect(showBubbles, SIGNAL(triggered(bool)), glwidget, SLOT(showBubbles(bool)));
    QObject::connect(slider, SIGNAL(valueChanged(int)), glwidget, SLOT(setScaling(int)));
    timer->start();
}
