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

#ifndef PATHDEFORM_H
#define PATHDEFORM_H

#include "arthurwidgets.h"

#include <QPainterPath>
#include <QBasicTimer>
#include <QDateTime>

class PathDeformRenderer : public ArthurFrame
{
    Q_OBJECT
    Q_PROPERTY(bool animated READ animated WRITE setAnimated)
    Q_PROPERTY(int radius READ radius WRITE setRadius)
    Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize)
    Q_PROPERTY(int intensity READ intensity WRITE setIntensity)
    Q_PROPERTY(QString text READ text WRITE setText)

public:
    PathDeformRenderer(QWidget *widget, bool smallScreen = false);

    void paint(QPainter *painter);

    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void timerEvent(QTimerEvent *e);

    QSize sizeHint() const { return QSize(600, 500); }

    bool animated() const { return m_animated; }
    int radius() const { return int(m_radius); }
    int fontSize() const { return m_fontSize; }
    int intensity() const { return int(m_intensity); }
    QString text() const { return m_text; }

public slots:
    void setRadius(int radius);
    void setFontSize(int fontSize) { m_fontSize = fontSize; setText(m_text); }
    void setText(const QString &text);
    void setIntensity(int intensity);

    void setAnimated(bool animated);

signals:
    void clicked();
//     void frameRate(double fps);

private:
    void generateLensPixmap();
    QPainterPath lensDeform(const QPainterPath &source, const QPointF &offset);

    QBasicTimer m_repaintTimer;
//     QBasicTimer m_fpsTimer;
//     int m_fpsCounter;
    QTime m_repaintTracker;

    QVector<QPainterPath> m_paths;
    QVector<QPointF> m_advances;
    QRectF m_pathBounds;
    QString m_text;

    QPixmap m_lens_pixmap;
    QImage m_lens_image;

    int m_fontSize;
    bool m_animated;

    qreal m_intensity;
    qreal m_radius;
    QPointF m_pos;
    QPointF m_offset;
    QPointF m_direction;
    QPointF m_mousePress;
    bool    m_mouseDrag;
    bool    m_smallScreen;
};

class PathDeformControls : public QWidget
{
    Q_OBJECT
public:
    PathDeformControls(QWidget *parent, PathDeformRenderer* renderer, bool smallScreen);
signals:
    void okPressed();
    void quitPressed();
private:
    PathDeformRenderer* m_renderer;
    void layoutForDesktop();
    void layoutForSmallScreen();
private slots:
    void emitQuitSignal();
    void emitOkSignal();
};

class PathDeformWidget : public QWidget
{
    Q_OBJECT
public:
    PathDeformWidget(QWidget *parent, bool smallScreen);
    void setStyle ( QStyle * style );

private:
    PathDeformRenderer *m_renderer;
    PathDeformControls *m_controls;

private slots:
    void showControls();
    void hideControls();
};

#endif // PATHDEFORM_H
