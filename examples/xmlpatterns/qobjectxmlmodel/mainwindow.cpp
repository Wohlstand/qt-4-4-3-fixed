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

#include "mainwindow.h"
#include "qobjectxmlmodel.h"

MainWindow::MainWindow()
{
    setupUi(this);

    /* Setup the font. */
    {
        QFont font("Courier");
        font.setFixedPitch(true);

        wholeTree->setFont(font);
        wholeTreeOutput->setFont(font);
        htmlQueryEdit->setFont(font);
    }

    /* We ensure the same name pool is used for our QXmlQuery
     * and node model. */
    QXmlNamePool namePool;

    QObjectXmlModel qObjectModel(this, namePool);

    QXmlQuery query(namePool);

    /* The basic XML view. */
    {
        query.bindVariable("root", qObjectModel.root());
        query.setQuery(QUrl("qrc:/queries/wholeTree.xq"));

        Q_ASSERT(query.isValid());
        QByteArray output;
        QBuffer buffer(&output);
        buffer.open(QIODevice::WriteOnly);

        /* Let's the use the formatter, so it's a bit easier to read. */
        QXmlFormatter serializer(query, &buffer);

        query.evaluateTo(&serializer);
        buffer.close();

        {
            QFile queryFile(":/queries/wholeTree.xq");
            queryFile.open(QIODevice::ReadOnly);
            wholeTree->setPlainText(QString::fromUtf8(queryFile.readAll()));
            wholeTreeOutput->setPlainText(QString::fromUtf8(output.constData()));
        }
    }

    /* The HTML view. */
    {
        query.setQuery(QUrl("qrc:/queries/statisticsInHTML.xq"));
        Q_ASSERT(query.isValid());

        QByteArray output;
        QBuffer buffer(&output);
        buffer.open(QIODevice::WriteOnly);

        /* Let's the use the serializer, so we gain a bit of speed. */
        QXmlSerializer serializer(query, &buffer);

        query.evaluateTo(&serializer);
        buffer.close();

        {
            QFile queryFile(":/queries/statisticsInHTML.xq");
            queryFile.open(QIODevice::ReadOnly);
            htmlQueryEdit->setPlainText(QString::fromUtf8(queryFile.readAll()));
            htmlOutput->setHtml(QString(output));
        }
    }
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("About QObject XML Model"),
                              tr("<p>The <b>QObject XML Model</b> example shows "
                                 "how to use XQuery on top of data of your choice without "
                                 "converting it to an XML document.</p>"
                                 "<p>In this example a QSimpleXmlNodeModel subclass makes it possible to "
                                 "query a QObject tree using XQuery and retrieve the result as pointers to QObjects, or as XML.</p>"
                                 "<p>A possible use case of this could be to write an application that tests a graphical interface "
                                 "against Human Interface Guidelines, or that queries an application's data which is modeled using "
                                 "a QObject tree and dynamic properties."));
}


