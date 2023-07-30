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

#include "score.h"
#include "colors.h"
#include "demoitem.h"

Score::Score()
{
}

Score::~Score()
{
    // NB! Deleting all movies.
    qDeleteAll(this->index);
}

void Score::prepare(Movie *movie, RUN_MODE runMode, LOCK_MODE lockMode)
{
    if (lockMode == LOCK_ITEMS){
        for (int i=0; i<movie->size(); ++i){
            if (runMode == ONLY_IF_VISIBLE && !movie->at(i)->demoItem()->isVisible())
                continue;
            movie->at(i)->lockItem(true);
            movie->at(i)->prepare();
        }
    }
    else if (lockMode == UNLOCK_ITEMS){
        for (int i=0; i<movie->size(); ++i){
            if (runMode == ONLY_IF_VISIBLE && !movie->at(i)->demoItem()->isVisible())
                continue;
            movie->at(i)->lockItem(false);
            movie->at(i)->prepare();
        }
    }
    else {
        for (int i=0; i<movie->size(); ++i){
            if (runMode == ONLY_IF_VISIBLE && !movie->at(i)->demoItem()->isVisible())
                continue;
            movie->at(i)->prepare();
        }
    }
}

void Score::play(Movie *movie, RUN_MODE runMode)
{
    if (runMode == NEW_ANIMATION_ONLY){
        for (int i=0; i<movie->size(); ++i)
            if (movie->at(i)->notOwnerOfItem())
                movie->at(i)->play(true);
    }
    else if (runMode == ONLY_IF_VISIBLE){
        for (int i=0; i<movie->size(); ++i)
            if (movie->at(i)->demoItem()->isVisible())
                movie->at(i)->play(runMode == FROM_START);
    }
    else {
        for (int i=0; i<movie->size(); ++i)
            movie->at(i)->play(runMode == FROM_START);
    }
}

void Score::playMovie(const QString &indexName, RUN_MODE runMode, LOCK_MODE lockMode)
{
    MovieIndex::iterator movieIterator = this->index.find(indexName);
    if (movieIterator == this->index.end())
        return;

    Movie *movie = *movieIterator;
    this->prepare(movie, runMode, lockMode);
    this->play(movie, runMode);
}

void Score::queueMovie(const QString &indexName, RUN_MODE runMode, LOCK_MODE lockMode)
{
    MovieIndex::iterator movieIterator = this->index.find(indexName);
    if (movieIterator == this->index.end()){
        if (Colors::verbose)
            qDebug() << "Queuing movie:" << indexName << "(does not exist)";
        return;
    }

    Movie *movie = *movieIterator;
    this->prepare(movie, runMode, lockMode);
    this->playList.append(PlayListMember(movie, int(runMode)));
    if (Colors::verbose)
        qDebug() << "Queuing movie:" << indexName;
}

void Score::playQue()
{
    int movieCount = this->playList.size();
    for (int i=0; i<movieCount; i++)
        this->play(this->playList.at(i).movie, RUN_MODE(this->playList.at(i).runMode));
    this->playList.clear();
    if (Colors::verbose)
        qDebug() << "********* Playing que *********";
}

void Score::insertMovie(const QString &indexName, Movie *movie)
{
    this->index.insert(indexName, movie);
}

Movie *Score::insertMovie(const QString &indexName)
{
    Movie *movie = new Movie();
    insertMovie(indexName, movie);
    return movie;
}

