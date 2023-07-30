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

#include "modelmenu.h"

#include <QtCore/QAbstractItemModel>
#include <qdebug.h>

ModelMenu::ModelMenu(QWidget * parent)
    : QMenu(parent)
    , m_maxRows(7)
    , m_firstSeparator(-1)
    , m_maxWidth(-1)
    , m_hoverRole(0)
    , m_separatorRole(0)
    , m_model(0)
{
    connect(this, SIGNAL(aboutToShow()), this, SLOT(aboutToShow()));
}

bool ModelMenu::prePopulated()
{
    return false;
}

void ModelMenu::postPopulated()
{
}

void ModelMenu::setModel(QAbstractItemModel *model)
{
    m_model = model;
}

QAbstractItemModel *ModelMenu::model() const
{
    return m_model;
}

void ModelMenu::setMaxRows(int max)
{
    m_maxRows = max;
}

int ModelMenu::maxRows() const
{
    return m_maxRows;
}

void ModelMenu::setFirstSeparator(int offset)
{
    m_firstSeparator = offset;
}

int ModelMenu::firstSeparator() const
{
    return m_firstSeparator;
}

void ModelMenu::setRootIndex(const QModelIndex &index)
{
    m_root = index;
}

QModelIndex ModelMenu::rootIndex() const
{
    return m_root;
}

void ModelMenu::setHoverRole(int role)
{
    m_hoverRole = role;
}

int ModelMenu::hoverRole() const
{
    return m_hoverRole;
}

void ModelMenu::setSeparatorRole(int role)
{
    m_separatorRole = role;
}

int ModelMenu::separatorRole() const
{
    return m_separatorRole;
}

Q_DECLARE_METATYPE(QModelIndex)
void ModelMenu::aboutToShow()
{
    if (QMenu *menu = qobject_cast<QMenu*>(sender())) {
        QVariant v = menu->menuAction()->data();
        if (v.canConvert<QModelIndex>()) {
            QModelIndex idx = qvariant_cast<QModelIndex>(v);
            createMenu(idx, -1, menu, menu);
            disconnect(menu, SIGNAL(aboutToShow()), this, SLOT(aboutToShow()));
            return;
        }
    }

    clear();
    if (prePopulated())
        addSeparator();
    int max = m_maxRows;
    if (max != -1)
        max += m_firstSeparator;
    createMenu(m_root, max, this, this);
    postPopulated();
}

void ModelMenu::createMenu(const QModelIndex &parent, int max, QMenu *parentMenu, QMenu *menu)
{
    if (!menu) {
        QString title = parent.data().toString();
        menu = new QMenu(title, this);
        QIcon icon = qvariant_cast<QIcon>(parent.data(Qt::DecorationRole));
        menu->setIcon(icon);
        parentMenu->addMenu(menu);
        QVariant v;
        v.setValue(parent);
        menu->menuAction()->setData(v);
        connect(menu, SIGNAL(aboutToShow()), this, SLOT(aboutToShow()));
        return;
    }

    int end = m_model->rowCount(parent);
    if (max != -1)
        end = qMin(max, end);

    connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(triggered(QAction*)));
    connect(menu, SIGNAL(hovered(QAction*)), this, SLOT(hovered(QAction*)));

    for (int i = 0; i < end; ++i) {
        QModelIndex idx = m_model->index(i, 0, parent);
        if (m_model->hasChildren(idx)) {
            createMenu(idx, -1, menu);
        } else {
            if (m_separatorRole != 0
                && idx.data(m_separatorRole).toBool())
                addSeparator();
            else
                menu->addAction(makeAction(idx));
        }
        if (menu == this && i == m_firstSeparator - 1)
            addSeparator();
    }
}

QAction *ModelMenu::makeAction(const QModelIndex &index)
{
    QIcon icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
    QAction *action = makeAction(icon, index.data().toString(), this);
    QVariant v;
    v.setValue(index);
    action->setData(v);
    return action;
}

QAction *ModelMenu::makeAction(const QIcon &icon, const QString &text, QObject *parent)
{
    QFontMetrics fm(font());
    if (-1 == m_maxWidth)
        m_maxWidth = fm.width(QLatin1Char('m')) * 30;
    QString smallText = fm.elidedText(text, Qt::ElideMiddle, m_maxWidth);
    return new QAction(icon, smallText, parent);
}

void ModelMenu::triggered(QAction *action)
{
    QVariant v = action->data();
    if (v.canConvert<QModelIndex>()) {
        QModelIndex idx = qvariant_cast<QModelIndex>(v);
        emit activated(idx);
    }
}

void ModelMenu::hovered(QAction *action)
{
    QVariant v = action->data();
    if (v.canConvert<QModelIndex>()) {
        QModelIndex idx = qvariant_cast<QModelIndex>(v);
        QString hoveredString = idx.data(m_hoverRole).toString();
        if (!hoveredString.isEmpty())
            emit hovered(hoveredString);
    }
}

