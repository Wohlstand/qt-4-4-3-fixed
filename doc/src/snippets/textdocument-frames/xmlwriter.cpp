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

#include "xmlwriter.h"

QDomDocument *XmlWriter::toXml()
{
    QDomImplementation implementation;
    QDomDocumentType docType = implementation.createDocumentType(
        "scribe-document", "scribe", "www.trolltech.com/scribe");

    document = new QDomDocument(docType);

    // ### This processing instruction is required to ensure that any kind
    // of encoding is given when the document is written.
    QDomProcessingInstruction process = document->createProcessingInstruction(
        "xml", "version=\"1.0\" encoding=\"utf-8\"");
    document->appendChild(process);

    QDomElement documentElement = document->createElement("document");
    document->appendChild(documentElement);

//! [0]
    QTextFrame *root = textDocument->rootFrame();
//! [0]
    
    if (root)
        processFrame(documentElement, root);

    return document;
}

void XmlWriter::processBlock(QDomElement &parent, const QTextBlock &block)
{
    QDomElement blockElement = document->createElement("block");
    blockElement.setAttribute("position", block.position());
    blockElement.setAttribute("length", block.length());
    parent.appendChild(blockElement);

    QTextBlock::iterator it;
    for (it = block.begin(); !(it.atEnd()); ++it) {
        QTextFragment fragment = it.fragment();

        if (fragment.isValid()) {
            QDomElement fragmentElement = document->createElement("fragment");
            blockElement.appendChild(fragmentElement);

            fragmentElement.setAttribute("length", fragment.length());
            QDomText fragmentText = document->createTextNode(fragment.text());

            fragmentElement.appendChild(fragmentText);
        }
    }
}

void XmlWriter::processFrame(QDomElement &parent, QTextFrame *frame)
{
    QDomElement frameElement = document->createElement("frame");
    frameElement.setAttribute("begin", frame->firstPosition());
    frameElement.setAttribute("end", frame->lastPosition());
    parent.appendChild(frameElement);

//! [1]
    QTextFrame::iterator it;
    for (it = frame->begin(); !(it.atEnd()); ++it) {

        QTextFrame *childFrame = it.currentFrame();
        QTextBlock childBlock = it.currentBlock();

        if (childFrame)
//! [1] //! [2]
            processFrame(frameElement, childFrame);
        else if (childBlock.isValid())
            processBlock(frameElement, childBlock);
    }
//! [2]
}
