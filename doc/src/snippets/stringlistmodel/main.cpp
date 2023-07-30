/****************************************************************************
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the documentation of the Qt Toolkit.
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

/*!
    The main function for the string list model example. This creates and
    populates a model with values from a string list then displays the
    contents of the model using a QListView widget.
*/

#include <QAbstractItemModel>
#include <QApplication>
#include <QListView>

#include "model.h"

//! [0]
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

// Unindented for quoting purposes:
//! [1]
QStringList numbers;
numbers << "One" << "Two" << "Three" << "Four" << "Five";

QAbstractItemModel *model = new StringListModel(numbers);
//! [0] //! [1] //! [2] //! [3]
QListView *view = new QListView;
//! [2]
view->setWindowTitle("View onto a string list model");
//! [4]
view->setModel(model);
//! [3] //! [4]

    model->insertRows(5, 7, QModelIndex());

    for (int row = 5; row < 12; ++row) {
        QModelIndex index = model->index(row, 0, QModelIndex());
        model->setData(index, QString::number(row+1));
    }

//! [5]
    view->show();
    return app.exec();
}
//! [5]
