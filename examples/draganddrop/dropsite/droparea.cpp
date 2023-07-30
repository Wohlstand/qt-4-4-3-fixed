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

#include "droparea.h"

//! [DropArea constructor]
DropArea::DropArea(QWidget *parent)
    : QLabel(parent)
{
    setMinimumSize(200, 200);
    setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
    setAlignment(Qt::AlignCenter);
    setAcceptDrops(true);
    setAutoFillBackground(true);
    clear();
}
//! [DropArea constructor]

//! [dragEnterEvent() function]
void DropArea::dragEnterEvent(QDragEnterEvent *event)
{
    setText(tr("<drop content>"));
    setBackgroundRole(QPalette::Highlight);

    event->acceptProposedAction();
    emit changed(event->mimeData());
}
//! [dragEnterEvent() function]

//! [dragMoveEvent() function]
void DropArea::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}
//! [dragMoveEvent() function]

//! [dropEvent() function part1]
void DropArea::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
//! [dropEvent() function part1]

//! [dropEvent() function part2]
    if (mimeData->hasImage()) {
        setPixmap(qvariant_cast<QPixmap>(mimeData->imageData()));
    } else if (mimeData->hasHtml()) {
        setText(mimeData->html());
        setTextFormat(Qt::RichText);
    } else if (mimeData->hasText()) {
        setText(mimeData->text());    
        setTextFormat(Qt::PlainText);
    } else if (mimeData->hasUrls()) {
        QList<QUrl> urlList = mimeData->urls();
        QString text;
        for (int i = 0; i < urlList.size() && i < 32; ++i) {
            QString url = urlList.at(i).path();
            text += url + QString("\n");
        }
        setText(text);
    } else {
        setText(tr("Cannot display data"));
    }
//! [dropEvent() function part2]

//! [dropEvent() function part3]    
    setBackgroundRole(QPalette::Dark);
    event->acceptProposedAction();
}
//! [dropEvent() function part3]

//! [dragLeaveEvent() function]
void DropArea::dragLeaveEvent(QDragLeaveEvent *event)
{
    clear();
    event->accept();
}
//! [dragLeaveEvent() function]

//! [clear() function]
void DropArea::clear()
{
    setText(tr("<drop content>"));
    setBackgroundRole(QPalette::Dark);

    emit changed();
}
//! [clear() function]
