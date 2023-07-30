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

#include <QApplication>
#include <QPushButton>
#include <QFileInfo>
#include <QDir>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

//! [0]
    QFileInfo fileInfo1("~/examples/191697/.");
    QFileInfo fileInfo2("~/examples/191697/..");
    QFileInfo fileInfo3("~/examples/191697/main.cpp");
//! [0]    
//! [1]    
    QFileInfo fileInfo4(".");
    QFileInfo fileInfo5("..");
    QFileInfo fileInfo6("main.cpp");
//! [1]    

    qDebug() << fileInfo1.fileName();
    qDebug() << fileInfo2.fileName();
    qDebug() << fileInfo3.fileName();
    qDebug() << fileInfo4.fileName();
    qDebug() << fileInfo5.fileName();
    qDebug() << fileInfo6.fileName();

    QPushButton* button1 = new QPushButton(fileInfo1.dir().path());
    QPushButton* button2 = new QPushButton(fileInfo2.dir().path());
    QPushButton* button3 = new QPushButton(fileInfo3.dir().path());
    QPushButton* button4 = new QPushButton(fileInfo4.dir().path());
    QPushButton* button5 = new QPushButton(fileInfo5.dir().path());
    QPushButton* button6 = new QPushButton(fileInfo6.dir().path());

    QVBoxLayout* vbox = new QVBoxLayout;
    vbox->addWidget(button1);
    vbox->addWidget(button2);
    vbox->addWidget(button3);
    vbox->addWidget(button4);
    vbox->addWidget(button5);
    vbox->addWidget(button6);
    vbox->addStretch(1);

    QGroupBox *groupBox = new QGroupBox("QFileInfo::dir() test");
    groupBox->setLayout(vbox);
    groupBox->show();

    return app.exec();
}
