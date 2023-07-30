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

#ifndef SCORE_H
#define SCORE_H

#include <QList>
#include <QHash>
#include "demoitemanimation.h"

typedef QList<DemoItemAnimation *> Movie;
typedef QHash<QString, Movie*> MovieIndex;

class PlayListMember
{
public:
    PlayListMember(Movie *movie, int runMode) : movie(movie), runMode(runMode){};
    Movie *movie;
    int runMode;
};
typedef QList<PlayListMember> PlayList;

class Score
{
public:
    enum LOCK_MODE {LOCK_ITEMS, UNLOCK_ITEMS, SKIP_LOCK};
    enum RUN_MODE {FROM_CURRENT, FROM_START, NEW_ANIMATION_ONLY, ONLY_IF_VISIBLE};

    Score();
    virtual ~Score();

    void playMovie(const QString &indexName, RUN_MODE runMode = FROM_START, LOCK_MODE lockMode = SKIP_LOCK);
    void insertMovie(const QString &indexName, Movie *movie);
    Movie *insertMovie(const QString &indexName);
    void queueMovie(const QString &indexName, RUN_MODE runMode = FROM_START, LOCK_MODE lockMode = SKIP_LOCK);
    void playQue();
    bool hasQueuedMovies(){ return this->playList.size() > 0; };

    MovieIndex index;
    PlayList playList;

private:
    void prepare(Movie *movie, RUN_MODE runMode, LOCK_MODE lockMode);
    void play(Movie *movie, RUN_MODE runMode);
};

#endif // SCORE_H

