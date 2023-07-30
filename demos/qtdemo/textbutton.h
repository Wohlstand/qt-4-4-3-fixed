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

#ifndef TEXT_BUTTON_H
#define TEXT_BUTTON_H

#include <QtGui>
#include "demoitem.h"
#include "demotextitem.h"
#include "scanitem.h"

class DemoItemAnimation;
class ButtonBackground;

class TextButton : public DemoItem
{
public:
    enum ALIGNMENT {LEFT, RIGHT};
    enum BUTTONTYPE {SIDEBAR, PANEL, UP, DOWN};
    enum STATE {ON, OFF, HIGHLIGHT, DISABLED};

    TextButton(const QString &text, ALIGNMENT align = LEFT, int userCode = 0,
        QGraphicsScene *scene = 0, QGraphicsItem *parent = 0, BUTTONTYPE color = SIDEBAR);
    virtual ~TextButton();

    // overidden methods:
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget * = 0){};
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    void animationStarted(int id = 0);
    void prepare();
    void setState(STATE state);
    void setMenuString(const QString &menu);
    void setDisabled(bool disabled);

private:
    void setupButtonBg();
    void setupScanItem();
    void setupHoverText();

    DemoItemAnimation *scanAnim;
    ButtonBackground *bgOn;
    ButtonBackground *bgOff;
    ButtonBackground *bgHighlight;
    ButtonBackground *bgDisabled;

    BUTTONTYPE buttonType;
    ALIGNMENT alignment;
    QString buttonLabel;
    QString menuString;
    int userCode;
    QSize logicalSize;

    STATE state;
};

#endif // TEXT_BUTTON_H

