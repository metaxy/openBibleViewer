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
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../../core/dbghelper.h"
#include <QtCore/QtDebug>
void MainWindow::loadStrongs()
{
    m_currentStrongID = "";
    m_currentStrongModule = -1;
    QList<int> moduleID;
    QStringList moduleTitle;
    for (int i = 0; i < m_set.module.size(); ++i) {
        if (m_set.module.at(i).moduleType == QString::number(Bible::ZefaniaStrong)) {
            moduleID << i;
            moduleTitle << m_set.module.at(i).moduleName;
        }
    }
    m_strongModuleID = moduleID;
    ui->comboBox_strong->clear();
    ui->comboBox_strong->insertItems(0, moduleTitle);

    return;
}
QString MainWindow::loadStrong(QString strongID)
{
    DEBUG_FUNC_NAME
    m_zefStrong.setSettings(m_set, moduleConfig());
    if (m_currentStrongModule == -1) {
        loadStrongModule(0);
    }
    QString strong;
    if (m_currentStrongModule >= 0) {
        m_currentStrongID = strongID;
        strong = m_zefStrong.getStrong(strongID);
    } else {
        strong = tr("No strong module loaded.");
    }
    if (strong.size() == 0)
        strong = tr("Strong not found.");
    return strong;
}
void MainWindow::showStrong(const QString &strongID)
{
    if (ui->dockWidget_strong->isHidden()) {
        ui->dockWidget_strong->show();
    }
    ui->lineEdit_strong->setText(strongID);
    ui->textBrowser_strong->setText(loadStrong(strongID));
}
void MainWindow::loadStrongModule(int lID)
{
    DEBUG_FUNC_NAME
    if (m_strongModuleID.size() > lID) {
        int id = m_strongModuleID.at(lID);
        m_zefStrong.setSettings(m_set, m_set.module.at(id));
        m_zefStrong.loadDataBase(m_set.module.at(id).modulePath);
        m_currentStrongModule = id;
        strongSearch();
    }
}
void MainWindow::strongSearch()
{
    DEBUG_FUNC_NAME
    QString search = ui->lineEdit_strong->text();
    if (search.size() > 0)
        showStrong(search);
}
