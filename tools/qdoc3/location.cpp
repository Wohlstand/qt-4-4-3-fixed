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

#include "config.h"
#include "location.h"

#include <qregexp.h>
#include <qtranslator.h>
#include <stdlib.h>
#include <limits.h>

#include <stdio.h>

QT_BEGIN_NAMESPACE

QT_STATIC_CONST_IMPL Location Location::null;

int Location::tabSize;
QString Location::programName;
QRegExp *Location::spuriousRegExp = 0;

/*!
  \class Location

  \brief The Location class keeps track of where we are in a file.

  It maintains a stack of file positions. A file position 
  consists of the file path, line number, and column number.
 */

/*!
  Constructs an empty location.
 */
Location::Location()
    : stk(0), stkTop(&stkBottom), stkDepth(0), etcetera(false)
{
    // nothing.
}

/*!
  Constructs a location with (fileName, 1, 1) on its file
  position stack.
 */
Location::Location(const QString& fileName)
    : stk(0), stkTop(&stkBottom), stkDepth(0), etcetera(false)
{
    push(fileName);
}

/*!
  The copy constructor copies the contents of \a other into
  this Location using the assignment operator.
 */
Location::Location(const Location& other)
    : stk(0), stkTop(&stkBottom), stkDepth(0), etcetera(false)
{
    *this = other;
}

/*!
  The assignment operator does a deep copy of the entire
  state of \a other into this Location.
 */
Location& Location::operator=(const Location& other)
{
    QStack<StackEntry> *oldStk = stk;

    stkBottom = other.stkBottom;
    if (other.stk == 0) {
	stk = 0;
	stkTop = &stkBottom;
    }
    else {
	stk = new QStack<StackEntry>(*other.stk);
	stkTop = &stk->top();
    }
    stkDepth = other.stkDepth;
    etcetera = other.etcetera;
    delete oldStk;
    return *this;
}

/*!
  If the file position on top of the stack has a line number
  less than 1, set its line number to 1 and its column number
  to 1. Otherwise, do nothing.
  */
void Location::start()
{
    if (stkTop->lineNo < 1) {
	stkTop->lineNo = 1;
	stkTop->columnNo = 1;
    }
}

/*!
  Advance the current file position, using \a ch to decide how to do
  that. If \a ch is a \c{'\\n'}, increment the current line number and
  set the column number to 1. If \ch is a \c{'\\t'}, increment to the
  next tab column. Otherwise, increment the column number by 1.

  The current file position is the one on top of the position stack.
 */
void Location::advance(QChar ch)
{
    if (ch == QLatin1Char('\n')) {
        stkTop->lineNo++;
        stkTop->columnNo = 1;
    }
    else if (ch == QLatin1Char('\t')) {
        stkTop->columnNo =
            1 + tabSize * (stkTop->columnNo + tabSize-1) / tabSize;
    }
    else {
        stkTop->columnNo++;
    }
}

/*!
  Pushes \a filePath onto the file position stack. The current
  file position becomes (\a filePath, 1, 1).

  \sa pop()
*/
void Location::push(const QString& filePath)
{
    if (stkDepth++ >= 1) {
	if (stk == 0)
	    stk = new QStack<StackEntry>;
	stk->push(StackEntry());
	stkTop = &stk->top();
    }

    stkTop->filePath = filePath;
    stkTop->lineNo = INT_MIN;
    stkTop->columnNo = 1;
}

/*!
  Pops the top of the internal stack. The current file position
  becomes the next one in the new top of stack.

  \sa push()
*/
void Location::pop()
{
    if (--stkDepth == 0) {
	stkBottom = StackEntry();
    }
    else {
	stk->pop();
	if (stk->isEmpty()) {
	    delete stk;
	    stk = 0;
	    stkTop = &stkBottom;
	}
        else {
	    stkTop = &stk->top();
	}
    }
}

/*! \fn bool Location::isEmpty() const

  Returns true if there is no file name set yet; returns false
  otherwise. The functions filePath(), lineNo() and columnNo()
  must not be called on an empty Location object.
 */

/*! \fn const QString& Location::filePath() const
  Returns the current path and file name.
  Must not be called on an empty Location object.

  \sa lineNo(), columnNo()
 */

