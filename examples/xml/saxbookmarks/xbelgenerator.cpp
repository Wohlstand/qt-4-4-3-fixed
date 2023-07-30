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

#include "xbelgenerator.h"

XbelGenerator::XbelGenerator(QTreeWidget *treeWidget)
    : treeWidget(treeWidget)
{
}

bool XbelGenerator::write(QIODevice *device)
{
    out.setDevice(device);
    out.setCodec("UTF-8");
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        << "<!DOCTYPE xbel>\n"
        << "<xbel version=\"1.0\">\n";

    for (int i = 0; i < treeWidget->topLevelItemCount(); ++i)
        generateItem(treeWidget->topLevelItem(i), 1);

    out << "</xbel>\n";
    return true;
}

QString XbelGenerator::indent(int depth)
{
    const int IndentSize = 4;
    return QString(IndentSize * depth, ' ');
}

QString XbelGenerator::escapedText(const QString &str)
{
    QString result = str;
    result.replace("&", "&amp;");
    result.replace("<", "&lt;");
    result.replace(">", "&gt;");
    return result;
}

QString XbelGenerator::escapedAttribute(const QString &str)
{
    QString result = escapedText(str);
    result.replace("\"", "&quot;");
    result.prepend("\"");
    result.append("\"");
    return result;
}

void XbelGenerator::generateItem(QTreeWidgetItem *item, int depth)
{
    QString tagName = item->data(0, Qt::UserRole).toString();
    if (tagName == "folder") {
        bool folded = !treeWidget->isItemExpanded(item);
        out << indent(depth) << "<folder folded=\"" << (folded ? "yes" : "no")
                             << "\">\n"
            << indent(depth + 1) << "<title>" << escapedText(item->text(0))
                                 << "</title>\n";

        for (int i = 0; i < item->childCount(); ++i)
            generateItem(item->child(i), depth + 1);

        out << indent(depth) << "</folder>\n";
    } else if (tagName == "bookmark") {
        out << indent(depth) << "<bookmark";
        if (!item->text(1).isEmpty())
            out << " href=" << escapedAttribute(item->text(1));
        out << ">\n"
            << indent(depth + 1) << "<title>" << escapedText(item->text(0))
                                 << "</title>\n"
            << indent(depth) << "</bookmark>\n";
    } else if (tagName == "separator") {
        out << indent(depth) << "<separator/>\n";
    }
}
