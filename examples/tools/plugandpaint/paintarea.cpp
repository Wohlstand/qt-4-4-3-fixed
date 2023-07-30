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


#include "interfaces.h"
#include "paintarea.h"

#include <QPainter>
#include <QMouseEvent>

PaintArea::PaintArea(QWidget *parent) :
    QWidget(parent),
    theImage(500, 400, QImage::Format_RGB32),
    color(Qt::blue),
    thickness(3),
    brushInterface(0),
    lastPos(-1, -1)
{
    setAttribute(Qt::WA_StaticContents);
    setAttribute(Qt::WA_NoBackground);

    theImage.fill(qRgb(255, 255, 255));
}

bool PaintArea::openImage(const QString &fileName)
{
    QImage image;
    if (!image.load(fileName))
        return false;

    setImage(image);
    return true;
}

bool PaintArea::saveImage(const QString &fileName, const char *fileFormat)
{
    return theImage.save(fileName, fileFormat);
}

void PaintArea::setImage(const QImage &image)
{
    theImage = image.convertToFormat(QImage::Format_RGB32);
    update();
    updateGeometry();
}

void PaintArea::insertShape(const QPainterPath &path)
{
    pendingPath = path;
#ifndef QT_NO_CURSOR
    setCursor(Qt::CrossCursor);
#endif
}

void PaintArea::setBrushColor(const QColor &color)
{
    this->color = color;
}

void PaintArea::setBrushWidth(int width)
{
    thickness = width;
}

//! [0]
void PaintArea::setBrush(BrushInterface *brushInterface, const QString &brush)
{
    this->brushInterface = brushInterface;
    this->brush = brush;
}
//! [0]

QSize PaintArea::sizeHint() const
{
    return theImage.size();
}

void PaintArea::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    painter.drawImage(QPoint(0, 0), theImage);
}

void PaintArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (!pendingPath.isEmpty()) {
            QPainter painter(&theImage);
            setupPainter(painter);

            const QRectF boundingRect = pendingPath.boundingRect();
            QLinearGradient gradient(boundingRect.topRight(),
                                     boundingRect.bottomLeft());
            gradient.setColorAt(0.0, QColor(color.red(), color.green(),
                                            color.blue(), 63));
            gradient.setColorAt(1.0, QColor(color.red(), color.green(),
                                            color.blue(), 191));
            painter.setBrush(gradient);
            painter.translate(event->pos() - boundingRect.center());
            painter.drawPath(pendingPath);

            pendingPath = QPainterPath();
#ifndef QT_NO_CURSOR
            unsetCursor();
#endif
            update();
        } else {
            if (brushInterface) {
                QPainter painter(&theImage);
                setupPainter(painter);
                const QRect rect = brushInterface->mousePress(brush, painter,
                                                              event->pos());
                update(rect);
            }

            lastPos = event->pos();
        }
    }
}

//! [1]
void PaintArea::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && lastPos != QPoint(-1, -1)) {
        if (brushInterface) {
            QPainter painter(&theImage);
            setupPainter(painter);
            const QRect rect = brushInterface->mouseMove(brush, painter, lastPos,
                                                         event->pos());
            update(rect);
        }

        lastPos = event->pos();
    }
}
//! [1]

void PaintArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && lastPos != QPoint(-1, -1)) {
        if (brushInterface) {
            QPainter painter(&theImage);
            setupPainter(painter);
            QRect rect = brushInterface->mouseRelease(brush, painter,
                                                      event->pos());
            update(rect);
        }

        lastPos = QPoint(-1, -1);
    }
}

void PaintArea::setupPainter(QPainter &painter)
{
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(color, thickness, Qt::SolidLine, Qt::RoundCap,
                   Qt::RoundJoin));
}
