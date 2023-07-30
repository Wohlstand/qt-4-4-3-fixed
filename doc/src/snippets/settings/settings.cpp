/****************************************************************************
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the documentation of the Qt Toolkit.
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

QWidget *win;
QWidget *panel;

void snippet_ctor1()
{
//! [0]
    QSettings settings("MySoft", "Star Runner");
//! [0]
}

void snippet_ctor2()
{
//! [1]
    QCoreApplication::setOrganizationName("MySoft");
//! [1] //! [2]
    QCoreApplication::setOrganizationDomain("mysoft.com");
//! [2] //! [3]
    QCoreApplication::setApplicationName("Star Runner");
//! [3]

//! [4]
    QSettings settings;
//! [4]

//! [5]
    settings.setValue("editor/wrapMargin", 68);
//! [5] //! [6]
    int margin = settings.value("editor/wrapMargin").toInt();
//! [6]
    {
//! [7]
    int margin = settings.value("editor/wrapMargin", 80).toInt();
//! [7]
    }

//! [8]
    settings.setValue("mainwindow/size", win->size());
//! [8] //! [9]
    settings.setValue("mainwindow/fullScreen", win->isFullScreen());
//! [9] //! [10]
    settings.setValue("outputpanel/visible", panel->isVisible());
//! [10]

//! [11]
    settings.beginGroup("mainwindow");
    settings.setValue("size", win->size());
    settings.setValue("fullScreen", win->isFullScreen());
    settings.endGroup();
//! [11]

//! [12]
    settings.beginGroup("outputpanel");
    settings.setValue("visible", panel->isVisible());
    settings.endGroup();
//! [12]
}

void snippet_locations()
{
//! [13]
    QSettings obj1("MySoft", "Star Runner");
//! [13] //! [14]
    QSettings obj2("MySoft");
    QSettings obj3(QSettings::SystemScope, "MySoft", "Star Runner");
    QSettings obj4(QSettings::SystemScope, "MySoft");
//! [14]

    {
//! [15]
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                       "MySoft", "Star Runner");
//! [15]
    }

    {
    QSettings settings("starrunner.ini", QSettings::IniFormat);
    }

    {
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft",
                       QSettings::NativeFormat);
    }
}

class MainWindow : public QMainWindow
{
public:
    MainWindow();

    void writeSettings();
    void readSettings();

protected:
    void closeEvent(QCloseEvent *event);
};

//! [16]
void MainWindow::writeSettings()
{
    QSettings settings("Moose Soft", "Clipper");

    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.endGroup();
}
//! [16]

//! [17]
void MainWindow::readSettings()
{
    QSettings settings("Moose Soft", "Clipper");

    settings.beginGroup("MainWindow");
    resize(settings.value("size", QSize(400, 400)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    settings.endGroup();
}
//! [17]

//! [18]
MainWindow::MainWindow()
{
//! [18] //! [19]
    readSettings();
//! [19] //! [20]
}
//! [20]

bool userReallyWantsToQuit() { return true; }

//! [21]
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (userReallyWantsToQuit()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}
//! [21]
