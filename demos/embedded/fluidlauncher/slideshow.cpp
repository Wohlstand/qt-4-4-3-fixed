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

#include <QBasicTimer>
#include <QList>
#include <QImage>
#include <QDir>
#include <QPainter>
#include <QPaintEvent>

#include <QDebug>


#include "slideshow.h"


class SlideShowPrivate
{
public:
    SlideShowPrivate();

    int currentSlide;
    int slideInterval;
    QBasicTimer interSlideTimer;
    QStringList imagePaths;

    void showNextSlide();
};



SlideShowPrivate::SlideShowPrivate()
{
    currentSlide = 0;
    slideInterval = 10000; // Default to 10 sec interval
}


void SlideShowPrivate::showNextSlide()
{
    currentSlide++;
    if (currentSlide >= imagePaths.size())
      currentSlide = 0;
}



SlideShow::SlideShow()
{
    d = new SlideShowPrivate;

    setAttribute(Qt::WA_StaticContents, true);
    setAttribute(Qt::WA_OpaquePaintEvent, true);
    setAttribute(Qt::WA_NoSystemBackground, true);

    setMouseTracking(true);
}


SlideShow::~SlideShow()
{
    delete d;
}


void SlideShow::addImageDir(QString dirName)
{
    QDir dir(dirName);

    QStringList fileNames = dir.entryList(QDir::Files | QDir::Readable, QDir::Name);

    for (int i=0; i<fileNames.count(); i++)
        d->imagePaths << dir.absoluteFilePath(fileNames[i]);
}

void SlideShow::addImage(QString filename)
{
    d->imagePaths << filename;
}


void SlideShow::clearImages()
{
    d->imagePaths.clear();
}


void SlideShow::startShow()
{
    showFullScreen();
    d->interSlideTimer.start(d->slideInterval, this);
    d->showNextSlide();
    update();
}


void SlideShow::stopShow()
{
    hide();
    d->interSlideTimer.stop();
}


int SlideShow::slideInterval()
{
    return d->slideInterval;
}

void SlideShow::setSlideInterval(int val)
{
    d->slideInterval = val;
}


void SlideShow::timerEvent(QTimerEvent* event)
{
    Q_UNUSED(event);
    d->showNextSlide();
    update();
}


void SlideShow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);

    if (d->imagePaths.size() > 0) {
        QImage slide = QImage(d->imagePaths[d->currentSlide]);
        QSize slideSize = slide.size();
        QSize scaledSize = QSize(qMin(slideSize.width(), size().width()),
            qMin(slideSize.height(), size().height()));
        if (slideSize != scaledSize)
            slide = slide.scaled(scaledSize, Qt::KeepAspectRatio);

        QRect imageRect( qMax( (size().width() - slide.width())/2, 0),
                         qMax( (size().height() - slide.height())/2, 0),
                         slide.width(),
                         slide.height());

        if (imageRect.top() > 0) {
            // Fill in top & bottom rectangles:
            painter.fillRect(0, 0, size().width(), imageRect.top(), Qt::black);
            painter.fillRect(0, imageRect.bottom(), size().width(), size().height(), Qt::black);
        } 

        if (imageRect.left() > 0) {
            // Fill in left & right rectangles:
            painter.fillRect(0, 0, imageRect.left(), size().height(), Qt::black);
            painter.fillRect(imageRect.right(), 0, size().width(), size().height(), Qt::black);
        }

        painter.drawImage(imageRect, slide);

    } else
        painter.fillRect(event->rect(), Qt::black);
}


void SlideShow::keyPressEvent(QKeyEvent* event)
{
    Q_UNUSED(event);
    emit inputReceived();
}


void SlideShow::mouseMoveEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    emit inputReceived();
}


void SlideShow::mousePressEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    emit inputReceived();
}


void SlideShow::mouseReleaseEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    emit inputReceived();
}
 

void SlideShow::showEvent(QShowEvent * event )
{
    Q_UNUSED(event);
#ifndef QT_NO_CURSOR
    setCursor(Qt::BlankCursor);
#endif
}

