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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDomDocument>
#include <QMainWindow>
#include <QModelIndex>

QT_BEGIN_NAMESPACE
class QComboBox;
class QFile;
class QGroupBox;
class QLabel;
class QListWidget;
class QSqlRelationalTableModel;
class QTableView;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QString &artistTable, const QString &albumTable,
               QFile *albumDetails, QWidget *parent = 0);

private slots:
    void about();
    void addAlbum();
    void changeArtist(int row);
    void deleteAlbum();
    void showAlbumDetails(QModelIndex index);
    void showArtistProfile(QModelIndex index);
    void updateHeader(QModelIndex, int, int);

private:
    void adjustHeader();
    QGroupBox *createAlbumGroupBox();
    QGroupBox *createArtistGroupBox();
    QGroupBox *createDetailsGroupBox();
    void createMenuBar();
    void decreaseAlbumCount(QModelIndex artistIndex);
    void getTrackList(QDomNode album);
    QModelIndex indexOfArtist(const QString &artist);
    void readAlbumData();
    void removeAlbumFromDatabase(QModelIndex album);
    void removeAlbumFromFile(int id);
    void showImageLabel();

    QTableView *albumView;
    QComboBox *artistView;
    QListWidget *trackList;

    QLabel *iconLabel;
    QLabel *imageLabel;
    QLabel *profileLabel;
    QLabel *titleLabel;

    QDomDocument albumData;
    QFile *file;
    QSqlRelationalTableModel *model;
};

#endif
