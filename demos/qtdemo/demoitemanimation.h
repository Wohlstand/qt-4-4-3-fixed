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

#ifndef DEMO_ITEM_ANIMATION_H
#define DEMO_ITEM_ANIMATION_H

#include <QtCore>
#include <QtGui>

class DemoItem;

class DemoItemAnimation : public QGraphicsItemAnimation
{
    Q_OBJECT

public:
    enum INOROUT {ANIM_IN, ANIM_OUT, ANIM_UNSPECIFIED};

    DemoItemAnimation(DemoItem *item, INOROUT inOrOut = ANIM_UNSPECIFIED);
    virtual ~DemoItemAnimation();

    virtual void play(bool fromStart = true, bool force = false);
    virtual void playReverse();
    virtual void stop(bool reset = true);
    virtual void setRepeat(int nr = 0);

    void setDuration(int duration);
    void setDuration(float duration){ setDuration(int(duration)); };
    void setOpacityAt0(qreal opacity);
    void setOpacityAt1(qreal opacity);
    void setOpacity(qreal step);
    void setCurrentTime(int ms);
    void setStartPos(const QPointF &pos);
    bool notOwnerOfItem();

    bool running();
    bool runningOrItemLocked();
    void lockItem(bool state);
    void prepare();

    DemoItem *demoItem();

    virtual void afterAnimationStep(qreal step); // overridden

    QTimeLine *timeline;
    qreal opacityAt0;
    qreal opacityAt1;
    int startDelay;
    QPointF startPos;
    bool hideOnFinished;
    bool moveOnPlay;
    bool forcePlay;
    bool fromStart;
    INOROUT inOrOut;

private slots:
    virtual void playWithoutDelay();
};

#endif // DEMO_ITEM_ANIMATION_H



