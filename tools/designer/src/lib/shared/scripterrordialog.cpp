/****************************************************************************
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the Qt Designer of the Qt Toolkit.
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

/*
TRANSLATOR qdesigner_internal::ScriptErrorDialog
*/

#include "scripterrordialog_p.h"

#include <QtGui/QTextEdit>
#include <QtGui/QTextCursor>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QPen>

QT_BEGIN_NAMESPACE

static void formatError(const QFormScriptRunner::Error &error,
                        QTextCursor &cursor)
{
    const QTextCharFormat oldFormat = cursor.charFormat();
    // Message
    cursor.insertText(QObject::tr("An error occurred while running the scripts for \"%1\":\n").arg(error.objectName));

    QTextCharFormat format(oldFormat);

    // verbatim listing
    format.setFontFamily(QLatin1String("Courier"));
    cursor.insertText(error.script, format);

    const QString newLine(QLatin1Char('\n'));

    cursor.insertText(newLine);

    // red error
    format = oldFormat;
    format.setTextOutline(QPen(Qt::red));
    cursor.insertText(error.errorMessage, format);
    cursor.insertText(newLine);
    cursor.setCharFormat (oldFormat);
}

namespace qdesigner_internal {

    // ScriptErrorDialog
    ScriptErrorDialog::ScriptErrorDialog(const Errors& errors, QWidget *parent) :
        QDialog(parent),
        m_textEdit(new QTextEdit)
    {
        setWindowTitle(tr("Script errors"));
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
        setModal(true);

        QVBoxLayout *vboxLayout = new QVBoxLayout(this);

        m_textEdit->setReadOnly(true);
        m_textEdit->setMinimumSize(QSize(600, 400));
        vboxLayout->addWidget(m_textEdit);
        // button box
        QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
        connect(buttonBox , SIGNAL(rejected()), this, SLOT(reject()));
        vboxLayout->addWidget(buttonBox);

        // Generate text
        QTextCursor cursor = m_textEdit->textCursor();
        cursor.movePosition (QTextCursor::End);
        foreach (const QFormScriptRunner::Error error, errors)
            formatError(error, cursor);
    }
} // namespace qdesigner_internal

QT_END_NAMESPACE
