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

/*
 * KAsteroids - Copyright (c) Martin R. Jones 1997
 *
 * Part of the KDE project
 */

#include <qpainter.h>
//Added by qt3to4:
#include <QResizeEvent>
#include <Q3Frame>
#include "ledmeter.h"

KALedMeter::KALedMeter( QWidget *parent ) : Q3Frame( parent )
{
    mCRanges.setAutoDelete( TRUE );
    mRange = 100;
    mCount = 20;
    mCurrentCount = 0;
    mValue = 0;
    setMinimumWidth( mCount * 2 + frameWidth() );
}

void KALedMeter::setRange( int r )
{
    mRange = r;
    if ( mRange < 1 )
        mRange = 1;
    setValue( mValue );
    update();
}

void KALedMeter::setCount( int c )
{
    mCount = c;
    if ( mCount < 1 )
        mCount = 1;
    setMinimumWidth( mCount * 2 + frameWidth() );
    calcColorRanges();
    setValue( mValue );
    update();
}

void KALedMeter::setValue( int v )
{
    mValue = v;
    if ( mValue > mRange )
        mValue = mRange;
    else if ( mValue < 0 )
        mValue = 0;
    int c = ( mValue + mRange / mCount - 1 ) * mCount / mRange;
    if ( c != mCurrentCount )
    {
        mCurrentCount = c;
        update();
    }
}

void KALedMeter::addColorRange( int pc, const QColor &c )
{
    ColorRange *cr = new ColorRange;
    cr->mPc = pc;
    cr->mColor = c;
    mCRanges.append( cr );
    calcColorRanges();
}

void KALedMeter::resizeEvent( QResizeEvent *e )
{
    Q3Frame::resizeEvent( e );
    int w = ( width() - frameWidth() - 2 ) / mCount * mCount;
    w += frameWidth() + 2;
    setFrameRect( QRect( 0, 0, w, height() ) );
}

void KALedMeter::drawContents( QPainter *p )
{
    QRect b = contentsRect();

    unsigned cidx = 0;
    int ncol = mCount;
    QColor col = colorGroup().foreground();
   
    if ( !mCRanges.isEmpty() )
    {
        col = mCRanges.at( cidx )->mColor;
        ncol = mCRanges.at( cidx )->mValue;
    }
    p->setBrush( col );
    p->setPen( col );

    int lw = b.width() / mCount;
    int lx = b.left() + 1;
    for ( int i = 0; i < mCurrentCount; i++, lx += lw )
    {
        if ( i > ncol )
        {
            if ( ++cidx < mCRanges.count() )
            {
                col = mCRanges.at( cidx )->mColor;
                ncol = mCRanges.at( cidx )->mValue;
                p->setBrush( col );
                p->setPen( col );
            }
        }

        p->drawRect( lx, b.top() + 1, lw - 1, b.height() - 2 );
    }
}

void KALedMeter::calcColorRanges()
{
    int prev = 0;
    ColorRange *cr;
    for ( cr = mCRanges.first(); cr; cr = mCRanges.next() )
    {
        cr->mValue = prev + cr->mPc * mCount / 100;
        prev = cr->mValue;
    }
}

