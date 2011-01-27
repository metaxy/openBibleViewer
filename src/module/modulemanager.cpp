/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger
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
#include "modulemanager.h"

ModuleManager::ModuleManager()
{

    m_moduleMap = new ModuleMap();
    m_dictionary = NULL;
}
ModuleManager::~ModuleManager()
{
    DEBUG_FUNC_NAME
    delete m_moduleMap;

    if(m_dictionary != NULL)
        delete m_dictionary;
}

void ModuleManager::setSettings(Settings *settings)
{
    m_settings = settings;
}

void ModuleManager::setNotes(Notes *notes)
{
    m_notes = notes;
}
void ModuleManager::setBibleDisplaySettings(BibleDisplaySettings *bibleDisplaySettings)
{
    m_bibleDisplaySettings = bibleDisplaySettings;
}


/**
  Load all Modules, and generate a QStandardItemModel.
  */
int ModuleManager::loadAllModules()
{
    DEBUG_FUNC_NAME;

    //The invisible root Module
    Module *root = new Module();
    root->setModuleID(-1);
    root->setModuleClass(OBVCore::FolderClass);
    root->setModuleType(OBVCore::NoneType);

    ModuleSettings *rootModuleSettings = m_settings->getModuleSettings(-1);//it the invisble root item
    if(rootModuleSettings != NULL) {
        foreach(ModuleSettings * s, rootModuleSettings->children()) {
            loadModule(root, s);
        }

        ModuleModel model;
        model.setSettings(m_settings);
        model.generate();
        m_moduleModel = model.itemModel();
    } else {
        m_moduleModel = new QStandardItemModel;
    }

    return 0;
}

void ModuleManager::loadModule(Module *parentModule, ModuleSettings *settings)
{
    if(settings == NULL)
        return;
    Module *module = new Module(parentModule);
    module->setPath(settings->modulePath);

    module->setModuleType(settings->moduleType);
    module->setTitle(settings->moduleName);
    module->setModuleID(settings->moduleID);
    parentModule->append(module);
    m_moduleMap->m_map.insert(settings->moduleID, module);

    if(settings->moduleType == OBVCore::BibleQuoteModule || settings->moduleType == OBVCore::ZefaniaBibleModule || settings->moduleType == OBVCore::TheWordBibleModule) {
        module->setModuleClass(OBVCore::BibleModuleClass);
    } else if(settings->moduleType == OBVCore::ZefaniaLexModule || settings->moduleType == OBVCore::BibleQuoteDictModule) {
        module->setModuleClass(OBVCore::DictionaryModuleClass);
    }

    //recursive
    foreach(ModuleSettings * s, settings->children()) {
        loadModule(module, s);
    }

}
void ModuleManager::initVerseModule(VerseModule *b)
{
    if(b != 0) {
        b->setSettings(m_settings);
        b->setNotes(m_notes);
        b->setModuleMap(m_moduleMap);
        b->setBibleDisplaySettings(m_bibleDisplaySettings);
    }
}

/**
  * Returns true, if a bible is loaded.
  */
bool ModuleManager::bibleLoaded()
{
    if(m_verseTable && verseModule() && m_moduleMap->m_map.contains(verseModule()->moduleID()) && verseModule()->moduleID() >= 0)
        return true;
    return false;
}


bool ModuleManager::hasBible()
{
    if(m_verseTable && verseModule())
        return true;
    return false;
}
/**
  * Returns true, if a strong module is loaded.
  */
bool ModuleManager::strongLoaded()
{

    if(m_moduleMap->m_map.contains(m_dictionary->moduleID())  &&  m_dictionary->moduleID() >= 0)
        return true;
    return false;
}
bool ModuleManager::contains(const int &moduleID)
{
    return m_moduleMap->m_map.contains(moduleID);
}

VerseModule * ModuleManager::verseModule()
{
    return verseTable()->verseModule();
}
VerseTable * ModuleManager::verseTable()
{
    return m_verseTable;
}
Module * ModuleManager::getModule(const int &moduleID)
{
    return m_moduleMap->m_map.value(moduleID);
}
Dictionary* ModuleManager::dictionary()
{
    return m_dictionary;
}

/**
  Converts a persistent url to a link.
  */
