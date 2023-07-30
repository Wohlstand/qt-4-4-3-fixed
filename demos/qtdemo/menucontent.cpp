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

#include "menucontent.h"
#include "colors.h"
#include "menumanager.h"
#include "demotextitem.h"
#include "headingitem.h"

MenuContentItem::MenuContentItem(const QDomElement &el, QGraphicsScene *scene, QGraphicsItem *parent)
    : DemoItem(scene, parent)
{
    this->name = el.attribute("name");
    this->heading = 0;
    this->description1 = 0;
    this->description2 = 0;

    if (el.tagName() == "demos")
        this->readmePath = QLibraryInfo::location(QLibraryInfo::DemosPath) + "/README";
    else
        this->readmePath = QLibraryInfo::location(QLibraryInfo::ExamplesPath) + "/" + el.attribute("dirname") + "/README";

}

void MenuContentItem::prepare()
{
    if (!this->prepared){
        this->prepared= true;
        this->createContent();
    }
}

void MenuContentItem::animationStopped(int id)
{
    if (this->name == Colors::rootMenuName)
        return; // Optimization hack.

    if (id == DemoItemAnimation::ANIM_OUT){
        // Free up some memory:
        delete this->heading;
        delete this->description1;
        delete this->description2;
        this->heading = 0;
        this->description1 = 0;
        this->description2 = 0;
        this->prepared = false;
    }
}

QString MenuContentItem::loadDescription(int startPara, int nrPara)
{
    QString result;
    QFile readme(this->readmePath);
    if (!readme.open(QFile::ReadOnly)){
        if (Colors::verbose)
            qDebug() << "- MenuContentItem::loadDescription: Could not load:" << this->readmePath;
        return "";
    }

    QTextStream in(&readme);
    // Skip a certain number of paragraphs:
    while (startPara)
        if (in.readLine().isEmpty()) --startPara;

    // Read in the number of wanted paragraphs:
    QString line = in.readLine();
    do {
        result += line + " ";
        line = in.readLine();
        if (line.isEmpty()){
            --nrPara;
            line = "<br><br>" + in.readLine();
        }
    } while (nrPara && !in.atEnd());

    return Colors::contentColor + result;
}

void MenuContentItem::createContent()
{
    // Create the items:
    this->heading = new HeadingItem(this->name, this->scene(), this);
    QString para1 = this->loadDescription(0, 1);
    if (para1.isEmpty())
        para1 = Colors::contentColor + QLatin1String("Could not load description. Ensure that the documentation for Qt is built.");
    QColor bgcolor = Colors::sceneBg1.darker(200);
    bgcolor.setAlpha(100);
    this->description1 = new DemoTextItem(para1, Colors::contentFont(), Colors::heading, 500, this->scene(), this, DemoTextItem::STATIC_TEXT);
    this->description2 = new DemoTextItem(this->loadDescription(1, 2), Colors::contentFont(), Colors::heading, 250, this->scene(), this, DemoTextItem::STATIC_TEXT);

    // Place the items on screen:
    this->heading->setPos(0, 3);
    this->description1->setPos(0, this->heading->pos().y() + this->heading->boundingRect().height() + 10);
    this->description2->setPos(0, this->description1->pos().y() + this->description1->boundingRect().height() + 15);
}

QRectF MenuContentItem::boundingRect() const
{
    return QRectF(0, 0, 500, 350);
}