/*!
  Returns the file name part of the file path, ie the
  current file.  Must not be called on an empty Location
  object.
 */
QString Location::fileName() const
{
    QString fp = filePath();
    return fp.mid(fp.lastIndexOf('/') + 1);
}

/*! \fn int Location::lineNo() const
  Returns the current line number.
  Must not be called on an empty Location object.

  \sa filePath(), columnNo()
*/

/*! \fn int Location::columnNo() const
  Returns the current column number.
  Must not be called on an empty Location object.

  \sa filePath(), lineNo()
*/

/*!
  Writes \a message and \a detals to stderr as a formatted
  warning message.
 */
void Location::warning(const QString& message, const QString& details) const
{
    emitMessage(Warning, message, details);
}

/*!
  Writes \a message and \a detals to stderr as a formatted
  error message.
 */
void Location::error(const QString& message, const QString& details) const
{
    emitMessage(Error, message, details);
}

/*!
  Writes \a message and \a detals to stderr as a formatted
  error message and then exits the program.
 */
void Location::fatal(const QString& message, const QString& details) const
{
    emitMessage(Error, message, details);
    information("Aborting");
    exit(EXIT_FAILURE);
}

/*!
  
 */
void Location::initialize(const Config& config)
{
    tabSize = config.getInt(CONFIG_TABSIZE);
    programName = config.programName();

    QRegExp regExp = config.getRegExp(CONFIG_SPURIOUS);
    if (regExp.isValid()) {
	spuriousRegExp = new QRegExp(regExp);
    }
    else {
	config.lastLocation().warning(tr("Invalid regular expression '%1'")
				       .arg(regExp.pattern()));
    }
}

/*!
  Apparently, all this does is delete a so-called
  \e{spurious regular expression} in the internal
  state and set the pointer to it to 0.
 */
void Location::terminate()
{
    delete spuriousRegExp;
    spuriousRegExp = 0;
}

/*!
  Prints \a message to \c stdout followed by a \c{'\n'}.
 */
void Location::information(const QString& message)
{
    printf("%s\n", message.toLatin1().data());
    fflush(stdout);
}

/*!
  Report a program bug, including the \a hint. 
 */
void Location::internalError(const QString& hint)
{
    Location::null.fatal(tr("Internal error (%1)").arg(hint),
			  tr("There is a bug in %1. Seek advice from your local"
			     " %2 guru.")
			  .arg(programName).arg(programName));
}

/*!
  Formats \a message and \a details into a single string
  and outputs that string to \c stderr. \a type specifies
  whether the \a message is an error or a warning.
 */
void Location::emitMessage(MessageType type,
                           const QString& message,
                           const QString& details) const
{
    if (type == Warning &&
        spuriousRegExp != 0 &&
        spuriousRegExp->exactMatch(message))
	return;

    QString result = message;
    if (!details.isEmpty())
	result += "\n[" + details + "]";
    result.replace("\n", "\n    ");
    if (type == Error)
	result.prepend(tr("error: "));
    result.prepend(toString());
    fprintf(stderr, "%s\n", result.toLatin1().data());
    fflush(stderr);
}

QString Location::toString() const
{
    QString str;

    if (isEmpty()) {
        str = programName;
    }
    else {
        Location loc2 = *this;
        loc2.setEtc(false);
        loc2.pop();
        if (!loc2.isEmpty()) {
            QString blah = tr("In file included from ");
            for (;;) {
                str += blah;
                str += loc2.top();
                loc2.pop();
                if (loc2.isEmpty())
                    break;
                str += tr(",");
                str += QLatin1Char('\n');
                blah.fill(' ');
            }
            str += tr(":");
            str += QLatin1Char('\n');
        }
        str += top();
    }
    str += QLatin1String(": ");
    return str;
}

QString Location::top() const
{
    QString str = filePath();
    if (lineNo() >= 1) {
        str += QLatin1Char(':'); 
        str += QString::number(lineNo());
#if 0
        if (columnNo() >= 1)
            str += ":" + QString::number(columnNo());
#endif
    }
    if (etc())
        str += QLatin1String(" (etc.)");
    return str;
}

QT_END_NAMESPACE
