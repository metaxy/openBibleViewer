/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2010 Paul Walger
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
    m_moduleModel = new QStandardItemModel;
    m_moduleMap = new ModuleMap();
    m_dictionary = NULL;
}
ModuleManager::~ModuleManager()
{
    DEBUG_FUNC_NAME
    delete m_moduleMap;
    delete m_moduleModel;
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
QStringList ModuleManager::scan(const QString &path, const int &level = 0)
{
    //todo: make faster using name filter
    QStringList ret;
    QDir dir(path);
    const QFileInfoList list = dir.entryInfoList();
    foreach(const QFileInfo & info, list) {
        if(info.fileName() != ".." && info.fileName() != ".") {
            if(info.isDir()) {
                if(level <= 2)//i think this is ok
                    ret.append(scan(info.absoluteFilePath(), level + 1));
            } else {
                ret.append(info.absoluteFilePath());
            }
        }
    }
    return ret;
}

/**
  Load all Modules, and generate a QStandardItemModel.
  */
int ModuleManager::loadAllModules()
{
    DEBUG_FUNC_NAME
    BibleQuote bq;
    bq.setSettings(m_settings);
    ZefaniaBible zef;
    zef.setSettings(m_settings);
    BibleQuoteDict bqDict;
    bqDict.setSettings(m_settings);

    m_moduleModel->clear();
    FastStart fastStart;
    fastStart.setSettings(m_settings);
    fastStart.load();
    QProgressDialog progress(QObject::tr("Loading Module"), QObject::tr("Cancel"), 0, m_settings->m_moduleSettings.size());
    progress.setWindowModality(Qt::WindowModal);
    int moduleID = 1;//Counter for the Module ID
    Module *root = new Module();
    root->setModuleID(-1);
    root->setModuleClass(Module::FolderClass);
    root->setModuleType(Module::NoneType);

    QStandardItem *parentItem = m_moduleModel->invisibleRootItem();

    QIcon bibleQuoteIcon = QIcon::fromTheme("text-x-generic", QIcon(":/icons/16x16/text-x-generic.png"));
    QStyle *style = QApplication::style();
    QIcon folderIcon;
    folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirClosedIcon), QIcon::Normal, QIcon::Off);
    folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirOpenIcon), QIcon::Normal, QIcon::On);

    QIcon bibleZefaniaIcon =  QIcon::fromTheme("text-xml", QIcon(":/icons/16x16/text-xml.png"));;

    for(int i = 0; i < m_settings->m_moduleSettings.size(); ++i) {  //read all modules
        if(progress.wasCanceled())
            break;
        if(m_settings->m_moduleSettings.at(i).isDir == true) {

            QStandardItem *top = new QStandardItem;

            top->setIcon(folderIcon);
            top->setText(m_settings->m_moduleSettings.at(i).moduleName);
            top->setData("-1");

            parentItem->appendRow(top);

            //search for bible in the dir
            QString rpath = m_settings->m_moduleSettings.at(i).modulePath + "/";
            Module::ModuleType moduleType = Module::NoneType;

            Module *folder = new Module(root);
            folder->setModuleID(moduleID);
            folder->setModuleType(Module::NoneType);
            folder->setModuleClass(Module::FolderClass);
            folder->setTitle(m_settings->m_moduleSettings.at(i).moduleName);
            moduleID++;
            QStringList list;
            QStringList newList;
            bool scaned = false;
            if(fastStart.hasCache(rpath)) {
                list = fastStart.getFileNames(rpath);
                if(list.isEmpty()) {
                    list = scan(rpath);
                    scaned = true;
                }
            } else {
                list = scan(rpath);
                scaned = true;
            }
            foreach(const QString & fileName, list) { //Alle Ordner auslesen
                QFile file;
                moduleType = recognizeModuleType(fileName);

                if(moduleType == Module::NoneType)
                    continue;
                file.setFileName(fileName);

                QString moduleTitle;
                if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    switch(moduleType) {
                    case Module::BibleQuoteModule: {
                        moduleTitle = bq.readInfo(file);
                        if(!moduleTitle.isEmpty()) {
                            Module *module = new Module(folder);
                            module->setPath(file.fileName());
                            module->setModuleClass(Module::BibleModuleClass);
                            module->setModuleType(Module::BibleQuoteModule);
                            module->setTitle(moduleTitle);
                            module->setModuleID(moduleID);
                            m_moduleMap->m_map.insert(moduleID, module);
                            folder->append(module);

                            QStandardItem *bibleItem = new QStandardItem;
                            bibleItem->setText(moduleTitle);
                            bibleItem->setData(QString::number(moduleID));
                            bibleItem->setToolTip(QObject::tr("BibleQuote Module") + " - " + module->path() + " (" + QString::number(module->moduleID()) + ")");

                            bibleItem->setIcon(bibleQuoteIcon);
                            top->appendRow(bibleItem);
                            checkCache(moduleID);
                            m_settings->setModuleIDinMap(moduleID, i);
                            moduleID++;

                        }
                        break;
                    }
                    case Module::ZefaniaBibleModule: {
                        moduleTitle = zef.readInfo(file);
                        if(!moduleTitle.isEmpty()) {

                            Module *module = new Module(folder);
                            module->setPath(file.fileName());
                            module->setModuleClass(Module::BibleModuleClass);
                            module->setModuleType(Module::ZefaniaBibleModule);
                            module->setTitle(moduleTitle);
                            module->setModuleID(moduleID);

                            m_moduleMap->m_map.insert(moduleID, module);
                            folder->append(module);

                            QStandardItem *bibleItem = new QStandardItem;
                            bibleItem->setText(moduleTitle);
                            bibleItem->setData(QString::number(moduleID));
                            bibleItem->setToolTip(QObject::tr("Zefania XML Module") + " - " + module->path() + " (" + QString::number(module->moduleID()) + ")");

                            bibleItem->setIcon(bibleZefaniaIcon);
                            top->appendRow(bibleItem);
                            checkCache(moduleID);
                            m_settings->setModuleIDinMap(moduleID, i);
                            moduleID++;
                        }
                        break;
                    }
                    case Module::BibleQuoteDictModule: {
                        moduleTitle = bqDict.readInfo(file);
                        if(!moduleTitle.isEmpty()) {
                            Module *module = new Module(folder);
                            module->setPath(file.fileName());
                            module->setModuleClass(Module::DictionaryModuleClass);
                            module->setModuleType(Module::BibleQuoteDictModule);
                            module->setTitle(moduleTitle);
                            module->setModuleID(moduleID);

                            m_moduleMap->m_map.insert(moduleID, module);
                            folder->append(module);
                            checkCache(moduleID);
                            m_settings->setModuleIDinMap(moduleID, i);
                            moduleID++;
                            //myDebug() << moduleID << bname;
                        }
                    }
                    default:
                        break;
                    }

                }
                if(!moduleTitle.isEmpty()) {
                    newList << fileName;
                }

            }
            if(scaned)
                fastStart.setFileNames(rpath, newList);
        } else {
            //load module
            int bibletype = m_settings->m_moduleSettings.at(i).moduleType.toInt();
            if(bibletype != 0) {
                switch(bibletype) {
                case Module::BibleQuoteModule: {
                    Module *module = new Module(root);
                    module->setPath(m_settings->m_moduleSettings.at(i).modulePath);
                    module->setModuleClass(Module::BibleModuleClass);
                    module->setModuleType(Module::BibleQuoteModule);
                    module->setTitle(m_settings->m_moduleSettings.at(i).moduleName);
                    module->setModuleID(moduleID);

                    m_moduleMap->m_map.insert(moduleID, module);
                    root->append(module);

                    QStandardItem *bibleItem = new QStandardItem;
                    bibleItem->setText(m_settings->m_moduleSettings.at(i).moduleName);
                    bibleItem->setData(QString::number(moduleID));
                    bibleItem->setToolTip(QObject::tr("BibleQuote Module") + " - " + module->path() + " (" + QString::number(module->moduleID()) + ")");

                    bibleItem->setIcon(bibleQuoteIcon);
                    parentItem->appendRow(bibleItem);
                    checkCache(moduleID);
                    m_settings->setModuleIDinMap(moduleID, i);
                    moduleID++;
                    break;
                }
                case Module::ZefaniaBibleModule: {
                    Module *module = new Module(root);
                    module->setPath(m_settings->m_moduleSettings.at(i).modulePath);
                    module->setModuleClass(Module::BibleModuleClass);
                    module->setModuleType(Module::ZefaniaBibleModule);
                    module->setTitle(m_settings->m_moduleSettings.at(i).moduleName);
                    module->setModuleID(moduleID);

                    m_moduleMap->m_map.insert(moduleID, module);
                    root->append(module);

                    QStandardItem *bibleItem = new QStandardItem;
                    bibleItem->setText(m_settings->m_moduleSettings.at(i).moduleName);
                    bibleItem->setData(QString::number(moduleID));
                    bibleItem->setToolTip(QObject::tr("Zefania XML Module") + " - " + module->path() + " (" + QString::number(module->moduleID()) + ")");

                    bibleItem->setIcon(bibleZefaniaIcon);
                    parentItem->appendRow(bibleItem);
                    checkCache(moduleID);
                    m_settings->setModuleIDinMap(moduleID, i);
                    moduleID++;
                    break;
                }
                case Module::ZefaniaLexModule: {

                    Module *module = new Module(root);
                    module->setPath(m_settings->m_moduleSettings.at(i).modulePath);
                    module->setModuleClass(Module::DictionaryModuleClass);
                    module->setModuleType(Module::ZefaniaLexModule);
                    module->setTitle(m_settings->m_moduleSettings.at(i).moduleName);
                    module->setModuleID(moduleID);

                    m_moduleMap->m_map.insert(moduleID, module);
                    m_settings->setModuleIDinMap(moduleID, i);
                    moduleID++;
                    break;
                }
                case Module::TheWordBibleModule: {
                    Module *module = new Module(root);
                    module->setPath(m_settings->m_moduleSettings.at(i).modulePath);
                    module->setModuleClass(Module::BibleModuleClass);
                    module->setModuleType(Module::TheWordBibleModule);
                    module->setTitle(m_settings->m_moduleSettings.at(i).moduleName);
                    module->setModuleID(moduleID);

                    m_moduleMap->m_map.insert(moduleID, module);
                    root->append(module);

                    QStandardItem *bibleItem = new QStandardItem;
                    bibleItem->setText(m_settings->m_moduleSettings.at(i).moduleName);
                    bibleItem->setData(QString::number(moduleID));
                    bibleItem->setToolTip(QObject::tr("The Word Bible Module") + " - " + module->path() + " (" + QString::number(module->moduleID()) + ")");

                    bibleItem->setIcon(bibleZefaniaIcon);
                    parentItem->appendRow(bibleItem);
                    checkCache(moduleID);
                    m_settings->setModuleIDinMap(moduleID, i);
                    moduleID++;
                    break;
                }

                }
            }
        }
    }
    if(fastStart.changed())
        fastStart.save();
    return 0;
}
void ModuleManager::initBible(Bible *b)
{
    if(b != 0) {
        b->setSettings(m_settings);
        b->setNotes(m_notes);
        b->setModuleMap(m_moduleMap);
        b->setBibleDisplaySettings(m_bibleDisplaySettings);
    }
}

