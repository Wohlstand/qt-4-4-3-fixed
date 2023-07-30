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
#include <QApplication>

class MyPushButton : public QPushButton
{
public:
    MyPushButton(QWidget *parent = 0);

    void paintEvent(QPaintEvent *);
};

MyPushButton::MyPushButton(QWidget *parent)
    : QPushButton(parent)
{
}

//! [0]
void MyPushButton::paintEvent(QPaintEvent *)
{
    QStyleOptionButton option;
    option.initFrom(this);
    option.state = isDown() ? QStyle::State_Sunken : QStyle::State_Raised;
    if (isDefault())
        option.features |= QStyleOptionButton::DefaultButton;
    option.text = text();
    option.icon = icon();

    QPainter painter(this);
    style()->drawControl(QStyle::CE_PushButton, &option, &painter, this);
}
//! [0]



class MyStyle : public QStyle
{
public:
    MyStyle();

    void drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                       QPainter *painter, const QWidget *widget);
};

MyStyle::MyStyle()
{
//! [1]
    QStyleOptionFrame *option;

    if (const QStyleOptionFrame *frameOption =
           qstyleoption_cast<const QStyleOptionFrame *>(option)) {
        QStyleOptionFrameV2 frameOptionV2(*frameOption);

        // draw the frame using frameOptionV2
    }
//! [1]

//! [2]
    if (const QStyleOptionProgressBar *progressBarOption =
           qstyleoption_cast<const QStyleOptionProgressBar *>(option)) {
        QStyleOptionProgressBarV2 progressBarV2(*progressBarOption);

        // draw the progress bar using progressBarV2
    }
//! [2]

//! [3]
    if (const QStyleOptionTab *tabOption =
           qstyleoption_cast<const QStyleOptionTab *>(option)) {
        QStyleOptionTabV2 tabV2(*tabOption);

        // draw the tab using tabV2
   }
//! [3]
}

//! [4]
void MyStyle::drawPrimitive(PrimitiveElement element,
                            const QStyleOption *option,
                            QPainter *painter,
                            const QWidget *widget)
{
    if (element == PE_FrameFocusRect) {
        const QStyleOptionFocusRect *focusRectOption =
                qstyleoption_cast<const QStyleOptionFocusRect *>(option);
        if (focusRectOption) {
            // ...
        }
    }
    // ...
}
//! [4]

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MyPushButton button;
    button.show();
    return app.exec();
}
