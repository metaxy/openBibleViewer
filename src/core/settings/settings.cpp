/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2012 Paul Walger <metaxy@walger.name>
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
#include "src/core/settings/settings.h"
#include "src/core/dbghelper.h"
#include <QUrl>
#include <QDir>
#include <QCryptographicHash>
Settings::Settings()
{
    homePath = "";
}
Settings::~Settings()
{
    defaultVersification.clear();
}


ModuleSettings * Settings::getModuleSettings(int moduleID) const
{
    if(m_moduleSettings.contains(moduleID)) {
        return m_moduleSettings.value(moduleID);
    } else {
        myWarning() << "no modulesettings found " << m_moduleSettings.keys() << moduleID;
        return NULL;
    }
}
QSharedPointer<Versification> Settings::getV11N(const int moduleID) const
{
    ModuleSettings *set = getModuleSettings(moduleID);
    if(!set)
        return defaultVersification;
    QSharedPointer<Versification> v11n = set->getV11n();
    if(!v11n)
        return defaultVersification;
    return v11n;
}
QString Settings::savableUrl(QString url) const
{
    if(url.startsWith(homePath)) {
        url.replace(homePath, "?obvHomePath?");
        const QByteArray ba = QUrl::toPercentEncoding(url);
        return QString(ba.constData());
    }
    if(url.startsWith(QDir::homePath())) {
        url.replace(QDir::homePath(), "?homePath?");
        const QByteArray ba = QUrl::toPercentEncoding(url);
        return QString(ba.constData());
    }
    return url;
}

QString Settings::recoverUrl(QString url) const
{
    url = QUrl::fromPercentEncoding(url.toLocal8Bit());
    if(url.startsWith("?obvHomePath?")) {
        url.replace("?obvHomePath?", homePath);
        return url;
    }
    if(url.startsWith("?homePath?")) {
        url.replace("?homePath?", QDir::homePath());
        return url;
    }
    return url;
}

QString Settings::hash(const QString &path) const
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(savableUrl(path).toLocal8Bit());
    return QString(hash.result().toHex());
}

QString Settings::v11nFile(const QString &path) const
{
    return homePath + "v11n/" + hash(path) + "/v11n.ini";
}

int Settings::newModuleID() const
{
    int max = 0;
    QMap<int, ModuleSettings *>::const_iterator i;
    for(i = m_moduleSettings.constBegin(); i != m_moduleSettings.constEnd(); ++i) {
        if(i.key() > max) {
            max = i.key();
        }
    }
    return max + 1;

}
int Settings::getDefaultModule(ModuleTools::ContentType c)
{
    ModuleTools::DefaultModule d = ModuleTools::toDefaultModule(c);

    QMap<int, ModuleSettings *>::const_iterator i;
    for(i = m_moduleSettings.constBegin(); i != m_moduleSettings.constEnd(); ++i) {
        if(i.value()->defaultModule == d) {
            return i.key();
        }
    }
    for(i = m_moduleSettings.constBegin(); i != m_moduleSettings.constEnd(); ++i) {
        if(i.value()->contentType == c) {
            return i.key();
        }
    }
    return 0;
}

ModuleSettings* Settings::newVirtualFolder(const int parentModuleID)
{
    ModuleSettings *m = new ModuleSettings();
    m->moduleID = newModuleID();
    m->moduleName = QObject::tr("New Folder");
    m->moduleType = ModuleTools::FolderModule;

    m->parentID = parentModuleID;

    getModuleSettings(parentModuleID)->appendChild(m);
    m_moduleSettings.insert(m->moduleID, m);
    return m;
}
ModuleSettings * Settings::newModuleSettings(const int parentID)
{
    ModuleSettings *m = new ModuleSettings();
    m->moduleID = newModuleID();
    m->parentID = parentID;

    getModuleSettings(parentID)->appendChild(m);
    m_moduleSettings.insert(m->moduleID, m);
    return m;
}
ModuleSettings * Settings::createRoot()
{
    ModuleSettings *root = new ModuleSettings();
    root->moduleID = -1;
    root->parentID = -2;

    QSharedPointer<ModuleDisplaySettings> displaySettings = QSharedPointer<ModuleDisplaySettings>(new ModuleDisplaySettings());

    //todo: what is that?
    displaySettings->setShowStudyNotes(true);
    displaySettings->setShowStrong(true);
    displaySettings->setShowRefLinks(false);
    displaySettings->setShowNotes(true);
    displaySettings->setShowMarks(true);
    displaySettings->setShowBottomToolBar(true);
    displaySettings->setShowRMAC(true);
    displaySettings->setShowCaptions(false);
    displaySettings->setShowProlog(false);

    root->setDisplaySettings(displaySettings);

    m_moduleSettings.insert(-1, root);
    return root;
}

void Settings::removeModule(const int moduleID)
{
    ModuleSettings *child = getModuleSettings(moduleID);
    removeModule(child);
}
void Settings::removeModule(ModuleSettings* module)
{
    if(getModuleSettings(module->parentID)) //it could be there is no more parent module
        getModuleSettings(module->parentID)->removeChild(module);
    if(!module->children().isEmpty()) { //remove also all it children
        foreach(ModuleSettings *cc, module->children()) {
            removeModule(cc);
        }
    }
    m_moduleSettings.remove(module->moduleID);
    delete module;
}

