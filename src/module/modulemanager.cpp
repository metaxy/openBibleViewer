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
#include "src/core/verse/reftext.h"

ModuleManager::ModuleManager()
{
    m_moduleMap = new ModuleMap();
    m_dictionary = NULL;
    m_rootModule = NULL;
}
ModuleManager::~ModuleManager()
{
    //DEBUG_FUNC_NAME
    delete m_moduleMap;

    if(m_dictionary != NULL) {
        delete m_dictionary;
        m_dictionary = NULL;
    }
    if(m_moduleModel != NULL) {
        delete m_moduleModel;
        m_moduleModel = NULL;
    }
    if(m_rootModule != NULL) {
        delete m_rootModule;
        m_rootModule = NULL;
    }
}

void ModuleManager::setSettings(Settings *settings)
{
    m_settings = settings;
}

void ModuleManager::setNotes(Notes *notes)
{
    m_notes = notes;
}
void ModuleManager::setModuleDisplaySettings(ModuleDisplaySettings *moduleDisplaySettings)
{
    m_moduleDisplaySettings = moduleDisplaySettings;
}


/**
  * Load all Modules, and generate a QStandardItemModel.
  */
int ModuleManager::loadAllModules()
{
    DEBUG_FUNC_NAME;
    //update module settings and display settings

    ModuleSettings *root = new ModuleSettings();
    root->moduleID = -1;
    root->parentID = -2;

    foreach(ModuleSettings * set, m_settings->m_moduleSettings) {
        set->clearChildren();
    }
    //set parents
    {
        QHashIterator<int, ModuleSettings*> it2(m_settings->m_moduleSettings);
        while(it2.hasNext())  {
            it2.next();
            ModuleSettings *child = it2.value();
            const int parentID = child->parentID;
            if(parentID == -1) {
                root->appendChild(child);
                child->setParent(root);
            } else if(m_settings->m_moduleSettings.contains(parentID)) {
                ModuleSettings *r = m_settings->m_moduleSettings.value(parentID);
                r->appendChild(child);
                child->setParent(r);
            }
        }
        QSharedPointer<ModuleDisplaySettings> displaySettings = QSharedPointer<ModuleDisplaySettings>(new ModuleDisplaySettings());
        displaySettings->setShowStudyNotes(true);
        displaySettings->setShowStrong(true);
        displaySettings->setShowRefLinks(false);
        displaySettings->setShowNotes(true);
        displaySettings->setShowMarks(true);
        displaySettings->setShowBottomToolBar(true);
        root->setDisplaySettings(displaySettings);

        m_settings->m_moduleSettings.insert(-1, root);
    }
    //use parent settings display
    {
        QHashIterator<int, ModuleSettings*> it2(m_settings->m_moduleSettings);
        while(it2.hasNext())  {
            it2.next();
            ModuleSettings *child = it2.value();
            if(child->useParentSettings) {
                const int parentID = child->parentID;
                if(m_settings->m_moduleSettings.contains(parentID)) {
                    child->removeDisplaySettings();
                    ModuleSettings *r = m_settings->m_moduleSettings.value(parentID);
                    makeSureItHasLoaded(r);
                    child->setDisplaySettings(r->displaySettings());
                }
            }
        }
    }


    //The invisible root Module
    m_rootModule = new Module();
    m_rootModule->setModuleID(-1);
    m_rootModule->setModuleClass(OBVCore::FolderClass);
    m_rootModule->setModuleType(OBVCore::NoneType);

    ModuleSettings *rootModuleSettings = m_settings->getModuleSettings(-1);//it the invisble root item
    if(rootModuleSettings != NULL) {
        foreach(ModuleSettings * s, rootModuleSettings->children()) {
            loadModule(m_rootModule, s);
        }
        ModuleModel model(0);
        model.setSettings(m_settings);
        model.setShowAll(true);
        model.generate();
        m_moduleModel = model.itemModel();
    } else {
        m_moduleModel = new QStandardItemModel;
    }


    return 0;
}
void ModuleManager::makeSureItHasLoaded(ModuleSettings *settings)
{
    if(settings->useParentSettings) {
        const int parentID = settings->parentID;
        if(m_settings->m_moduleSettings.contains(parentID)) {
            settings->removeDisplaySettings();
            ModuleSettings *r = m_settings->m_moduleSettings.value(parentID);
            makeSureItHasLoaded(r);
            settings->setDisplaySettings(r->displaySettings());
        }
    }
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

    if(settings->moduleType == OBVCore::BibleQuoteModule
            || settings->moduleType == OBVCore::ZefaniaBibleModule
            || settings->moduleType == OBVCore::TheWordBibleModule
            || settings->moduleType == OBVCore::SwordBibleModule) {
        module->setModuleClass(OBVCore::BibleModuleClass);
    } else if(settings->moduleType == OBVCore::ZefaniaLexModule || settings->moduleType == OBVCore::BibleQuoteDictModule) {
        module->setModuleClass(OBVCore::DictionaryModuleClass);
    }

    //recursive
    foreach(ModuleSettings * s, settings->children()) {
        loadModule(module, s);
    }

}
void ModuleManager::initVerseModule(VerseModule *b) const
{
    if(b != NULL) {
        b->setSettings(m_settings);
        b->setNotes(m_notes);
        b->setModuleMap(m_moduleMap);
        b->setModuleDisplaySettings(m_moduleDisplaySettings);
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
bool ModuleManager::dictionaryLoaded()
{
    return dictionaryLoaded(m_dictionary);
}

bool ModuleManager::dictionaryLoaded(const Dictionary *dict)
{
    if(m_moduleMap->m_map.contains(dict->moduleID()) && dict->moduleID() >= 0)
        return true;
    return false;
}
bool ModuleManager::metaModuleLoaded(const SimpleModuleClass *m) const
{
    return (m && m_moduleMap->m_map.contains(m->moduleID()) && m->moduleID() >= 0);
}
bool ModuleManager::verseTableLoaded(const VerseTable *table) const
{
     return (table && table->verseModule() && m_moduleMap->m_map.contains(table->verseModule()->moduleID()) && table->verseModule()->moduleID() >= 0);
}
bool ModuleManager::contains(const int moduleID)
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
Module * ModuleManager::getModule(const int moduleID)
{
    return m_moduleMap->m_map.value(moduleID);
}
Dictionary* ModuleManager::dictionary()
{
    return m_dictionary;
}

/**
  * Converts a persistent url to a link.
  */
QString ModuleManager::notePos2Link(const QString &pos)
{
    VerseUrl url;
    url.fromString(pos);
    UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::InterfaceUrl, url);
    urlConverter.setSettings(m_settings);
    urlConverter.setModuleMap(m_moduleMap);
    VerseUrl newUrl = urlConverter.convert();
    RefText refText;
    refText.setSettings(m_settings);
    const QString link = newUrl.toString();
    return "<a href=\"" + link + "\">" + refText.toString(newUrl) + "</a>";
}
QString ModuleManager::notePos2Text(const QString &pos)
{
    VerseUrl url;
    url.fromString(pos);
    UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::InterfaceUrl, url);
    urlConverter.setSettings(m_settings);
    urlConverter.setModuleMap(m_moduleMap);
    VerseUrl newUrl = urlConverter.convert();
    RefText refText;
    refText.setSettings(m_settings);

    const QString string = refText.toString(newUrl);
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
void ModuleManager::checkCache(const int moduleID)
{
    /*Module *m = m_moduleMap->m_map.value(moduleID);
    if(m->moduleClass() == OBVCore::BibleModuleClass && !m_settings->m_moduleCache.keys().contains(m->path())) {
        Bible *b = new Bible();
        initVerseModule(b);
        b->setModuleType(m->moduleType());
        b->loadModuleData(moduleID);//set cache
    }*/
}
VerseModule * ModuleManager::newVerseModule(const int moduleID, QPoint p, VerseTable *table)
{
    DEBUG_FUNC_NAME
    if(!contains(moduleID)) {
        myWarning() << "invalid moduleID = " << moduleID;
        return NULL;
    }

    const int id = table->m_points.key(p, -1);
    VerseModule *m = NULL;
    /*if(id != -1) {
        m = verseTable()->verseModule(id);
        initVerseModule(m);
    } else {*/
    //todo: support for other VerseModules
    if(getModule(moduleID)->moduleClass() == OBVCore::BibleModuleClass) {
        m = new Bible();
        initVerseModule(m);
    }
    /*}*/
    //todo: check if this is possible b->moduleID() != moduleID

    OBVCore::ModuleType type = getModule(moduleID)->moduleType();
    m->setModuleType(type);
    m->setModuleID(moduleID);
    myDebug() << moduleID;
    //todo: load module data?
    /* if(getModule(moduleID)->moduleClass() == OBVCore::BibleModuleClass) {
         myDebug() << "loading the module data";
         ((Bible*)m)->loadModuleData(moduleID);
     }*/

    table->addModule(m, p);

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
