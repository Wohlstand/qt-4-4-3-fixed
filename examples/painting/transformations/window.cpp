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

#include "window.h"

//! [0]
Window::Window()
{
    originalRenderArea = new RenderArea;

    shapeComboBox = new QComboBox;
    shapeComboBox->addItem(tr("Clock"));
    shapeComboBox->addItem(tr("House"));
    shapeComboBox->addItem(tr("Text"));
    shapeComboBox->addItem(tr("Truck"));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(originalRenderArea, 0, 0);
    layout->addWidget(shapeComboBox, 1, 0);
//! [0]

//! [1]
    for (int i = 0; i < NumTransformedAreas; ++i) {
        transformedRenderAreas[i] = new RenderArea;

        operationComboBoxes[i] = new QComboBox;
        operationComboBoxes[i]->addItem(tr("No transformation"));
        operationComboBoxes[i]->addItem(tr("Rotate by 60\xB0"));
        operationComboBoxes[i]->addItem(tr("Scale to 75%"));
        operationComboBoxes[i]->addItem(tr("Translate by (50, 50)"));

        connect(operationComboBoxes[i], SIGNAL(activated(int)),
                this, SLOT(operationChanged()));

        layout->addWidget(transformedRenderAreas[i], 0, i + 1);
        layout->addWidget(operationComboBoxes[i], 1, i + 1);
    }
//! [1]

//! [2]
    setLayout(layout);
    setupShapes();
    shapeSelected(0);

    setWindowTitle(tr("Transformations"));
}
//! [2]

//! [3]
void Window::setupShapes()
{
    QPainterPath truck;
//! [3]
    truck.setFillRule(Qt::WindingFill);
    truck.moveTo(0.0, 87.0);
    truck.lineTo(0.0, 60.0);
    truck.lineTo(10.0, 60.0);
    truck.lineTo(35.0, 35.0);
    truck.lineTo(100.0, 35.0);
    truck.lineTo(100.0, 87.0);
    truck.lineTo(0.0, 87.0);
    truck.moveTo(17.0, 60.0);
    truck.lineTo(55.0, 60.0);
    truck.lineTo(55.0, 40.0);
    truck.lineTo(37.0, 40.0);
    truck.lineTo(17.0, 60.0);
    truck.addEllipse(17.0, 75.0, 25.0, 25.0);
    truck.addEllipse(63.0, 75.0, 25.0, 25.0);

//! [4]
    QPainterPath clock;
//! [4]
    clock.addEllipse(-50.0, -50.0, 100.0, 100.0);
    clock.addEllipse(-48.0, -48.0, 96.0, 96.0);
    clock.moveTo(0.0, 0.0);
    clock.lineTo(-2.0, -2.0);
    clock.lineTo(0.0, -42.0);
    clock.lineTo(2.0, -2.0);
    clock.lineTo(0.0, 0.0);
    clock.moveTo(0.0, 0.0);
    clock.lineTo(2.732, -0.732);
    clock.lineTo(24.495, 14.142);
    clock.lineTo(0.732, 2.732);
    clock.lineTo(0.0, 0.0);

//! [5]
    QPainterPath house;
//! [5]
    house.moveTo(-45.0, -20.0);
    house.lineTo(0.0, -45.0);
    house.lineTo(45.0, -20.0);
    house.lineTo(45.0, 45.0);
    house.lineTo(-45.0, 45.0);
    house.lineTo(-45.0, -20.0);
    house.addRect(15.0, 5.0, 20.0, 35.0);
    house.addRect(-35.0, -15.0, 25.0, 25.0);

//! [6]
    QPainterPath text;
//! [6]
    QFont font;
    font.setPixelSize(50);
    QRect fontBoundingRect = QFontMetrics(font).boundingRect(tr("Qt"));
    text.addText(-QPointF(fontBoundingRect.center()), font, tr("Qt"));

//! [7]
    shapes.append(clock);
    shapes.append(house);
    shapes.append(text);
    shapes.append(truck);

    connect(shapeComboBox, SIGNAL(activated(int)),
            this, SLOT(shapeSelected(int)));
}
//! [7]

//! [8]
void Window::operationChanged()
{
    static const Operation operationTable[] = {
        NoTransformation, Rotate, Scale, Translate
    };

    QList<Operation> operations;
    for (int i = 0; i < NumTransformedAreas; ++i) {
        int index = operationComboBoxes[i]->currentIndex();
        operations.append(operationTable[index]);
        transformedRenderAreas[i]->setOperations(operations);
    }
}
//! [8]

//! [9]
void Window::shapeSelected(int index)
{
    QPainterPath shape = shapes[index];
    originalRenderArea->setShape(shape);
    for (int i = 0; i < NumTransformedAreas; ++i)
        transformedRenderAreas[i]->setShape(shape);
}
//! [9]
