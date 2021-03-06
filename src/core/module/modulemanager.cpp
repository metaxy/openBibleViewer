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
#include "modulemanager.h"
#include "src/core/verse/reftext.h"

#define HAS_MEM_FUNC(func, name)                                        \
    template<typename T, typename Sign>                                 \
    struct name {                                                       \
        typedef char yes[1];                                            \
        typedef char no [2];                                            \
        template <typename U, U> struct type_check;                     \
        template <typename _1> static yes &chk(type_check<Sign, &_1::func> *); \
        template <typename   > static no  &chk(...);                    \
        static bool const value = sizeof(chk<T>(0)) == sizeof(yes);     \
    }

ModuleManager::ModuleManager()
{
    m_moduleMap = QSharedPointer<ModuleMap>(new ModuleMap());
    m_rootModule = nullptr;
}
ModuleManager::~ModuleManager()
{
    //DEBUG_FUNC_NAME
    m_moduleMap.clear();

    if(m_rootModule != nullptr) {
        delete m_rootModule;
        m_rootModule = nullptr;
    }
    if(m_moduleDisplaySettings != nullptr) {
        delete m_moduleDisplaySettings;
        m_moduleDisplaySettings = nullptr;
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
void ModuleManager::newDisplaySettings()
{
    m_moduleDisplaySettings = new ModuleDisplaySettings();
}
ModuleDisplaySettings * ModuleManager::moduleDisplaySetings()
{
    return m_moduleDisplaySettings;
}

/**
  * Load all Modules, and generate a QStandardItemModel.
  */
int ModuleManager::loadAllModules()
{
    DEBUG_FUNC_NAME;
    //update module settings and display settings
    //m_moduleMap.clear();
    //todo: clear and renew
    ModuleSettings *root = m_settings->createRoot();

    foreach(ModuleSettings * set, m_settings->m_moduleSettings) {
        set->clearChildren();
    }
    //set parents
    {
        QMapIterator<int, ModuleSettings*> it(m_settings->m_moduleSettings);
        while(it.hasNext())  {
            it.next();
            ModuleSettings *child = it.value();
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
    }
    //use parent settings display
    {
        QMapIterator<int, ModuleSettings*> it2(m_settings->m_moduleSettings);
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
    m_rootModule->setSettings(m_settings);
    m_rootModule->setModuleID(-1);
    m_rootModule->setModuleClass(ModuleTools::FolderClass);
    m_rootModule->setModuleType(ModuleTools::NoneType);

    ModuleSettings *rootModuleSettings = m_settings->getModuleSettings(-1);//it the invisble root item
    if(rootModuleSettings != nullptr) {
        foreach(ModuleSettings * s, rootModuleSettings->children()) {
            loadModule(m_rootModule, s);
        }

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
    Q_ASSERT(settings != nullptr);
    if(settings == nullptr)
        return;
    Module *module = new Module(parentModule);
    module->setSettings(m_settings);
    module->setPath(settings->modulePath);
    module->setModuleType(settings->moduleType);

    module->setModuleID(settings->moduleID);

    //m_map deletes them
    m_moduleMap->insert(module);

    module->setModuleClass(ModuleTools::typeToClass(settings->moduleType));
    //recursive
    foreach(ModuleSettings * s, settings->children()) {
        loadModule(module, s);
    }

}
void ModuleManager::initVerseModule(VerseModule *m) const
{
    Q_ASSERT(m != nullptr);
    if(m != nullptr) {
        m->setSettings(m_settings);
        m->setNotes(m_notes);
        m->setModuleMap(m_moduleMap);
        m->setModuleDisplaySettings(m_moduleDisplaySettings);
    }
}
void ModuleManager::initSimpleModule(SimpleModuleClass *m) const
{
    Q_ASSERT(m != nullptr);
    if(m != nullptr) {
        m->setSettings(m_settings);
        m->setNotes(m_notes);
        m->setModuleMap(m_moduleMap);
    }
}

bool ModuleManager::dictionaryLoaded(const Dictionary *dict)
{
    if(dict && m_moduleMap->contains(dict->moduleID()) && dict->moduleID() >= 0)
        return true;
    return false;
}
bool ModuleManager::metaModuleLoaded(const SimpleModuleClass *m) const
{
    return (m && m_moduleMap->contains(m->moduleID()) && m->moduleID() >= 0);
}
bool ModuleManager::verseTableLoaded(const VerseTable *table) const
{
    return (table && table->verseModule() && m_moduleMap->contains(table->verseModule()->moduleID()) && table->verseModule()->moduleID() >= 0);
}
bool ModuleManager::contains(const int moduleID)
{
    return m_moduleMap->contains(moduleID);
}

Module * ModuleManager::getModule(const int moduleID)
{
    return m_moduleMap->module(moduleID);
}

/**
  * Converts a persistent url to a link.
  */
QString ModuleManager::notePos2Link(const QString &pos)
{
    VerseUrl url;
    url.fromStringUrl(pos);
    UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::InterfaceUrl, url);
    urlConverter.setSettings(m_settings);
    urlConverter.setModuleMap(m_moduleMap.data());
    VerseUrl newUrl = urlConverter.convert();
    RefText refText(m_settings);
    refText.setShowModuleName(true);
    const QString link = newUrl.toString();
    return "<a href=\"" + link + "\">" + refText.toString(newUrl) + "</a>";
}
QString ModuleManager::notePos2Text(const QString &pos)
{
    VerseUrl url;
    url.fromStringUrl(pos);
    UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::InterfaceUrl, url);
    urlConverter.setSettings(m_settings);
    urlConverter.setModuleMap(m_moduleMap.data());
    VerseUrl newUrl = urlConverter.convert();
    RefText refText(m_settings);
    refText.setShowModuleName(true);

    const QString string = refText.toString(newUrl);
    return string;
}

QStringList ModuleManager::getBiblePaths()
{
    QStringList paths;
    auto i = m_moduleMap->it();
    while(i.hasNext()) {
        i.next();
        if(i.value()->moduleClass() == ModuleTools::BibleModuleClass)
            paths.append(i.value()->path());
    }
    return paths;
}
QList<int> ModuleManager::getBibleIDs()
{
    QList<int> ids;
    auto i = m_moduleMap->it();
    while(i.hasNext()) {
        i.next();
        if(i.value()->moduleClass() == ModuleTools::BibleModuleClass)
            ids.append(i.value()->moduleID());
    }
    return ids;
}
void ModuleManager::checkCache(const int moduleID)
{
    //todo: do we need this method?
    Q_UNUSED(moduleID);
    /*Module *m = m_moduleMap->m_map.value(moduleID);
    if(m->moduleClass() == ModuleTools::BibleModuleClass && !m_settings->m_moduleCache.keys().contains(m->path())) {
        Bible *b = new Bible();
        initVerseModule(b);
        b->setModuleType(m->moduleType());
        b->loadModuleData(moduleID);//set cache
    }*/
}
TextRangesVerseModule * ModuleManager::newTextRangesVerseModule(const int moduleID, QPoint p, VerseTable *table)
{
    if(!contains(moduleID)) {
        myWarning() << "invalid moduleID = " << moduleID;
        Q_ASSERT(false);
        return nullptr;
    }

    TextRangesVerseModule *m = newTextRangesVerseModule(moduleID);
    table->addModule(m, p);
    return m;
}
TextRangesVerseModule * ModuleManager::newTextRangesVerseModule(const int moduleID)
{
    if(!contains(moduleID)) {
        myWarning() << "invalid moduleID = " << moduleID;
        Q_ASSERT(false);
        return nullptr;
    }
    TextRangesVerseModule *m = nullptr;
    //currently we have only bibles
    if(getModule(moduleID)->moduleClass() == ModuleTools::BibleModuleClass) {
        m = new Bible();
        initVerseModule(m);
    } else {
        myWarning() << "not a verse module " << getModule(moduleID)->moduleClass();
        Q_ASSERT(false);
        return nullptr;
    }

    ModuleTools::ModuleType type = getModule(moduleID)->moduleType();
    m->setModuleType(type);
    m->setModuleID(moduleID);
    return m;
}

/**
  * Add also a bible at (0,0)
  */
VerseTable * ModuleManager::newVerseTable()
{
    VerseTable *verseTable = new VerseTable();
    Bible *b = new Bible();

    initVerseModule(b);
    verseTable->addModule(b, QPoint(0, 0));

    return verseTable;
}
bool ModuleManager::sortModuleByPop(const ModuleSettings* s1, const ModuleSettings* s2)
{
    return s1->stats_timesOpened > s2->stats_timesOpened;
}
