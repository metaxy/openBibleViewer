#include "src/module/strong.h"
#include "src/module/module.h"
#include "src/core/dbghelper.h"
#include <QtCore/QStringList>
Strong::Strong()
{
}

QStringList Strong::loadStrongs()
{
    /*m_currentStrongID = "";
    m_currentStrongModule = -1;
    QList<int> moduleID;
    QStringList moduleTitle;
    for (int i = 0; i < m_settings->module.size(); ++i) {
        if (m_settings->module.at(i).m_moduleClass == QString::number(Module::StrongModule)) {
            moduleID << i;
            moduleTitle << m_settings->module.at(i).moduleName;
        }
    }
    m_strongModuleID = moduleID;
    return moduleTitle;*/
}
/*!
  Pharse a strong from current module
  */
QString Strong::loadStrong(QString strongID)
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
        strong = QObject::tr("No strong module loaded.");
    }
    if (strong.size() == 0)
        strong = QObject::tr("Strong not found.");
    return strong;
}
/*!
  Show a strong from current module
  */
void Strong::showStrong(const QString &strongID)
{
  /*  if (ui->dockWidget_strong->isHidden()) {
        ui->dockWidget_strong->show();
    }
    ui->lineEdit_strong->setText(strongID);
    ui->textBrowser_strong->setText(loadStrong(strongID));*/
}
/*!
  Load a single strong module
*/

void Strong::loadStrongModule(int lID)
{
    DEBUG_FUNC_NAME
    if (m_strongModuleID.size() > lID) {
        int id = m_strongModuleID.at(lID);
        if (id < m_settings->module.size() && id >= 0) {
            m_zefStrong.setSettings(m_settings);
            m_zefStrong.loadDataBase(m_settings->module.at(id).modulePath);
            m_currentStrongModule = id;
            //strongSearch();
        }
    }
}
