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

MainWindow::MainWindow()
    : QMainWindow()
{
    area = new SvgWindow;

    QMenu *fileMenu = new QMenu(tr("&File"), this);
    QAction *openAction = fileMenu->addAction(tr("&Open..."));
    openAction->setShortcut(QKeySequence(tr("Ctrl+O")));
    QAction *quitAction = fileMenu->addAction(tr("E&xit"));
    quitAction->setShortcut(QKeySequence(tr("Ctrl+Q")));

    menuBar()->addMenu(fileMenu);

    QMenu *rendererMenu = new QMenu(tr("&Renderer"), this);
    nativeAction = rendererMenu->addAction(tr("&Native"));
    nativeAction->setCheckable(true);
    nativeAction->setChecked(true);
    #ifndef QT_NO_OPENGL
    glAction = rendererMenu->addAction(tr("&OpenGL"));
    glAction->setCheckable(true);
    #endif
    imageAction = rendererMenu->addAction(tr("&Image"));
    imageAction->setCheckable(true);

    #ifndef QT_NO_OPENGL
    rendererMenu->addSeparator();
    highQualityAntialiasingAction = rendererMenu->addAction(tr("&High Quality Antialiasing"));
    highQualityAntialiasingAction->setEnabled(false);
    highQualityAntialiasingAction->setCheckable(true);
    highQualityAntialiasingAction->setChecked(false);
    connect(highQualityAntialiasingAction, SIGNAL(toggled(bool)), this, SLOT(setHighQualityAntialiasing(bool)));
    #endif

    QActionGroup *rendererGroup = new QActionGroup(this);
    rendererGroup->addAction(nativeAction);
    #ifndef QT_NO_OPENGL
    rendererGroup->addAction(glAction);
    #endif
    rendererGroup->addAction(imageAction);

    menuBar()->addMenu(rendererMenu);

    connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(rendererGroup, SIGNAL(triggered(QAction *)),
            this, SLOT(setRenderer(QAction *)));

    setCentralWidget(area);
    setWindowTitle(tr("SVG Viewer"));
}

void MainWindow::setHighQualityAntialiasing(bool highQualityAntialiasing)
{
    area->setHighQualityAntialiasing(highQualityAntialiasing);
}

void MainWindow::openFile(const QString &path)
{
    QString fileName;
    if (path.isNull())
        fileName = QFileDialog::getOpenFileName(this, tr("Open SVG File"),
                                                currentPath, "*.svg");
    else
        fileName = path;

    if (!fileName.isEmpty()) {
        area->openFile(fileName);
        if (!fileName.startsWith(":/")) {
            currentPath = fileName;
            setWindowTitle(tr("%1 - SVGViewer").arg(currentPath));
        }
    }
}

void MainWindow::setRenderer(QAction *action)
{
    #ifndef QT_NO_OPENGL
    highQualityAntialiasingAction->setEnabled(false);
    #endif

    if (action == nativeAction)
        area->setRenderer(SvgWindow::Native);
    #ifndef QT_NO_OPENGL
    else if (action == glAction) {
        area->setRenderer(SvgWindow::OpenGL);
        highQualityAntialiasingAction->setEnabled(true);
    }
    #endif
    else if (action == imageAction)
        area->setRenderer(SvgWindow::Image);
}
