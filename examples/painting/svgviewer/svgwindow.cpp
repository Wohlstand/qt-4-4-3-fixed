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

#include <QtGui>

#include "svgview.h"
#include "svgwindow.h"

SvgWindow::SvgWindow()
    : QScrollArea(),
      highQualityAntialiasing(false)
{
    QWidget *view = new QWidget(this);
    renderer = SvgWindow::Native;
    setWidget(view);
}

void SvgWindow::openFile(const QString &file)
{
    currentPath = file;
    setRenderer(renderer);
}

void SvgWindow::setRenderer(RendererType type)
{
    renderer = type;
    QWidget *view;

    if (renderer == OpenGL) {
        #ifndef QT_NO_OPENGL
        view = new SvgGLView(currentPath, this);
        qobject_cast<SvgGLView *>(view)->setHighQualityAntialiasing(highQualityAntialiasing);
        #endif
    } else if (renderer == Image) {
        view = new SvgRasterView(currentPath, this);
    } else {
        view = new SvgNativeView(currentPath, this);
    }

    setWidget(view);
    view->show();
}

void SvgWindow::setHighQualityAntialiasing(bool hq)
{
    highQualityAntialiasing = hq;

    #ifndef QT_NO_OPENGL
    QWidget *view = widget();
    if (renderer == OpenGL)
        qobject_cast<SvgGLView *>(view)->setHighQualityAntialiasing(highQualityAntialiasing);
    #endif
}

void SvgWindow::mousePressEvent(QMouseEvent *event)
{
    mousePressPos = event->pos();
    scrollBarValuesOnMousePress.rx() = horizontalScrollBar()->value();
    scrollBarValuesOnMousePress.ry() = verticalScrollBar()->value();
    event->accept();
}

void SvgWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (mousePressPos.isNull()) {
        event->ignore();
        return;
    }

    horizontalScrollBar()->setValue(scrollBarValuesOnMousePress.x() - event->pos().x() + mousePressPos.x());
    verticalScrollBar()->setValue(scrollBarValuesOnMousePress.y() - event->pos().y() + mousePressPos.y());
    horizontalScrollBar()->update();
    verticalScrollBar()->update();
    event->accept();
}

void SvgWindow::mouseReleaseEvent(QMouseEvent *event)
{
    mousePressPos = QPoint();
    event->accept();
}
