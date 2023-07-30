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

#include "circlewidget.h"

#include <stdlib.h>

//! [0]
CircleWidget::CircleWidget(QWidget *parent)
    : QWidget(parent)
{
    floatBased = false;
    antialiased = false;
    frameNo = 0;

    setBackgroundRole(QPalette::Base);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}
//! [0]

//! [1]
void CircleWidget::setFloatBased(bool floatBased)
{
    this->floatBased = floatBased;
    update();
}
//! [1]

//! [2]
void CircleWidget::setAntialiased(bool antialiased)
{
    this->antialiased = antialiased;
    update();
}
//! [2]

//! [3]
QSize CircleWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}
//! [3]

//! [4]
QSize CircleWidget::sizeHint() const
{
    return QSize(180, 180);
}
//! [4]

//! [5]
void CircleWidget::nextAnimationFrame()
{
    ++frameNo;
    update();
}
//! [5]

//! [6]
void CircleWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, antialiased);
    painter.translate(width() / 2, height() / 2);
//! [6]

//! [7]
    for (int diameter = 0; diameter < 256; diameter += 9) {
        int delta = abs((frameNo % 128) - diameter / 2);
        int alpha = 255 - (delta * delta) / 4 - diameter;
//! [7] //! [8]
        if (alpha > 0) {
            painter.setPen(QPen(QColor(0, diameter / 2, 127, alpha), 3));

            if (floatBased) {
                painter.drawEllipse(QRectF(-diameter / 2.0, -diameter / 2.0,
                                           diameter, diameter));
            } else {
                painter.drawEllipse(QRect(-diameter / 2, -diameter / 2,
                                          diameter, diameter));
            }
        }
    }
}
//! [8]
