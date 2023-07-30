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

class MyScrollArea : public QAbstractScrollArea
{
public:
    MyScrollArea(QWidget *w);
    void setWidget(QWidget *w);

protected:
    void scrollContentsBy(int dx, int dy);
    void resizeEvent(QResizeEvent *event);

private:
    void updateWidgetPosition();
    void updateArea();

    QWidget *widget;
};

MyScrollArea::MyScrollArea(QWidget *widget)
    : QAbstractScrollArea()
{
    setWidget(widget);
}

void MyScrollArea::setWidget(QWidget *w)
{
    widget = w;
    widget->setParent(viewport());
    if (!widget->testAttribute(Qt::WA_Resized))
        widget->resize(widget->sizeHint());

    verticalScrollBar()->setValue(0);
    verticalScrollBar()->setValue(0);

    updateArea();
}

void MyScrollArea::updateWidgetPosition()
{
//! [0]
    int hvalue = horizontalScrollBar()->value();
    int vvalue = verticalScrollBar()->value();
    QPoint topLeft = viewport()->rect().topLeft();

    widget->move(topLeft.x() - hvalue, topLeft.y() - vvalue);
//! [0]
}

void MyScrollArea::scrollContentsBy(int /*dx*/, int /*dy*/)
{
    updateWidgetPosition();
}

void MyScrollArea::updateArea()
{
//! [1]
    QSize areaSize = viewport()->size();
    QSize  widgetSize = widget->size();

    verticalScrollBar()->setPageStep(widgetSize.height());
    horizontalScrollBar()->setPageStep(widgetSize.width());
    verticalScrollBar()->setRange(0, widgetSize.height() - areaSize.height());
    horizontalScrollBar()->setRange(0, widgetSize.width() - areaSize.width());
    updateWidgetPosition();
//! [1]
}

void MyScrollArea::resizeEvent(QResizeEvent *event)
{
    updateArea();
}

int main(int argv, char **args)
{
    QApplication app(argv, args);

    QPixmap pixmap("mypixmap.png");
    QLabel label;
    label.setPixmap(pixmap);
    MyScrollArea area(&label);
    area.resize(300, 300);
    area.show();
    
    area.setWidget(&label);

    return app.exec();
}
