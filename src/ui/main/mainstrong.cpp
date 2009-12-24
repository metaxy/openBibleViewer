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
/*!
  Generate a comboc with all strong modules
  */
void MainWindow::loadStrongs()
{
    m_currentStrongID = "";
    m_currentStrongModule = -1;
    QList<int> moduleID;
    QStringList moduleTitle;
    for (int i = 0; i < m_settings->module.size(); ++i) {
        if (m_settings->module.at(i).moduleType == QString::number(Bible::ZefaniaStrongModule)) {
            moduleID << i;
            moduleTitle << m_settings->module.at(i).moduleName;
        }
    }
    m_strongModuleID = moduleID;
    ui->comboBox_strong->clear();
    ui->comboBox_strong->insertItems(0, moduleTitle);

    return;
}
/*!
  Pharse a strong from current module
  */
QString MainWindow::loadStrong(QString strongID)
{
    DEBUG_FUNC_NAME
    m_zefStrong.setSettings(m_settings);
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
/*!
  Show a strong from current module
  */
void MainWindow::showStrong(const QString &strongID)
{
    if (ui->dockWidget_strong->isHidden()) {
        ui->dockWidget_strong->show();
    }
    ui->lineEdit_strong->setText(strongID);
    ui->textBrowser_strong->setText(loadStrong(strongID));
}
/*!
  Load a single strong module
*/

void MainWindow::loadStrongModule(int lID)
{
    DEBUG_FUNC_NAME
    if (m_strongModuleID.size() > lID) {
        int id = m_strongModuleID.at(lID);
        if (id < m_settings->module.size() && id >= 0) {
            m_zefStrong.setSettings(m_settings);
            m_zefStrong.loadDataBase(m_settings->module.at(id).modulePath);
            m_currentStrongModule = id;
            strongSearch();
        }
    }
}
/*!
  Search after a strong in current module
  */
void MainWindow::strongSearch()
{
    DEBUG_FUNC_NAME
    QString search = ui->lineEdit_strong->text();
    if (search.size() > 0)
        showStrong(search);
}
