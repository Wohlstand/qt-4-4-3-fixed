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

/*
rsslisting.cpp

Provides a widget for displaying news items from RDF news sources.
RDF is an XML-based format for storing items of information (see
http://www.w3.org/RDF/ for details).

The widget itself provides a simple user interface for specifying
the URL of a news source, and controlling the downloading of news.

The widget downloads and parses the XML asynchronously, feeding the
data to an XML reader in pieces. This allows the user to interrupt
its operation, and also allows very large data sources to be read.
*/


#include <QtCore>
#include <QtGui>
#include <QtNetwork>
#include <QtXml>

#include "rsslisting.h"


/*
    Constructs an RSSListing widget with a simple user interface, and sets
    up the XML reader to use a custom handler class.

    The user interface consists of a line edit, two push buttons, and a
    list view widget. The line edit is used for entering the URLs of news
    sources; the push buttons start and abort the process of reading the
    news.
*/

RSSListing::RSSListing(QWidget *parent)
    : QWidget(parent)
{
    lineEdit = new QLineEdit(this);

    fetchButton = new QPushButton(tr("Fetch"), this);
    abortButton = new QPushButton(tr("Abort"), this);
    abortButton->setEnabled(false);

    treeWidget = new QTreeWidget(this);
    QStringList headerLabels;
    headerLabels << tr("Title") << tr("Link");
    treeWidget->setHeaderLabels(headerLabels);

    handler = 0;

    connect(&http, SIGNAL(readyRead(const QHttpResponseHeader &)),
             this, SLOT(readData(const QHttpResponseHeader &)));

    connect(&http, SIGNAL(requestFinished(int, bool)),
             this, SLOT(finished(int, bool)));

    connect(lineEdit, SIGNAL(returnPressed()), this, SLOT(fetch()));
    connect(fetchButton, SIGNAL(clicked()), this, SLOT(fetch()));
    connect(abortButton, SIGNAL(clicked()), &http, SLOT(abort()));

    QVBoxLayout *layout = new QVBoxLayout(this);

    QHBoxLayout *hboxLayout = new QHBoxLayout;

    hboxLayout->addWidget(lineEdit);
    hboxLayout->addWidget(fetchButton);
    hboxLayout->addWidget(abortButton);

    layout->addLayout(hboxLayout);
    layout->addWidget(treeWidget);

    setWindowTitle(tr("RSS listing example"));
}

/*
    Starts fetching data from a news source specified in the line
    edit widget.

    The line edit is made read only to prevent the user from modifying its
    contents during the fetch; this is only for cosmetic purposes.
    The fetch button is disabled, and the abort button is enabled to allow
    the user to interrupt processing. The list view is cleared, and we
    define the last list view item to be 0, meaning that there are no
    existing items in the list.

    We reset the flag used to determine whether parsing should begin again
    or continue. A new handler is created, if required, and made available
    to the reader.

    The HTTP handler is supplied with the raw contents of the line edit and
    a fetch is initiated. We keep the ID value returned by the HTTP handler
    for future reference.
*/

void RSSListing::fetch()
{
    lineEdit->setReadOnly(true);
    fetchButton->setEnabled(false);
    abortButton->setEnabled(true);
    treeWidget->clear();

    lastItemCreated = 0;

    newInformation = true;

    if (handler != 0)
        delete handler;
    handler = new Handler;

//! [0]
    xmlReader.setContentHandler(handler);
    xmlReader.setErrorHandler(handler);
//! [0]

    connect(handler, SIGNAL(newItem(QString &, QString &)),
             this, SLOT(addItem(QString &, QString &)));

    QUrl url(lineEdit->text());

    http.setHost(url.host());
    connectionId = http.get(url.path());
}

/*
    Reads data received from the RDF source.

    We read all the available data, and pass it to the XML
    input source. The first time we receive new information,
    the reader is set up for a new incremental parse;
    we continue parsing using a different function on
    subsequent calls involving the same data source.

    If parsing fails for any reason, we abort the fetch.
*/

//! [1]
void RSSListing::readData(const QHttpResponseHeader &resp)
{
    bool ok;

    if (resp.statusCode() != 200)
        http.abort();
    else {
        xmlInput.setData(http.readAll());

        if (newInformation) {
            ok = xmlReader.parse(&xmlInput, true);
            newInformation = false;
        }
        else
            ok = xmlReader.parseContinue();

        if (!ok)
            http.abort();
    }
}
//! [1]

/*
    Finishes processing an HTTP request.

    The default behavior is to keep the text edit read only.

    If an error has occurred, the user interface is made available
    to the user for further input, allowing a new fetch to be
    started.

    If the HTTP get request has finished, we perform a final
    parsing operation on the data returned to ensure that it was
    well-formed. Whether this is successful or not, we make the
    user interface available to the user for further input.
*/

void RSSListing::finished(int id, bool error)
{
    if (error) {
        qWarning("Received error during HTTP fetch.");
        lineEdit->setReadOnly(false);
        abortButton->setEnabled(false);
        fetchButton->setEnabled(true);
    }
    else if (id == connectionId) {

        bool ok = xmlReader.parseContinue();
        if (!ok)
            qWarning("Parse error at the end of input.");

        lineEdit->setReadOnly(false);
        abortButton->setEnabled(false);
        fetchButton->setEnabled(true);
    }
}

/*
    Adds an item to the list view as it is reported by the handler.

    We keep a record of the last item created to ensure that the
    items are created in sequence.
*/

void RSSListing::addItem(QString &title, QString &link)
{
    QTreeWidgetItem *item;

    item = new QTreeWidgetItem(treeWidget, lastItemCreated);
    item->setText(0, title);
    item->setText(1, link);

    lastItemCreated = item;
}

