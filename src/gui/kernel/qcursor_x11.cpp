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

#include <qdatastream.h>
#include <private/qcursor_p.h>
#include <private/qt_x11_p.h>
#include <qbitmap.h>
#include <qcursor.h>
#include <X11/cursorfont.h>

#include <qlibrary.h>

#ifndef QT_NO_XCURSOR
#  include <X11/Xcursor/Xcursor.h>
#endif // QT_NO_XCURSOR

#ifndef QT_NO_XFIXES
#  include <X11/extensions/Xfixes.h>
#endif // QT_NO_XFIXES

#include "qx11info_x11.h"

QT_BEGIN_NAMESPACE

// Define QT_USE_APPROXIMATE_CURSORS when compiling if you REALLY want to
// use the ugly X11 cursors.

extern QCursorData *qt_cursorTable[Qt::LastCursor + 1]; // qcursor.cpp

/*****************************************************************************
  Internal QCursorData class
 *****************************************************************************/

QCursorData::QCursorData(Qt::CursorShape s)
    : cshape(s), bm(0), bmm(0), hx(0), hy(0), hcurs(0), pm(0), pmm(0)
{
    ref = 1;
}

QCursorData::~QCursorData()
{
    Display *dpy = X11 ? X11->display : (Display*)0;

    // Add in checking for the display too as on HP-UX
    // we seem to get a core dump as the cursor data is
    // deleted again from main() on exit...
    if (hcurs && dpy)
        XFreeCursor(dpy, hcurs);
    if (pm && dpy)
        XFreePixmap(dpy, pm);
    if (pmm && dpy)
        XFreePixmap(dpy, pmm);
    delete bm;
    delete bmm;
}

QCursor::QCursor(Qt::HANDLE cursor)
{
    if (!QCursorData::initialized)
        QCursorData::initialize();
    d = new QCursorData(Qt::CustomCursor);
    d->hcurs = cursor;
}

QCursorData *QCursorData::setBitmap(const QBitmap &bitmap, const QBitmap &mask, int hotX, int hotY)
{
    if (!QCursorData::initialized)
        QCursorData::initialize();
    if (bitmap.depth() != 1 || mask.depth() != 1 || bitmap.size() != mask.size()) {
        qWarning("QCursor: Cannot create bitmap cursor; invalid bitmap(s)");
        QCursorData *c = qt_cursorTable[0];
        c->ref.ref();
        return c;
    }
    QCursorData *d = new QCursorData;
    d->ref = 1;
    d->bm  = new QBitmap(bitmap);
    d->bmm = new QBitmap(mask);
    d->hcurs = 0;
    d->cshape = Qt::BitmapCursor;
    d->hx = hotX >= 0 ? hotX : bitmap.width() / 2;
    d->hy = hotY >= 0 ? hotY : bitmap.height() / 2;
    d->fg.red   = 0x0000;
    d->fg.green = 0x0000;
    d->fg.blue  = 0x0000;
    d->bg.red   = 0xffff;
    d->bg.green = 0xffff;
    d->bg.blue  = 0xffff;
    return d;
}



Qt::HANDLE QCursor::handle() const
{
    if (!QCursorData::initialized)
        QCursorData::initialize();
    if (!d->hcurs)
        d->update();
    return d->hcurs;
}

QPoint QCursor::pos()
{
    Window root;
    Window child;
    int root_x, root_y, win_x, win_y;
    uint buttons;
    Display* dpy = X11->display;
    for (int i = 0; i < ScreenCount(dpy); ++i) {
        if (XQueryPointer(dpy, QX11Info::appRootWindow(i), &root, &child, &root_x, &root_y,
                          &win_x, &win_y, &buttons))

            return QPoint(root_x, root_y);
    }
    return QPoint();
}

/*! \internal
*/
int QCursor::x11Screen()
{
    Window root;
    Window child;
    int root_x, root_y, win_x, win_y;
    uint buttons;
    Display* dpy = X11->display;
    for (int i = 0; i < ScreenCount(dpy); ++i) {
        if (XQueryPointer(dpy, QX11Info::appRootWindow(i), &root, &child, &root_x, &root_y,
                          &win_x, &win_y, &buttons))
            return i;
    }
    return -1;
}

