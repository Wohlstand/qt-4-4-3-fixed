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

#ifndef COLORS_H
#define COLORS_H

#include <QtGui>
#include <QBrush>

class Colors
{
private:
    Colors(){};

public:
    static void parseArgs(int argc, char *argv[]);
    static void detectSystemResources();
    static void postConfigure();
    static void setLowSettings();

    // Colors:
    static QColor sceneBg1;
    static QColor sceneBg2;
    static QColor sceneBg1Line;
    static QColor paperBg;
    static QColor menuTextFg;
    static QColor buttonText;
    static QColor buttonBgLow;
    static QColor buttonBgHigh;
    static QColor tt_green;
    static QColor fadeOut;
    static QColor sceneLine;
    static QColor heading;
    static QString contentColor;
    static QString glVersion;

    // Guides:
    static int stageStartY;
    static int stageHeight;
    static int stageStartX;
    static int stageWidth;
    static int contentStartY;
    static int contentHeight;

    // properties:
    static bool openGlRendering;
    static bool direct3dRendering;
    static bool softwareRendering;
    static bool openGlAwailable;
    static bool direct3dAwailable;
    static bool xRenderPresent;
    static bool noAdapt;
    static bool noTicker;
    static bool noRescale;
    static bool noAnimations;
    static bool noBlending;
    static bool noScreenSync;
    static bool useLoop;
    static bool noWindowMask;
    static bool usePixmaps;
    static bool useEightBitPalette;
    static bool fullscreen;
    static bool showBoundingRect;
    static bool showFps;
    static bool noTimerUpdate;
    static bool noTickerMorph;
    static bool useButtonBalls;
    static bool adapted;
    static bool verbose;
    static bool pause;

    static float animSpeed;
    static float animSpeedButtons;
    static float benchmarkFps;
    static int tickerLetterCount;
    static int fps;
    static int menuCount;
    static float tickerMoveSpeed;
    static float tickerMorphSpeed;
    static QString tickerText;
    static QString rootMenuName;

    // fonts
    static QFont contentFont();
    static QFont headingFont();
    static QFont buttonFont();
    static QFont tickerFont();

};

#endif // COLORS_H

