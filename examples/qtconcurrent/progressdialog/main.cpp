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

#ifndef QT_NO_CONCURRENT

using namespace QtConcurrent;

const int iterations = 20;

void spin(int &iteration)
{
    const int work = 1000 * 1000 * 40;
    volatile int v = 0;
    for (int j = 0; j < work; ++j)
        ++v;

    qDebug() << "iteration" << iteration << "in thread" << QThread::currentThreadId();
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    // Prepare the vector.
    QVector<int> vector;
    for (int i = 0; i < iterations; ++i)
        vector.append(i);

    // Create a progress dialog.
    QProgressDialog dialog;
    dialog.setLabelText(QString("Progressing using %1 thread(s)...").arg(QThread::idealThreadCount()));
 
    // Create a QFutureWatcher and conncect signals and slots.
    QFutureWatcher<void> futureWatcher;
    QObject::connect(&futureWatcher, SIGNAL(finished()), &dialog, SLOT(reset()));
    QObject::connect(&dialog, SIGNAL(canceled()), &futureWatcher, SLOT(cancel()));
    QObject::connect(&futureWatcher, SIGNAL(progressRangeChanged(int, int)), &dialog, SLOT(setRange(int, int)));
    QObject::connect(&futureWatcher, SIGNAL(progressValueChanged(int)), &dialog, SLOT(setValue(int)));

    // Start the computation.
    futureWatcher.setFuture(QtConcurrent::map(vector, spin));

    // Display the dialog and start the event loop.
    dialog.exec();
    
    futureWatcher.waitForFinished();

    // Query the future to check if was canceled.
    qDebug() << "Canceled?" << futureWatcher.future().isCanceled();
}

#else

int main()
{
	qDebug() << "Qt Concurrent is not yet supported on this platform";
}

#endif

