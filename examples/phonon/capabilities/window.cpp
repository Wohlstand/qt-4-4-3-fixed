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

#include "window.h"

//! [constructor]
Window::Window()
{
    setupUi();
    updateWidgets();

    connect(Phonon::BackendCapabilities::notifier(),
            SIGNAL(capabilitiesChanged()), this, SLOT(updateWidgets()));
    connect(Phonon::BackendCapabilities::notifier(),
            SIGNAL(availableAudioOutputDevicesChanged()), SLOT(updateWidgets()));
}
//! [constructor]

//! [outputDevices]
void Window::updateWidgets()
{
    devicesListView->setModel(new QStandardItemModel());
    Phonon::ObjectDescriptionModel<Phonon::AudioOutputDeviceType> *model =
            new Phonon::ObjectDescriptionModel<Phonon::AudioOutputDeviceType>();
    model->setModelData(Phonon::BackendCapabilities::availableAudioOutputDevices());
    devicesListView->setModel(model);
//! [outputDevices]

//! [mimeTypes]
    mimeListWidget->clear();
    QStringList mimeTypes =
            Phonon::BackendCapabilities::availableMimeTypes();
    foreach (QString mimeType, mimeTypes) {
        QListWidgetItem *item = new QListWidgetItem(mimeListWidget);
        item->setText(mimeType);
    }
//! [mimeTypes]

//! [effects]
    effectsTreeWidget->clear();
    QList<Phonon::EffectDescription> effects =
        Phonon::BackendCapabilities::availableAudioEffects();
    foreach (Phonon::EffectDescription effect, effects) {
        QTreeWidgetItem *item = new QTreeWidgetItem(effectsTreeWidget);
        item->setText(0, tr("Effect"));
        item->setText(1, effect.name());
        item->setText(2, effect.description());
//! [effects]

//! [effectsParameters]
        Phonon::Effect *instance = new Phonon::Effect(effect, this);
        QList<Phonon::EffectParameter> parameters = instance->parameters();
        for (int i = 0; i < parameters.size(); ++i) {
                Phonon::EffectParameter parameter = parameters.at(i);

            QVariant defaultValue = parameter.defaultValue();
            QVariant minimumValue = parameter.minimumValue();
            QVariant maximumValue = parameter.maximumValue();

            QString valueString = QString("%1 / %2 / %3")
                    .arg(defaultValue.toString()).arg(minimumValue.toString())
                    .arg(maximumValue.toString()); 

            QTreeWidgetItem *parameterItem = new QTreeWidgetItem(item);
            parameterItem->setText(0, tr("Parameter"));
            parameterItem->setText(1, parameter.name());
            parameterItem->setText(2, parameter.description());
            parameterItem->setText(3, QVariant::typeToName(parameter.type()));
            parameterItem->setText(4, valueString);
        }
    }
//! [effectsParameters]
    for (int i = 0; i < effectsTreeWidget->columnCount(); ++i) {
        if (i == 0)
            effectsTreeWidget->setColumnWidth(0, 150);
        else if (i == 2)
            effectsTreeWidget->setColumnWidth(2, 350);
        else
            effectsTreeWidget->resizeColumnToContents(i);
    }
}

void Window::setupUi()
{
    setupBackendBox();

    QLayout *layout = new QVBoxLayout;
    layout->addWidget(backendBox);

    setLayout(layout);
    setWindowTitle(tr("Backend Capabilities Example"));
}

void Window::setupBackendBox()
{
    backendBox = new QGroupBox(tr("Backend Capabilities"));

    devicesLabel = new QLabel(tr("Available Audio Devices:"));
    devicesListView = new QListView;

    mimeTypesLabel = new QLabel(tr("Supported MIME Types:"));
    mimeListWidget = new QListWidget;

    effectsLabel = new QLabel(tr("Available Audio Effects:"));

    QStringList headerLabels;
    headerLabels << tr("Type") << tr("Name") << tr("Description") <<
                    tr("Value Type") << tr("Default/Min/Max Values");

    effectsTreeWidget = new QTreeWidget;
    effectsTreeWidget->setHeaderLabels(headerLabels);
    effectsTreeWidget->setColumnCount(5);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(devicesLabel, 0, 0);
    layout->addWidget(devicesListView, 1, 0);
    layout->addWidget(mimeTypesLabel, 0, 1);
    layout->addWidget(mimeListWidget, 1, 1);
    layout->addWidget(effectsLabel, 2, 0);
    layout->addWidget(effectsTreeWidget, 3, 0, 2, 2);
    layout->setRowStretch(3, 100);

    backendBox = new QGroupBox(tr("Backend Capabilities"));
    backendBox->setLayout(layout);
}

