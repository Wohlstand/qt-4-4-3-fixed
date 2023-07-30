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

#include "bencodeparser.h"

#include <QList>
#include <QMetaType>

BencodeParser::BencodeParser()
{
}

bool BencodeParser::parse(const QByteArray &content)
{
    if (content.isEmpty()) {
        errString = QString("No content");
        return false;
    }

    this->content = content;
    index = 0;
    infoStart = 0;
    infoLength = 0;
    return getDictionary(&dictionaryValue);
}

QString BencodeParser::errorString() const
{
    return errString;
}

QMap<QByteArray, QVariant> BencodeParser::dictionary() const
{
    return dictionaryValue;
}

QByteArray BencodeParser::infoSection() const
{
    return content.mid(infoStart, infoLength);
}

bool BencodeParser::getByteString(QByteArray *byteString)
{
    const int contentSize = content.size();
    int size = -1;
    do {
	char c = content.at(index);
	if (c < '0' || c > '9') {
	    if (size == -1)
		return false;
	    if (c != ':') {
		errString = QString("Unexpected character at pos %1: %2")
		    .arg(index).arg(c);
		return false;
	    }
	    ++index;
	    break;
	}
	if (size == -1)
	    size = 0;
	size *= 10;
	size += c - '0';
    } while (++index < contentSize);

    if (byteString)
	*byteString = content.mid(index, size);
    index += size;
    return true;
}

bool BencodeParser::getInteger(qint64 *integer)
{
    const int contentSize = content.size();
    if (content.at(index) != 'i')
	return false;

    ++index;
    qint64 num = -1;
    bool negative = false;

    do {
	char c = content.at(index);
	if (c < '0' || c > '9') {
	    if (num == -1) {
		if (c != '-' || negative)
		    return false;
		negative = true;
		continue;
	    } else {
		if (c != 'e') {
		    errString = QString("Unexpected character at pos %1: %2")
			.arg(index).arg(c);
		    return false;
		}
		++index;
		break;
	    }
	}
	if (num == -1)
	    num = 0;
	num *= 10;
	num += c - '0';
    } while (++index < contentSize);

    if (integer)
	*integer = negative ? -num : num;
    return true;
}

bool BencodeParser::getList(QList<QVariant> *list)
{
    const int contentSize = content.size();
    if (content.at(index) != 'l')
	return false;

    QList<QVariant> tmp;    
    ++index;

    do {
	if (content.at(index) == 'e') {
	    ++index;
	    break;
	}

	qint64 number;
	QByteArray byteString;
	QList<QVariant> tmpList;
	QMap<QByteArray, QVariant> dictionary;

	if (getInteger(&number))
	    tmp << number;
	else if (getByteString(&byteString))
	    tmp << byteString;
	else if (getList(&tmpList))
	    tmp << tmpList;
	else if (getDictionary(&dictionary))
	    tmp << qVariantFromValue<QMap<QByteArray, QVariant> >(dictionary);
	else {
	    errString = QString("error at index %1").arg(index);
	    return false;
	}
    } while (index < contentSize);

    if (list)
	*list = tmp;
    return true;
}

bool BencodeParser::getDictionary(QMap<QByteArray, QVariant> *dictionary)
{
    const int contentSize = content.size();
    if (content.at(index) != 'd')
	return false;

    QMap<QByteArray, QVariant> tmp;    
    ++index;

    do {
	if (content.at(index) == 'e') {
	    ++index;
	    break;
	}

	QByteArray key;
	if (!getByteString(&key))
	    break;

	if (key == "info")
	  infoStart = index;

	qint64 number;
	QByteArray byteString;
	QList<QVariant> tmpList;
	QMap<QByteArray, QVariant> dictionary;

	if (getInteger(&number))
	    tmp.insert(key, number);
	else if (getByteString(&byteString))
	    tmp.insert(key, byteString);
	else if (getList(&tmpList))
	    tmp.insert(key, tmpList);
	else if (getDictionary(&dictionary))
	    tmp.insert(key, qVariantFromValue<QMap<QByteArray, QVariant> >(dictionary));
	else {
	    errString = QString("error at index %1").arg(index);
	    return false;
	}

	if (key == "info")
	  infoLength = index - infoStart;

    } while (index < contentSize);

    if (dictionary)
	*dictionary = tmp;
    return true;
}
