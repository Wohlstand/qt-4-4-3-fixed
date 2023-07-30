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

#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QVector>
#include <QtXmlPatterns/QSimpleXmlNodeModel>

class FileTree : public QSimpleXmlNodeModel
{
public:
    FileTree(const QXmlNamePool &namePool);

    QXmlNodeModelIndex nodeFor(const QString &fileName) const;

    /* Reimplement pure functions in QAbstractXmlNodeModel. */
    virtual QXmlNodeModelIndex::DocumentOrder compareOrder(const QXmlNodeModelIndex&,
                                                           const QXmlNodeModelIndex&) const;
    virtual QXmlName name(const QXmlNodeModelIndex &node) const;
    virtual QUrl documentUri(const QXmlNodeModelIndex &node) const;
    virtual QXmlNodeModelIndex::NodeKind kind(const QXmlNodeModelIndex &node) const;
    virtual QXmlNodeModelIndex root(const QXmlNodeModelIndex &node) const;
    virtual QVariant typedValue(const QXmlNodeModelIndex &node) const;
    virtual QVector<QXmlNodeModelIndex> attributes(const QXmlNodeModelIndex &element) const;
protected:
    virtual QXmlNodeModelIndex nextFromSimpleAxis(SimpleAxis, const QXmlNodeModelIndex&) const;

private:
    enum Type {
        File,
        Directory,
        AttributeFileName,
        AttributeFilePath,
        AttributeSize,
        AttributeMIMEType,
        AttributeSuffix
    };

    inline QXmlNodeModelIndex nextSibling(const QXmlNodeModelIndex &nodeIndex,
                                          const QFileInfo &from,
                                          qint8 offset) const;
    inline const QFileInfo &toFileInfo(const QXmlNodeModelIndex &index) const;
    inline QXmlNodeModelIndex toNodeIndex(const QFileInfo &index,
                                          Type attributeName) const;
    /**
     * Same as above, but derives the type from the QFileInfo.
     */
    inline QXmlNodeModelIndex toNodeIndex(const QFileInfo &index) const;

    /*
     One possible improvement is to use a hash, and use the &*&value()
     trick to get a pointer, which would be stored in data() instead
     of the index.
    */
    mutable QVector<QFileInfo>  m_fileInfos;
    QVector<QXmlName>           m_names;
    const QDir::Filters         m_filterAllowAll;
    const QDir::SortFlags       m_sortFlags;
};

