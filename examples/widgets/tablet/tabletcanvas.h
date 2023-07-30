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

#ifndef TABLETCANVAS_H
#define TABLETCANVAS_H

#include <QWidget>
#include <QImage>
#include <QPoint>
#include <QTabletEvent>
#include <QColor>
#include <QBrush>
#include <QPen>
#include <QPoint>

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QString;
QT_END_NAMESPACE

//! [0]
class TabletCanvas : public QWidget
{
    Q_OBJECT

public:
    enum AlphaChannelType { AlphaPressure, AlphaTilt, NoAlpha };
    enum ColorSaturationType { SaturationVTilt, SaturationHTilt,
                               SaturationPressure, NoSaturation };
    enum LineWidthType { LineWidthPressure, LineWidthTilt, NoLineWidth };

    TabletCanvas();

    bool saveImage(const QString &file);
    bool loadImage(const QString &file);
    void setAlphaChannelType(AlphaChannelType type)
        { alphaChannelType = type; }
    void setColorSaturationType(ColorSaturationType type)
        { colorSaturationType = type; }
    void setLineWidthType(LineWidthType type)
        { lineWidthType = type; }
    void setColor(const QColor &color)
        { myColor = color; }
    QColor color() const
        { return myColor; }
    void setTabletDevice(QTabletEvent::TabletDevice device)
        { myTabletDevice = device; }
    int maximum(int a, int b)
        { return a > b ? a : b; }

protected:
    void tabletEvent(QTabletEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    void paintImage(QPainter &painter, QTabletEvent *event);
    Qt::BrushStyle brushPattern(qreal value);
    void updateBrush(QTabletEvent *event);

    AlphaChannelType alphaChannelType;
    ColorSaturationType colorSaturationType;
    LineWidthType lineWidthType;
    QTabletEvent::PointerType pointerType;
    QTabletEvent::TabletDevice myTabletDevice;
    QColor myColor;

    QImage image;
    QBrush myBrush;
    QPen myPen;
    bool deviceDown;
    QPoint polyLine[3];
};
//! [0]

#endif
