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

#include "stylesheeteditor.h"

StyleSheetEditor::StyleSheetEditor(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    QRegExp regExp("Q(.*)Style");
    QString defaultStyle = QApplication::style()->metaObject()->className();

    if (defaultStyle == QLatin1String("QMacStyle"))
        defaultStyle = QLatin1String("Macintosh (Aqua)");
    else if (regExp.exactMatch(defaultStyle))
        defaultStyle = regExp.cap(1);

    ui.styleCombo->addItems(QStyleFactory::keys());
    ui.styleCombo->setCurrentIndex(ui.styleCombo->findText(defaultStyle));
    ui.styleSheetCombo->setCurrentIndex(ui.styleSheetCombo->findText("Coffee"));
    loadStyleSheet("Coffee");
}

void StyleSheetEditor::on_styleCombo_activated(const QString &styleName)
{
    qApp->setStyle(styleName);
    ui.applyButton->setEnabled(false);
}

void StyleSheetEditor::on_styleSheetCombo_activated(const QString &sheetName)
{
    loadStyleSheet(sheetName);
}

void StyleSheetEditor::on_styleTextEdit_textChanged()
{
    ui.applyButton->setEnabled(true);
}

void StyleSheetEditor::on_applyButton_clicked()
{
    qApp->setStyleSheet(ui.styleTextEdit->toPlainText());
    ui.applyButton->setEnabled(false);
}

void StyleSheetEditor::loadStyleSheet(const QString &sheetName)
{
    QFile file(":/qss/" + sheetName.toLower() + ".qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());

    ui.styleTextEdit->setPlainText(styleSheet);
    qApp->setStyleSheet(styleSheet);
    ui.applyButton->setEnabled(false);
}
