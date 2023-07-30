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

#include "embeddeddialog.h"
#include "ui_embeddeddialog.h"

#include <QtGui>

EmbeddedDialog::EmbeddedDialog(QWidget *parent)
    : QDialog(parent)
{
    ui = new Ui_embeddedDialog;
    ui->setupUi(this);
    ui->layoutDirection->setCurrentIndex(layoutDirection() != Qt::LeftToRight);

    foreach (QString styleName, QStyleFactory::keys()) {
        ui->style->addItem(styleName);
        if (style()->objectName().toLower() == styleName.toLower())
            ui->style->setCurrentIndex(ui->style->count() - 1);
    }

    connect(ui->layoutDirection, SIGNAL(activated(int)),
            this, SLOT(layoutDirectionChanged(int)));
    connect(ui->spacing, SIGNAL(valueChanged(int)),
            this, SLOT(spacingChanged(int)));
    connect(ui->fontComboBox, SIGNAL(currentFontChanged(const QFont &)),
            this, SLOT(fontChanged(const QFont &)));
    connect(ui->style, SIGNAL(activated(QString)),
            this, SLOT(styleChanged(QString)));
}

EmbeddedDialog::~EmbeddedDialog()
{
    delete ui;
}

void EmbeddedDialog::layoutDirectionChanged(int index)
{
    setLayoutDirection(index == 0 ? Qt::LeftToRight : Qt::RightToLeft);
}

void EmbeddedDialog::spacingChanged(int spacing)
{
    layout()->setSpacing(spacing);
    adjustSize();
}

void EmbeddedDialog::fontChanged(const QFont &font)
{
    setFont(font);
}

static void setStyleHelper(QWidget *widget, QStyle *style)
{
    widget->setStyle(style);
    widget->setPalette(style->standardPalette());
    foreach (QObject *child, widget->children()) {
        if (QWidget *childWidget = qobject_cast<QWidget *>(child))
            setStyleHelper(childWidget, style);
    }
}

void EmbeddedDialog::styleChanged(const QString &styleName)
{
    QStyle *style = QStyleFactory::create(styleName);
    if (style)
        setStyleHelper(this, style);
}
