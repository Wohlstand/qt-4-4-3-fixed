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

#ifndef EMBEDDED_SVG_VIEWER_H
#define EMBEDDED_SVG_VIEWER_H

#include <QWidget>
#include <QString>
#include <QSvgRenderer>
#include <QSize>
#include <QMouseEvent>
#include <QSlider>
#include <QPushButton>

class EmbeddedSvgViewer : public QWidget
{
    Q_OBJECT
public:
    EmbeddedSvgViewer(const QString& filePath);
    virtual void paintEvent(QPaintEvent *event);
    void mouseMoveEvent ( QMouseEvent * event );
    void mousePressEvent ( QMouseEvent * event );
    void resizeEvent ( QResizeEvent * event );

public slots:
    void setZoom(int); // 100 <= newZoom < 0

private:
    QSvgRenderer* m_renderer;
    QSlider* m_zoomSlider;
    QPushButton* m_quitButton;
    QSize m_imageSize;
    qreal m_zoomLevel;
    qreal m_imageScale; // How many Image coords 1 widget pixel is worth

    QRectF m_viewBox;
    QRectF m_viewBoxBounds;
    QSizeF m_viewBoxSize;
    QPointF m_viewBoxCenter;
    QPointF m_viewBoxCenterOnMousePress;
    QPoint m_mousePress;

    void updateImageScale();
    QRectF getViewBox(QPointF viewBoxCenter);
};



#endif
