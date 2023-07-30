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

static const char * const listEntries[] = {
    QT_TRANSLATE_NOOP("MainWindow", "First"),
    QT_TRANSLATE_NOOP("MainWindow", "Second"),
    QT_TRANSLATE_NOOP("MainWindow", "Third"),
    0
};

MainWindow::MainWindow()
{
    centralWidget = new QWidget;
    setCentralWidget(centralWidget);

    createGroupBox();

    listWidget = new QListWidget;
    for (int i = 0; listEntries[i]; ++i)
        listWidget->addItem(tr(listEntries[i]));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(groupBox);
    mainLayout->addWidget(listWidget);
    centralWidget->setLayout(mainLayout);

    exitAction = new QAction(tr("E&xit"), this);
    connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->setPalette(QPalette(Qt::red));
    fileMenu->addAction(exitAction);

    setWindowTitle(tr("Language: %1").arg(tr("English")));
    statusBar()->showMessage(tr("Internationalization Example"));

    if (tr("LTR") == "RTL")
        setLayoutDirection(Qt::RightToLeft);
}

void MainWindow::createGroupBox()
{
    groupBox = new QGroupBox(tr("View"));
    perspectiveRadioButton = new QRadioButton(tr("Perspective"));
    isometricRadioButton = new QRadioButton(tr("Isometric"));
    obliqueRadioButton = new QRadioButton(tr("Oblique"));
    perspectiveRadioButton->setChecked(true);

    QVBoxLayout *groupBoxLayout = new QVBoxLayout;
    groupBoxLayout->addWidget(perspectiveRadioButton);
    groupBoxLayout->addWidget(isometricRadioButton);
    groupBoxLayout->addWidget(obliqueRadioButton);
    groupBox->setLayout(groupBoxLayout);
}
