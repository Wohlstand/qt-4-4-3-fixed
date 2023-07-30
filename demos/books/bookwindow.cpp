/****************************************************************************
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the demonstration applications of the Qt Toolkit.
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

#include "bookwindow.h"
#include "bookdelegate.h"
#include "initdb.h"

#include <QtSql>

BookWindow::BookWindow()
{
    ui.setupUi(this);

    if (!QSqlDatabase::drivers().contains("QSQLITE"))
        QMessageBox::critical(this, "Unable to load database", "This demo needs the SQLITE driver");

    // initialize the database
    QSqlError err = initDb();
    if (err.type() != QSqlError::NoError) {
        showError(err);
        return;
    }

    // Create the data model
    model = new QSqlRelationalTableModel(ui.bookTable);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setTable("books");

    // Remeber the indexes of the columns
    authorIdx = model->fieldIndex("author");
    genreIdx = model->fieldIndex("genre");

    // Set the relations to the other database tables
    model->setRelation(authorIdx, QSqlRelation("authors", "id", "name"));
    model->setRelation(genreIdx, QSqlRelation("genres", "id", "name"));

    // Set the localized header captions
    model->setHeaderData(authorIdx, Qt::Horizontal, tr("Author Name"));
    model->setHeaderData(genreIdx, Qt::Horizontal, tr("Genre"));
    model->setHeaderData(model->fieldIndex("title"), Qt::Horizontal, tr("Title"));
    model->setHeaderData(model->fieldIndex("year"), Qt::Horizontal, tr("Year"));
    model->setHeaderData(model->fieldIndex("rating"), Qt::Horizontal, tr("Rating"));

    // Populate the model
    if (!model->select()) {
        showError(model->lastError());
        return;
    }

    // Set the model and hide the ID column
    ui.bookTable->setModel(model);
    ui.bookTable->setItemDelegate(new BookDelegate(ui.bookTable));
    ui.bookTable->setColumnHidden(model->fieldIndex("id"), true);
    ui.bookTable->setSelectionMode(QAbstractItemView::SingleSelection);

    // Initialize the Author combo box
    ui.authorEdit->setModel(model->relationModel(authorIdx));
    ui.authorEdit->setModelColumn(model->relationModel(authorIdx)->fieldIndex("name"));

    ui.genreEdit->setModel(model->relationModel(genreIdx));
    ui.genreEdit->setModelColumn(model->relationModel(genreIdx)->fieldIndex("name"));

    QDataWidgetMapper *mapper = new QDataWidgetMapper(this);
    mapper->setModel(model);
    mapper->setItemDelegate(new BookDelegate(this));
    mapper->addMapping(ui.titleEdit, model->fieldIndex("title"));
    mapper->addMapping(ui.yearEdit, model->fieldIndex("year"));
    mapper->addMapping(ui.authorEdit, authorIdx);
    mapper->addMapping(ui.genreEdit, genreIdx);
    mapper->addMapping(ui.ratingEdit, model->fieldIndex("rating"));

    connect(ui.bookTable->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            mapper, SLOT(setCurrentModelIndex(QModelIndex)));

    ui.bookTable->setCurrentIndex(model->index(0, 0));
}

void BookWindow::showError(const QSqlError &err)
{
    QMessageBox::critical(this, "Unable to initialize Database",
                "Error initializing database: " + err.text());
}

