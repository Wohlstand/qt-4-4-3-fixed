/****************************************************************************
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the QtXMLPatterns module of the Qt Toolkit.
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

#include <QtDebug>

#include "qacceliterators_p.h"

QT_BEGIN_NAMESPACE

using namespace QPatternist;

xsInteger AccelIterator::position() const
{
    return m_position;
}

QXmlNodeModelIndex AccelIterator::current() const
{
    return m_current;
}

QXmlNodeModelIndex FollowingIterator::next()
{
    /* "the following axis contains all nodes that are descendants
     *  of the root of the tree in which the context node is found,
     *  are not descendants of the context node, and occur after
     *  the context node in document order." */

    if(m_position == 0)
    {
        /* Skip the descendants. */
        m_currentPre += m_document->size(m_preNumber) + 1;
    }

    if(m_currentPre > m_document->maximumPreNumber())
        return closedExit();

    while(m_document->kind(m_currentPre) == QXmlNodeModelIndex::Attribute)
    {
        ++m_currentPre;
        if(m_currentPre > m_document->maximumPreNumber())
            return closedExit();
    }

    m_current = m_document->createIndex(m_currentPre);
    ++m_position;
    ++m_currentPre;
    return m_current;
}

QXmlNodeModelIndex PrecedingIterator::next()
{
    if(m_currentPre == -1)
        return closedExit();

    /* We skip ancestors and attributes and take into account that they can be intermixed. If one
     * skips them in two separate loops, one can end up with skipping all the attributes to then
     * be positioned at an ancestor(which will be accepted because the ancestor loop was before the
     * attributes loop).  */
    while(m_document->kind(m_currentPre) == QXmlNodeModelIndex::Attribute ||
          m_document->postNumber(m_currentPre) > m_postNumber)
    {
        --m_currentPre;
        if(m_currentPre == -1)
            return closedExit();
    }

    if(m_currentPre == -1)
    {
        m_currentPre = -1;
        return closedExit();
    }

    /* Phew, m_currentPre is now 1) not an ancestor; and
     * 2) not an attribute; and 3) preceds the context node. */

    m_current = m_document->createIndex(m_currentPre);
    ++m_position;
    --m_currentPre;

    return m_current;
}

QXmlNodeModelIndex::Iterator::Ptr PrecedingIterator::copy() const
{
    return QXmlNodeModelIndex::Iterator::Ptr(new PrecedingIterator(m_document, m_preNumber));
}

QXmlNodeModelIndex::Iterator::Ptr FollowingIterator::copy() const
{
    return QXmlNodeModelIndex::Iterator::Ptr(new FollowingIterator(m_document, m_preNumber));
}

QXmlNodeModelIndex ChildIterator::next()
{
    if(m_currentPre == -1)
        return closedExit();

    ++m_position;
    m_current = m_document->createIndex(m_currentPre);

    /* We get the count of the descendants, and increment m_currentPre. After
     * this, m_currentPre is the node after the descendants. */
    m_currentPre += m_document->size(m_currentPre);
    ++m_currentPre;

    if(m_currentPre > m_document->maximumPreNumber() || m_document->depth(m_currentPre) != m_depth)
        m_currentPre = -1;

    return m_current;
}

QXmlNodeModelIndex::Iterator::Ptr ChildIterator::copy() const
{
    return QXmlNodeModelIndex::Iterator::Ptr(new ChildIterator(m_document, m_preNumber));
}

QXmlNodeModelIndex AttributeIterator::next()
{
    if(m_currentPre == -1)
        return closedExit();
    else
    {
        m_current = m_document->createIndex(m_currentPre);
        ++m_position;

        ++m_currentPre;

        if(m_currentPre > m_document->maximumPreNumber() ||
           m_document->kind(m_currentPre) != QXmlNodeModelIndex::Attribute)
            m_currentPre = -1;

        return m_current;
    }
}

QXmlNodeModelIndex::Iterator::Ptr AttributeIterator::copy() const
{
    return QXmlNodeModelIndex::Iterator::Ptr(new AttributeIterator(m_document, m_preNumber));
}

QT_END_NAMESPACE

