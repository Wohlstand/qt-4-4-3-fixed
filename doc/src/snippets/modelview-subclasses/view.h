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

#ifndef VIEW_H
#define VIEW_H

#include <QAbstractItemView>
#include <QItemSelection>
#include <QItemSelectionModel>
#include <QModelIndex>
#include <QRect>
#include <QSize>
#include <QWidget>

class LinearView : public QAbstractItemView
{
    Q_OBJECT
public:
    LinearView(QWidget *parent = 0);

    QRect itemViewportRect(const QModelIndex &index) const;
    void ensureVisible(const QModelIndex &index);
    QModelIndex itemAt(int x, int y) const;

protected slots:
    /*void dataChanged(const QModelIndex &topLeft, const QModelIndex
    &bottomRight);*/
    void rowsInserted(const QModelIndex &parent, int start, int end);
    void rowsRemoved(const QModelIndex &parent, int start, int end);
    /*void selectionChanged(const QItemSelection &deselected, const QItemSelection &selected);
    void verticalScrollbarAction(int action);
    void horizontalScrollbarAction(int action);*/

protected:
    void setSelection(const QRect&, QItemSelectionModel::SelectionFlags command);
    QRect selectionViewportRect(const QItemSelection &selection) const;
    QRect itemRect(const QModelIndex &item) const;
    bool isIndexHidden(const QModelIndex &index) const;
    int horizontalOffset() const;
    int verticalOffset() const;
    QModelIndex moveCursor(QAbstractItemView::CursorAction cursorAction,
                           Qt::KeyboardModifiers modifiers);

    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    QSize sizeHint() const;

private:
    int rows(const QModelIndex &index = QModelIndex()) const;
    void updateGeometries();
};

#endif
