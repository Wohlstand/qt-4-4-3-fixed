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
TRANSLATOR qdesigner_internal::CodeDialog
*/

#include "codedialog_p.h"
#include "qdesigner_utils_p.h"
#include "iconloader_p.h"

#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QClipboard>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFileDialog>
#include <QtGui/QIcon>
#include <QtGui/QKeyEvent>
#include <QtGui/QMessageBox>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QTemporaryFile>

#include "findwidget.h"

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {
// ----------------- CodeDialogPrivate
struct CodeDialog::CodeDialogPrivate {
    CodeDialogPrivate();

    QTextEdit *m_textEdit;
    FindWidget *m_findWidget;
    QString m_formFileName;
};

CodeDialog::CodeDialogPrivate::CodeDialogPrivate()
    : m_textEdit(new QTextEdit)
    , m_findWidget(new FindWidget)
{
}

// ----------------- CodeDialog
CodeDialog::CodeDialog(QWidget *parent) :
    QDialog(parent),
    m_impl(new CodeDialogPrivate)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    QVBoxLayout *vBoxLayout = new QVBoxLayout;

    // Edit tool bar
    QToolBar *toolBar = new QToolBar;

    const QIcon saveIcon = createIconSet(QLatin1String("filesave.png"));
    QAction *saveAction = toolBar->addAction(saveIcon, tr("Save..."));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(slotSaveAs()));

    const QIcon copyIcon = createIconSet(QLatin1String("editcopy.png"));
    QAction *copyAction = toolBar->addAction(copyIcon, tr("Copy All"));
    connect(copyAction, SIGNAL(triggered()), this, SLOT(copyAll()));

    QAction *findAction = toolBar->addAction(
            FindWidget::findIconSet(),
            tr("&Find in Text..."),
            m_impl->m_findWidget, SLOT(activate()));
    findAction->setShortcut(QKeySequence::Find);

    vBoxLayout->addWidget(toolBar);

    // Edit
    m_impl->m_textEdit->setReadOnly(true);
    m_impl->m_textEdit->setMinimumSize(QSize(
                m_impl->m_findWidget->minimumSize().width(),
                500));
    vBoxLayout->addWidget(m_impl->m_textEdit);

    // Find
    m_impl->m_findWidget->setTextEdit(m_impl->m_textEdit);
    vBoxLayout->addWidget(m_impl->m_findWidget);

    // Button box
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    // Disable auto default
    QPushButton *closeButton = buttonBox->button(QDialogButtonBox::Close);
    closeButton->setAutoDefault(false);
    vBoxLayout->addWidget(buttonBox);

    setLayout(vBoxLayout);
}

CodeDialog::~CodeDialog()
{
    delete m_impl;
}

void CodeDialog::setCode(const QString &code)
{
    m_impl->m_textEdit->setPlainText(code);
}

QString CodeDialog::code() const
{
   return m_impl->m_textEdit->toPlainText();
}

void CodeDialog::setFormFileName(const QString &f)
{
    m_impl->m_formFileName = f;
}

QString CodeDialog::formFileName() const
{
    return m_impl->m_formFileName;
}

bool CodeDialog::generateCode(const QDesignerFormWindowInterface *fw,
                              QString *code,
                              QString *errorMessage)
{
    // Generate temporary file name similar to form file name
    // (for header guards)
    QString tempPattern = QDir::tempPath();
    if (!tempPattern.endsWith(QDir::separator())) // platform-dependant
        tempPattern += QDir::separator();
    const QString fileName = fw->fileName();
    if (fileName.isEmpty()) {
        tempPattern += QLatin1String("designer");
    } else {
        tempPattern += QFileInfo(fileName).baseName();
    }
    tempPattern += QLatin1String("XXXXXX.ui");
    // Write to temp file
    QTemporaryFile tempFormFile(tempPattern);

    tempFormFile.setAutoRemove(true);
    if (!tempFormFile.open()) {
        *errorMessage = tr("A temporary form file could not be created in %1.").arg(QDir::tempPath());
        return false;
    }
    const QString tempFormFileName = tempFormFile.fileName();
    tempFormFile.write(fw->contents().toUtf8());
    if (!tempFormFile.flush())  {
        *errorMessage = tr("The temporary form file %1 could not be written.").arg(tempFormFileName);
        return false;
    }
    tempFormFile.close();
    // Run uic
    QByteArray rc;
    if (!runUIC(tempFormFileName, UIC_GenerateCode, rc, *errorMessage))
        return false;
    *code = QString::fromUtf8(rc);
    return true;
}

bool CodeDialog::showCodeDialog(const QDesignerFormWindowInterface *fw,
                                QWidget *parent,
                                QString *errorMessage)
{
    QString code;
    if (!generateCode(fw, &code, errorMessage))
        return false;

    CodeDialog dialog(parent);
    dialog.setWindowTitle(QObject::tr("%1 - [Code]").arg(fw->mainContainer()->windowTitle()));
    dialog.setCode(code);
    dialog.setFormFileName(fw->fileName());
    dialog.exec();
    return true;
}

void CodeDialog::slotSaveAs()
{
    // build the default relative name 'ui_sth.h'
    const QString headerSuffix = QString(QLatin1Char('h'));
    QString filter;
    const QString uiFile = formFileName();

    if (!uiFile.isEmpty()) {
        filter = QLatin1String("ui_");
        filter += QFileInfo(uiFile).baseName();
        filter += QLatin1Char('.');
        filter += headerSuffix;
    }
    // file dialog
    while (true) {
        const QString fileName =
            QFileDialog::getSaveFileName (this, tr("Save Code"), filter, tr("Header Files (*.%1)").arg(headerSuffix));
        if (fileName.isEmpty())
            break;

         QFile file(fileName);
         if (!file.open(QIODevice::WriteOnly|QIODevice::Text)) {
             warning(tr("The file %1 could not be opened: %2").arg(fileName).arg(file.errorString()));
             continue;
         }
         file.write(code().toUtf8());
         if (!file.flush()) {
             warning(tr("The file %1 could not be written: %2").arg(fileName).arg(file.errorString()));
             continue;
         }
         file.close();
         break;
    }
}

void CodeDialog::warning(const QString &msg)
{
     QMessageBox::warning(
             this, tr("%1 - Error").arg(windowTitle()),
             msg, QMessageBox::Close);
}

void CodeDialog::copyAll()
{
    QApplication::clipboard()->setText(code());
}

} // namespace qdesigner_internal

QT_END_NAMESPACE