/**
  Returns true, if a bible is loaded.
  */
bool ModuleManager::bibleLoaded()
{
    if(m_moduleMap->m_map.contains(bible()->moduleID()) && bible()->moduleID() >= 0)
        return true;
    return false;
}
/**
  Returns true, if a strong module is loaded.
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

Bible * ModuleManager::bible()
{
    return bibleList()->bible();
}
BibleList * ModuleManager::bibleList()
{
    return m_bibleList;
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
    const QString string = url.getParam("b0") + " " + QString::number(newUrl.ranges().first().startChapterID() + 1) + QString::number(newUrl.ranges().first().activeVerseID() + 1);

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
    const QString string = url.getParam("b0") + " " + QString::number(newUrl.ranges().first().startChapterID() + 1) + QString::number(newUrl.ranges().first().activeVerseID() + 1);
    return string;
}
QStringList ModuleManager::getBibleTitles()
{
    QStringList titles;
    QMapIterator<int, Module *> i(m_moduleMap->m_map);
    while(i.hasNext()) {
        i.next();
        if(i.value()->moduleClass() == Module::BibleModuleClass)
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
        if(i.value()->moduleClass() == Module::BibleModuleClass)
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
        if(i.value()->moduleClass() == Module::BibleModuleClass)
            ids.append(i.value()->moduleID());
    }
    return ids;
}
void ModuleManager::checkCache(const int &moduleID)
{
    Module *m = m_moduleMap->m_map.value(moduleID);
    if(m->moduleClass() == Module::BibleModuleClass && !m_settings->m_moduleCache.keys().contains(m->path())) {
        Bible *b = new Bible();
        initBible(b);
        b->setModuleType(m->moduleType());
        b->loadModuleData(moduleID);//set cache
    }

}
Bible * ModuleManager::newBible(const int &moduleID, QPoint p)
{
    if(!contains(moduleID)) {
        myWarning() << "invalid moduleID = " << moduleID;
        return NULL;
    }
    int id = bibleList()->m_biblePoints.key(p, -1);
    Bible *b;
    if(id != -1) {
        b = bibleList()->bible(id);
        initBible(b);
    } else {
        b = new Bible();
        initBible(b);
    }
    //todo: check if this is possible
    /*if(b->moduleID() != moduleID) {*/
    Module::ModuleType type = getModule(moduleID)->moduleType();
    b->setModuleType(type);
    b->setModuleID(moduleID);
    b->loadModuleData(moduleID);
    bibleList()->addBible(b, p);
    /*}*/
    return b;
}
Module::ModuleType ModuleManager::recognizeModuleType(const QString &fileName)
{
    myDebug() << fileName;
    if(fileName.endsWith("bibleqt.ini", Qt::CaseInsensitive)) {
        return Module::BibleQuoteModule;
    } else if(fileName.endsWith(".xml", Qt::CaseInsensitive)) {
        QFile data(fileName);
        if(data.open(QFile::ReadOnly)) {
            QString fileData = "";
            QTextStream in(&data);
            for(int i = 0; i < 10; i++)
                fileData += in.readLine();
            myDebug() << "fileData = " << fileData;
            if(fileData.contains("XMLBIBLE", Qt::CaseInsensitive) && !(fileData.contains("x-quran", Qt::CaseInsensitive) || // i cannot allow this
                    fileData.contains("x-cult", Qt::CaseInsensitive) ||
                    fileData.contains("x-mormon", Qt::CaseInsensitive))) {
                return Module::ZefaniaBibleModule;
            } else if(fileData.contains("<dictionary", Qt::CaseInsensitive)) {
                return Module::ZefaniaLexModule;
            }

        }
    } else if(fileName.endsWith(".idx", Qt::CaseInsensitive)) {
        return Module::BibleQuoteDictModule;
    } else if(fileName.endsWith(".nt", Qt::CaseInsensitive) || fileName.endsWith(".ot", Qt::CaseInsensitive) || fileName.endsWith(".ont", Qt::CaseInsensitive)) {
        myDebug() << "the word module";
        return Module::TheWordBibleModule;
    }
    return Module::NoneType;
}
