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

#ifndef ITEM_CIRCLE_ANIMATION_H
#define ITEM_CIRCLE_ANIMATION_H

#include <QtCore>
#include <QObject>
#include <QtGui>
#include <QTimeLine>
#include <QList>
#include "demoitem.h"
#include "letteritem.h"
#include "guideline.h"
#include "guidecircle.h"

typedef QList<LetterItem *> LetterList;
class TickerEffect;

class ItemCircleAnimation : public QObject, public DemoItem
{
public:
    ItemCircleAnimation(QGraphicsScene *scene = 0, QGraphicsItem *parent = 0);
    virtual ~ItemCircleAnimation();

    // overidden methods:
    QRectF boundingRect() const;
    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget * = 0);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void wheelEvent(QGraphicsSceneWheelEvent *event);
    void animationStarted(int id = 0);
    void animationStopped(int id = 0);
    void prepare();
    void tick();
    void switchToNextEffect();
    void useGuideQt();
    void useGuideTt();
    void pause(bool on);

    bool tickOnPaint;
    bool paused;
    bool doIntroTransitions;

private:
    void setupLetters();
    void createLetter(char c);
    void setupGuides();
    void useGuide(Guide *guide, int firstLetter, int lastLetter);
    void swapModel();
    void setTickerScale(float s);

    int showCount;
    float scale;
    QPointF mouseMoveLastPosition;
    int letterCount;
    LetterList *letterList;
    Guide *qtGuide1;
    Guide *qtGuide2;
    Guide *qtGuide3;
    Guide *currGuide;
    TickerEffect *effect;
    QTime tickTimer;
};

#endif // ITEM_CIRCLE_ANIMATION_H



