/****************************************************************************
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the tools applications of the Qt Toolkit.
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

#include <qfileinfo.h>
#include <qregexp.h>
#include <qdebug.h>

#include "quoter.h"

QT_BEGIN_NAMESPACE

static void replaceMultipleNewlines(QString &s)
{
    const int n = s.size();
    bool slurping = false;
    int j = -1;
    const QChar newLine = QLatin1Char('\n');
    QChar *d = s.data();
    for (int i = 0; i != n; ++i) {
        const QChar c = d[i];
        bool hit = (c == newLine);
        if (slurping && hit)
            continue;
        d[++j] = c;
        slurping = hit;
    }
    s.resize(++j);
}

// This is equivalent to  line.split( QRegExp("\n(?!\n|$)") ) but much faster
static QStringList splitLines(const QString &line)
{
    QStringList result;
    int i = line.size();
    while (true) {
        int j = i - 1;
        while (j >= 0 && line.at(j) == QLatin1Char('\n'))
            --j; 
        while (j >= 0 && line.at(j) != QLatin1Char('\n'))
            --j; 
        result.prepend(line.mid(j + 1, i - j - 1));
        if (j < 0)
            break;
        i = j;
    }
    return result;
}

/*
  Transforms 'int x = 3 + 4' into 'int x=3+4'. A white space is kept
  between 'int' and 'x' because it is meaningful in C++.
*/
static void trimWhiteSpace( QString& str )
{
    enum { Normal, MetAlnum, MetSpace } state = Normal;
    const int n = str.length();

    int j = -1;
    QChar *d = str.data();
    for ( int i = 0; i != n; ++i ) {
        const QChar c = d[i];
        if ( c.isLetterOrNumber() ) {
            if ( state == Normal ) {
                state = MetAlnum;
            } else {
                if ( state == MetSpace )
                    str[++j] = c;
                state = Normal;
            }
            str[++j] = c;
        } else if ( c.isSpace() ) {
            if ( state == MetAlnum )
                state = MetSpace;
        } else {
            state = Normal;
            str[++j] = c;
        }
    }
    str.resize(++j);
}

Quoter::Quoter()
    : silent( false )
{
    /* We're going to hard code these delimiters:
        * C++, Qt, Qt Script, Java:
          //! [<id>]
        * .pro files:
          #! [<id>]
        * .xq, .xml, .html files:
          <!-- [<id>] -->
    */
    commentHash["pro"] = "#!";
    commentHash["py"] = "#!";
    commentHash["html"] = "<!--";
    commentHash["qrc"] = "<!--";
    commentHash["ui"] = "<!--";
    commentHash["xml"] = "<!--";
    commentHash["xq"] = "<!--";
}

void Quoter::reset()
{
    silent = false;
    plainLines.clear();
    markedLines.clear();
    codeLocation = Location::null;
}

void Quoter::quoteFromFile( const QString& userFriendlyFilePath,
			    const QString& plainCode,
			    const QString& markedCode )
{
    silent = false;

    /*
      Split the source code into logical lines. Empty lines are
      treated specially. Before:

	  p->alpha();
	  p->beta();

	  p->gamma();


	  p->delta();

      After:

	  p->alpha();
	  p->beta();\n
	  p->gamma();\n\n
	  p->delta();

      Newlines are preserved because they affect codeLocation.
    */
    codeLocation = Location( userFriendlyFilePath );

    plainLines = splitLines(plainCode);
    markedLines = splitLines(markedCode);
    if (markedLines.count() != plainLines.count()) {
        codeLocation.warning(tr("Something is wrong with qdoc's handling of marked code"));
        markedLines = plainLines;
    }

    /*
      Squeeze blanks (cat -s).
    */
    QStringList::Iterator m = markedLines.begin();
    while ( m != markedLines.end() ) {
        replaceMultipleNewlines( *m );
        ++m;
    }
    codeLocation.start();
}

QString Quoter::quoteLine( const Location& docLocation, const QString& command,
    const QString& pattern )
{
    if ( plainLines.isEmpty() ) {
        failedAtEnd( docLocation, command );
        return QString();
    }

    if ( pattern.isEmpty() ) {
        docLocation.warning( tr("Missing pattern after '\\%1'").arg(command) );
        return QString();
    }

    if ( match(docLocation, pattern, plainLines.first()) )
        return getLine();

    if ( !silent ) {
        docLocation.warning( tr("Command '\\%1' failed").arg(command) );
        codeLocation.warning( tr("Pattern '%1' didn't match here")
                  .arg(pattern) );
        silent = true;
    }
    return QString();
}

