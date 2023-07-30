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

#include "filewriter.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <ctype.h>

QT_BEGIN_NAMESPACE

FileWriter *FileWriter::theInstance  = 0;

FileWriter *FileWriter::instance()
{
     if(!theInstance)
        theInstance = new FileWriter();

        return theInstance;
}

void FileWriter::deleteInstance()
{
    if(theInstance) {
        delete theInstance;
        theInstance=0;
    }
}

FileWriter::FileWriter(OverWriteFiles overWrite, QString overwriteMsg)
:overWriteFiles(overWrite)
,overwriteMessage(overwriteMsg)
{
    if(overwriteMessage.isEmpty())
       overwriteMessage = QLatin1String("Convert file ");
}

FileWriter::WriteResult FileWriter::writeFileVerbously(QString filePath, QByteArray contents)
{
    const WriteResult result = writeFile(filePath, contents);
    if (result == WriteSucceeded) {
        QString cleanPath = QDir::cleanPath(filePath);
        printf("Wrote to file: %s \n", QDir::toNativeSeparators(cleanPath).toLocal8Bit().constData());
    }
    return result;
}

FileWriter::WriteResult FileWriter::writeFile(QString filePath, QByteArray contents)
{
    if(filePath.isEmpty())
        return WriteFailed;
    QString path = QFileInfo(filePath).path();
    if (!QDir().mkpath(path)){
         printf("Error creating path %s \n", QDir::toNativeSeparators(path).toLocal8Bit().constData());
    }

    QString cleanPath = QDir::cleanPath(filePath);
    QFile f(cleanPath);
    if (f.exists()) {
        if (overWriteFiles == DontOverWrite) {
            printf("Error writing file %s: It already exists \n",
                QDir::toNativeSeparators(cleanPath).toLatin1().constData());
            return WriteFailed;
        } else if(overWriteFiles == AskOnOverWrite) {
            printf("%s%s? (Y)es, (N)o, (A)ll ", overwriteMessage.toLatin1().constData(),
                QDir::toNativeSeparators(cleanPath).toLatin1().constData());
            
            char answer = 0;
            while (answer != 'y' && answer != 'n' && answer != 'a') {
#if defined(Q_OS_WIN) && defined(_MSC_VER) && _MSC_VER >= 1400
                scanf_s("%c", &answer);
#else
                scanf("%c", &answer);
#endif
                answer = tolower(answer);
            }

            if(answer == 'n')
                return WriteSkipped;
            else if(answer == 'a')
                overWriteFiles=AlwaysOverWrite;
        }
    }

    f.open(QFile::WriteOnly);
    if (f.isOpen() && f.write(contents) == contents.size())
        return WriteSucceeded;

    printf("Could not write to to file: %s. Is it write protected?\n",
        QDir::toNativeSeparators(filePath).toLatin1().constData());

    return WriteFailed;
}

/*
    Sets the write mode for the file writer. writeMode is one of
    DontOverWrite, AlwaysOverWrite, AskOnOverWrite.
*/
void FileWriter::setOverwriteFiles(OverWriteFiles writeMode)
{
    overWriteFiles = writeMode;
}

QByteArray detectLineEndings(const QByteArray &array)
{
    if (array.contains("\r\n")) {
        return "\r\n";
    } else {
        return "\n";
    }
}

QT_END_NAMESPACE
