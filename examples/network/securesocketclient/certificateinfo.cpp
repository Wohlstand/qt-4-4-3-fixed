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

#include "certificateinfo.h"
#include "ui_certificateinfo.h"

CertificateInfo::CertificateInfo(QWidget *parent)
    : QDialog(parent)
{
    form = new Ui_CertificateInfo;
    form->setupUi(this);

    connect(form->certificationPathView, SIGNAL(currentRowChanged(int)),
            this, SLOT(updateCertificateInfo(int)));
}

CertificateInfo::~CertificateInfo()
{
    delete form;
}

void CertificateInfo::setCertificateChain(const QList<QSslCertificate> &chain)
{
    this->chain = chain;

    form->certificationPathView->clear();

    for (int i = 0; i < chain.size(); ++i) {
        const QSslCertificate &cert = chain.at(i);
        form->certificationPathView->addItem(tr("%1%2 (%3)").arg(!i ? QString() : tr("Issued by: "))
                                             .arg(cert.subjectInfo(QSslCertificate::Organization))
                                             .arg(cert.subjectInfo(QSslCertificate::CommonName)));
    }

    form->certificationPathView->setCurrentRow(0);
}

void CertificateInfo::updateCertificateInfo(int index)
{
    form->certificateInfoView->clear();
    if (index >= 0 && index < chain.size()) {
        const QSslCertificate &cert = chain.at(index);
        QStringList lines;
        lines << tr("Organization: %1").arg(cert.subjectInfo(QSslCertificate::Organization))
              << tr("Subunit: %1").arg(cert.subjectInfo(QSslCertificate::OrganizationalUnitName))
              << tr("Country: %1").arg(cert.subjectInfo(QSslCertificate::CountryName))
              << tr("Locality: %1").arg(cert.subjectInfo(QSslCertificate::LocalityName))
              << tr("State/Province: %1").arg(cert.subjectInfo(QSslCertificate::StateOrProvinceName))
              << tr("Common Name: %1").arg(cert.subjectInfo(QSslCertificate::CommonName))
              << QString()
              << tr("Issuer Organization: %1").arg(cert.issuerInfo(QSslCertificate::Organization))
              << tr("Issuer Unit Name: %1").arg(cert.issuerInfo(QSslCertificate::OrganizationalUnitName))
              << tr("Issuer Country: %1").arg(cert.issuerInfo(QSslCertificate::CountryName))
              << tr("Issuer Locality: %1").arg(cert.issuerInfo(QSslCertificate::LocalityName))
              << tr("Issuer State/Province: %1").arg(cert.issuerInfo(QSslCertificate::StateOrProvinceName))
              << tr("Issuer Common Name: %1").arg(cert.issuerInfo(QSslCertificate::CommonName));
        foreach (QString line, lines)
            form->certificateInfoView->addItem(line);
    } else {
        form->certificateInfoView->clear();
    }
}