QString ModuleManager::notePos2Link(const QString &pos)
{
    //todo: use ranges
    VerseUrl url;
    url.fromString(pos);
    UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::InterfaceUrl, url);
    urlConverter.setSettings(m_settings);
    urlConverter.setModuleMap(m_moduleMap);
    VerseUrl newUrl = urlConverter.convert();
    const QString string = url.getParam("b0") + " " + QString::number(newUrl.ranges().first().startChapterID() + 1) + ":" + QString::number(newUrl.ranges().first().startVerseID() + 1);

    const QString link = newUrl.toString();
    return "<a href=\"" + link + "\" >" + string + "</a>";
}
QString ModuleManager::notePos2Text(const QString &pos)
{
    VerseUrl url;
    url.fromString(pos);
    UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::InterfaceUrl, url);
    urlConverter.setSettings(m_settings);
    urlConverter.setModuleMap(m_moduleMap);
    VerseUrl newUrl = urlConverter.convert();
    const QString string = url.getParam("b0") + " " + QString::number(newUrl.ranges().first().startChapterID() + 1) + ":" + QString::number(newUrl.ranges().first().startVerseID() + 1);
    return string;
}
QStringList ModuleManager::getBibleTitles()
{
    QStringList titles;
    QMapIterator<int, Module *> i(m_moduleMap->m_map);
    while(i.hasNext()) {
        i.next();
        if(i.value()->moduleClass() == OBVCore::BibleModuleClass)
            titles.append(i.value()->title());
    }
    return titles;
}
QStringList ModuleManager::getBiblePaths()
{
    QStringList paths;
    QMapIterator<int, Module *> i(m_moduleMap->m_map);
    while(i.hasNext()) {
        i.next();
        if(i.value()->moduleClass() == OBVCore::BibleModuleClass)
            paths.append(i.value()->path());
    }
    return paths;
}
QList<int> ModuleManager::getBibleIDs()
{
    QList<int> ids;
    QMapIterator<int, Module *> i(m_moduleMap->m_map);
    while(i.hasNext()) {
        i.next();
        if(i.value()->moduleClass() == OBVCore::BibleModuleClass)
            ids.append(i.value()->moduleID());
    }
    return ids;
}
void ModuleManager::checkCache(const int &moduleID)
{
    //todo: load versification
    /*Module *m = m_moduleMap->m_map.value(moduleID);
    if(m->moduleClass() == OBVCore::BibleModuleClass && !m_settings->m_moduleCache.keys().contains(m->path())) {
        Bible *b = new Bible();
        initVerseModule(b);
        b->setModuleType(m->moduleType());
        b->loadModuleData(moduleID);//set cache
    }*/
}
VerseModule * ModuleManager::newVerseModule(const int &moduleID, QPoint p)
{
    DEBUG_FUNC_NAME
    if(!contains(moduleID)) {
        myWarning() << "invalid moduleID = " << moduleID;
        return NULL;
    }

    const int id = verseTable()->m_points.key(p, -1);
    VerseModule *m;
    if(id != -1) {
        m = verseTable()->verseModule(id);
        initVerseModule(m);
    } else {
        //todo: support for other VerseModules

        if(getModule(moduleID)->moduleClass() == OBVCore::BibleModuleClass) {
            m = new Bible();
            initVerseModule(m);
        }
    }
    //todo: check if this is possible b->moduleID() != moduleID

    OBVCore::ModuleType type = getModule(moduleID)->moduleType();
    m->setModuleType(type);
    m->setModuleID(moduleID);
    //todo: load module data?
    /* if(getModule(moduleID)->moduleClass() == OBVCore::BibleModuleClass) {
         myDebug() << "loading the module data";
         ((Bible*)m)->loadModuleData(moduleID);
     }*/

    verseTable()->addModule(m, p);
    return m;
}
OBVCore::ModuleType ModuleManager::recognizeModuleType(const QString &fileName)
{
    //myDebug() << fileName;
    if(fileName.endsWith("bibleqt.ini", Qt::CaseInsensitive)) {
        return OBVCore::BibleQuoteModule;
    } else if(fileName.endsWith(".xml", Qt::CaseInsensitive)) {
        QFile data(fileName);
        if(data.open(QFile::ReadOnly)) {
            QString fileData = "";
            QTextStream in(&data);
            for(int i = 0; i < 10; i++)
                fileData += in.readLine();
            //myDebug() << "fileData = " << fileData;
            if(fileData.contains("XMLBIBLE", Qt::CaseInsensitive) && !(fileData.contains("x-quran", Qt::CaseInsensitive) || // i cannot allow this
                    fileData.contains("x-cult", Qt::CaseInsensitive) ||
                    fileData.contains("x-mormon", Qt::CaseInsensitive))) {
                return OBVCore::ZefaniaBibleModule;
            } else if(fileData.contains("<dictionary", Qt::CaseInsensitive)) {
                return OBVCore::ZefaniaLexModule;
            }

        }
    } else if(fileName.endsWith(".idx", Qt::CaseInsensitive)) {
        return OBVCore::BibleQuoteDictModule;
    } else if(fileName.endsWith(".nt", Qt::CaseInsensitive) || fileName.endsWith(".ot", Qt::CaseInsensitive) || fileName.endsWith(".ont", Qt::CaseInsensitive)) {
        myDebug() << "the word module";
        return OBVCore::TheWordBibleModule;
    }
    return OBVCore::NoneType;
}
