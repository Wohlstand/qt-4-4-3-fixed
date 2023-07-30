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

#include "employee.h"

//! [0]
EmployeeData::EmployeeData()
{
    id = -1;
    name = 0;
}
//! [0]

//! [1]
EmployeeData::EmployeeData(const EmployeeData &other)
//! [1] //! [2]
    : QSharedData(other)
{
    id = other.id;
    if (other.name) {
        name = new QString(*other.name);
    } else {
        name = 0;
    }
}
//! [2]

//! [3]
EmployeeData::~EmployeeData()
//! [3] //! [4]
{
    delete name;
}
//! [4]

//! [5]
Employee::Employee()
//! [5] //! [6]
{
    d = new EmployeeData;
}
//! [6]

//! [7]
Employee::Employee(int id, const QString &name)
//! [7] //! [8]
{
    d = new EmployeeData;
    setId(id);
    setName(name);
}
//! [8]

//! [9]
void Employee::setName(const QString &name)
//! [9] //! [10]
{
    if (!d->name)
        d->name = new QString;
    *d->name = name;
}
//! [10]

//! [11]
QString Employee::name() const
//! [11] //! [12]
{
    if (!d->name)
        return QString();
    return *d->name;
}
//! [12]
