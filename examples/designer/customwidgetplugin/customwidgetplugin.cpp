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

#include "analogclock.h"
#include "customwidgetplugin.h"

#include <QtPlugin>

//! [0]
AnalogClockPlugin::AnalogClockPlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}
//! [0]

//! [1]
void AnalogClockPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (initialized)
        return;

    initialized = true;
}
//! [1]

//! [2]
bool AnalogClockPlugin::isInitialized() const
{
    return initialized;
}
//! [2]

//! [3]
QWidget *AnalogClockPlugin::createWidget(QWidget *parent)
{
    return new AnalogClock(parent);
}
//! [3]

//! [4]
QString AnalogClockPlugin::name() const
{
    return "AnalogClock";
}
//! [4]

//! [5]
QString AnalogClockPlugin::group() const
{
    return "Display Widgets [Examples]";
}
//! [5]

//! [6]
QIcon AnalogClockPlugin::icon() const
{
    return QIcon();
}
//! [6]

//! [7]
QString AnalogClockPlugin::toolTip() const
{
    return "";
}
//! [7]

//! [8]
QString AnalogClockPlugin::whatsThis() const
{
    return "";
}
//! [8]

//! [9]
bool AnalogClockPlugin::isContainer() const
{
    return false;
}
//! [9]

//! [10]
QString AnalogClockPlugin::domXml() const
{
    return "<widget class=\"AnalogClock\" name=\"analogClock\">\n"
//! [11]
           " <property name=\"geometry\">\n"
           "  <rect>\n"
           "   <x>0</x>\n"
           "   <y>0</y>\n"
           "   <width>100</width>\n"
           "   <height>100</height>\n"
           "  </rect>\n"
           " </property>\n"
//! [11]
           " <property name=\"toolTip\" >\n"
           "  <string>The current time</string>\n"
           " </property>\n"
           " <property name=\"whatsThis\" >\n"
           "  <string>The analog clock widget displays "
           "the current time.</string>\n"
           " </property>\n"
           "</widget>\n";
}
//! [10]

//! [12]
QString AnalogClockPlugin::includeFile() const
{
    return "analogclock.h";
}
//! [12]

//! [13]
Q_EXPORT_PLUGIN2(customwidgetplugin, AnalogClockPlugin)
//! [13]
