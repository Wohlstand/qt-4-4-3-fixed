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

#include "customproxy.h"
#include "embeddeddialog.h"

#include <QtGui>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(embeddeddialogs);
    QApplication app(argc, argv);

    QGraphicsScene scene;
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            CustomProxy *proxy = new CustomProxy(0, Qt::Window);
            proxy->setWidget(new EmbeddedDialog);

            QRectF rect = proxy->boundingRect();

            proxy->setPos(x * rect.width() * 1.05, y * rect.height() * 1.05);
	    proxy->setCacheMode(QGraphicsItem::DeviceCoordinateCache);

            scene.addItem(proxy);
        }
    }
    scene.setSceneRect(scene.itemsBoundingRect());

    QGraphicsView view(&scene);
    view.scale(0.5, 0.5);
    view.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    view.setBackgroundBrush(QPixmap(":/No-Ones-Laughing-3.jpg"));
    view.setCacheMode(QGraphicsView::CacheBackground);
    view.setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    view.show();
    view.setWindowTitle("Embedded Dialogs Demo");

    return app.exec();
}
