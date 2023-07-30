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

#ifndef Patternist_QObjectNodeModel_H
#define Patternist_QObjectNodeModel_H

#include <QSimpleXmlNodeModel>

QT_BEGIN_HEADER
QT_BEGIN_NAMESPACE

class QObject;
class PropertyToAtomicValue;

/**
 * @short Delegates QtCore's QObject into Patternist's QAbstractXmlNodeModel.
 * known as pre/post numbering.
 *
 * QObjectXmlModel sets the toggle on QXmlNodeModelIndex to @c true, if it
 * represents a property of the QObject. That is, if the QXmlNodeModelIndex is
 * an attribute.
 *
 * @author Frans Englich <fenglich@trolltech.com>
 */
class QObjectXmlModel : public QSimpleXmlNodeModel
{
public:
    QObjectXmlModel(QObject *const object,
                    const QXmlNamePool &np);

    QXmlNodeModelIndex root() const;

    /**
     * @short Returns the same as baseURI().
     */
    virtual QUrl documentUri(const QXmlNodeModelIndex &ni) const;

    virtual QXmlNodeModelIndex::NodeKind kind(const QXmlNodeModelIndex &ni) const;
    virtual QXmlNodeModelIndex::DocumentOrder compareOrder(const QXmlNodeModelIndex &ni1, const QXmlNodeModelIndex &ni2) const;
    virtual QXmlNodeModelIndex root(const QXmlNodeModelIndex &n) const;
    virtual QXmlNodeModelIndex parent(const QXmlNodeModelIndex &ni) const;
    virtual QXmlName name(const QXmlNodeModelIndex &ni) const;
    virtual QVariant typedValue(const QXmlNodeModelIndex &n) const;
    virtual QXmlNodeModelIndex nextFromSimpleAxis(SimpleAxis, const QXmlNodeModelIndex&) const;
    virtual QVector<QXmlNodeModelIndex> attributes(const QXmlNodeModelIndex&) const;

private:
    /**
     * The highest three bits are used to signify whether the node index
     * is an artificial node.
     */
    enum QObjectNodeType
    {
        IsQObject               = 0,
        /**
         * @short if QXmlNodeModelIndex::additionalData() has this flag set, then
         * the QXmlNodeModelIndex is an attribute of the QObject element, and the
         * remaining bits forms an offset to the QObject's property
         * that this QXmlNodeModelIndex corresponds to.
         */
        QObjectProperty         = 1 << 26,
        MetaObjects             = 2 << 26,
        MetaObject              = 3 << 26,

        /**
         * metaObjects/metaObject/@superClass
         */
        MetaObjectClassName     = 4 << 26,
        /**
         * metaObjects/metaObject/@superClass
         */
        MetaObjectSuperClass    = 5 << 26,
        /**
         * QObject/@className
         */
        QObjectClassName        = 6 << 26
    };

    typedef QVector<const QMetaObject *> AllMetaObjects;
    AllMetaObjects allMetaObjects() const;

    static QObjectNodeType toNodeType(const QXmlNodeModelIndex &ni);

    static bool isTypeSupported(QVariant::Type type);
    static inline QObject *asQObject(const QXmlNodeModelIndex &n);
    static inline bool isProperty(const QXmlNodeModelIndex n);
    static inline QMetaProperty toMetaProperty(const QXmlNodeModelIndex &n);
    /**
     * Returns the ancestors of @p n. Does therefore not include
     * @p n.
     */
    inline QXmlNodeModelIndex::List ancestors(const QXmlNodeModelIndex n) const;
    QXmlNodeModelIndex qObjectSibling(const int pos,
                                      const QXmlNodeModelIndex &ni) const;
    QXmlNodeModelIndex metaObjectSibling(const int pos,
                                         const QXmlNodeModelIndex &ni) const;

    const QUrl              m_baseURI;
    QObject *const          m_root;
    const AllMetaObjects    m_allMetaObjects;
};

QT_END_NAMESPACE

QT_END_HEADER

#endif
