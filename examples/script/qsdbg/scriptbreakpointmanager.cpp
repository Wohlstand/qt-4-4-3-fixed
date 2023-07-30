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

#include "scriptbreakpointmanager.h"

ScriptBreakpointManager::ScriptBreakpointManager()
{
}

ScriptBreakpointManager::~ScriptBreakpointManager()
{
}

bool ScriptBreakpointManager::hasBreakpoints() const
{
    return !breakpoints.isEmpty();
}

int ScriptBreakpointManager::setBreakpoint(const QString &fileName, int lineNumber)
{
    breakpoints.append(ScriptBreakpointInfo(fileName, lineNumber));
    return breakpoints.size() - 1;
}

int ScriptBreakpointManager::setBreakpoint(const QString &functionName, const QString &fileName)
{
    breakpoints.append(ScriptBreakpointInfo(functionName, fileName));
    return breakpoints.size() - 1;
}

int ScriptBreakpointManager::setBreakpoint(const QScriptValue &function)
{
    breakpoints.append(ScriptBreakpointInfo(function));
    return breakpoints.size() - 1;
}

void ScriptBreakpointManager::removeBreakpoint(int id)
{
    if (id >= 0 && id < breakpoints.size())
        breakpoints[id] = ScriptBreakpointInfo();
}

int ScriptBreakpointManager::findBreakpoint(const QString &fileName, int lineNumber) const
{
    for (int i = 0; i < breakpoints.size(); ++i) {
        const ScriptBreakpointInfo &brk = breakpoints.at(i);
        if (brk.type != ScriptBreakpointInfo::File)
            continue;
        if (brk.fileName == fileName && brk.lineNumber == lineNumber)
            return i;
    }
    return -1;
}

int ScriptBreakpointManager::findBreakpoint(const QString &functionName, const QString &fileName) const
{
    for (int i = 0; i < breakpoints.size(); ++i) {
        const ScriptBreakpointInfo &brk = breakpoints.at(i);
        if (brk.type != ScriptBreakpointInfo::FunctionName)
            continue;
        if (brk.functionName == functionName && brk.fileName == fileName)
            return i;
    }
    return -1;
}

int ScriptBreakpointManager::findBreakpoint(const QScriptValue &function) const
{
    for (int i = 0; i < breakpoints.size(); ++i) {
        const ScriptBreakpointInfo &brk = breakpoints.at(i);
        if (brk.type != ScriptBreakpointInfo::Function)
            continue;
        if (brk.function.strictlyEquals(function))
            return i;
    }
    return -1;
}

bool ScriptBreakpointManager::isBreakpointEnabled(int id) const
{
    return breakpoints.value(id).enabled;
}

void ScriptBreakpointManager::setBreakpointEnabled(int id, bool enabled)
{
    if (id >= 0 && id < breakpoints.size())
        breakpoints[id].enabled = enabled;
}

QString ScriptBreakpointManager::breakpointCondition(int id) const
{
    return breakpoints.value(id).condition;
}

void ScriptBreakpointManager::setBreakpointCondition(int id, const QString &expression)
{
    if (id >= 0 && id < breakpoints.size())
        breakpoints[id].condition = expression;
}

int ScriptBreakpointManager::breakpointIgnoreCount(int id) const
{
    return breakpoints.value(id).ignoreCount;
}

void ScriptBreakpointManager::setBreakpointIgnoreCount(int id, int ignoreCount)
{
    if (id >= 0 && id < breakpoints.size())
        breakpoints[id].ignoreCount = ignoreCount;
}

bool ScriptBreakpointManager::isBreakpointSingleShot(int id) const
{
    return breakpoints.value(id).singleShot;
}

void ScriptBreakpointManager::setBreakpointSingleShot(int id, bool singleShot)
{
    if (id >= 0 && id < breakpoints.size())
        breakpoints[id].singleShot = singleShot;
}
