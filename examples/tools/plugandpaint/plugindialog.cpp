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


#include "interfaces.h"
#include "plugindialog.h"

#include <QPluginLoader>
#include <QStringList>
#include <QDir>

#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QHeaderView>

PluginDialog::PluginDialog(const QString &path, const QStringList &fileNames,
                           QWidget *parent) :
    QDialog(parent),
    label(new QLabel),
    treeWidget(new QTreeWidget),
    okButton(new QPushButton(tr("OK")))    
{
    treeWidget->setAlternatingRowColors(false);
    treeWidget->setSelectionMode(QAbstractItemView::NoSelection);
    treeWidget->setColumnCount(1);
    treeWidget->header()->hide();

    okButton->setDefault(true);

    connect(okButton, SIGNAL(clicked()), this, SLOT(close()));

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(2, 1);
    mainLayout->addWidget(label, 0, 0, 1, 3);
    mainLayout->addWidget(treeWidget, 1, 0, 1, 3);
    mainLayout->addWidget(okButton, 2, 1);
    setLayout(mainLayout);

    interfaceIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirOpenIcon),
                            QIcon::Normal, QIcon::On);
    interfaceIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirClosedIcon),
                            QIcon::Normal, QIcon::Off);
    featureIcon.addPixmap(style()->standardPixmap(QStyle::SP_FileIcon));

    setWindowTitle(tr("Plugin Information"));
    findPlugins(path, fileNames);
}

//! [0]
void PluginDialog::findPlugins(const QString &path,
                               const QStringList &fileNames)
{
    label->setText(tr("Plug & Paint found the following plugins\n"
                      "(looked in %1):")
                   .arg(QDir::toNativeSeparators(path)));

    const QDir dir(path);

    foreach (QObject *plugin, QPluginLoader::staticInstances())
        populateTreeWidget(plugin, tr("%1 (Static Plugin)")
                                   .arg(plugin->metaObject()->className()));

    foreach (QString fileName, fileNames) {
        QPluginLoader loader(dir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();
        if (plugin)
            populateTreeWidget(plugin, fileName);
    }
}
//! [0]

//! [1]
void PluginDialog::populateTreeWidget(QObject *plugin, const QString &text)
{
    QTreeWidgetItem *pluginItem = new QTreeWidgetItem(treeWidget);
    pluginItem->setText(0, text);
    treeWidget->setItemExpanded(pluginItem, true);

    QFont boldFont = pluginItem->font(0);
    boldFont.setBold(true);
    pluginItem->setFont(0, boldFont);

    if (plugin) {
        BrushInterface *iBrush = qobject_cast<BrushInterface *>(plugin);
        if (iBrush)
            addItems(pluginItem, "BrushInterface", iBrush->brushes());

        ShapeInterface *iShape = qobject_cast<ShapeInterface *>(plugin);
        if (iShape)
            addItems(pluginItem, "ShapeInterface", iShape->shapes());

        FilterInterface *iFilter =
                qobject_cast<FilterInterface *>(plugin);
        if (iFilter)
            addItems(pluginItem, "FilterInterface", iFilter->filters());
    }
}
//! [1]

void PluginDialog::addItems(QTreeWidgetItem *pluginItem,
                            const char *interfaceName,
                            const QStringList &features)
{
    QTreeWidgetItem *interfaceItem = new QTreeWidgetItem(pluginItem);
    interfaceItem->setText(0, interfaceName);
    interfaceItem->setIcon(0, interfaceIcon);

    foreach (QString feature, features) {
        if (feature.endsWith("..."))
            feature.chop(3);
        QTreeWidgetItem *featureItem = new QTreeWidgetItem(interfaceItem);
        featureItem->setText(0, feature);
        featureItem->setIcon(0, featureIcon);
    }
}
