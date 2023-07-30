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

#ifndef SCRIPTBREAKPOINTMANAGER_H
#define SCRIPTBREAKPOINTMANAGER_H

#include <QtCore/qlist.h>
#include <QtCore/qstring.h>
#include <QtScript/qscriptvalue.h>

class ScriptBreakpointInfo
{
public:
    enum Type {
        File,
        FunctionName,
        Function,
        Invalid
    };

    Type type;
    QString functionName;
    QString fileName;
    int lineNumber;
    QScriptValue function;
    bool enabled;
    QString condition;
    int ignoreCount;
    bool singleShot;

    ScriptBreakpointInfo(const QString &fileName, int lineNumber)
        : type(File), fileName(fileName), lineNumber(lineNumber),
          enabled(true), ignoreCount(0), singleShot(false)
        { }
    ScriptBreakpointInfo(const QString &functionName, const QString &fileName = QString())
        : type(FunctionName), functionName(functionName), fileName(fileName),
          enabled(true), ignoreCount(0), singleShot(false)
        { }
    ScriptBreakpointInfo(const QScriptValue &function)
        : type(Function), function(function),
          enabled(true), ignoreCount(0), singleShot(false)
        { }
    ScriptBreakpointInfo()
        : type(Invalid)
        { }
};

class ScriptBreakpointManager
{
public:
    ScriptBreakpointManager();
    ~ScriptBreakpointManager();

    bool hasBreakpoints() const;

    int setBreakpoint(const QString &fileName, int lineNumber);
    int setBreakpoint(const QString &functionName, const QString &fileName = QString());
    int setBreakpoint(const QScriptValue &function);

    void removeBreakpoint(int id);

    int findBreakpoint(const QString &fileName, int lineNumber) const;
    int findBreakpoint(const QString &functionName, const QString &fileName = QString()) const;
    int findBreakpoint(const QScriptValue &function) const;

    bool isBreakpointEnabled(int id) const;
    void setBreakpointEnabled(int id, bool enabled);

    QString breakpointCondition(int id) const;
    void setBreakpointCondition(int id, const QString &expression);

    int breakpointIgnoreCount(int id) const;
    void setBreakpointIgnoreCount(int id, int ignoreCount);

    bool isBreakpointSingleShot(int id) const;
    void setBreakpointSingleShot(int id, bool singleShot);

private:
    QList<ScriptBreakpointInfo> breakpoints;

    Q_DISABLE_COPY(ScriptBreakpointManager)
};

#endif // SCRIPTBREAKPOINTMANAGER_H
