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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QMenu;
class QStatusBar;
QT_END_NAMESPACE
class TabletCanvas;

//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(TabletCanvas *canvas);

private slots:
    void brushColorAct();
    void alphaActionTriggered(QAction *action);
    void lineWidthActionTriggered(QAction *action);
    void saturationActionTriggered(QAction *action);
    void saveAct();
    void loadAct();
    void aboutAct();

private:
    void createActions();
    void createMenus();

    TabletCanvas *myCanvas;

    QAction *brushColorAction;
    QActionGroup *brushActionGroup;

    QActionGroup *alphaChannelGroup;
    QAction *alphaChannelPressureAction;
    QAction *alphaChannelTiltAction;
    QAction *noAlphaChannelAction;

    QActionGroup *colorSaturationGroup;
    QAction *colorSaturationVTiltAction;
    QAction *colorSaturationHTiltAction;
    QAction *colorSaturationPressureAction;
    QAction *noColorSaturationAction;

    QActionGroup *lineWidthGroup;
    QAction *lineWidthPressureAction;
    QAction *lineWidthTiltAction;
    QAction *lineWidthFixedAction;

    QAction *exitAction;
    QAction *saveAction;
    QAction *loadAction;

    QAction *aboutAction;
    QAction *aboutQtAction;

    QMenu *fileMenu;
    QMenu *brushMenu;
    QMenu *tabletMenu;
    QMenu *helpMenu;
    QMenu *colorSaturationMenu;
    QMenu *lineWidthMenu;
    QMenu *alphaChannelMenu;
};
//! [0]

#endif