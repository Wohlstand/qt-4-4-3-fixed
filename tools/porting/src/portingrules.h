/****************************************************************************
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the qt3to4 porting application of the Qt Toolkit.
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

#ifndef PORTINGRULES_H
#define PORTINGRULES_H

#include "qtsimplexml.h"
#include "tokenreplacements.h"
#include <QList>
#include <QPair>
#include <QHash>
#include <QSet>
#include <QStringList>

QT_BEGIN_NAMESPACE

class RuleDescription
{
public:
    explicit RuleDescription(QtSimpleXml &replacementRule) {
        qt3 = replacementRule[QLatin1String("Qt3")].text();
        qt4 = replacementRule[QLatin1String("Qt4")].text();
        ruleType = replacementRule.attribute(QLatin1String("Type"));
    }
    QString qt3;
    QString qt4;
    QString ruleType;
    bool operator==(const RuleDescription &other) const
    {
        return (qt3 == other.qt3 && qt4 == other.qt4 && ruleType == other.ruleType);
    }
};

class PortingRules
{
public:
    static void createInstance(QString xmlFilePath);
    static PortingRules *instance();
    static void deleteInstance();

    enum QtVersion{Qt3, Qt4};
    PortingRules(QString xmlFilePath);
    QList<TokenReplacement*> getTokenReplacementRules();
    QStringList getHeaderList(QtVersion qtVersion);
    QHash<QByteArray, QByteArray> getNeededHeaders();
    QStringList getInheritsQt();
    QHash<QByteArray, QByteArray> getClassLibraryList();
    QHash<QByteArray, QByteArray> getHeaderReplacements();
private:
    static PortingRules *theInstance;

    QList<TokenReplacement*> tokenRules;
    QStringList qt3Headers;
    QStringList qt4Headers;
    QHash<QByteArray, QByteArray> neededHeaders;
    QStringList inheritsQtClass;
    QList<RuleDescription> disabledRules;
    QHash<QByteArray, QByteArray> classLibraryList;
    QHash<QByteArray, QByteArray> headerReplacements;


    void parseXml(const QString fileName);
    void checkScopeAddRule(/*const */QtSimpleXml &currentRule);
    QtSimpleXml *loadXml(const QString fileName) const ;
    QString resolveFileName(const QString currentFileName,
                            const QString includeFileName) const;
    bool isReplacementRule(const QString ruleType) const;
    void disableRule(QtSimpleXml &replacementRule);
    bool isRuleDisabled(QtSimpleXml &replacementRule) const;
    void addLogWarning(const QString text) const;
    void addLogError(const QString text) const;
};

QT_END_NAMESPACE

#endif // PORTINGRULES_H
