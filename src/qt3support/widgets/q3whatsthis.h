/****************************************************************************
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the Qt3Support module of the Qt Toolkit.
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

#ifndef Q3WHATSTHIS_H
#define Q3WHATSTHIS_H

#include <QtGui/qcursor.h>
#include <QtGui/qwhatsthis.h>
#include <QtGui/qwidget.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Qt3SupportLight)

#ifndef QT_NO_WHATSTHIS

class QToolButton;

class Q_COMPAT_EXPORT Q3WhatsThis: public QObject
{
    Q_OBJECT
public:
    Q3WhatsThis(QWidget *);
    ~Q3WhatsThis();
    bool eventFilter(QObject *, QEvent *);

    static inline void enterWhatsThisMode() { QWhatsThis::enterWhatsThisMode(); }
    static inline bool inWhatsThisMode() { return QWhatsThis::inWhatsThisMode(); }

    static inline void add(QWidget *w, const QString &s) { w->setWhatsThis(s); }
    static inline void remove(QWidget *w) { w->setWhatsThis(QString()); }
    static QToolButton * whatsThisButton(QWidget * parent);
    static inline void leaveWhatsThisMode(const QString& text = QString(), const QPoint& pos = QCursor::pos(), QWidget* w = 0)
        { QWhatsThis::showText(pos, text, w); }
    static inline void display(const QString& text, const QPoint& pos = QCursor::pos(), QWidget* w = 0)
        { QWhatsThis::showText(pos, text, w); }

    virtual QString text(const QPoint &);
    virtual bool clicked(const QString& href);

};

#endif // QT_NO_WHATSTHIS

QT_END_NAMESPACE

QT_END_HEADER

#endif // Q3WHATSTHIS_H
