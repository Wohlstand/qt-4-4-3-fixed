/****************************************************************************
 * ** * ** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
 * **
 * ** This file is part of the Patternist project on Trolltech Labs.  * **
 * ** Commercial Usage
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
 * **
 * ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 * **
 * ****************************************************************************/

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#ifndef Patternist_ColorOutput_h
#define Patternist_ColorOutput_h

#include <QtCore/QtGlobal>
#include <QtCore/QHash>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

namespace QPatternist
{
    class ColorOutputPrivate;

    class ColorOutput
    {
        enum
        {
            ForegroundShift = 10,
            BackgroundShift = 20,
            SpecialShift    = 20,
            ForegroundMask  = 0x1f << ForegroundShift,
            BackgroundMask  = 0x7 << BackgroundShift
        };

    public:
        enum ColorCodeComponent
        {
            BlackForeground         = 1 << ForegroundShift,
            BlueForeground          = 2 << ForegroundShift,
            GreenForeground         = 3 << ForegroundShift,
            CyanForeground          = 4 << ForegroundShift,
            RedForeground           = 5 << ForegroundShift,
            PurpleForeground        = 6 << ForegroundShift,
            BrownForeground         = 7 << ForegroundShift,
            LightGrayForeground     = 8 << ForegroundShift,
            DarkGrayForeground      = 9 << ForegroundShift,
            LightBlueForeground     = 10 << ForegroundShift,
            LightGreenForeground    = 11 << ForegroundShift,
            LightCyanForeground     = 12 << ForegroundShift,
            LightRedForeground      = 13 << ForegroundShift,
            LightPurpleForeground   = 14 << ForegroundShift,
            YellowForeground        = 15 << ForegroundShift,
            WhiteForeground         = 16 << ForegroundShift,

            BlackBackground         = 1 << BackgroundShift,
            BlueBackground          = 2 << BackgroundShift,
            GreenBackground         = 3 << BackgroundShift,
            CyanBackground          = 4 << BackgroundShift,
            RedBackground           = 5 << BackgroundShift,
            PurpleBackground        = 6 << BackgroundShift,
            BrownBackground         = 7 << BackgroundShift,
            DefaultColor            = 1 << SpecialShift
        };

        typedef QFlags<ColorCodeComponent> ColorCode;
        typedef QHash<int, ColorCode> ColorMapping;

        ColorOutput();
        ~ColorOutput();

        void setColorMapping(const ColorMapping &cMapping);
        ColorMapping colorMapping() const;
        void insertMapping(int colorID, const ColorCode colorCode);

        void writeUncolored(const QString &message);
        void write(const QString &message, int color = -1);
        QString colorify(const QString &message, int color = -1) const;

    private:
        ColorOutputPrivate *d;
        Q_DISABLE_COPY(ColorOutput)
    };
}

Q_DECLARE_OPERATORS_FOR_FLAGS(QPatternist::ColorOutput::ColorCode)

QT_END_NAMESPACE

QT_END_HEADER 

#endif
