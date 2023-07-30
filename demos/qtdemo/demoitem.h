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

#ifndef DEMO_ITEM_H
#define DEMO_ITEM_H

#include <QtGui>

class DemoItemAnimation;
class Guide;

class SharedImage
{
public:
    SharedImage() : refCount(0), image(0), pixmap(0){};
    ~SharedImage()
    {
        delete image;
        delete pixmap;
    }

    int refCount;
    QImage *image;
    QPixmap *pixmap;
    QMatrix matrix;
    QRectF unscaledBoundingRect;
};

class DemoItem : public QGraphicsItem
{

public:
    DemoItem(QGraphicsScene *scene = 0, QGraphicsItem *parent = 0);
    virtual ~DemoItem();

    bool inTransition();
    virtual void animationStarted(int id = 0){ Q_UNUSED(id); };
    virtual void animationStopped(int id = 0){ Q_UNUSED(id); };
    virtual void prepare(){};
    void setRecursiveVisible(bool visible);
    void useSharedImage(const QString &hashKey);
    void setNeverVisible(bool never = true);
    static void setMatrix(const QMatrix &matrix);
    virtual QRectF boundingRect() const; // overridden
    void setPosUsingSheepDog(const QPointF &dest, const QRectF &sceneFence);

    qreal opacity;
    bool locked;
    DemoItemAnimation *currentAnimation;
    bool noSubPixeling;

    // Used if controlled by a guide:
    void useGuide(Guide *guide, float startFrame = 0);
    void guideAdvance(float distance);
    void guideMove(float moveSpeed);
    void setGuidedPos(const QPointF &position);
    QPointF getGuidedPos();
    float startFrame;
    float guideFrame;
    Guide *currGuide;

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option = 0, QWidget *widget = 0); // overridden
    virtual QImage *createImage(const QMatrix &) const { return 0; };
    virtual bool collidesWithItem(const QGraphicsItem *, Qt::ItemSelectionMode) const { return false; };
    bool prepared;

private:
    SharedImage *sharedImage;
    QString hashKey;
    bool neverVisible;
    bool validateImage();

    // Used if controlled by a guide:
    void switchGuide(Guide *guide);
    friend class Guide;
    QPointF guidedPos;

    // The next static hash is shared amongst all demo items, and
    // has the purpose of reusing images to save memory and time
    static QHash<QString, SharedImage *> sharedImageHash;
    static QMatrix matrix;
};

#endif // DEMO_ITEM_H

