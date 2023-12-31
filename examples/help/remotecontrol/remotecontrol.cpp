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

#include <QtCore/QDir>
#include <QtCore/QProcess>
#include <QtCore/QTextStream>
#include <QtCore/QLibraryInfo>

#include <QtGui/QMessageBox>

#include "remotecontrol.h"

RemoteControl::RemoteControl(QWidget *parent, Qt::WFlags flags)
        : QMainWindow(parent, flags)
{
    ui.setupUi(this);
    connect(ui.indexLineEdit, SIGNAL(returnPressed()),
        this, SLOT(on_indexButton_clicked()));
    connect(ui.identifierLineEdit, SIGNAL(returnPressed()),
        this, SLOT(on_identifierButton_clicked()));
    connect(ui.urlLineEdit, SIGNAL(returnPressed()),
        this, SLOT(on_urlButton_clicked()));

    QString rc;
    QTextStream(&rc) << QLatin1String("qthelp://com.trolltech.qt.")
                     << (QT_VERSION >> 16) << ((QT_VERSION >> 8) & 0xFF)
                     << (QT_VERSION & 0xFF)
                     << QLatin1String("/qdoc/index.html");

    ui.startUrlLineEdit->setText(rc);

    process = new QProcess(this);
    connect(process, SIGNAL(finished(int, QProcess::ExitStatus)),
        this, SLOT(helpViewerClosed()));
}

RemoteControl::~RemoteControl()
{
    if (process->state() == QProcess::Running) {
        process->terminate();
        process->waitForFinished(3000);
    }
}

void RemoteControl::on_actionQuit_triggered()
{
    close();
}

void RemoteControl::on_launchButton_clicked()
{
    if (process->state() == QProcess::Running)
        return;

    QString app = QLibraryInfo::location(QLibraryInfo::BinariesPath) + QDir::separator();
#if !defined(Q_OS_MAC)
    app += QLatin1String("assistant");
#else
    app += QLatin1String("Assistant.app/Contents/MacOS/Assistant");
#endif

    ui.contentsCheckBox->setChecked(true);
    ui.indexCheckBox->setChecked(true);
    ui.bookmarksCheckBox->setChecked(true);

    QStringList args;
    args << QLatin1String("-enableRemoteControl");
    process->start(app, args);
    if (!process->waitForStarted()) {
        QMessageBox::critical(this, tr("Remote Control"),
            tr("Could not start Qt Assistant from %1.").arg(app));
        return;
    }

    if (!ui.startUrlLineEdit->text().isEmpty())
        sendCommand(QLatin1String("SetSource ")
            + ui.startUrlLineEdit->text());
        
    ui.launchButton->setEnabled(false);
    ui.startUrlLineEdit->setEnabled(false);
    ui.actionGroupBox->setEnabled(true);
}

void RemoteControl::sendCommand(const QString &cmd)
{
    if (process->state() != QProcess::Running)
        return;
    QTextStream str(process);
    str << cmd << QLatin1Char('\0') << endl;
}

void RemoteControl::on_indexButton_clicked()
{
    sendCommand(QLatin1String("ActivateKeyword ")
        + ui.indexLineEdit->text());
}

void RemoteControl::on_identifierButton_clicked()
{
    sendCommand(QLatin1String("ActivateIdentifier ")
        + ui.identifierLineEdit->text());
}

void RemoteControl::on_urlButton_clicked()
{
    sendCommand(QLatin1String("SetSource ")
        + ui.urlLineEdit->text());
}

void RemoteControl::on_syncContentsButton_clicked()
{
    sendCommand(QLatin1String("SyncContents"));
}

void RemoteControl::on_contentsCheckBox_toggled(bool checked)
{
    sendCommand(checked ?
        QLatin1String("Show Contents") : QLatin1String("Hide Contents"));
}

void RemoteControl::on_indexCheckBox_toggled(bool checked)
{
    sendCommand(checked ?
        QLatin1String("Show Index") : QLatin1String("Hide Index"));
}

void RemoteControl::on_bookmarksCheckBox_toggled(bool checked)
{
    sendCommand(checked ?
        QLatin1String("Show Bookmarks") : QLatin1String("Hide Bookmarks"));
}

void RemoteControl::helpViewerClosed()
{
    ui.launchButton->setEnabled(true);
    ui.startUrlLineEdit->setEnabled(true);
    ui.actionGroupBox->setEnabled(false);
}
