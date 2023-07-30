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

#ifndef LCDRANGE_H
#define LCDRANGE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
//! [0]
class QLabel;
//! [0] //! [1]
class QSlider;
//! [1]
QT_END_NAMESPACE

//! [2]
class LCDRange : public QWidget
{
    Q_OBJECT

public:
    LCDRange(QWidget *parent = 0);
//! [2] //! [3]
    LCDRange(const QString &text, QWidget *parent = 0);
//! [3]

    int value() const;
//! [4]
    QString text() const;
//! [4]

public slots:
    void setValue(int value);
    void setRange(int minValue, int maxValue);
//! [5]
    void setText(const QString &text);
//! [5]

signals:
    void valueChanged(int newValue);

//! [6]
private:
    void init();
//! [6]

    QSlider *slider;
//! [7]
    QLabel *label;
//! [7]
};

#endif
