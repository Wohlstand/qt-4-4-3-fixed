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

#include <QtGui>

#include "mainwindow.h"
#include "tabletcanvas.h"

//! [0]
MainWindow::MainWindow(TabletCanvas *canvas)
{
    myCanvas = canvas;
    createActions();
    createMenus();

    myCanvas->setColor(Qt::red);
    myCanvas->setLineWidthType(TabletCanvas::LineWidthPressure);
    myCanvas->setAlphaChannelType(TabletCanvas::NoAlpha);
    myCanvas->setColorSaturationType(TabletCanvas::NoSaturation);

    setWindowTitle(tr("Tablet Example"));
    setCentralWidget(myCanvas);
}
//! [0]

//! [1]
void MainWindow::brushColorAct()
{
    QColor color = QColorDialog::getColor(myCanvas->color());

    if (color.isValid())
        myCanvas->setColor(color);
}
//! [1]

//! [2]
void MainWindow::alphaActionTriggered(QAction *action)
{
    if (action == alphaChannelPressureAction) {
        myCanvas->setAlphaChannelType(TabletCanvas::AlphaPressure);
    } else if (action == alphaChannelTiltAction) {
        myCanvas->setAlphaChannelType(TabletCanvas::AlphaTilt);
    } else {
        myCanvas->setAlphaChannelType(TabletCanvas::NoAlpha);
    }
}
//! [2]

//! [3]
void MainWindow::lineWidthActionTriggered(QAction *action)
{
    if (action == lineWidthPressureAction) {
        myCanvas->setLineWidthType(TabletCanvas::LineWidthPressure);
    } else if (action == lineWidthTiltAction) {
        myCanvas->setLineWidthType(TabletCanvas::LineWidthTilt);
    } else {
        myCanvas->setLineWidthType(TabletCanvas::NoLineWidth);
    }
}
//! [3]

//! [4]
void MainWindow::saturationActionTriggered(QAction *action)
{
    if (action == colorSaturationVTiltAction) {
        myCanvas->setColorSaturationType(TabletCanvas::SaturationVTilt);
    } else if (action == colorSaturationHTiltAction) {
        myCanvas->setColorSaturationType(TabletCanvas::SaturationHTilt);
    } else if (action == colorSaturationPressureAction) {
        myCanvas->setColorSaturationType(TabletCanvas::SaturationPressure);
    } else {
        myCanvas->setColorSaturationType(TabletCanvas::NoSaturation);
    }
}
//! [4]

//! [5]
void MainWindow::saveAct()
{
    QString path = QDir::currentPath() + "/untitled.png";
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Picture"),
                             path);

    if (!myCanvas->saveImage(fileName))
        QMessageBox::information(this, "Error Saving Picture",
                                 "Could not save the image");
}
//! [5]

//! [6]
void MainWindow::loadAct()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Picture"),
                                                    QDir::currentPath());

    if (!myCanvas->loadImage(fileName))
        QMessageBox::information(this, "Error Opening Picture",
                                 "Could not open picture");
}
//! [6]

//! [7]
void MainWindow::aboutAct()
{
    QMessageBox::about(this, tr("About Tablet Example"),
                       tr("This example shows use of a Wacom tablet in Qt"));
}
//! [7]

