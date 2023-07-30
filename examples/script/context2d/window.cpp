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

#include "window.h"
#include "qcontext2dcanvas.h"
#include <QHBoxLayout>
#include <QListWidget>
#include <QDir>

static QString scriptsDir()
{
    if (QFile::exists("./scripts"))
        return "./scripts";
    return ":/scripts";
}

Window::Window(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *hbox = new QHBoxLayout(this);
    QListWidget *view = new QListWidget(this);
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    canvas = new QContext2DCanvas(this);
    canvas->setFixedSize(150, 150);
    hbox->addWidget(view);
    hbox->addWidget(canvas);

    QDir dir(scriptsDir());
    QFileInfoList entries = dir.entryInfoList(QStringList() << "*.js");
    for (int i = 0; i < entries.size(); ++i)
        view->addItem(entries.at(i).fileName());
    connect(view, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
            this, SLOT(selectScript(QListWidgetItem*)));

    setWindowTitle(tr("Context 2D"));
}

void Window::selectScript(QListWidgetItem *item)
{
    QFile file(scriptsDir() + "/" + item->text());
    file.open(QIODevice::ReadOnly);
    QString contents = file.readAll();
    file.close();
    canvas->setScriptContents(contents);
}
