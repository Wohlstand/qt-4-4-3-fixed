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

#include <QList>
#include <QMap>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QTime>
#include <QApplication>
#include <QDebug>

#include <qtconcurrentmap.h>

#ifndef QT_NO_CONCURRENT

using namespace QtConcurrent;

/*
    Utility function that recursivily searches for files.
*/
QStringList findFiles(const QString &startDir, QStringList filters)
{
    QStringList names;
    QDir dir(startDir);

    foreach (QString file, dir.entryList(filters, QDir::Files))
        names += startDir + "/" + file;

    foreach (QString subdir, dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot))
        names += findFiles(startDir + "/" + subdir, filters);
    return names;
}

typedef QMap<QString, int> WordCount;

/*
    Single threaded word counter function.
*/
WordCount singleThreadedWordCount(QStringList files)
{
    WordCount wordCount;
    foreach (QString file, files) {
        QFile f(file);
        f.open(QIODevice::ReadOnly);
        QTextStream textStream(&f);
        while (textStream.atEnd() == false)
            foreach(QString word, textStream.readLine().split(" "))
                wordCount[word] += 1;

    }
    return wordCount;
}


// countWords counts the words in a single file. This function is
// called in parallel by several threads and must be thread
// safe.
WordCount countWords(const QString &file)
{
    QFile f(file);
    f.open(QIODevice::ReadOnly);
    QTextStream textStream(&f);
    WordCount wordCount;

    while (textStream.atEnd() == false)
        foreach (QString word, textStream.readLine().split(" "))
            wordCount[word] += 1;

    return wordCount;
}

// reduce adds the results from map to the final
// result. This functor will only be called by one thread
// at a time.
void reduce(WordCount &result, const WordCount &w)
{
    QMapIterator<QString, int> i(w);
    while (i.hasNext()) {
        i.next();
        result[i.key()] += i.value();
    }
}

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    qDebug() << "finding files...";
    QStringList files = findFiles("../../", QStringList() << "*.cpp" << "*.h");
    qDebug() << files.count() << "files";

    qDebug() << "warmup";
    {
        QTime time;
        time.start();
        WordCount total = singleThreadedWordCount(files);
    }

    qDebug() << "warmup done";

    int singleThreadTime = 0;
    {
        QTime time;
        time.start();
        WordCount total = singleThreadedWordCount(files);
        singleThreadTime = time.elapsed();
        qDebug() << "single thread" << singleThreadTime;
    }

    int mapReduceTime = 0;
    {
        QTime time;
        time.start();
        WordCount total = mappedReduced(files, countWords, reduce);
        mapReduceTime = time.elapsed();
        qDebug() << "MapReduce" << mapReduceTime;
    }
    qDebug() << "MapReduce speedup x" << ((double)singleThreadTime - (double)mapReduceTime) / (double)mapReduceTime + 1;
}

#else

int main()
{
	qDebug() << "Qt Concurrent is not yet supported on this platform";
}

#endif
