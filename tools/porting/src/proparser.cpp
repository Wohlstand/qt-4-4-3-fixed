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

#include "proparser.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QRegExp>
#include <QStringList>
#include <QTextStream>

#ifdef Q_OS_UNIX
#include <unistd.h>
#endif

#ifdef Q_OS_WIN32
#define QT_POPEN _popen
#else
#define QT_POPEN popen
#endif

QT_BEGIN_NAMESPACE

QString loadFile( const QString &fileName )
{
    QFile file( fileName );
    if ( !file.open(QIODevice::ReadOnly) ) {
        fprintf( stderr, "error: Cannot load '%s': %s\n",
                 file.fileName().toLocal8Bit().constData(),
                 file.errorString().toLatin1().constData() );
        return QString();
    }

    QTextStream in( &file );
    return in.readAll();
}

QMap<QString, QString> proFileTagMap( const QString& text, QString currentPath )
{
    QString t = text;
    if (currentPath.isEmpty())
        currentPath = QDir::currentPath();


    QMap<QString, QString> tagMap;
        /*
            Strip any commments before we try to include.  We
            still need to do it after we include to make sure the
            included file does not have comments
        */
        t.replace( QRegExp(QLatin1String("#[^\n]*\n")), QLatin1String(" ") );
      /*
            Strip comments, merge lines ending with backslash, add
            spaces around '=' and '+=', replace '\n' with ';', and
            simplify white spaces.
        */
        t.replace( QRegExp(QLatin1String("#[^\n]*\n")), QLatin1String(" ") );
        t.replace( QRegExp(QLatin1String("\\\\[^\n\\S]*\n")), QLatin1String(" ") );
        t.replace( QLatin1String("="), QLatin1String(" = ") );
        t.replace( QLatin1String("+ ="), QLatin1String(" += ") );
        t.replace( QLatin1String("\n"), QLatin1String(";") );
        t = t.simplified();

        /*
            Populate tagMap with 'key = value' entries.
        */
        QStringList lines = t.split(QLatin1Char(';'));
        QStringList::Iterator line;
        for ( line = lines.begin(); line != lines.end(); ++line ) {
            QStringList toks = (*line).split(QLatin1Char(' '), QString::SkipEmptyParts);

            if ( toks.count() >= 3 &&
                (toks[1] == QLatin1String("=") || toks[1] == QLatin1String("+=") ||
                toks[1] == QLatin1String("*=")) ) {
                QString tag = toks.first();
                int k = tag.lastIndexOf( QLatin1Char(':') ); // as in 'unix:'
                if ( k != -1 )
                    tag = tag.mid( k + 1 );
                toks.erase( toks.begin() );

                QString action = toks.first();
                toks.erase( toks.begin() );

                if ( tagMap.contains(tag) ) {
                    if ( action == QLatin1String("=") )
                        tagMap.insert( tag, toks.join(QLatin1String(" ")) );
                    else
                        tagMap[tag] += QLatin1Char( ' ' ) + toks.join( QLatin1String(" ") );
                } else {
                    tagMap[tag] = toks.join( QLatin1String(" ") );
                }
            }
        }
        /*
            Expand $$variables within the 'value' part of a 'key = value'
            pair.
        */
        QRegExp var( QLatin1String("\\$\\$[({]?([a-zA-Z0-9_]+)[)}]?") );
        QMap<QString, QString>::Iterator it;
        for ( it = tagMap.begin(); it != tagMap.end(); ++it ) {
            int i = 0;
            while ( (i = var.indexIn((*it), i)) != -1 ) {
                int len = var.matchedLength();
                QString invocation = var.cap(1);
                QString after;

                if ( invocation == QLatin1String("system") ) {
                    // skip system(); it will be handled in the next pass
                    ++i;
                } else {
                    if ( tagMap.contains(invocation) )
                        after = tagMap[invocation];
                    else if (invocation.toLower() == QLatin1String("pwd"))
                        after = currentPath;
                    (*it).replace( i, len, after );
                    i += after.length();
                }
            }
        }

        /*
          Execute system() calls.
        */
        QRegExp callToSystem( QLatin1String("\\$\\$system\\s*\\(([^()]*)\\)") );
        for ( it = tagMap.begin(); it != tagMap.end(); ++it ) {
            int i = 0;
            while ( (i = callToSystem.indexIn((*it), i)) != -1 ) {
                /*
                  This code is stolen from qmake's project.cpp file.
                  Ideally we would use the same parser, so we wouldn't
                  have this code duplication.
                */
                QString after;
                char buff[256];
                FILE *proc = QT_POPEN( callToSystem.cap(1).toLatin1().constData(), "r" );
                while ( proc && !feof(proc) ) {
                    int read_in = int(fread( buff, 1, 255, proc ));
                    if ( !read_in )
                        break;
                    for ( int i = 0; i < read_in; i++ ) {
                        if ( buff[i] == '\n' || buff[i] == '\t' )
                            buff[i] = ' ';
                    }
                    buff[read_in] = '\0';
                    after += QLatin1String(buff);
                }
                (*it).replace( i, callToSystem.matchedLength(), after );
                i += after.length();
            }
        }
    return tagMap;
}

QT_END_NAMESPACE
