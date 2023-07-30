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

#include "mysortfilterproxymodel.h"
#include "window.h"

//! [0]
Window::Window()
{
    proxyModel = new MySortFilterProxyModel(this);
    proxyModel->setDynamicSortFilter(true);
//! [0]

//! [1]
    sourceView = new QTreeView;
    sourceView->setRootIsDecorated(false);
    sourceView->setAlternatingRowColors(true);
//! [1]

    QHBoxLayout *sourceLayout = new QHBoxLayout;
//! [2]
    sourceLayout->addWidget(sourceView);
    sourceGroupBox = new QGroupBox(tr("Original Model"));
    sourceGroupBox->setLayout(sourceLayout);
//! [2]

//! [3]
    filterCaseSensitivityCheckBox = new QCheckBox(tr("Case sensitive filter"));
    filterCaseSensitivityCheckBox->setChecked(true);

    filterPatternLineEdit = new QLineEdit;
    filterPatternLineEdit->setText("Grace|Sports");

    filterPatternLabel = new QLabel(tr("&Filter pattern:"));
    filterPatternLabel->setBuddy(filterPatternLineEdit);

    filterSyntaxComboBox = new QComboBox;
    filterSyntaxComboBox->addItem(tr("Regular expression"), QRegExp::RegExp);
    filterSyntaxComboBox->addItem(tr("Wildcard"), QRegExp::Wildcard);
    filterSyntaxComboBox->addItem(tr("Fixed string"), QRegExp::FixedString);

    fromDateEdit = new QDateEdit;
    fromDateEdit->setDate(QDate(1970, 01, 01));
    fromLabel = new QLabel(tr("F&rom:"));
    fromLabel->setBuddy(fromDateEdit);

    toDateEdit = new QDateEdit;
    toDateEdit->setDate(QDate(2099, 12, 31));
    toLabel = new QLabel(tr("&To:"));
    toLabel->setBuddy(toDateEdit);

    connect(filterPatternLineEdit, SIGNAL(textChanged(const QString &)),
            this, SLOT(textFilterChanged()));
    connect(filterSyntaxComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(textFilterChanged()));
    connect(filterCaseSensitivityCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(textFilterChanged()));
    connect(fromDateEdit, SIGNAL(dateChanged(const QDate &)),
            this, SLOT(dateFilterChanged()));
    connect(toDateEdit, SIGNAL(dateChanged(const QDate &)),
//! [3] //! [4]
            this, SLOT(dateFilterChanged()));
//! [4]

//! [5]
    proxyView = new QTreeView;
    proxyView->setRootIsDecorated(false);
    proxyView->setAlternatingRowColors(true);
    proxyView->setModel(proxyModel);
    proxyView->setSortingEnabled(true);
    proxyView->sortByColumn(1, Qt::AscendingOrder);

    QGridLayout *proxyLayout = new QGridLayout;
    proxyLayout->addWidget(proxyView, 0, 0, 1, 3);
    proxyLayout->addWidget(filterPatternLabel, 1, 0);
    proxyLayout->addWidget(filterPatternLineEdit, 1, 1);
    proxyLayout->addWidget(filterSyntaxComboBox, 1, 2);
    proxyLayout->addWidget(filterCaseSensitivityCheckBox, 2, 0, 1, 3);
    proxyLayout->addWidget(fromLabel, 3, 0);
    proxyLayout->addWidget(fromDateEdit, 3, 1, 1, 2);
    proxyLayout->addWidget(toLabel, 4, 0);
    proxyLayout->addWidget(toDateEdit, 4, 1, 1, 2);

    proxyGroupBox = new QGroupBox(tr("Sorted/Filtered Model"));
    proxyGroupBox->setLayout(proxyLayout);
//! [5]

//! [6]
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(sourceGroupBox);
    mainLayout->addWidget(proxyGroupBox);
    setLayout(mainLayout);

    setWindowTitle(tr("Custom Sort/Filter Model"));
    resize(500, 450);
}
//! [6]

//! [7]
void Window::setSourceModel(QAbstractItemModel *model)
{
    proxyModel->setSourceModel(model);
    sourceView->setModel(model);
}
//! [7]

//! [8]
void Window::textFilterChanged()
{
    QRegExp::PatternSyntax syntax =
            QRegExp::PatternSyntax(filterSyntaxComboBox->itemData(
                    filterSyntaxComboBox->currentIndex()).toInt());
    Qt::CaseSensitivity caseSensitivity =
            filterCaseSensitivityCheckBox->isChecked() ? Qt::CaseSensitive
                                                       : Qt::CaseInsensitive;

    QRegExp regExp(filterPatternLineEdit->text(), caseSensitivity, syntax);
    proxyModel->setFilterRegExp(regExp);
}
//! [8]

//! [9]
void Window::dateFilterChanged()
{
    proxyModel->setFilterMinimumDate(fromDateEdit->date());
    proxyModel->setFilterMaximumDate(toDateEdit->date());
}
//! [9]
