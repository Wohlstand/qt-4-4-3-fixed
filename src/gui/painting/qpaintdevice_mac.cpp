/****************************************************************************
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the QtGui module of the Qt Toolkit.
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

#include "qpaintdevice.h"
#include "qpainter.h"
#include "qwidget.h"
#include "qbitmap.h"
#include "qapplication.h"
#include "qprinter.h"
#include <qdebug.h>
#include <private/qt_mac_p.h>
#include <private/qprintengine_mac_p.h>
#include <private/qpixmap_mac_p.h>

QT_BEGIN_NAMESPACE

/*****************************************************************************
  Internal variables and functions
 *****************************************************************************/


/*****************************************************************************
  External functions
 *****************************************************************************/

extern void qt_painter_removePaintDevice(QPaintDevice *); //qpainter.cpp

/*****************************************************************************
  QPaintDevice member functions
 *****************************************************************************/
QPaintDevice::QPaintDevice()
{
    painters = 0;
}

QPaintDevice::~QPaintDevice()
{
    if(paintingActive())
        qWarning("QPaintDevice: Cannot destroy paint device that is being "
                 "painted, be sure to QPainter::end() painters");
    qt_painter_removePaintDevice(this);
}

int QPaintDevice::metric(PaintDeviceMetric) const
{
    return 0;
}

/*! \internal */
float qt_mac_defaultDpi_x()
{
    // Mac OS X currently assumes things to be 72 dpi.
    // (see http://developer.apple.com/releasenotes/GraphicsImaging/RN-ResolutionIndependentUI/)
    // This may need to be re-worked as we go further in the resolution-independence stuff.
    return 72;
}

/*! \internal */
float qt_mac_defaultDpi_y()
{
    // Mac OS X currently assumes things to be 72 dpi.
    // (see http://developer.apple.com/releasenotes/GraphicsImaging/RN-ResolutionIndependentUI/)
    // This may need to be re-worked as we go further in the resolution-independence stuff.
    return 72;
}


/*! \internal

    Returns the QuickDraw CGrafPtr of the paint device. 0 is returned
    if it can't be obtained. Do not hold the pointer around for long
    as it can be relocated.

    \warning This function is only available on Mac OS X.
*/

Q_GUI_EXPORT GrafPtr qt_mac_qd_context(const QPaintDevice *device)
{
    if (device->devType() == QInternal::Pixmap) {
        return static_cast<GrafPtr>(static_cast<const QPixmap *>(device)->macQDHandle());
    } else if(device->devType() == QInternal::Widget) {
        return static_cast<GrafPtr>(static_cast<const QWidget *>(device)->macQDHandle());
    } else if(device->devType() == QInternal::Printer) {
        QPaintEngine *engine = static_cast<const QPrinter *>(device)->paintEngine();
        return static_cast<GrafPtr>(static_cast<const QMacPrintEngine *>(engine)->handle());
    }
    return 0;
}

extern CGColorSpaceRef qt_mac_colorSpaceForDeviceType(const QPaintDevice *pdev);

/*! \internal

    Returns the CoreGraphics CGContextRef of the paint device. 0 is
    returned if it can't be obtained. It is the caller's responsiblity to
    CGContextRelease the context when finished using it.

    \warning This function is only available on Mac OS X.
*/

Q_GUI_EXPORT CGContextRef qt_mac_cg_context(const QPaintDevice *pdev)
{
    if (pdev->devType() == QInternal::Pixmap) {
        const QPixmap *pm = static_cast<const QPixmap*>(pdev);
        CGColorSpaceRef colorspace = qt_mac_colorSpaceForDeviceType(pdev);
#if (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_4)
        uint flags = kCGImageAlphaPremultipliedFirst;
#ifdef kCGBitmapByteOrder32Host //only needed because CGImage.h added symbols in the minor version
        if(QSysInfo::MacintoshVersion >= QSysInfo::MV_10_4)
            flags |= kCGBitmapByteOrder32Host;
#endif
#else
        CGImageAlphaInfo flags = kCGImageAlphaPremultipliedFirst;
#endif
        const QMacPixmapData *pmData = static_cast<const QMacPixmapData*>(pm->data);
        CGContextRef ret = CGBitmapContextCreate(pmData->pixels, pmData->w, pmData->h,
                                                 8, pmData->bytesPerRow, colorspace,
                                                 flags);
        if(!ret)
            qWarning("QPaintDevice: Unable to create context for pixmap (%d/%d/%d)",
                     pmData->w, pmData->h, (pmData->bytesPerRow * pmData->h));
        CGContextTranslateCTM(ret, 0, pmData->h);
        CGContextScaleCTM(ret, 1, -1);
        return ret;
    } else if (pdev->devType() == QInternal::Widget) {
        CGContextRef ret = static_cast<CGContextRef>(static_cast<const QWidget *>(pdev)->macCGHandle());
        CGContextRetain(ret);
        return ret;
    } else if (pdev->devType() == QInternal::MacQuartz) {
        return static_cast<const QMacQuartzPaintDevice *>(pdev)->cgContext();
    }
    return 0;
}

QT_END_NAMESPACE
