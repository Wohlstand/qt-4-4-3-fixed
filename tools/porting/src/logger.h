/****************************************************************************
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the qt3to4 porting application of the Qt Toolkit.
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

#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QMap>

QT_BEGIN_NAMESPACE
/*
    Base class for logger entries;
    description() should return a text for this entry,
*/
class LogEntry
{
public:
    LogEntry(QString type, QString location);
    virtual ~LogEntry(){};
    virtual QString description() const =0;
    virtual void updateLinePos(int threshold, int delta) {Q_UNUSED(threshold); Q_UNUSED(delta)};
protected:
    QString type; // Error, Warning, Info, etc
    QString location;// preprocessor, c++parser, porting, etc
};

class PlainLogEntry: public LogEntry
{
public:
     PlainLogEntry(QString type, QString lcation, QString text);
     QString description() const {return text;};
protected:
     QString text;
};

/*
    A log entry that stores a source point: file, line and column.
*/
class SourcePointLogEntry : public LogEntry
{
public:
    SourcePointLogEntry(QString type, QString location, QString file, int line, int column, QString text);
    QString description() const;
    void updateLinePos(int threshold, int delta);
protected:
    QString file;
    int line;
    int column;
    QString text;
};


class Logger
{
public:
    Logger(){};
    ~Logger();
    static Logger *instance();
    static void deleteInstance();

    void beginSection();
    void commitSection();
    void revertSection();
    void addEntry(LogEntry *entry);

    QStringList fullReport();
    int numEntries();
    void updateLineNumbers(int insertLine, int numLines);
    /*
        glabalState can be used for storage of application state
        together with the logger. This can be useful in some cases,
        for example the current filename is stored here when processing
        files.
    */
    QMap<QString, QString> globalState;
private:
    static Logger *theInstance;
    QList<LogEntry*> logEntries;
    QList<LogEntry*> pendingLogEntries;
};

QT_END_NAMESPACE

#endif