void QCursor::setPos(int x, int y)
{
    QPoint current, target(x, y);

    // this is copied from pos(), since we need the screen number for the correct
    // root window in the XWarpPointer call
    Window root;
    Window child;
    int root_x, root_y, win_x, win_y;
    uint buttons;
    Display* dpy = X11->display;
    int screen;
    for (screen = 0; screen < ScreenCount(dpy); ++screen) {
        if (XQueryPointer(dpy, QX11Info::appRootWindow(screen), &root, &child, &root_x, &root_y,
                          &win_x, &win_y, &buttons)) {
            current = QPoint(root_x, root_y);
            break;
        }
    }

    if (screen >= ScreenCount(dpy))
        return;

    // Need to check, since some X servers generate null mouse move
    // events, causing looping in applications which call setPos() on
    // every mouse move event.
    //
    if (current == target)
        return;

    XWarpPointer(X11->display, XNone, QX11Info::appRootWindow(screen), 0, 0, 0, 0, x, y);
}


/*!
    \internal

    Creates the cursor.
*/

void QCursorData::update()
{
    if (!QCursorData::initialized)
        QCursorData::initialize();
    if (hcurs)
        return;

    Display *dpy = X11->display;
    Window rootwin = QX11Info::appRootWindow();

    if (cshape == Qt::BitmapCursor) {
#ifndef QT_NO_XRENDER
        if (!pixmap.isNull() && X11->use_xrender) {
            hcurs = XRenderCreateCursor (X11->display, pixmap.x11PictureHandle(), hx, hy);
        } else
#endif
        {
            hcurs = XCreatePixmapCursor(dpy, bm->handle(), bmm->handle(), &fg, &bg, hx, hy);
        }
        return;
    }

    static const char *cursorNames[] = {
        "left_ptr",
        "up_arrow",
        "cross",
        "wait",
        "ibeam",
        "size_ver",
        "size_hor",
        "size_bdiag",
        "size_fdiag",
        "size_all",
        "blank",
        "split_v",
        "split_h",
        "pointing_hand",
        "forbidden",
        "whats_this",
        "left_ptr_watch",
        "openhand",
        "closedhand"
    };

#ifndef QT_NO_XCURSOR
    if (X11->ptrXcursorLibraryLoadCursor)
        hcurs = X11->ptrXcursorLibraryLoadCursor(dpy, cursorNames[cshape]);
    if (hcurs)
        return;
#endif // QT_NO_XCURSOR

    static const char cur_blank_bits[] = {
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00,
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00,
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00 };

    // Non-standard X11 cursors are created from bitmaps

#ifndef QT_USE_APPROXIMATE_CURSORS
    static const char cur_ver_bits[] = {
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x80, (char)0x01, (char)0xc0, (char)0x03, (char)0xe0, (char)0x07, (char)0xf0, (char)0x0f,
        (char)0x80, (char)0x01, (char)0x80, (char)0x01, (char)0x80, (char)0x01, (char)0x80, (char)0x01, (char)0x80, (char)0x01, (char)0xf0, (char)0x0f,
        (char)0xe0, (char)0x07, (char)0xc0, (char)0x03, (char)0x80, (char)0x01, (char)0x00, (char)0x00 };
    static const char mcur_ver_bits[] = {
        (char)0x00, (char)0x00, (char)0x80, (char)0x03, (char)0xc0, (char)0x07, (char)0xe0, (char)0x0f, (char)0xf0, (char)0x1f, (char)0xf8, (char)0x3f,
        (char)0xfc, (char)0x7f, (char)0xc0, (char)0x07, (char)0xc0, (char)0x07, (char)0xc0, (char)0x07, (char)0xfc, (char)0x7f, (char)0xf8, (char)0x3f,
        (char)0xf0, (char)0x1f, (char)0xe0, (char)0x0f, (char)0xc0, (char)0x07, (char)0x80, (char)0x03 };
    static const char cur_hor_bits[] = {
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x20, (char)0x08, (char)0x30, (char)0x18,
        (char)0x38, (char)0x38, (char)0xfc, (char)0x7f, (char)0xfc, (char)0x7f, (char)0x38, (char)0x38, (char)0x30, (char)0x18, (char)0x20, (char)0x08,
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00 };
    static const char mcur_hor_bits[] = {
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x40, (char)0x04, (char)0x60, (char)0x0c, (char)0x70, (char)0x1c, (char)0x78, (char)0x3c,
        (char)0xfc, (char)0x7f, (char)0xfe, (char)0xff, (char)0xfe, (char)0xff, (char)0xfe, (char)0xff, (char)0xfc, (char)0x7f, (char)0x78, (char)0x3c,
        (char)0x70, (char)0x1c, (char)0x60, (char)0x0c, (char)0x40, (char)0x04, (char)0x00, (char)0x00 };
    static const char cur_bdiag_bits[] = {
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x3f, (char)0x00, (char)0x3e, (char)0x00, (char)0x3c, (char)0x00, (char)0x3e,
        (char)0x00, (char)0x37, (char)0x88, (char)0x23, (char)0xd8, (char)0x01, (char)0xf8, (char)0x00, (char)0x78, (char)0x00, (char)0xf8, (char)0x00,
        (char)0xf8, (char)0x01, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00 };
    static const char mcur_bdiag_bits[] = {
        (char)0x00, (char)0x00, (char)0xc0, (char)0x7f, (char)0x80, (char)0x7f, (char)0x00, (char)0x7f, (char)0x00, (char)0x7e, (char)0x04, (char)0x7f,
        (char)0x8c, (char)0x7f, (char)0xdc, (char)0x77, (char)0xfc, (char)0x63, (char)0xfc, (char)0x41, (char)0xfc, (char)0x00, (char)0xfc, (char)0x01,
        (char)0xfc, (char)0x03, (char)0xfc, (char)0x07, (char)0x00, (char)0x00, (char)0x00, (char)0x00 };
    static const char cur_fdiag_bits[] = {
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0xf8, (char)0x01, (char)0xf8, (char)0x00, (char)0x78, (char)0x00,
        (char)0xf8, (char)0x00, (char)0xd8, (char)0x01, (char)0x88, (char)0x23, (char)0x00, (char)0x37, (char)0x00, (char)0x3e, (char)0x00, (char)0x3c,
        (char)0x00, (char)0x3e, (char)0x00, (char)0x3f, (char)0x00, (char)0x00, (char)0x00, (char)0x00 };
    static const char mcur_fdiag_bits[] = {
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0xfc, (char)0x07, (char)0xfc, (char)0x03, (char)0xfc, (char)0x01, (char)0xfc, (char)0x00,
        (char)0xfc, (char)0x41, (char)0xfc, (char)0x63, (char)0xdc, (char)0x77, (char)0x8c, (char)0x7f, (char)0x04, (char)0x7f, (char)0x00, (char)0x7e,
        (char)0x00, (char)0x7f, (char)0x80, (char)0x7f, (char)0xc0, (char)0x7f, (char)0x00, (char)0x00 };
    static const char *cursor_bits16[] = {
        cur_ver_bits, mcur_ver_bits, cur_hor_bits, mcur_hor_bits,
        cur_bdiag_bits, mcur_bdiag_bits, cur_fdiag_bits, mcur_fdiag_bits,
        0, 0, cur_blank_bits, cur_blank_bits };

    static const char vsplit_bits[] = {
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00,
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00,
        (char)0x00, (char)0x80, (char)0x00, (char)0x00, (char)0x00, (char)0xc0, (char)0x01, (char)0x00, (char)0x00, (char)0xe0, (char)0x03, (char)0x00,
        (char)0x00, (char)0x80, (char)0x00, (char)0x00, (char)0x00, (char)0x80, (char)0x00, (char)0x00, (char)0x00, (char)0x80, (char)0x00, (char)0x00,
        (char)0x00, (char)0x80, (char)0x00, (char)0x00, (char)0x00, (char)0x80, (char)0x00, (char)0x00, (char)0x00, (char)0xff, (char)0x7f, (char)0x00,
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0xff, (char)0x7f, (char)0x00,
        (char)0x00, (char)0x80, (char)0x00, (char)0x00, (char)0x00, (char)0x80, (char)0x00, (char)0x00, (char)0x00, (char)0x80, (char)0x00, (char)0x00,
        (char)0x00, (char)0x80, (char)0x00, (char)0x00, (char)0x00, (char)0x80, (char)0x00, (char)0x00, (char)0x00, (char)0xe0, (char)0x03, (char)0x00,
        (char)0x00, (char)0xc0, (char)0x01, (char)0x00, (char)0x00, (char)0x80, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00,
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00,
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00 };
    static const char vsplitm_bits[] = {
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00,
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x80, (char)0x00, (char)0x00,
        (char)0x00, (char)0xc0, (char)0x01, (char)0x00, (char)0x00, (char)0xe0, (char)0x03, (char)0x00, (char)0x00, (char)0xf0, (char)0x07, (char)0x00,
        (char)0x00, (char)0xf8, (char)0x0f, (char)0x00, (char)0x00, (char)0xc0, (char)0x01, (char)0x00, (char)0x00, (char)0xc0, (char)0x01, (char)0x00,
        (char)0x00, (char)0xc0, (char)0x01, (char)0x00, (char)0x80, (char)0xff, (char)0xff, (char)0x00, (char)0x80, (char)0xff, (char)0xff, (char)0x00,
        (char)0x80, (char)0xff, (char)0xff, (char)0x00, (char)0x80, (char)0xff, (char)0xff, (char)0x00, (char)0x80, (char)0xff, (char)0xff, (char)0x00,
        (char)0x80, (char)0xff, (char)0xff, (char)0x00, (char)0x00, (char)0xc0, (char)0x01, (char)0x00, (char)0x00, (char)0xc0, (char)0x01, (char)0x00,
        (char)0x00, (char)0xc0, (char)0x01, (char)0x00, (char)0x00, (char)0xf8, (char)0x0f, (char)0x00, (char)0x00, (char)0xf0, (char)0x07, (char)0x00,
        (char)0x00, (char)0xe0, (char)0x03, (char)0x00, (char)0x00, (char)0xc0, (char)0x01, (char)0x00, (char)0x00, (char)0x80, (char)0x00, (char)0x00,
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00,
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00 };
    static const char hsplit_bits[] = {
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00,
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00,
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x40, (char)0x02, (char)0x00, (char)0x00, (char)0x40, (char)0x02, (char)0x00,
        (char)0x00, (char)0x40, (char)0x02, (char)0x00, (char)0x00, (char)0x40, (char)0x02, (char)0x00, (char)0x00, (char)0x40, (char)0x02, (char)0x00,
        (char)0x00, (char)0x41, (char)0x82, (char)0x00, (char)0x80, (char)0x41, (char)0x82, (char)0x01, (char)0xc0, (char)0x7f, (char)0xfe, (char)0x03,
        (char)0x80, (char)0x41, (char)0x82, (char)0x01, (char)0x00, (char)0x41, (char)0x82, (char)0x00, (char)0x00, (char)0x40, (char)0x02, (char)0x00,
        (char)0x00, (char)0x40, (char)0x02, (char)0x00, (char)0x00, (char)0x40, (char)0x02, (char)0x00, (char)0x00, (char)0x40, (char)0x02, (char)0x00,
        (char)0x00, (char)0x40, (char)0x02, (char)0x00, (char)0x00, (char)0x40, (char)0x02, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00,
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00,
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00,
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00 };
    static const char hsplitm_bits[] = {
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00,
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00,
        (char)0x00, (char)0xe0, (char)0x07, (char)0x00, (char)0x00, (char)0xe0, (char)0x07, (char)0x00, (char)0x00, (char)0xe0, (char)0x07, (char)0x00,
        (char)0x00, (char)0xe0, (char)0x07, (char)0x00, (char)0x00, (char)0xe2, (char)0x47, (char)0x00, (char)0x00, (char)0xe3, (char)0xc7, (char)0x00,
        (char)0x80, (char)0xe3, (char)0xc7, (char)0x01, (char)0xc0, (char)0xff, (char)0xff, (char)0x03, (char)0xe0, (char)0xff, (char)0xff, (char)0x07,
        (char)0xc0, (char)0xff, (char)0xff, (char)0x03, (char)0x80, (char)0xe3, (char)0xc7, (char)0x01, (char)0x00, (char)0xe3, (char)0xc7, (char)0x00,
        (char)0x00, (char)0xe2, (char)0x47, (char)0x00, (char)0x00, (char)0xe0, (char)0x07, (char)0x00, (char)0x00, (char)0xe0, (char)0x07, (char)0x00,
        (char)0x00, (char)0xe0, (char)0x07, (char)0x00, (char)0x00, (char)0xe0, (char)0x07, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00,
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00,
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00,
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00 };
    static const char whatsthis_bits[] = {
        (char)0x01, (char)0x00, (char)0x00, (char)0x00, (char)0x03, (char)0x00, (char)0x00, (char)0x00, (char)0x05, (char)0xf0, (char)0x07, (char)0x00,
        (char)0x09, (char)0x18, (char)0x0e, (char)0x00, (char)0x11, (char)0x1c, (char)0x0e, (char)0x00, (char)0x21, (char)0x1c, (char)0x0e, (char)0x00,
        (char)0x41, (char)0x1c, (char)0x0e, (char)0x00, (char)0x81, (char)0x1c, (char)0x0e, (char)0x00, (char)0x01, (char)0x01, (char)0x07, (char)0x00,
        (char)0x01, (char)0x82, (char)0x03, (char)0x00, (char)0xc1, (char)0xc7, (char)0x01, (char)0x00, (char)0x49, (char)0xc0, (char)0x01, (char)0x00,
        (char)0x95, (char)0xc0, (char)0x01, (char)0x00, (char)0x93, (char)0xc0, (char)0x01, (char)0x00, (char)0x21, (char)0x01, (char)0x00, (char)0x00,
        (char)0x20, (char)0xc1, (char)0x01, (char)0x00, (char)0x40, (char)0xc2, (char)0x01, (char)0x00, (char)0x40, (char)0x02, (char)0x00, (char)0x00,
        (char)0x80, (char)0x01, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00,
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00,
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00,
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00,
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, };
    static const char whatsthism_bits[] = {
        (char)0x01, (char)0x00, (char)0x00, (char)0x00, (char)0x03, (char)0xf0, (char)0x07, (char)0x00, (char)0x07, (char)0xf8, (char)0x0f, (char)0x00,
        (char)0x0f, (char)0xfc, (char)0x1f, (char)0x00, (char)0x1f, (char)0x3e, (char)0x1f, (char)0x00, (char)0x3f, (char)0x3e, (char)0x1f, (char)0x00,
        (char)0x7f, (char)0x3e, (char)0x1f, (char)0x00, (char)0xff, (char)0x3e, (char)0x1f, (char)0x00, (char)0xff, (char)0x9d, (char)0x0f, (char)0x00,
        (char)0xff, (char)0xc3, (char)0x07, (char)0x00, (char)0xff, (char)0xe7, (char)0x03, (char)0x00, (char)0x7f, (char)0xe0, (char)0x03, (char)0x00,
        (char)0xf7, (char)0xe0, (char)0x03, (char)0x00, (char)0xf3, (char)0xe0, (char)0x03, (char)0x00, (char)0xe1, (char)0xe1, (char)0x03, (char)0x00,
        (char)0xe0, (char)0xe1, (char)0x03, (char)0x00, (char)0xc0, (char)0xe3, (char)0x03, (char)0x00, (char)0xc0, (char)0xe3, (char)0x03, (char)0x00,
        (char)0x80, (char)0x01, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00,
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00,
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00,
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00,
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, };
    static const char busy_bits[] = {
        (char)0x01, (char)0x00, (char)0x00, (char)0x00, (char)0x03, (char)0x00, (char)0x00, (char)0x00, (char)0x05, (char)0x00, (char)0x00, (char)0x00,
        (char)0x09, (char)0x00, (char)0x00, (char)0x00, (char)0x11, (char)0x00, (char)0x00, (char)0x00, (char)0x21, (char)0x00, (char)0x00, (char)0x00,
        (char)0x41, (char)0xe0, (char)0xff, (char)0x00, (char)0x81, (char)0x20, (char)0x80, (char)0x00, (char)0x01, (char)0xe1, (char)0xff, (char)0x00,
        (char)0x01, (char)0x42, (char)0x40, (char)0x00, (char)0xc1, (char)0x47, (char)0x40, (char)0x00, (char)0x49, (char)0x40, (char)0x55, (char)0x00,
        (char)0x95, (char)0x80, (char)0x2a, (char)0x00, (char)0x93, (char)0x00, (char)0x15, (char)0x00, (char)0x21, (char)0x01, (char)0x0a, (char)0x00,
        (char)0x20, (char)0x01, (char)0x11, (char)0x00, (char)0x40, (char)0x82, (char)0x20, (char)0x00, (char)0x40, (char)0x42, (char)0x44, (char)0x00,
        (char)0x80, (char)0x41, (char)0x4a, (char)0x00, (char)0x00, (char)0x40, (char)0x55, (char)0x00, (char)0x00, (char)0xe0, (char)0xff, (char)0x00,
        (char)0x00, (char)0x20, (char)0x80, (char)0x00, (char)0x00, (char)0xe0, (char)0xff, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00,
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00,
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00,
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00};
    static const char busym_bits[] = {
        (char)0x01, (char)0x00, (char)0x00, (char)0x00, (char)0x03, (char)0x00, (char)0x00, (char)0x00, (char)0x07, (char)0x00, (char)0x00, (char)0x00,
        (char)0x0f, (char)0x00, (char)0x00, (char)0x00, (char)0x1f, (char)0x00, (char)0x00, (char)0x00, (char)0x3f, (char)0x00, (char)0x00, (char)0x00,
        (char)0x7f, (char)0xe0, (char)0xff, (char)0x00, (char)0xff, (char)0xe0, (char)0xff, (char)0x00, (char)0xff, (char)0xe1, (char)0xff, (char)0x00,
        (char)0xff, (char)0xc3, (char)0x7f, (char)0x00, (char)0xff, (char)0xc7, (char)0x7f, (char)0x00, (char)0x7f, (char)0xc0, (char)0x7f, (char)0x00,
        (char)0xf7, (char)0x80, (char)0x3f, (char)0x00, (char)0xf3, (char)0x00, (char)0x1f, (char)0x00, (char)0xe1, (char)0x01, (char)0x0e, (char)0x00,
        (char)0xe0, (char)0x01, (char)0x1f, (char)0x00, (char)0xc0, (char)0x83, (char)0x3f, (char)0x00, (char)0xc0, (char)0xc3, (char)0x7f, (char)0x00,
        (char)0x80, (char)0xc1, (char)0x7f, (char)0x00, (char)0x00, (char)0xc0, (char)0x7f, (char)0x00, (char)0x00, (char)0xe0, (char)0xff, (char)0x00,
        (char)0x00, (char)0xe0, (char)0xff, (char)0x00, (char)0x00, (char)0xe0, (char)0xff, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00,
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00,
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00,
        (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00};

    static const char * const cursor_bits32[] = {
        vsplit_bits, vsplitm_bits, hsplit_bits, hsplitm_bits,
        0, 0, 0, 0, whatsthis_bits, whatsthism_bits, busy_bits, busym_bits
    };

    static const char forbidden_bits[] = {
        (char)0x00,(char)0x00,(char)0x00,(char)0x80,(char)0x1f,(char)0x00,(char)0xe0,(char)0x7f,(char)0x00,(char)0xf0,(char)0xf0,(char)0x00,(char)0x38,(char)0xc0,(char)0x01,
        (char)0x7c,(char)0x80,(char)0x03,(char)0xec,(char)0x00,(char)0x03,(char)0xce,(char)0x01,(char)0x07,(char)0x86,(char)0x03,(char)0x06,(char)0x06,(char)0x07,(char)0x06,
        (char)0x06,(char)0x0e,(char)0x06,(char)0x06,(char)0x1c,(char)0x06,(char)0x0e,(char)0x38,(char)0x07,(char)0x0c,(char)0x70,(char)0x03,(char)0x1c,(char)0xe0,(char)0x03,
        (char)0x38,(char)0xc0,(char)0x01,(char)0xf0,(char)0xe0,(char)0x00,(char)0xe0,(char)0x7f,(char)0x00,(char)0x80,(char)0x1f,(char)0x00,(char)0x00,(char)0x00,(char)0x00 };

    static const char forbiddenm_bits[] = {
        (char)0x80,(char)0x1f,(char)0x00,(char)0xe0,(char)0x7f,(char)0x00,(char)0xf0,(char)0xff,(char)0x00,(char)0xf8,(char)0xff,(char)0x01,(char)0xfc,(char)0xf0,(char)0x03,
        (char)0xfe,(char)0xc0,(char)0x07,(char)0xfe,(char)0x81,(char)0x07,(char)0xff,(char)0x83,(char)0x0f,(char)0xcf,(char)0x07,(char)0x0f,(char)0x8f,(char)0x0f,(char)0x0f,
        (char)0x0f,(char)0x1f,(char)0x0f,(char)0x0f,(char)0x3e,(char)0x0f,(char)0x1f,(char)0xfc,(char)0x0f,(char)0x1e,(char)0xf8,(char)0x07,(char)0x3e,(char)0xf0,(char)0x07,
        (char)0xfc,(char)0xe0,(char)0x03,(char)0xf8,(char)0xff,(char)0x01,(char)0xf0,(char)0xff,(char)0x00,(char)0xe0,(char)0x7f,(char)0x00,(char)0x80,(char)0x1f,(char)0x00};

    static const char openhand_bits[] = {
        (char)0x80,(char)0x01,(char)0x58,(char)0x0e,(char)0x64,(char)0x12,(char)0x64,(char)0x52,(char)0x48,(char)0xb2,(char)0x48,(char)0x92,
        (char)0x16,(char)0x90,(char)0x19,(char)0x80,(char)0x11,(char)0x40,(char)0x02,(char)0x40,(char)0x04,(char)0x40,(char)0x04,(char)0x20,
        (char)0x08,(char)0x20,(char)0x10,(char)0x10,(char)0x20,(char)0x10,(char)0x00,(char)0x00};
    static const char openhandm_bits[] = {
       (char)0x80,(char)0x01,(char)0xd8,(char)0x0f,(char)0xfc,(char)0x1f,(char)0xfc,(char)0x5f,(char)0xf8,(char)0xff,(char)0xf8,(char)0xff,
       (char)0xf6,(char)0xff,(char)0xff,(char)0xff,(char)0xff,(char)0x7f,(char)0xfe,(char)0x7f,(char)0xfc,(char)0x7f,(char)0xfc,(char)0x3f,
       (char)0xf8,(char)0x3f,(char)0xf0,(char)0x1f,(char)0xe0,(char)0x1f,(char)0x00,(char)0x00};
    static const char closedhand_bits[] = {
        (char)0x00,(char)0x00,(char)0x00,(char)0x00,(char)0x00,(char)0x00,(char)0xb0,(char)0x0d,(char)0x48,(char)0x32,(char)0x08,(char)0x50,
        (char)0x10,(char)0x40,(char)0x18,(char)0x40,(char)0x04,(char)0x40,(char)0x04,(char)0x20,(char)0x08,(char)0x20,(char)0x10,(char)0x10,
        (char)0x20,(char)0x10,(char)0x20,(char)0x10,(char)0x00,(char)0x00,(char)0x00,(char)0x00};
    static const char closedhandm_bits[] = {
        (char)0x00,(char)0x00,(char)0x00,(char)0x00,(char)0x00,(char)0x00,(char)0xb0,(char)0x0d,(char)0xf8,(char)0x3f,(char)0xf8,(char)0x7f,
        (char)0xf0,(char)0x7f,(char)0xf8,(char)0x7f,(char)0xfc,(char)0x7f,(char)0xfc,(char)0x3f,(char)0xf8,(char)0x3f,(char)0xf0,(char)0x1f,
        (char)0xe0,(char)0x1f,(char)0xe0,(char)0x1f,(char)0x00,(char)0x00,(char)0x00,(char)0x00};

    static const char * const cursor_bits20[] = {
        forbidden_bits, forbiddenm_bits
    };

    if ((cshape >= Qt::SizeVerCursor && cshape < Qt::SizeAllCursor)
        || cshape == Qt::BlankCursor) {
        XColor bg, fg;
        bg.red   = 255 << 8;
        bg.green = 255 << 8;
        bg.blue  = 255 << 8;
        fg.red   = 0;
        fg.green = 0;
        fg.blue  = 0;
        int i = (cshape - Qt::SizeVerCursor) * 2;
        pm  = XCreateBitmapFromData(dpy, rootwin, cursor_bits16[i], 16, 16);
        pmm = XCreateBitmapFromData(dpy, rootwin, cursor_bits16[i + 1], 16, 16);
        hcurs = XCreatePixmapCursor(dpy, pm, pmm, &fg, &bg, 8, 8);
    } else if ((cshape >= Qt::SplitVCursor && cshape <= Qt::SplitHCursor)
               || cshape == Qt::WhatsThisCursor || cshape == Qt::BusyCursor) {
        XColor bg, fg;
        bg.red   = 255 << 8;
        bg.green = 255 << 8;
        bg.blue  = 255 << 8;
        fg.red   = 0;
        fg.green = 0;
        fg.blue  = 0;
        int i = (cshape - Qt::SplitVCursor) * 2;
        pm  = XCreateBitmapFromData(dpy, rootwin, cursor_bits32[i], 32, 32);
        pmm = XCreateBitmapFromData(dpy, rootwin, cursor_bits32[i + 1], 32, 32);
        int hs = (cshape == Qt::PointingHandCursor || cshape == Qt::WhatsThisCursor
                  || cshape == Qt::BusyCursor) ? 0 : 16;
        hcurs = XCreatePixmapCursor(dpy, pm, pmm, &fg, &bg, hs, hs);
    } else if (cshape == Qt::ForbiddenCursor) {
        XColor bg, fg;
        bg.red   = 255 << 8;
        bg.green = 255 << 8;
        bg.blue  = 255 << 8;
        fg.red   = 0;
        fg.green = 0;
        fg.blue  = 0;
        int i = (cshape - Qt::ForbiddenCursor) * 2;
        pm  = XCreateBitmapFromData(dpy, rootwin, cursor_bits20[i], 20, 20);
        pmm = XCreateBitmapFromData(dpy, rootwin, cursor_bits20[i + 1], 20, 20);
        hcurs = XCreatePixmapCursor(dpy, pm, pmm, &fg, &bg, 10, 10);
    } else if (cshape == Qt::OpenHandCursor || cshape == Qt::ClosedHandCursor) {
        XColor bg, fg;
        bg.red   = 255 << 8;
        bg.green = 255 << 8;
        bg.blue  = 255 << 8;
        fg.red   = 0;
        fg.green = 0;
        fg.blue  = 0;
        bool open = cshape == Qt::OpenHandCursor;
        pm  = XCreateBitmapFromData(dpy, rootwin, open ? openhand_bits : closedhand_bits, 16, 16);
        pmm = XCreateBitmapFromData(dpy, rootwin, open ? openhandm_bits : closedhandm_bits, 16, 16);
        hcurs = XCreatePixmapCursor(dpy, pm, pmm, &fg, &bg, 8, 8);
    }

    if (hcurs)
    {
#ifndef QT_NO_XFIXES
        if (X11->use_xfixes)
            X11->ptrXFixesSetCursorName(dpy, hcurs, cursorNames[cshape]);
#endif /* ! QT_NO_XFIXES */
        return;
    }

#endif /* ! QT_USE_APPROXIMATE_CURSORS */

    uint sh;
    switch (cshape) {                        // map Q cursor to X cursor
    case Qt::ArrowCursor:
        sh = XC_left_ptr;
        break;
    case Qt::UpArrowCursor:
        sh = XC_center_ptr;
        break;
    case Qt::CrossCursor:
        sh = XC_crosshair;
        break;
    case Qt::WaitCursor:
        sh = XC_watch;
        break;
    case Qt::IBeamCursor:
        sh = XC_xterm;
        break;
    case Qt::SizeAllCursor:
        sh = XC_fleur;
        break;
    case Qt::PointingHandCursor:
        sh = XC_hand2;
        break;
#ifdef QT_USE_APPROXIMATE_CURSORS
    case Qt::SizeBDiagCursor:
        sh = XC_top_right_corner;
        break;
    case Qt::SizeFDiagCursor:
        sh = XC_bottom_right_corner;
        break;
    case Qt::BlankCursor:
        XColor bg, fg;
        bg.red   = 255 << 8;
        bg.green = 255 << 8;
        bg.blue  = 255 << 8;
        fg.red   = 0;
        fg.green = 0;
        fg.blue  = 0;
        pm  = XCreateBitmapFromData(dpy, rootwin, cur_blank_bits, 16, 16);
        pmm = XCreateBitmapFromData(dpy, rootwin, cur_blank_bits, 16, 16);
        hcurs = XCreatePixmapCursor(dpy, pm, pmm, &fg, &bg, 8, 8);
        return;
        break;
    case Qt::SizeVerCursor:
    case Qt::SplitVCursor:
        sh = XC_sb_v_double_arrow;
        break;
    case Qt::SizeHorCursor:
    case Qt::SplitHCursor:
        sh = XC_sb_h_double_arrow;
        break;
    case Qt::WhatsThisCursor:
        sh = XC_question_arrow;
        break;
    case Qt::ForbiddenCursor:
        sh = XC_circle;
        break;
    case Qt::BusyCursor:
        sh = XC_watch;
        break;
#endif /* QT_USE_APPROXIMATE_CURSORS */
    default:
        qWarning("QCursor::update: Invalid cursor shape %d", cshape);
        return;
    }
    hcurs = XCreateFontCursor(dpy, sh);

#ifndef QT_NO_XFIXES
    if (X11->use_xfixes)
        X11->ptrXFixesSetCursorName(dpy, hcurs, cursorNames[cshape]);
#endif /* ! QT_NO_XFIXES */
}

QT_END_NAMESPACE
