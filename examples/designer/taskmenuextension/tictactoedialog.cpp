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
#include <QtDesigner>

#include "tictactoe.h"
#include "tictactoedialog.h"

//! [0]
TicTacToeDialog::TicTacToeDialog(TicTacToe *tic, QWidget *parent)
    : QDialog(parent)
{
    ticTacToe = tic;
    editor = new TicTacToe;
    editor->setState(ticTacToe->state());

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel
                                     | QDialogButtonBox::Reset);

    connect(buttonBox->button(QDialogButtonBox::Reset), SIGNAL(clicked()),
            this, SLOT(resetState()));
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(saveState()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(editor);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);
    setWindowTitle(tr("Edit State"));
}
//! [0]

//! [1]
QSize TicTacToeDialog::sizeHint() const
{
    return QSize(250, 250);
}
//! [1]

//! [2]
void TicTacToeDialog::resetState()
{
    editor->clearBoard();
}
//! [2]

//! [3]
void TicTacToeDialog::saveState()
{
//! [3] //! [4]
    if (QDesignerFormWindowInterface *formWindow
            = QDesignerFormWindowInterface::findFormWindow(ticTacToe)) {
        formWindow->cursor()->setProperty("state", editor->state());
    }
//! [4] //! [5]
    accept();
}
//! [5]
