/****************************************************************************
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the documentation of the Qt Toolkit.
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

#include "customstyle.h"

CustomStyle::CustomStyle()
{
//! [0]
    QSpinBox *spinBox = qobject_cast<QSpinBox *>(widget);
    if (spinBox) {
//! [0] //! [1]
    }
//! [1]
}

//! [2]
void CustomStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                                QPainter *painter, const QWidget *widget) const
{
    if (element == PE_IndicatorSpinUp || element == PE_IndicatorSpinDown) {
	QPolygon points(3);
	int x = option->rect.x();
	int y = option->rect.y();
	int w = option->rect.width() / 2;
	int h = option->rect.height() / 2;
	x += (option->rect.width() - w) / 2;
	y += (option->rect.height() - h) / 2;

	if (element == PE_IndicatorSpinUp) {
	    points[0] = QPoint(x, y + h);
	    points[1] = QPoint(x + w, y + h);
	    points[2] = QPoint(x + w / 2, y);
	} else { // PE_SpinBoxDown
	    points[0] = QPoint(x, y);
	    points[1] = QPoint(x + w, y);
	    points[2] = QPoint(x + w / 2, y + h);
	}

	if (option->state & State_Enabled) {
	    painter->setPen(option->palette.mid().color());
	    painter->setBrush(option->palette.buttonText());
	} else {
	    painter->setPen(option->palette.buttonText().color());
	    painter->setBrush(option->palette.mid());
	}
	painter->drawPolygon(points);
    } else {
	QWindowsStyle::drawPrimitive(element, option, painter, widget);
//! [2] //! [3]
    }
//! [3] //! [4]
}
//! [4]
