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
handler.cpp

Provides a handler for processing XML elements found by the reader.

The handler looks for <title> and <link> elements within <item> elements,
and records the text found within them. Link information stored within
rdf:about attributes of <item> elements is also recorded when it is
available.

For each item found, a signal is emitted which specifies its title and
link information. This may be used by user interfaces for the purpose of
displaying items as they are read.
*/

#include <QtGui>

#include "handler.h"

/*
    Reset the state of the handler to ensure that new documents are
    read correctly.

    We return true to indicate that parsing should continue.
*/

bool Handler::startDocument()
{
    inItem = false;
    inTitle = false;
    inLink = false;

    return true;
}

/*
    Process each starting element in the XML document.

    Nested item, title, or link elements are not allowed, so we return false
    if we encounter any of these. We also prohibit multiple definitions of
    title strings.

    Link destinations are read by this function if they are specified as
    attributes in item elements.

    For all cases not explicitly checked for, we return true to indicate that
    the element is acceptable, and that parsing should continue. By doing
    this, we can ignore elements in which we are not interested.
*/

bool Handler::startElement(const QString &, const QString &,
    const QString & qName, const QXmlAttributes &attr)
{
    if (qName == "item") {

        if (inItem)
            return false;
        else {
            inItem = true;
            linkString = attr.value("rdf:about");
        }
    }
    else if (qName == "title") {

        if (inTitle)
            return false;
        else if (!titleString.isEmpty())
            return false;
        else if (inItem)
            inTitle = true;
    }
    else if (qName == "link") {

        if (inLink)
            return false;
        else if (inItem)
            inLink = true;
    }

    return true;
}

/*
    Process each ending element in the XML document.

    For recognized elements, we reset flags to ensure that we can read new
    instances of these elements. If we have read an item element, emit a
    signal to indicate that a new item is available for display.

    We return true to indicate that parsing should continue.
*/

bool Handler::endElement(const QString &, const QString &,
    const QString & qName)
{
    if (qName == "title" && inTitle)
        inTitle = false;
    else if (qName == "link" && inLink)
        inLink = false;
    else if (qName == "item") {
        if (!titleString.isEmpty() && !linkString.isEmpty())
            emit newItem(titleString, linkString);
        inItem = false;
        titleString = "";
        linkString = "";
    }

    return true;
}

/*
    Collect characters when reading the contents of title or link elements
    when they occur within an item element.

    We return true to indicate that parsing should continue.
*/

bool Handler::characters (const QString &chars)
{
    if (inTitle)
        titleString += chars;
    else if (inLink)
        linkString += chars;

    return true;
}

/*
    Report a fatal parsing error, and return false to indicate to the reader
    that parsing should stop.
*/

//! [0]
bool Handler::fatalError (const QXmlParseException & exception)
{
    qWarning() << "Fatal error on line" << exception.lineNumber()
               << ", column" << exception.columnNumber() << ":"
               << exception.message();

    return false;
}
//! [0]
