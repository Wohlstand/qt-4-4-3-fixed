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

#ifndef PROJECTPORTER_H
#define PROJECTPORTER_H

#include "fileporter.h"
#include "preprocessorcontrol.h"
#include <QString>
#include <QSet>

QT_BEGIN_NAMESPACE

class ProjectPorter : public QObject
{
Q_OBJECT
public:
    ProjectPorter(QString basePath, QStringList includeDirectories, QStringList qt3HeadersFilenames = QStringList());
    void enableCppParsing(bool enable);
    void enableMissingFilesWarnings(bool enable);
    void portProject(QString filePath);
    void portFile(QString filePath);
private slots:
    void error(QString type, QString text);
private:
    void portProject(QString inPath, QString proFileName);
    void portProFile(QString fileName, QMap<QString, QString> tagMap);
    void portFiles(QString basePath, QStringList fileNames);
    void enableAttributes(const IncludeFiles &includeFiles, const QString &fileName);

    QSet<QString> processedFilesSet;
    QString basePath;
    QStringList includeDirectories;
    PreprocessorCache preprocessorCache;
    Rpp::DefineMap *defaultDefinitions;
    FilePorter filePorter;
    QStringList qt3HeadersFilenames;
    bool analyze;
    bool warnings;
};

QT_END_NAMESPACE

#endif
