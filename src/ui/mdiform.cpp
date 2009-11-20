/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009 Paul Walger
This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3 of the License, or (at your option)
any later version.
This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with
this program; if not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/
#include "mdiform.h"
#include "ui_mdiform.h"
#include <QtCore/QtDebug>


mdiForm::mdiForm(QWidget *parent) :
        QWidget(parent),
        m_ui(new Ui::mdiForm)
{
    m_ui->setupUi(this);
    m_ui->textBrowser->setOpenLinks(false);
    m_ui->textBrowser->setOpenExternalLinks(false);
    connect(m_ui->toolButton_backward, SIGNAL(clicked()), this, SLOT(backward()));
    connect(m_ui->toolButton_forward, SIGNAL(clicked()), this, SLOT(forward()));
    setButtons();

}
/*void mdiForm::changeEvent(QEvent *e)
{
    qDebug() << "mdiForm::changeEvent() = " << e->type();
    switch (e->type())
    {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    case QEvent::Close:
        qDebug() << "mdiForm::changeEvent() close";
        break;
    default:
        break;
    }
}*/
void mdiForm::historyGetUrl(QString url)
{
    //qDebug() << " mdiForm::historyGetUrl() url = " << url;
    browserHistory.setCurrent(url);
    setButtons();
}
void mdiForm::backward()
{
    qDebug() << " mdiForm::backward()";
    emit historyGo(browserHistory.backward());
    setButtons();
}
void mdiForm::forward()
{
    qDebug() << " mdiForm::forward()";
    emit historyGo(browserHistory.forward());
    setButtons();
}
void mdiForm::setButtons()
{
    if (browserHistory.backwardAvailable()) {
        m_ui->toolButton_backward->setDisabled(false);
    } else {
        m_ui->toolButton_backward->setDisabled(true);
    }
    if (browserHistory.forwardAvailable()) {
        m_ui->toolButton_forward->setDisabled(false);
    } else {
        m_ui->toolButton_forward->setDisabled(true);
    }
}
mdiForm::~mdiForm()
{
    delete m_ui;
}
