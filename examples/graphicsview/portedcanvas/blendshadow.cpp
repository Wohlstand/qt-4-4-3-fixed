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

#include <qimage.h>
#include <qcolor.h>

static inline int blendComponent( int v, int av, int s, int as )
{
    return as*s + av*v -(av*as*s)/255;  
}
    
static inline QRgb blendShade( QRgb v, QRgb s )
{
    //shadow image is already reduced and blurred
    int as = qAlpha(s); 
    int av = qAlpha(v);
    if ( as == 0 || av == 255 )
	return v;

    int a = as + av -(as*av)/255;
    
    int r = blendComponent( qRed(v),av, qRed(s), as)/a;
    int g = blendComponent( qGreen(v),av, qGreen(s), as)/a;
    int b = blendComponent( qBlue(v),av, qBlue(s), as)/a;

    return qRgba(r,g,b,a);
}

 

int main( int*, char**)
{
    QImage image( "out.png" );
    image.convertDepth( 32 );
    QImage shade( "outshade.png" );
    shade.convertDepth( 32 );
    int dx = 10;
    int dy = 5;
	
    int w = image.width();
    int h = image.height();
	
    QImage img( w+dx, h+dy, 32 );
    img.setAlphaBuffer( TRUE );

    for ( int y = 0; y < h+dy; y++ ) {
	for ( int x = 0; x < w+dx; x++ ) {
	    QRgb sh =  (x<dx||y<dy) ? 0 : shade.pixel( x-dx, y-dy );
	    QRgb pixel = (x<w&y<h) ? image.pixel( x, y ) : 0;
	    img.setPixel( x, y, blendShade( pixel, sh ) ); 
	}
    }
    img.save("blend.png", "PNG" );
}


