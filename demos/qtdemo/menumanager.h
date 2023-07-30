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

#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include <QtGui>
#include <QtXml>
#include <QtHelp/QHelpEngineCore>

#include "score.h"
#include "textbutton.h"
#include "mainwindow.h"
#include "itemcircleanimation.h"

typedef QHash<QString, QString> StringHash;
typedef QHash<QString, StringHash> HashHash;

class TextButton;

class MenuManager : public QObject
{
    Q_OBJECT

public:
    enum BUTTON_TYPE {ROOT, MENU1, MENU2, LAUNCH, DOCUMENTATION, QUIT, FULLSCREEN, UP, DOWN, BACK};

    // singleton pattern:
    static MenuManager *instance();
    virtual ~MenuManager();

    void init(MainWindow *window);
    void itemSelected(int userCode, const QString &menuName = "");

    QByteArray getHtml(const QString &name);
    QByteArray getImage(const QString &name);
    QString resolveExeFile(const QString &name);
    QString resolveDocUrl(const QString &name);
    QString resolveImageUrl(const QString &name);
    QString resolveDataDir(const QString &name);

    HashHash info;
    ItemCircleAnimation *ticker;
    MainWindow *window;
    Score *score;
    int currentMenuCode;

private slots:
    void exampleFinished();
    void exampleError(QProcess::ProcessError error);

private:
    // singleton pattern:
    MenuManager();
    static MenuManager *pInstance;

    QByteArray getResource(const QString &name);

    void readXmlDocument();
    void initHelpEngine();
    void getDocumentationDir();
    void readInfoAboutExample(const QDomElement &example);
    void showDocInAssistant(const QString &docFile);
    void launchExample(const QString &uniqueName);

    void createMenu(const QDomElement &category, BUTTON_TYPE type);
    void createLowLeftButton(const QString &label, BUTTON_TYPE type,
        Movie *movieIn, Movie *movieOut, Movie *movieShake, const QString &menuString = QString());
    void createLowRightButton(const QString &label, BUTTON_TYPE type, Movie *movieIn, Movie *movieOut, Movie *movieShake);
    void createLowRightLeafButton(const QString &label, int pos, BUTTON_TYPE type, Movie *movieIn, Movie *movieOut, Movie * /*movieShake*/);
    void createRootMenu(const QDomElement &el);
    void createSubMenu(const QDomElement &el);
    void createLeafMenu(const QDomElement &el);
    void createInfo(DemoItem *item, const QString &name);
    void createTicker();
    void createUpnDownButtons();
    void createBackButton();

    QDomDocument *contentsDoc;
    QProcess assistantProcess;
    QString currentMenu;
    QString currentCategory;
    QString currentMenuButtons;
    QString currentInfo;
    QString helpRootUrl;
    DemoItemAnimation *tickerInAnim;
    QDir docDir;
    QDir imgDir;
    QHelpEngineCore *helpEngine;

    TextButton *upButton;
    TextButton *downButton;
};

#endif // MENU_MANAGER_H

