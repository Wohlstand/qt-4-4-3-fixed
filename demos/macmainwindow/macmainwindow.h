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
#ifndef MACMAINWINDOW_H
#define MACMAINWINDOW_H

#include <QtGui>

#ifdef Q_WS_MAC

#include <Carbon/Carbon.h>

// The SearchWidget class wraps a native HISearchField.
class SearchWidget : public QWidget
{
    Q_OBJECT
private:
    HIViewRef searchField;
    CFStringRef searchFieldText;

public:
    QSize sizeHint() const;
    SearchWidget(QWidget *parent = 0);
    ~SearchWidget();
};

class SearchWrapper : public QWidget
{
Q_OBJECT
public:
    SearchWrapper(QWidget *parent = 0);
    QSize sizeHint() const;
    SearchWidget *s;
};

class Spacer : public QWidget
{
Q_OBJECT
public:
    Spacer(QWidget *parent = 0);
    QSize sizeHint() const;
};

class MacSplitterHandle : public QSplitterHandle
{
Q_OBJECT
public:
    MacSplitterHandle(Qt::Orientation orientation, QSplitter *parent);
    void paintEvent(QPaintEvent *);
    QSize sizeHint() const;
};

class MacSplitter : public QSplitter
{
public:
    QSplitterHandle *createHandle();
};

class MacMainWindow : public QMainWindow
{
Q_OBJECT
public:
    MacMainWindow();
    ~MacMainWindow();
    QAbstractItemModel *createItemModel();
    void resizeEvent(QResizeEvent *e);
    QAbstractItemModel *createDocumentModel();
public:
    QSplitter *splitter;
    QSplitter *horizontalSplitter;
    QTreeView *sidebar;
    QListView *documents;
    QTextEdit *textedit;
    QVBoxLayout *layout;
    SearchWidget *searchWidget;
    QToolBar * toolBar;
};

#endif // Q_WS_MAC

#endif //MACMAINWINDOW_H
