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

#include <QtGui>

#include <math.h>
#include <stdlib.h>

#include "extrafiltersplugin.h"

QStringList ExtraFiltersPlugin::filters() const
{
    return QStringList() << tr("Flip Horizontally") << tr("Flip Vertically")
                         << tr("Smudge...") << tr("Threshold...");
}

QImage ExtraFiltersPlugin::filterImage(const QString &filter,
                                       const QImage &image, QWidget *parent)
{
    QImage original = image.convertToFormat(QImage::Format_RGB32);
    QImage result = original;

    if (filter == tr("Flip Horizontally")) {
        for (int y = 0; y < original.height(); ++y) {
            for (int x = 0; x < original.width(); ++x) {
                int pixel = original.pixel(original.width() - x - 1, y);
                result.setPixel(x, y, pixel);
            }
        }
    } else if (filter == tr("Flip Vertically")) {
        for (int y = 0; y < original.height(); ++y) {
            for (int x = 0; x < original.width(); ++x) {
                int pixel = original.pixel(x, original.height() - y - 1);
                result.setPixel(x, y, pixel);
            }
        }
    } else if (filter == tr("Smudge...")) {
        bool ok;
        int numIters = QInputDialog::getInteger(parent, tr("Smudge Filter"),
                                    tr("Enter number of iterations:"),
                                    5, 1, 20, 1, &ok);
        if (ok) {
            for (int i = 0; i < numIters; ++i) {
                for (int y = 1; y < original.height() - 1; ++y) {
                    for (int x = 1; x < original.width() - 1; ++x) {
                        int p1 = original.pixel(x, y);
                        int p2 = original.pixel(x, y + 1);
                        int p3 = original.pixel(x, y - 1);
                        int p4 = original.pixel(x + 1, y);
                        int p5 = original.pixel(x - 1, y);

                        int red = (qRed(p1) + qRed(p2) + qRed(p3) + qRed(p4)
                                   + qRed(p5)) / 5;
                        int green = (qGreen(p1) + qGreen(p2) + qGreen(p3)
                                     + qGreen(p4) + qGreen(p5)) / 5;
                        int blue = (qBlue(p1) + qBlue(p2) + qBlue(p3)
                                    + qBlue(p4) + qBlue(p5)) / 5;
                        int alpha = (qAlpha(p1) + qAlpha(p2) + qAlpha(p3)
                                     + qAlpha(p4) + qAlpha(p5)) / 5;

                        result.setPixel(x, y, qRgba(red, green, blue, alpha));
                    }
                }
            }
        }
    } else if (filter == tr("Threshold...")) {
        bool ok;
        int threshold = QInputDialog::getInteger(parent, tr("Threshold Filter"),
                                                 tr("Enter threshold:"),
                                                 10, 1, 256, 1, &ok);
        if (ok) {
            int factor = 256 / threshold;
            for (int y = 0; y < original.height(); ++y) {
                for (int x = 0; x < original.width(); ++x) {
                    int pixel = original.pixel(x, y);
                    result.setPixel(x, y, qRgba(qRed(pixel) / factor * factor,
                                                qGreen(pixel) / factor * factor,
                                                qBlue(pixel) / factor * factor,
                                                qAlpha(pixel)));
                }
            }
        }
    }
    return result;
}

Q_EXPORT_PLUGIN2(pnp_extrafilters, ExtraFiltersPlugin)
