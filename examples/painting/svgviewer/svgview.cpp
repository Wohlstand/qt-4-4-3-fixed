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
#include "svgview.h"

#include <QSvgRenderer>

#include <QApplication>
#include <QPainter>
#include <QImage>
#include <QWheelEvent>
#include <QtDebug>

SvgRasterView::SvgRasterView(const QString &file, QWidget *parent)
    : QWidget(parent)
{
    doc = new QSvgRenderer(file, this);
    connect(doc, SIGNAL(repaintNeeded()),
            this, SLOT(poluteImage()));
}

void SvgRasterView::paintEvent(QPaintEvent *)
{
    if (buffer.size() != size() ||
        m_dirty) {
        buffer = QImage(size(), QImage::Format_ARGB32_Premultiplied);
        buffer.fill(0x0);
        QPainter p(&buffer);
        p.setViewport(0, 0, width(), height());
        doc->render(&p);
    }
    QPainter pt(this);
    pt.drawImage(0, 0, buffer);
}

QSize SvgRasterView::sizeHint() const
{
    if (doc)
        return doc->defaultSize();
    return QWidget::sizeHint();
}


void SvgRasterView::poluteImage()
{
    m_dirty = true;
    update();
}

void SvgRasterView::wheelEvent(QWheelEvent *e)
{
    const double diff = 0.1;
    QSize size = doc->defaultSize();
    int width  = size.width();
    int height = size.height();
    if (e->delta() > 0) {
        width = int(this->width()+this->width()*diff);
        height = int(this->height()+this->height()*diff);
    } else {
        width  = int(this->width()-this->width()*diff);
        height = int(this->height()-this->height()*diff);
    }

    resize(width, height);
}

SvgNativeView::SvgNativeView(const QString &file, QWidget *parent)
    : QWidget(parent)
{
    doc = new QSvgRenderer(file, this);
    connect(doc, SIGNAL(repaintNeeded()),
            this, SLOT(update()));
}

void SvgNativeView::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setViewport(0, 0, width(), height());
    doc->render(&p);
}

QSize SvgNativeView::sizeHint() const
{
    if (doc)
        return doc->defaultSize();
    return QWidget::sizeHint();
}

void SvgNativeView::wheelEvent(QWheelEvent *e)
{
    const double diff = 0.1;
    QSize size = doc->defaultSize();
    int width  = size.width();
    int height = size.height();
    if (e->delta() > 0) {
        width = int(this->width()+this->width()*diff);
        height = int(this->height()+this->height()*diff);
    } else {
        width  = int(this->width()-this->width()*diff);
        height = int(this->height()-this->height()*diff);
    }
    resize(width, height);
}

#ifndef QT_NO_OPENGL
SvgGLView::SvgGLView(const QString &file, QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
      highQualityAntialiasing(false)
{
    doc = new QSvgRenderer(file, this);
    connect(doc, SIGNAL(repaintNeeded()),
            this, SLOT(update()));
}

void SvgGLView::setHighQualityAntialiasing(bool hq)
{
    highQualityAntialiasing = hq;
    update();
}

void SvgGLView::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::HighQualityAntialiasing, highQualityAntialiasing);
    doc->render(&p);
}

QSize SvgGLView::sizeHint() const
{
    if (doc)
        return doc->defaultSize();
    return QGLWidget::sizeHint();
}

void SvgGLView::wheelEvent(QWheelEvent *e)
{
    const double diff = 0.1;
    QSize size = doc->defaultSize();
    int width  = size.width();
    int height = size.height();
    if (e->delta() > 0) {
        width = int(this->width()+this->width()*diff);
        height = int(this->height()+this->height()*diff);
    } else {
        width  = int(this->width()-this->width()*diff);
        height = int(this->height()-this->height()*diff);
    }
    resize(width, height);
}
#endif
