#include "src/module/strong.h"
#include "src/module/module.h"
#include "src/core/dbghelper.h"
#include <QtCore/QStringList>
Strong::Strong()
{
}
/**
  Pharse a strong from current module
  */
QString Strong::loadStrong(QString strongString)
{
    //DEBUG_FUNC_NAME
    m_zefStrong.setSettings(m_settings);
    if (m_strongModuleID == -1) {
        myDebug() << "Nothing loaded";//should never happen
    }
    QString ret;
    if (m_strongModuleID >= 0) {
        m_strongString = strongString;
        ret = m_zefStrong.getStrong(strongString);
    } else {
        ret = QObject::tr("No strong module loaded.");
    }
    if (ret.size() == 0)
        ret = QObject::tr("Strong not found.");
    return ret;
}
/**
  Load a single strong module
*/
void Strong::loadStrongModule(const int &id, QString path)
{
    //DEBUG_FUNC_NAME
    m_zefStrong.setSettings(m_settings);
    m_zefStrong.loadDataBase(path);
    m_strongModuleID = id;
}
