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

#include "dialog.h"

int uniqueAlbumId;
int uniqueArtistId;

Dialog::Dialog(QSqlRelationalTableModel *albums, QDomDocument details,
               QFile *output, QWidget *parent)
     : QDialog(parent)
{
    model = albums;
    albumDetails = details;
    outputFile = output;

    QGroupBox *inputWidgetBox = createInputWidgets();
    QDialogButtonBox *buttonBox = createButtons();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(inputWidgetBox);
    layout->addWidget(buttonBox);
    setLayout(layout);

    setWindowTitle(tr("Add Album"));
}

void Dialog::submit()
{
    QString artist = artistEditor->text();
    QString title = titleEditor->text();

    if (artist.isEmpty() || title.isEmpty()) {
        QString message(tr("Please provide both the name of the artist "
                           "and the title of the album."));
        QMessageBox::information(this, tr("Add Album"), message);
    } else {
        int artistId = findArtistId(artist);
        int albumId = addNewAlbum(title, artistId);

        QStringList tracks;
        tracks = tracksEditor->text().split(",", QString::SkipEmptyParts);
        addTracks(albumId, tracks);

        increaseAlbumCount(indexOfArtist(artist));
        accept();
    }
}

int Dialog::findArtistId(const QString &artist)
{
    QSqlTableModel *artistModel = model->relationModel(2);
    int row = 0;

    while (row < artistModel->rowCount()) {
        QSqlRecord record = artistModel->record(row);
        if (record.value("artist") == artist)
            return record.value("id").toInt();
        else
            row++;
    }
    return addNewArtist(artist);
}


int Dialog::addNewArtist(const QString &name)
{
    QSqlTableModel *artistModel = model->relationModel(2);
    QSqlRecord record;

    int id = generateArtistId();

    QSqlField f1("id", QVariant::Int);
    QSqlField f2("artist", QVariant::String);
    QSqlField f3("albumcount", QVariant::Int);

    f1.setValue(QVariant(id));
    f2.setValue(QVariant(name));
    f3.setValue(QVariant(0));
    record.append(f1);
    record.append(f2);
    record.append(f3);

    artistModel->insertRecord(-1, record);
    return id;
}

int Dialog::addNewAlbum(const QString &title, int artistId)
{
    int id = generateAlbumId();
    QSqlRecord record;

    QSqlField f1("albumid", QVariant::Int);
    QSqlField f2("title", QVariant::String);
    QSqlField f3("artistid", QVariant::Int);
    QSqlField f4("year", QVariant::Int);

    f1.setValue(QVariant(id));
    f2.setValue(QVariant(title));
    f3.setValue(QVariant(artistId));
    f4.setValue(QVariant(yearEditor->value()));
    record.append(f1);
    record.append(f2);
    record.append(f3);
    record.append(f4);

    model->insertRecord(-1, record);
    return id;
}

void Dialog::addTracks(int albumId, QStringList tracks)
{
    QDomElement albumNode = albumDetails.createElement("album");
    albumNode.setAttribute("id", albumId);

    for (int i = 0; i < tracks.count(); i++) {
        QString trackNumber = QString::number(i);
        if (i < 10)
            trackNumber.prepend("0");

        QDomText textNode = albumDetails.createTextNode(tracks.at(i));

        QDomElement trackNode = albumDetails.createElement("track");
        trackNode.setAttribute("number", trackNumber);
        trackNode.appendChild(textNode);

        albumNode.appendChild(trackNode);
    }

    QDomNodeList archive = albumDetails.elementsByTagName("archive");
    archive.item(0).appendChild(albumNode);

/*
    The following code is commented out since the example uses an in
    memory database, i.e., altering the XML file will bring the data
    out of sync.

    if (!outputFile->open(QIODevice::WriteOnly)) {
        return;
    } else {
        QTextStream stream(outputFile);
        archive.item(0).save(stream, 4);
        outputFile->close();
    }
*/
}

void Dialog::increaseAlbumCount(QModelIndex artistIndex)
{
    QSqlTableModel *artistModel = model->relationModel(2);

    QModelIndex albumCountIndex;
    albumCountIndex = artistIndex.sibling(artistIndex.row(), 2);

    int albumCount = albumCountIndex.data().toInt();
    artistModel->setData(albumCountIndex, QVariant(albumCount + 1));
}


void Dialog::revert()
{
    artistEditor->clear();
    titleEditor->clear();
    yearEditor->setValue(QDate::currentDate().year());
    tracksEditor->clear();
}

QGroupBox *Dialog::createInputWidgets()
{
    QGroupBox *box = new QGroupBox(tr("Add Album"));

    QLabel *artistLabel = new QLabel(tr("Artist:"));
    QLabel *titleLabel = new QLabel(tr("Title:"));
    QLabel *yearLabel = new QLabel(tr("Year:"));
    QLabel *tracksLabel = new QLabel(tr("Tracks (separated by comma):"));

    artistEditor = new QLineEdit;
    titleEditor = new QLineEdit;

    yearEditor = new QSpinBox;
    yearEditor->setMinimum(1900);
    yearEditor->setMaximum(QDate::currentDate().year());
    yearEditor->setValue(yearEditor->maximum());
    yearEditor->setReadOnly(false);

    tracksEditor = new QLineEdit;

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(artistLabel, 0, 0);
    layout->addWidget(artistEditor, 0, 1);
    layout->addWidget(titleLabel, 1, 0);
    layout->addWidget(titleEditor, 1, 1);
    layout->addWidget(yearLabel, 2, 0);
    layout->addWidget(yearEditor, 2, 1);
    layout->addWidget(tracksLabel, 3, 0, 1, 2);
    layout->addWidget(tracksEditor, 4, 0, 1, 2);
    box->setLayout(layout);

    return box;
}

QDialogButtonBox *Dialog::createButtons()
{
    QPushButton *closeButton = new QPushButton(tr("&Close"));
    QPushButton *revertButton = new QPushButton(tr("&Revert"));
    QPushButton *submitButton = new QPushButton(tr("&Submit"));

    closeButton->setDefault(true);

    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(revertButton, SIGNAL(clicked()), this, SLOT(revert()));
    connect(submitButton, SIGNAL(clicked()), this, SLOT(submit()));

    QDialogButtonBox *buttonBox = new QDialogButtonBox;
    buttonBox->addButton(submitButton, QDialogButtonBox::ResetRole);
    buttonBox->addButton(revertButton, QDialogButtonBox::ResetRole);
    buttonBox->addButton(closeButton, QDialogButtonBox::RejectRole);

    return buttonBox;
}

QModelIndex Dialog::indexOfArtist(const QString &artist)
{
    QSqlTableModel *artistModel = model->relationModel(2);

    for (int i = 0; i < artistModel->rowCount(); i++) {
        QSqlRecord record =  artistModel->record(i);
        if (record.value("artist") == artist)
            return artistModel->index(i, 1);
    }

    return QModelIndex();
}

int Dialog::generateArtistId()
{
    uniqueArtistId += 1;
    return uniqueArtistId;
}

int Dialog::generateAlbumId()
{
    uniqueAlbumId += 1;
    return uniqueAlbumId;
}
