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

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QtGui>
#include <QPixmap>

class DemoTextItem;
class ImageItem;

class MainWindow : public QGraphicsView
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void enableMask(bool enable);
    void toggleFullscreen();
    int performBenchmark();
    void switchTimerOnOff(bool on);
    void start();

    QGraphicsScene *scene;
    bool loop;

    // FPS stuff:
    QList<QTime> frameTimeList;
    QList<float> fpsHistory;
    float currentFps;
    float fpsMedian;
    DemoTextItem *fpsLabel;

protected:
    // Overidden methods:
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void resizeEvent(QResizeEvent *event);
    void drawBackground(QPainter *painter, const QRectF &rect);
    void drawItems(QPainter *painter, int numItems, QGraphicsItem ** items, const QStyleOptionGraphicsItem* options);
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);

private slots:
    void tick();

private:
    void setupWidget();
    void setupSceneItems();
    void drawBackgroundToPixmap();
    void setupScene();
    bool measureFps();
    void forceFpsMedianCalculation();
    void checkAdapt();
    void setRenderingSystem();

    QTimer updateTimer;
    QTime demoStartTime;
    QTime fpsTime;
    QPixmap background;
    ImageItem *trolltechLogo;
    ImageItem *qtLogo;
    bool doneAdapt;
    bool useTimer;
    DemoTextItem *pausedLabel;
};

#endif // MAIN_WINDOW_H

