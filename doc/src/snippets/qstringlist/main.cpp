/****************************************************************************
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the documentation of the Qt Toolkit.
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
#include <iostream>
using namespace std;

class Widget : public QWidget
{
public:
    Widget(QWidget *parent = 0);
};

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
//! [0]
    QStringList fonts;
    fonts << "Arial" << "Helvetica" << "Times" << "Courier";
//! [0]

//! [1]
    for (int i = 0; i < fonts.size(); ++i)
         cout << fonts.at(i).toLocal8Bit().constData() << endl;
//! [1]

//! [2]
    QStringListIterator javaStyleIterator(fonts);
    while (javaStyleIterator.hasNext())
         cout << javaStyleIterator.next().toLocal8Bit().constData() << endl;
//! [2]

//! [3]
    QStringList::const_iterator constIterator;
    for (constIterator = fonts.constBegin(); constIterator != fonts.constEnd();
           ++constIterator)
        cout << (*constIterator).toLocal8Bit().constData() << endl;
//! [3]

//! [4]
    QString str = fonts.join(",");
     // str == "Arial,Helvetica,Times,Courier"
//! [4]

//! [5] //! [6]
    QStringList list;
//! [5]
    list = str.split(",");
     // list: ["Arial", "Helvetica", "Times", "Courier"]
//! [6]

//! [7]
    QStringList monospacedFonts = fonts.filter(QRegExp("Courier|Fixed"));
//! [7]

//! [8]
    QStringList files;
    files << "$QTDIR/src/moc/moc.y"
          << "$QTDIR/src/moc/moc.l"
          << "$QTDIR/include/qconfig.h";

    files.replaceInStrings("$QTDIR", "/usr/lib/qt");
    // files: [ "/usr/lib/qt/src/moc/moc.y", ...]
//! [8]

    QString str1, str2, str3;
//! [9]
    QStringList longerList = (QStringList() << str1 << str2 << str3);
//! [9]

    list.clear();
//! [10]
    list << "Bill Murray" << "John Doe" << "Bill Clinton";

//! [11]
    QStringList result;
//! [11]
    result = list.filter("Bill");
    // result: ["Bill Murray", "Bill Clinton"]
//! [10]

    result.clear();
//! [12]
    foreach (QString str, list) {
        if (str.contains("Bill"))
            result += str;
    }
//! [12]

    list.clear();
//! [13]
    list << "alpha" << "beta" << "gamma" << "epsilon";
    list.replaceInStrings("a", "o");
    // list == ["olpho", "beto", "gommo", "epsilon"]
//! [13]

    list.clear();
//! [14]
    list << "alpha" << "beta" << "gamma" << "epsilon";
    list.replaceInStrings(QRegExp("^a"), "o");
    // list == ["olpha", "beta", "gamma", "epsilon"]
//! [14]

    list.clear();
//! [15]
    list << "Bill Clinton" << "Murray, Bill";
    list.replaceInStrings(QRegExp("^(.*), (.*)$"), "\\2 \\1");
    // list == ["Bill Clinton", "Bill Murray"]
//! [15]
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Widget widget;
    widget.show();
    return app.exec();
}
