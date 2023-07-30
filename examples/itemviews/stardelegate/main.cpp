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

#include "stardelegate.h"
#include "stareditor.h"
#include "starrating.h"

//! [0]
void populateTableWidget(QTableWidget *tableWidget)
{
    static const struct {
        const char *title;
        const char *genre;
        const char *artist;
        int rating;
    } staticData[] = {
//! [0] //! [1]
        { "Mass in B-Minor", "Baroque", "J.S. Bach", 5 },
//! [1]
        { "Three More Foxes", "Jazz", "Maynard Ferguson", 4 },
        { "Sex Bomb", "Pop", "Tom Jones", 3 },
        { "Barbie Girl", "Pop", "Aqua", 5 },
//! [2]
        { 0, 0, 0, 0 }
//! [2] //! [3]
    };
//! [3] //! [4]

    for (int row = 0; staticData[row].title != 0; ++row) {
        QTableWidgetItem *item0 = new QTableWidgetItem(staticData[row].title);
        QTableWidgetItem *item1 = new QTableWidgetItem(staticData[row].genre);
        QTableWidgetItem *item2 = new QTableWidgetItem(staticData[row].artist);
        QTableWidgetItem *item3 = new QTableWidgetItem;
        item3->setData(0,
                       qVariantFromValue(StarRating(staticData[row].rating)));

        tableWidget->setItem(row, 0, item0);
        tableWidget->setItem(row, 1, item1);
        tableWidget->setItem(row, 2, item2);
        tableWidget->setItem(row, 3, item3);
    }
}
//! [4]

//! [5]
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTableWidget tableWidget(4, 4);
    tableWidget.setItemDelegate(new StarDelegate);
    tableWidget.setEditTriggers(QAbstractItemView::DoubleClicked
                                | QAbstractItemView::SelectedClicked);
    tableWidget.setSelectionBehavior(QAbstractItemView::SelectRows);

    QStringList headerLabels;
    headerLabels << "Title" << "Genre" << "Artist" << "Rating";
    tableWidget.setHorizontalHeaderLabels(headerLabels);

    populateTableWidget(&tableWidget);

    tableWidget.resizeColumnsToContents();
    tableWidget.resize(500, 300);
    tableWidget.show();

    return app.exec();
}
//! [5]
