#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore/QtDebug>
void MainWindow::loadStrongs()
{
    m_currentStrongID = "";
    m_currentStrongModule = -1;
    QList<int> moduleID;
    QStringList moduleTitle;
    for (int i = 0; i < set.module.size(); ++i) {
        if (set.module.at(i).moduleType == "3") {
            moduleID << i;
            moduleTitle << set.module.at(i).moduleName;
        }
    }
    m_strongModuleID = moduleID;
    ui->comboBox_strong->clear();
    ui->comboBox_strong->insertItems(0, moduleTitle);

    return;
}
QString MainWindow::loadStrong(QString strongID)
{
    qDebug() << "MainWindow::loadStrong";
    zefStrong.setSettings(set, moduleConfig());
    if (m_currentStrongModule == -1) {
        loadStrongModule(0);
    }
    QString strong;
    if (m_currentStrongModule >= 0) {
        m_currentStrongID = strongID;
        strong = zefStrong.getStrong(strongID);
    } else {
        strong = tr("No strong module loaded.");
    }
    if (strong.size() == 0)
        strong = tr("Strong not found.");
    return strong;
}
void MainWindow::showStrong(QString strongID)
{
    if (ui->dockWidget_strong->isHidden()) {
        ui->dockWidget_strong->show();
    }
    ui->lineEdit_strong->setText(strongID);
    ui->textBrowser_strong->setText(loadStrong(strongID));
}
void MainWindow::loadStrongModule(int lID)
{
    qDebug() << "MainWindow::loadStrongModule()";
    if (m_strongModuleID.size() > lID) {
        int id = m_strongModuleID.at(lID);
        zefStrong.setSettings(set, set.module.at(id));
        zefStrong.loadDataBase(set.module.at(id).modulePath);
        m_currentStrongModule = id;
        strongSearch();
    }
}
void MainWindow::strongSearch()
{
    qDebug() << "MainWindow::strongSearch()";
    QString search = ui->lineEdit_strong->text();
    if (search.size() > 0)
        showStrong(search);
}