QString Quoter::quoteSnippet(const Location &docLocation, const QString &identifier)
{
    QString comment = commentForCode();
    QString delimiter = comment + QString(" [%1]").arg(identifier);
    QString t;

    while (!plainLines.isEmpty()) {
        if (match(docLocation, delimiter, plainLines.first())) {
            getLine();
            break;
        }
        getLine();
    }
    while (!plainLines.isEmpty()) {
        QString line = plainLines.first();
        if (match(docLocation, delimiter, line)) {
            QString lastLine = getLine();
            int dIndex = lastLine.indexOf(delimiter);
            if (dIndex > 0) {
                QString leading = lastLine.left(dIndex);
                dIndex = leading.indexOf(comment);
                if (dIndex != -1)
                    leading = leading.left(dIndex);
                if (!leading.trimmed().isEmpty())
                    t += leading;
            }
            return t;
        }
        // Remove special macros to support Qt namespacing.
	if (line.startsWith("QT_BEGIN_NAMESPACE")) {
            getLine();
        } else if (line.startsWith("QT_END_NAMESPACE")) {
            getLine();
            t += QLatin1Char('\n');
        } else if (!line.startsWith(comment)) {
            // Ordinary code
            t += getLine();
        } else {
            // Normal comments
            if (line.contains(QLatin1Char('\n')))
                t += QLatin1Char('\n');
            getLine();
        }
    }
    failedAtEnd(docLocation, QString("snippet (%1)").arg(delimiter));
    return t;
}

QString Quoter::quoteTo( const Location& docLocation, const QString& command,
			 const QString& pattern )
{
    QString t;
    QString comment = commentForCode();

    if ( pattern.isEmpty() ) {
        while ( !plainLines.isEmpty() ) {
            QString line = plainLines.first();
            // Remove special macros to support Qt namespacing.
	    if (line.startsWith("QT_BEGIN_NAMESPACE")) {
                getLine();
            } else if (line.startsWith("QT_END_NAMESPACE")) {
                getLine();
                t += QLatin1Char('\n');
            } else if (!line.startsWith(comment))
                // Ordinary code
                t += getLine();
            else {
                // Normal comments
                if (line.contains(QLatin1Char('\n')))
                    t += QLatin1Char('\n');
                getLine();
            }
        }
    } else {
        while ( !plainLines.isEmpty() ) {
            if ( match(docLocation, pattern, plainLines.first()) ) {
                return t;
            }
            t += getLine();
        }
        failedAtEnd( docLocation, command );
    }
    return t;
}

QString Quoter::quoteUntil( const Location& docLocation, const QString& command,
			    const QString& pattern )
{
    QString t = quoteTo( docLocation, command, pattern );
    t += getLine();
    return t;
}

QString Quoter::getLine()
{
    if ( plainLines.isEmpty() )
        return QString();

    plainLines.removeFirst();

    QString t = markedLines.takeFirst();
    t += QLatin1Char('\n');
    codeLocation.advanceLines( t.count( QLatin1Char('\n') ) );
    return t;
}

bool Quoter::match( const Location& docLocation, const QString& pattern0,
                    const QString& line )
{
    QString str = line;
    while ( str.endsWith(QLatin1Char('\n')) )
        str.truncate( str.length() - 1 );

    QString pattern = pattern0;
    if ( pattern.startsWith(QLatin1Char('/'))
        && pattern.endsWith(QLatin1Char('/'))
        && pattern.length() > 2 ) {
        QRegExp rx( pattern.mid(1, pattern.length() - 2) );
        if ( !silent && !rx.isValid() ) {
            docLocation.warning( tr("Invalid regular expression '%1'")
                                 .arg(rx.pattern()) );
            silent = true;
        }
        return str.indexOf( rx ) != -1;
    }
    trimWhiteSpace(str);
    trimWhiteSpace(pattern);
    return str.indexOf(pattern) != -1;
}

void Quoter::failedAtEnd( const Location& docLocation, const QString& command )
{
    if (!silent && !command.isEmpty()) {
	if ( codeLocation.filePath().isEmpty() ) {
	    docLocation.warning( tr("Unexpected '\\%1'").arg(command) );
	} else {
	    docLocation.warning( tr("Command '\\%1' failed at end of file '%2'")
				 .arg(command).arg(codeLocation.filePath()) );
	}
	silent = true;
    }
}

QString Quoter::commentForCode() const
{
    QString suffix = QFileInfo(codeLocation.fileName()).suffix();
    return commentHash.value(suffix, "//!");
}

QT_END_NAMESPACE
