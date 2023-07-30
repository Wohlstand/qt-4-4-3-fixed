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

#ifndef QCONTEXT2DCANVAS_H
#define QCONTEXT2DCANVAS_H

#include <qscriptengine.h>
#include <qscriptcontext.h>
#include <qscriptvalue.h>

#include <QWidget>
#include <QTimer>

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QMouseEvent;
QT_END_NAMESPACE
class Context2D;

class QContext2DCanvas : public QWidget
{
    Q_OBJECT
public:
    QContext2DCanvas(QWidget *parent=0);
    ~QContext2DCanvas();

    QScriptEngine *engine();

public slots:
    void setScriptContents(const QString &txt);
    QObject *getContext(const QString &str) const;
    void setInterval(const QScriptValue &func,
                     qreal interval);
    void resize(int w, int h);
signals:
    void error(const QString &error, int lineno);
protected:
    virtual void paintEvent(QPaintEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void keyReleaseEvent(QKeyEvent *e);
    virtual void resizeEvent(QResizeEvent *e);

private:
    QScriptEngine      m_engine;
    QString            m_script;

    Context2D         *m_context;
    QScriptValue       m_self;
    QScriptValue       m_doc;

    QScriptValue       m_intervalFunc;
    QTimer             m_timer;

    bool               m_firstRun;

    QScriptValue       m_keyDownHandler;
    QScriptValue       m_keyUpHandler;
    QScriptValue       m_mouseDownHandler;
    QScriptValue       m_mouseUpHandler;
    QScriptValue       m_mouseMoveHandler;
};

#endif
