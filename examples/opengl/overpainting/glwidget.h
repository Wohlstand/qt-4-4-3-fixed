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

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QBrush>
#include <QFont>
#include <QImage>
#include <QPen>
#include <QGLWidget>
#include <QTimer>

class Bubble;
QT_BEGIN_NAMESPACE
class QPaintEvent;
class QWidget;
QT_END_NAMESPACE

//! [0]
class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();
//! [0]

    QSize sizeHint() const;
    int xRotation() const { return xRot; }
    int yRotation() const { return yRot; }
    int zRotation() const { return zRot; }

public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

//! [1]
protected:
    void initializeGL();
    void paintEvent(QPaintEvent *event);
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void showEvent(QShowEvent *event);

private slots:
    void animate();

private:
    GLuint makeObject();
    void createBubbles(int number);
    void drawInstructions(QPainter *painter);
//! [1]
    void extrude(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
    void normalizeAngle(int *angle);
    void quad(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2,
              GLdouble x3, GLdouble y3, GLdouble x4, GLdouble y4);
    void setupViewport(int width, int height);

    GLuint object;
    int xRot;
    int yRot;
    int zRot;
    QPoint lastPos;
    QColor trolltechGreen;
    QColor trolltechPurple;
//! [4]
    QList<Bubble*> bubbles;
    QTimer animationTimer;
};
//! [4]

#endif
