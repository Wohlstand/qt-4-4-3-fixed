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

void mainWindowExample()
{
    QMdiArea *mdiArea = new QMdiArea;
//! [0]
    QMainWindow *mainWindow = new QMainWindow;
    mainWindow->setCentralWidget(mdiArea);
//! [0]

    mdiArea->addSubWindow(new QPushButton("Push Me Now!"));

    mainWindow->show();
}

void addingSubWindowsExample()
{
    QWidget *internalWidget1 = new QWidget;
    QWidget *internalWidget2 = new QWidget;
    
//! [1]
    QMdiArea mdiArea;
    QMdiSubWindow *subWindow1 = new QMdiSubWindow;
    subWindow1->setWidget(internalWidget1);
    subWindow1->setAttribute(Qt::WA_DeleteOnClose);
    mdiArea.addSubWindow(subWindow1);

    QMdiSubWindow *subWindow2 =
	mdiArea.addSubWindow(internalWidget2);

//! [1]
    subWindow1->show();
    subWindow2->show();

    mdiArea.show();
}

int main(int argv, char **args)
{
    QApplication app(argv, args);

    mainWindowExample();
    //addingSubWindowsExample();

   QAction *act = new QAction(qApp);
   act->setShortcut(Qt::ALT + Qt::Key_S);
   act->setShortcutContext( Qt::ApplicationShortcut );
   QObject::connect(act, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    QWidget widget5;
    widget5.show();
    widget5.addAction(act);

    return app.exec();
}


