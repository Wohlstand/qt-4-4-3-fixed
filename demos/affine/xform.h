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

#ifndef XFORM_H
#define XFORM_H

#include "arthurwidgets.h"

#include <QBasicTimer>
#include <QPolygonF>

class HoverPoints;
QT_FORWARD_DECLARE_CLASS(QLineEdit)

class XFormView : public ArthurFrame
{
    Q_OBJECT

    Q_PROPERTY(bool animation READ animation WRITE setAnimation)
    Q_PROPERTY(qreal shear READ shear WRITE setShear)
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation)
    Q_PROPERTY(qreal scale READ scale WRITE setScale)

public:
    XFormView(QWidget *parent);
    void paint(QPainter *);
    void drawVectorType(QPainter *painter);
    void drawPixmapType(QPainter *painter);
    void drawTextType(QPainter *painter);
    QSize sizeHint() const { return QSize(500, 500); }

    void mousePressEvent(QMouseEvent *e);
    void resizeEvent(QResizeEvent *e);
    HoverPoints *hoverPoints() { return pts; }

    QLineEdit *textEditor;

    bool animation() const { return timer.isActive(); }
    qreal shear() const { return m_shear; }
    qreal scale() const { return m_scale; }
    qreal rotation() const { return m_rotation; }
    void setShear(qreal s);
    void setScale(qreal s);
    void setRotation(qreal r);

public slots:
    void setAnimation(bool animate);
    void updateCtrlPoints(const QPolygonF &);
    void changeRotation(int rotation);
    void changeScale(int scale);
    void changeShear(int shear);

    void setVectorType();
    void setPixmapType();
    void setTextType();
    void reset();

signals:
    void rotationChanged(int rotation);
    void scaleChanged(int scale);
    void shearChanged(int shear);

protected:
    void timerEvent(QTimerEvent *e);
    void wheelEvent(QWheelEvent *);

private:
    enum XFormType { VectorType, PixmapType, TextType };

    QPolygonF ctrlPoints;
    HoverPoints *pts;
    qreal m_rotation;
    qreal m_scale;
    qreal m_shear;
    XFormType type;
    QPixmap pixmap;
    QBasicTimer timer;
};

class XFormWidget : public QWidget
{
    Q_OBJECT
public:
    XFormWidget(QWidget *parent);

private:
    XFormView *view;
};

#endif // XFORM_H
