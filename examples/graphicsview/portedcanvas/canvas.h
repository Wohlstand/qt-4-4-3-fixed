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

#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <q3popupmenu.h>
#include <q3mainwindow.h>
#include <q3intdict.h>
#include <QMouseEvent>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>

class BouncyLogo : public QGraphicsPixmapItem {
    void initSpeed();
public:
    BouncyLogo();
    void advance(int);
    int type() const;

    QPainterPath shape() const;

    void initPos();
private:
    qreal xvel;
    qreal yvel;
};


class FigureEditor : public QGraphicsView {
    Q_OBJECT

public:
    FigureEditor(QGraphicsScene&, QWidget* parent=0, const char* name=0, Qt::WindowFlags f=0);
    void clear();

signals:
    void status(const QString&);
};

class Main : public Q3MainWindow {
    Q_OBJECT

public:
    Main(QGraphicsScene&, QWidget* parent=0, const char* name=0, Qt::WindowFlags f=0);
    ~Main();

public slots:
    void help();

private slots:
    void aboutQt();
    void newView();
    void clear();
    void init();

    void addSprite();
    void addCircle();
    void addHexagon();
    void addPolygon();
    void addSpline();
    void addText();
    void addLine();
    void addRectangle();
    void addMesh();
    void addLogo();
    void addButterfly();

    void enlarge();
    void shrink();
    void rotateClockwise();
    void rotateCounterClockwise();
    void zoomIn();
    void zoomOut();
    void mirror();
    void moveL();
    void moveR();
    void moveU();
    void moveD();

    void print();

private:
    QGraphicsScene& canvas;
    FigureEditor *editor;

    Q3PopupMenu* options;
    QPrinter* printer;
    int dbf_id;
};

#endif
