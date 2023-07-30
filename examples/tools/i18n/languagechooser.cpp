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

#include "languagechooser.h"
#include "mainwindow.h"

#ifdef Q_WS_MAC
QT_BEGIN_NAMESPACE
extern void qt_mac_set_menubar_merge(bool merge);
QT_END_NAMESPACE
#endif

LanguageChooser::LanguageChooser(QWidget *parent)
    : QDialog(parent, Qt::WindowStaysOnTopHint)
{
    groupBox = new QGroupBox("Languages");

    QGridLayout *groupBoxLayout = new QGridLayout;

    QStringList qmFiles = findQmFiles();
    for (int i = 0; i < qmFiles.size(); ++i) {
        QCheckBox *checkBox = new QCheckBox(languageName(qmFiles[i]));
        qmFileForCheckBoxMap.insert(checkBox, qmFiles[i]);
        connect(checkBox, SIGNAL(toggled(bool)), this, SLOT(checkBoxToggled()));
        groupBoxLayout->addWidget(checkBox, i / 2, i % 2);
    }
    groupBox->setLayout(groupBoxLayout);

    buttonBox = new QDialogButtonBox;

    showAllButton = buttonBox->addButton("Show All",
                                         QDialogButtonBox::ActionRole);
    hideAllButton = buttonBox->addButton("Hide All",
                                         QDialogButtonBox::ActionRole);

    connect(showAllButton, SIGNAL(clicked()), this, SLOT(showAll()));
    connect(hideAllButton, SIGNAL(clicked()), this, SLOT(hideAll()));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(groupBox);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

#ifdef Q_WS_MAC
    qt_mac_set_menubar_merge(false);
#endif

    setWindowTitle("I18N");
}

bool LanguageChooser::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::Close) {
        MainWindow *window = qobject_cast<MainWindow *>(object);
        if (window) {
            QCheckBox *checkBox = mainWindowForCheckBoxMap.key(window);
            if (checkBox)
                checkBox->setChecked(false);
        }
    }
    return QWidget::eventFilter(object, event);
}

void LanguageChooser::closeEvent(QCloseEvent * /* event */)
{
    qApp->quit();
}

void LanguageChooser::checkBoxToggled()
{
    QCheckBox *checkBox = qobject_cast<QCheckBox *>(sender());
    MainWindow *window = mainWindowForCheckBoxMap[checkBox];
    if (!window) {
        QTranslator translator;
        translator.load(qmFileForCheckBoxMap[checkBox]);
        qApp->installTranslator(&translator);

        window = new MainWindow;
        window->setPalette(colorForLanguage(checkBox->text()));

        window->installEventFilter(this);
        mainWindowForCheckBoxMap.insert(checkBox, window);
    }
    window->setVisible(checkBox->isChecked());
}

void LanguageChooser::showAll()
{
    foreach (QCheckBox *checkBox, qmFileForCheckBoxMap.keys())
        checkBox->setChecked(true);
}

void LanguageChooser::hideAll()
{
    foreach (QCheckBox *checkBox, qmFileForCheckBoxMap.keys())
        checkBox->setChecked(false);
}

QStringList LanguageChooser::findQmFiles()
{
    QDir dir(":/translations");
    QStringList fileNames = dir.entryList(QStringList("*.qm"), QDir::Files,
                                          QDir::Name);
    QMutableStringListIterator i(fileNames);
    while (i.hasNext()) {
        i.next();
        i.setValue(dir.filePath(i.value()));
    }
    return fileNames;
}

QString LanguageChooser::languageName(const QString &qmFile)
{
    QTranslator translator;
    translator.load(qmFile);

    return translator.translate("MainWindow", "English");
}

QColor LanguageChooser::colorForLanguage(const QString &language)
{
    uint hashValue = qHash(language);
    int red = 156 + (hashValue & 0x3F);
    int green = 156 + ((hashValue >> 6) & 0x3F);
    int blue = 156 + ((hashValue >> 12) & 0x3F);
    return QColor(red, green, blue);
}
