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
#include <QLibraryInfo>
#include <QtXmlPatterns>

#include "mainwindow.h"

MainWindow::MainWindow() : m_fileTree(m_namePool)
{
    setupUi(this);

    /* Setup the font. */
    {
        QFont font("Courier",10);
        font.setFixedPitch(true);

        fileTree->setFont(font);
        queryEdit->setFont(font);
        output->setFont(font);
    }

    /* We load one which isn't too big. */
    const QString dir(QLibraryInfo::location(QLibraryInfo::ExamplesPath)  + "/xmlpatterns/filetree");
    qDebug() << dir;

    if(QDir(dir).exists())
        loadDirectory(dir);
    else
        fileTree->setPlainText(tr("Use the Open menu entry to select a directory."));

    const QStringList queries(QDir(":/queries/", "*.xq").entryList());
    int len = queries.count();

    for(int i = 0; i < len; ++i)
        queryBox->addItem(queries.at(i));

}

void MainWindow::on_queryBox_currentIndexChanged()
{
    QFile queryFile(":/queries/" + queryBox->currentText());
    queryFile.open(QIODevice::ReadOnly);

    queryEdit->setPlainText(QString::fromLatin1(queryFile.readAll()));
    evaluateResult();
}

void MainWindow::evaluateResult()
{
    if(queryBox->currentText().isEmpty())
        return;

    QXmlQuery query(m_namePool);
    query.bindVariable("fileTree", m_fileNode);
    query.setQuery(QUrl("qrc:/queries/" + queryBox->currentText()));

    QByteArray formatterOutput;
    QBuffer buffer(&formatterOutput);
    buffer.open(QIODevice::WriteOnly);

    QXmlFormatter formatter(query, &buffer);
    query.evaluateTo(&formatter);

    output->setText(QString::fromLatin1(formatterOutput.constData()));
}

void MainWindow::on_actionOpenDirectory_triggered()
{
    loadDirectory(QFileDialog::getExistingDirectory(this));
}

void MainWindow::loadDirectory(const QString &directory)
{
    Q_ASSERT(QDir(directory).exists());

    m_fileNode = m_fileTree.nodeFor(directory);

    QXmlQuery query(m_namePool);
    query.bindVariable("fileTree", m_fileNode);
    query.setQuery(QUrl("qrc:/queries/wholeTree.xq"));

    QByteArray output;
    QBuffer buffer(&output);
    buffer.open(QIODevice::WriteOnly);

    QXmlFormatter formatter(query, &buffer);
    query.evaluateTo(&formatter);

    treeInfo->setText((QString(tr("%1 rendered as XML.")).arg(directory)));
    fileTree->setText(QString::fromLatin1(output.constData()));
    evaluateResult();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("About File Tree"),
                             tr("<p>The <b>File Tree</b> example shows "
                                "how to use XQuery on top of data of your choice without "
                                "converting it to an XML document.</p>"
                                "<p>In this example a QSimpleXmlNodeModel sub-class makes it possible to "
                                "use the XQuery language's strength of selecting and aggregating on top of "
                                "the file system.</p>"));
}


