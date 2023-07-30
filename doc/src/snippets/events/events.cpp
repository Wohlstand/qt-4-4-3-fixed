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

#include <QCheckBox>
#include <QMouseEvent>

class MyCheckBox : public QCheckBox
{
public:
    void mousePressEvent(QMouseEvent *event);
};

//! [0]
void MyCheckBox::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // handle left mouse button here
    } else {
        // pass on other buttons to base class
        QCheckBox::mousePressEvent(event);
    }
}
//! [0]

class MyWidget : public QWidget
{
public:
    bool event(QEvent *event);
};

static const int MyCustomEventType = 1099;

class MyCustomEvent : public QEvent
{
public:
    MyCustomEvent() : QEvent((QEvent::Type)MyCustomEventType) {}
};

//! [1]
bool MyWidget::event(QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
	QKeyEvent *ke = static_cast<QKeyEvent *>(event);
	if (ke->key() == Qt::Key_Tab) {
	    // special tab handling here
	    return true;
	}
    } else if (event->type() == MyCustomEventType) {
	MyCustomEvent *myEvent = static_cast<MyCustomEvent *>(event);
	// custom event handling here
	return true;
    }

    return QWidget::event(event);
}
//! [1]

int main()
{
}
