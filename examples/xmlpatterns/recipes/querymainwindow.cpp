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
#include <QtXmlPatterns>

#include "querymainwindow.h"

QueryMainWindow::QueryMainWindow() : ui_defaultQueries(0)
{
    setupUi(this);

    ui_defaultQueries = qFindChild<QComboBox*>(this, "defaultQueries");

    QMetaObject::connectSlotsByName(this);

    connect(ui_defaultQueries, SIGNAL(currentIndexChanged(int)), SLOT(displayQuery(int)));

    loadInputFile();

    const QStringList queries(QDir(":/files/", "*.xq").entryList());
    int len = queries.count();

    for(int i = 0; i < len; ++i)
        ui_defaultQueries->addItem(queries.at(i));
}

void QueryMainWindow::displayQuery(int index)
{
    QFile queryFile(QString(":files/") + ui_defaultQueries->itemText(index));
    queryFile.open(QIODevice::ReadOnly);

    const QString query(QString::fromLatin1(queryFile.readAll()));

    qFindChild<QTextEdit*>(this, "queryTextEdit")->setPlainText(query);

    evaluate(query);
}

void QueryMainWindow::loadInputFile()
{
    QFile forView;
    forView.setFileName(":/files/cookbook.xml");
    if (!forView.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this,
            tr("Unable to open file"), forView.errorString());
        return;
    }

    QTextStream in(&forView);
    QString inputDocument = in.readAll();
    qFindChild<QTextEdit*>(this, "inputTextEdit")->setPlainText(inputDocument);
}

void QueryMainWindow::evaluate(const QString &str)
{
    /* This function takes the input string and displays the
     * appropriate output using QXmlQuery.
     */
    QXmlQuery query;

    QFile sourceDocument;
    sourceDocument.setFileName(":/files/cookbook.xml");
    sourceDocument.open(QIODevice::ReadOnly);
    query.bindVariable("inputDocument", &sourceDocument);

    query.setQuery(str);

    if(!query.isValid())
        return;

    QByteArray outArray;
    QBuffer buffer(&outArray);
    buffer.open(QIODevice::ReadWrite);
    
    QXmlFormatter formatter(query, &buffer);

    if(!query.evaluateTo(&formatter))
        return;
 
    buffer.close();
    qFindChild<QTextEdit*>(this, "outputTextEdit")->setPlainText(QString::fromUtf8(outArray.constData()));
    
}