//! [8]
void MainWindow::createActions()
{
//! [8]
    brushColorAction = new QAction(tr("&Brush Color..."), this);
    brushColorAction->setShortcut(tr("Ctrl+C"));
    connect(brushColorAction, SIGNAL(triggered()),
            this, SLOT(brushColorAct()));

//! [9]
    alphaChannelPressureAction = new QAction(tr("&Pressure"), this);
    alphaChannelPressureAction->setCheckable(true);

    alphaChannelTiltAction = new QAction(tr("&Tilt"), this);
    alphaChannelTiltAction->setCheckable(true);

    noAlphaChannelAction = new QAction(tr("No Alpha Channel"), this);
    noAlphaChannelAction->setCheckable(true);
    noAlphaChannelAction->setChecked(true);

    alphaChannelGroup = new QActionGroup(this);
    alphaChannelGroup->addAction(alphaChannelPressureAction);
    alphaChannelGroup->addAction(alphaChannelTiltAction);
    alphaChannelGroup->addAction(noAlphaChannelAction);
    connect(alphaChannelGroup, SIGNAL(triggered(QAction *)),
            this, SLOT(alphaActionTriggered(QAction *)));

//! [9]
    colorSaturationVTiltAction = new QAction(tr("&Vertical Tilt"), this);
    colorSaturationVTiltAction->setCheckable(true);

    colorSaturationHTiltAction = new QAction(tr("&Horizontal Tilt"), this);
    colorSaturationHTiltAction->setCheckable(true);

    colorSaturationPressureAction = new QAction(tr("&Pressure"), this);
    colorSaturationPressureAction->setCheckable(true);

    noColorSaturationAction = new QAction(tr("&No Color Saturation"), this);
    noColorSaturationAction->setCheckable(true);
    noColorSaturationAction->setChecked(true);

    colorSaturationGroup = new QActionGroup(this);
    colorSaturationGroup->addAction(colorSaturationVTiltAction);
    colorSaturationGroup->addAction(colorSaturationHTiltAction);
    colorSaturationGroup->addAction(colorSaturationPressureAction);
    colorSaturationGroup->addAction(noColorSaturationAction);
    connect(colorSaturationGroup, SIGNAL(triggered(QAction *)),
            this, SLOT(saturationActionTriggered(QAction *)));

    lineWidthPressureAction = new QAction(tr("&Pressure"), this);
    lineWidthPressureAction->setCheckable(true);
    lineWidthPressureAction->setChecked(true);

    lineWidthTiltAction = new QAction(tr("&Tilt"), this);
    lineWidthTiltAction->setCheckable(true);

    lineWidthFixedAction = new QAction(tr("&Fixed"), this);
    lineWidthFixedAction->setCheckable(true);

    lineWidthGroup = new QActionGroup(this);
    lineWidthGroup->addAction(lineWidthPressureAction);
    lineWidthGroup->addAction(lineWidthTiltAction);
    lineWidthGroup->addAction(lineWidthFixedAction);
    connect(lineWidthGroup, SIGNAL(triggered(QAction *)),
            this, SLOT(lineWidthActionTriggered(QAction *)));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcut(tr("Ctrl+X"));
    connect(exitAction, SIGNAL(triggered()),
            this, SLOT(close()));

    loadAction = new QAction(tr("&Open..."), this);
    loadAction->setShortcut(tr("Ctrl+O"));
    connect(loadAction, SIGNAL(triggered()),
            this, SLOT(loadAct()));

    saveAction = new QAction(tr("&Save As..."), this);
    saveAction->setShortcut(tr("Ctrl+S"));
    connect(saveAction, SIGNAL(triggered()),
            this, SLOT(saveAct()));

    aboutAction = new QAction(tr("A&bout"), this);
    aboutAction->setShortcut(tr("Ctrl+B"));
    connect(aboutAction, SIGNAL(triggered()),
            this, SLOT(aboutAct()));

    aboutQtAction = new QAction(tr("About &Qt"), this);
    aboutQtAction->setShortcut(tr("Ctrl+Q"));
    connect(aboutQtAction, SIGNAL(triggered()),
            qApp, SLOT(aboutQt()));
//! [10]
}
//! [10]

//! [11]
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(loadAction);
    fileMenu->addAction(saveAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    brushMenu = menuBar()->addMenu(tr("&Brush"));
    brushMenu->addAction(brushColorAction);

    tabletMenu = menuBar()->addMenu(tr("&Tablet"));

    lineWidthMenu = tabletMenu->addMenu(tr("&Line Width"));
    lineWidthMenu->addAction(lineWidthPressureAction);
    lineWidthMenu->addAction(lineWidthTiltAction);
    lineWidthMenu->addAction(lineWidthFixedAction);

    alphaChannelMenu = tabletMenu->addMenu(tr("&Alpha Channel"));
    alphaChannelMenu->addAction(alphaChannelPressureAction);
    alphaChannelMenu->addAction(alphaChannelTiltAction);
    alphaChannelMenu->addAction(noAlphaChannelAction);

    colorSaturationMenu = tabletMenu->addMenu(tr("&Color Saturation"));
    colorSaturationMenu->addAction(colorSaturationVTiltAction);
    colorSaturationMenu->addAction(colorSaturationHTiltAction);
    colorSaturationMenu->addAction(noColorSaturationAction);

    helpMenu = menuBar()->addMenu("&Help");
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);
}
//! [11]
