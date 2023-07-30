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

#include "locationdialog.h"
#include "mainwindow.h"
#include "settingstree.h"

MainWindow::MainWindow()
{
    settingsTree = new SettingsTree;
    setCentralWidget(settingsTree);

    locationDialog = 0;

    createActions();
    createMenus();

    autoRefreshAct->setChecked(true);
    fallbacksAct->setChecked(true);

    setWindowTitle(tr("Settings Editor"));
    resize(500, 600);
}

void MainWindow::openSettings()
{
    if (!locationDialog)
        locationDialog = new LocationDialog(this);

    if (locationDialog->exec()) {
        QSettings *settings = new QSettings(locationDialog->format(),
                                            locationDialog->scope(),
                                            locationDialog->organization(),
                                            locationDialog->application());
        setSettingsObject(settings);
        fallbacksAct->setEnabled(true);
    }
}

void MainWindow::openIniFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open INI File"),
                               "", tr("INI Files (*.ini *.conf)"));
    if (!fileName.isEmpty()) {
        QSettings *settings = new QSettings(fileName, QSettings::IniFormat);
        setSettingsObject(settings);
        fallbacksAct->setEnabled(false);
    }
}

void MainWindow::openPropertyList()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                               tr("Open Property List"),
                               "", tr("Property List Files (*.plist)"));
    if (!fileName.isEmpty()) {
        QSettings *settings = new QSettings(fileName, QSettings::NativeFormat);
        setSettingsObject(settings);
        fallbacksAct->setEnabled(false);
    }
}

void MainWindow::openRegistryPath()
{
    QString path = QInputDialog::getText(this, tr("Open Registry Path"),
                           tr("Enter the path in the Windows registry:"),
                           QLineEdit::Normal, "HKEY_CURRENT_USER\\");
    if (!path.isEmpty()) {
        QSettings *settings = new QSettings(path, QSettings::NativeFormat);
        setSettingsObject(settings);
        fallbacksAct->setEnabled(false);
    }
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Settings Editor"),
            tr("The <b>Settings Editor</b> example shows how to access "
               "application settings using Qt."));
}

void MainWindow::createActions()
{
    openSettingsAct = new QAction(tr("&Open Application Settings..."), this);
    openSettingsAct->setShortcut(tr("Ctrl+O"));
    connect(openSettingsAct, SIGNAL(triggered()), this, SLOT(openSettings()));

    openIniFileAct = new QAction(tr("Open I&NI File..."), this);
    openIniFileAct->setShortcut(tr("Ctrl+N"));
    connect(openIniFileAct, SIGNAL(triggered()), this, SLOT(openIniFile()));

    openPropertyListAct = new QAction(tr("Open Mac &Property List..."), this);
    openPropertyListAct->setShortcut(tr("Ctrl+P"));
    connect(openPropertyListAct, SIGNAL(triggered()),
            this, SLOT(openPropertyList()));

    openRegistryPathAct = new QAction(tr("Open Windows &Registry Path..."),
                                      this);
    openRegistryPathAct->setShortcut(tr("Ctrl+G"));
    connect(openRegistryPathAct, SIGNAL(triggered()),
            this, SLOT(openRegistryPath()));

    refreshAct = new QAction(tr("&Refresh"), this);
    refreshAct->setShortcut(tr("Ctrl+R"));
    refreshAct->setEnabled(false);
    connect(refreshAct, SIGNAL(triggered()), settingsTree, SLOT(refresh()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    autoRefreshAct = new QAction(tr("&Auto-Refresh"), this);
    autoRefreshAct->setShortcut(tr("Ctrl+A"));
    autoRefreshAct->setCheckable(true);
    autoRefreshAct->setEnabled(false);
    connect(autoRefreshAct, SIGNAL(triggered(bool)),
            settingsTree, SLOT(setAutoRefresh(bool)));
    connect(autoRefreshAct, SIGNAL(triggered(bool)),
            refreshAct, SLOT(setDisabled(bool)));

    fallbacksAct = new QAction(tr("&Fallbacks"), this);
    fallbacksAct->setShortcut(tr("Ctrl+F"));
    fallbacksAct->setCheckable(true);
    fallbacksAct->setEnabled(false);
    connect(fallbacksAct, SIGNAL(triggered(bool)),
            settingsTree, SLOT(setFallbacksEnabled(bool)));

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

#ifndef Q_WS_MAC
    openPropertyListAct->setEnabled(false);
#endif
#ifndef Q_WS_WIN
    openRegistryPathAct->setEnabled(false);
#endif
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openSettingsAct);
    fileMenu->addAction(openIniFileAct);
    fileMenu->addAction(openPropertyListAct);
    fileMenu->addAction(openRegistryPathAct);
    fileMenu->addSeparator();
    fileMenu->addAction(refreshAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    optionsMenu = menuBar()->addMenu(tr("&Options"));
    optionsMenu->addAction(autoRefreshAct);
    optionsMenu->addAction(fallbacksAct);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

void MainWindow::setSettingsObject(QSettings *settings)
{
    settings->setFallbacksEnabled(fallbacksAct->isChecked());
    settingsTree->setSettingsObject(settings);

    refreshAct->setEnabled(true);
    autoRefreshAct->setEnabled(true);

    QString niceName = settings->fileName();
    niceName.replace("\\", "/");
    int pos = niceName.lastIndexOf("/");
    if (pos != -1)
        niceName.remove(0, pos + 1);

    if (!settings->isWritable())
        niceName = tr("%1 (read only)").arg(niceName);

    setWindowTitle(tr("%1 - %2").arg(niceName).arg(tr("Settings Editor")));
}
