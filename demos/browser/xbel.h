/****************************************************************************
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the demonstration applications of the Qt Toolkit.
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

#ifndef XBEL_H
#define XBEL_H

#include <QtCore/QXmlStreamReader>
#include <QtCore/QDateTime>

class BookmarkNode
{
public:
    enum Type {
        Root,
        Folder,
        Bookmark,
        Separator
    };

    BookmarkNode(Type type = Root, BookmarkNode *parent = 0);
    ~BookmarkNode();
    bool operator==(const BookmarkNode &other);

    Type type() const;
    void setType(Type type);
    QList<BookmarkNode *> children() const;
    BookmarkNode *parent() const;

    void add(BookmarkNode *child, int offset = -1);
    void remove(BookmarkNode *child);

    QString url;
    QString title;
    QString desc;
    bool expanded;

private:
    BookmarkNode *m_parent;
    Type m_type;
    QList<BookmarkNode *> m_children;

};

class XbelReader : public QXmlStreamReader
{
public:
    XbelReader();
    BookmarkNode *read(const QString &fileName);
    BookmarkNode *read(QIODevice *device);

private:
    void skipUnknownElement();
    void readXBEL(BookmarkNode *parent);
    void readTitle(BookmarkNode *parent);
    void readDescription(BookmarkNode *parent);
    void readSeparator(BookmarkNode *parent);
    void readFolder(BookmarkNode *parent);
    void readBookmarkNode(BookmarkNode *parent);
};

#include <QtCore/QXmlStreamWriter>

class XbelWriter : public QXmlStreamWriter
{
public:
    XbelWriter();
    bool write(const QString &fileName, const BookmarkNode *root);
    bool write(QIODevice *device, const BookmarkNode *root);

private:
    void writeItem(const BookmarkNode *parent);
};

#endif // XBEL_H

