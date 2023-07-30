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

#ifndef STYLEWIDGET_H
#define STYLEWIDGET_H

#include <QWidget>
#include <QPainterPath>
#include "renderarea.h"

class QLabel;

class StyleWidget : public QWidget
{
    Q_OBJECT

public:
    StyleWidget(QWidget *parent = 0);

private:
    RenderArea *solid;
    RenderArea *dense1;
    RenderArea *dense2;
    RenderArea *dense3;
    RenderArea *dense4;
    RenderArea *dense5;
    RenderArea *dense6;
    RenderArea *dense7;
    RenderArea *no;
    RenderArea *hor;
    RenderArea *ver;
    RenderArea *cross;
    RenderArea *bdiag;
    RenderArea *fdiag;
    RenderArea *diagCross;
    RenderArea *linear;
    RenderArea *radial;
    RenderArea *conical;
    RenderArea *texture;

    QLabel *solidLabel;
    QLabel *dense1Label;
    QLabel *dense2Label;
    QLabel *dense3Label;
    QLabel *dense4Label;
    QLabel *dense5Label;
    QLabel *dense6Label;
    QLabel *dense7Label;
    QLabel *noLabel;
    QLabel *horLabel;
    QLabel *verLabel;
    QLabel *crossLabel;
    QLabel *bdiagLabel;
    QLabel *fdiagLabel;
    QLabel *diagCrossLabel;
    QLabel *linearLabel;
    QLabel *radialLabel;
    QLabel *conicalLabel;
    QLabel *textureLabel;
};
#endif
