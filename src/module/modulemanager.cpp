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
#include "src/module/modulemanager.h"
#include <QtGui/QProgressDialog>
#include <QtGui/QApplication>
#include <QtCore/QDir>
#include <QtCore/QObject>
#include <QStandardItem>
#include "src/module/bible.h"
#include "src/module/module.h"
#include "src/module/modulemap.h"
#include "src/core/dbghelper.h"
#include "src/core/urlconverter.h"
#include "src/core/faststart.h"
ModuleManager::ModuleManager()
{
    m_moduleModel = new QStandardItemModel;
    m_moduleMap = new ModuleMap();
}
ModuleManager::~ModuleManager()
{
    DEBUG_FUNC_NAME
    delete m_moduleMap;
    delete m_moduleModel;
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
    foreach(QFileInfo info, list) {
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
    root->m_id = 0;
    root->m_moduleClass = Module::FolderClass;
    root->m_moduleType = Module::NoneType;

    QStandardItem *parentItem = m_moduleModel->invisibleRootItem();

    QIcon bibleQuoteIcon = QIcon::fromTheme("text-x-generic", QIcon(":/icons/16x16/text-x-generic.png"));
    QStyle *style = QApplication::style();
    QIcon folderIcon;
    folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirClosedIcon), QIcon::Normal, QIcon::Off);
    folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirOpenIcon), QIcon::Normal, QIcon::On);

    QIcon bibleZefaniaIcon =  QIcon::fromTheme("text-xml", QIcon(":/icons/16x16/text-xml.png"));;

    for(int i = 0; i < m_settings->m_moduleSettings.size(); ++i) {  //real all modules
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
            folder->m_moduleClass = Module::FolderClass;
            folder->m_moduleType = Module::NoneType;
            folder->m_id = moduleID;
            folder->m_title = m_settings->m_moduleSettings.at(i).moduleName;
            moduleID++;
            QStringList list;
            bool scaned = false;
            if(fastStart.hasCache(rpath)) {
                list = fastStart.getFileNames(rpath);
            } else {
                list = scan(rpath);
                scaned = true;
            }
            foreach(QString fileName, list) { //Alle Ordner auslesen
                QFile file;
                moduleType = Module::NoneType;
                if(fileName.compare(fileName, "bibleqt.ini", Qt::CaseInsensitive) == 0) {
                    moduleType = Module::BibleQuoteModule;
                } else  if(fileName.endsWith(".xml", Qt::CaseInsensitive)) {
                    moduleType = Module::ZefaniaBibleModule;//todo: cannot detect other xml files
                } else  if(fileName.endsWith(".idx", Qt::CaseInsensitive) ) {
                    moduleType = Module::BibleQuoteDictModule;//todo: cannot detect other xml files
                }

                file.setFileName(fileName);
                if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    QString bname;
                    switch(moduleType) {
                    case Module::BibleQuoteModule: {
                        bname = bq.readInfo(file);
                        if(!bname.isEmpty()) {
                            Module *module = new Module(folder);
                            module->m_path = file.fileName();
                            module->m_moduleClass = Module::BibleModule;
                            module->m_moduleType = Module::BibleQuoteModule;
                            module->m_title = bname;
                            module->m_id = moduleID;
                            m_moduleMap->m_map.insert(moduleID, module);
                            folder->append(module);

                            QStandardItem *bibleItem = new QStandardItem;
                            bibleItem->setText(bname);
                            bibleItem->setData(QString::number(moduleID));
                            bibleItem->setToolTip(QObject::tr("BibleQuote Module") + " - " + module->m_path + " (" + QString::number(module->m_id) + ")");

                            bibleItem->setIcon(bibleQuoteIcon);
                            top->appendRow(bibleItem);
                            checkCache(moduleID);
                            m_settings->setModuleIDinMap(moduleID, i);
                            moduleID++;

                        }
                        break;
                    }
                    case Module::ZefaniaBibleModule: {
                        bname = zef.readInfo(file);
                        if(!bname.isEmpty()) {

                            Module *module = new Module(folder);
                            module->m_path = file.fileName();
                            module->m_moduleClass = Module::BibleModule;
                            module->m_moduleType = Module::ZefaniaBibleModule;
                            module->m_title = bname;
                            module->m_id = moduleID;
                            m_moduleMap->m_map.insert(moduleID, module);
                            folder->append(module);

                            QStandardItem *bibleItem = new QStandardItem;
                            bibleItem->setText(bname);
                            bibleItem->setData(QString::number(moduleID));
                            bibleItem->setToolTip(QObject::tr("Zefania XML Module") + " - " + module->m_path + " (" + QString::number(module->m_id) + ")");

                            bibleItem->setIcon(bibleZefaniaIcon);
                            top->appendRow(bibleItem);
                            checkCache(moduleID);
                            m_settings->setModuleIDinMap(moduleID, i);
                            moduleID++;
                        }
                        break;
                    }
                    case Module::BibleQuoteDictModule: {
                        bname = bqDict.readInfo(file);
                        if(!bname.isEmpty()) {
                            Module *module = new Module(folder);
                            module->m_path = file.fileName();
                            module->m_moduleClass = Module::DictionaryModule;
                            module->m_moduleType = Module::BibleQuoteDictModule;
                            module->m_title = bname;
                            module->m_id = moduleID;
                            m_moduleMap->m_map.insert(moduleID, module);
                            folder->append(module);
                            checkCache(moduleID);
                            m_settings->setModuleIDinMap(moduleID, i);
                            moduleID++;
                            myDebug() << moduleID << bname;
                        }
                    }
                    default:
                        break;
                    }

                }

            }
            if(scaned)
                fastStart.setFileNames(rpath, list);
        } else {
            //load module
            int bibletype = m_settings->m_moduleSettings.at(i).moduleType.toInt();
            if(bibletype != 0) {
                switch(bibletype) {
                case Module::BibleQuoteModule: {
                    Module *module = new Module(root);
                    module->m_path = m_settings->m_moduleSettings.at(i).modulePath;
                    module->m_moduleClass = Module::BibleModule;
                    module->m_moduleType = Module::BibleQuoteModule;
                    module->m_title = m_settings->m_moduleSettings.at(i).moduleName;
                    module->m_id = moduleID;

                    m_moduleMap->m_map.insert(moduleID, module);
                    root->append(module);

                    QStandardItem *bibleItem = new QStandardItem;
                    bibleItem->setText(m_settings->m_moduleSettings.at(i).moduleName);
                    bibleItem->setData(QString::number(moduleID));
                    bibleItem->setToolTip(QObject::tr("BibleQuote Module") + " - " + module->m_path + " (" + QString::number(module->m_id) + ")");

                    bibleItem->setIcon(bibleQuoteIcon);
                    parentItem->appendRow(bibleItem);
                    checkCache(moduleID);
                    m_settings->setModuleIDinMap(moduleID, i);
                    moduleID++;
                    break;
                }
                case Module::ZefaniaBibleModule: {
                    Module *module = new Module(root);
                    module->m_path = m_settings->m_moduleSettings.at(i).modulePath;
                    module->m_moduleClass = Module::BibleModule;
                    module->m_moduleType = Module::ZefaniaBibleModule;
                    module->m_title = m_settings->m_moduleSettings.at(i).moduleName;
                    module->m_id = moduleID;

                    m_moduleMap->m_map.insert(moduleID, module);
                    root->append(module);

                    QStandardItem *bibleItem = new QStandardItem;
                    bibleItem->setText(m_settings->m_moduleSettings.at(i).moduleName);
                    bibleItem->setData(QString::number(moduleID));
                    bibleItem->setToolTip(QObject::tr("Zefania XML Module") + " - " + module->m_path + " (" + QString::number(module->m_id) + ")");

                    bibleItem->setIcon(bibleZefaniaIcon);
                    parentItem->appendRow(bibleItem);
                    checkCache(moduleID);
                    m_settings->setModuleIDinMap(moduleID, i);
                    moduleID++;
                    break;
                }
                case Module::ZefaniaLexModule: {

                    Module *module = new Module(root);
                    module->m_path = m_settings->m_moduleSettings.at(i).modulePath;
                    module->m_moduleClass = Module::DictionaryModule;
                    module->m_moduleType = Module::ZefaniaLexModule;
                    module->m_title = m_settings->m_moduleSettings.at(i).moduleName;
                    module->m_id = moduleID;
                    myDebug() << "zefania " << module->m_title << module->m_id;
                    m_moduleMap->m_map.insert(moduleID, module);
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
    QString string;
    UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::InterfaceUrl, pos);
    urlConverter.setSettings(m_settings);
    urlConverter.setModuleMap(m_moduleMap);
    urlConverter.pharse();
    QString link = urlConverter.convert();

    string =  urlConverter.m_bookName + " " + QString::number(urlConverter.m_chapterID + 1) + ":" + QString::number(urlConverter.m_verseID + 1);
    return "<a href=\"" + link + "\" >" + string + "</a>";
}
QString ModuleManager::notePos2Text(const QString &pos)
{
    QString string;
    UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::InterfaceUrl, pos);
    urlConverter.setSettings(m_settings);
    urlConverter.setModuleMap(m_moduleMap);
    urlConverter.pharse();
    urlConverter.convert();

    string = urlConverter.m_bookName + " " + QString::number(urlConverter.m_chapterID + 1) + ":" + QString::number(urlConverter.m_verseID + 1);
    return string;
}
QStringList ModuleManager::getBibleTitles()
{
    QStringList titles;
    QMapIterator<int, Module *> i(m_moduleMap->m_map);
    while(i.hasNext()) {
        i.next();
        if(i.value()->m_moduleClass == Module::BibleModule)
            titles.append(i.value()->m_title);
    }
    return titles;
}
QStringList ModuleManager::getBiblePaths()
{
    QStringList paths;
    QMapIterator<int, Module *> i(m_moduleMap->m_map);
    while(i.hasNext()) {
        i.next();
        if(i.value()->m_moduleClass == Module::BibleModule)
            paths.append(i.value()->m_path);
    }
    return paths;
}
QList<int> ModuleManager::getBibleIDs()
{
    QList<int> ids;
    QMapIterator<int, Module *> i(m_moduleMap->m_map);
    while(i.hasNext()) {
        i.next();
        if(i.value()->m_moduleClass == Module::BibleModule)
            ids.append(i.value()->m_id);
    }
    return ids;
}
void ModuleManager::checkCache(const int &moduleID)
{
    Module* m = m_moduleMap->m_map.value(moduleID);
    if(!m_settings->m_moduleCache.keys().contains(m->m_path)) {
        Bible *b = new Bible();
        initBible(b);
        b->setModuleType(m->m_moduleType);
        b->loadModuleData(moduleID);//set cache
    }

}
Bible * ModuleManager::newBible(const int &moduleID, QPoint p)
{
    if(!contains(moduleID)) {
        myWarning() << "invalid moduleID = " << moduleID;
        return 0;
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
    Module::ModuleType type = getModule(moduleID)->m_moduleType;
    b->setModuleType(type);
    b->loadModuleData(moduleID);
    bibleList()->addBible(b, p);
    /*}*/
    return b;
}
