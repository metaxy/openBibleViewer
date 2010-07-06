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
#include "src/module/strong.h"
#include "src/module/module.h"
#include "src/module/modulemap.h"
#include "src/core/dbghelper.h"
#include "src/core/urlconverter.h"
ModuleManager::ModuleManager()
{
    m_moduleModel = new QStandardItemModel;
    m_bible = new Bible();
    m_moduleMap = new ModuleMap();
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
    DEBUG_FUNC_NAME
    BibleQuote bq;
    bq.setSettings(m_settings);
    ZefaniaBible zef;
    zef.setSettings(m_settings);


    m_moduleModel->clear();

    QProgressDialog progress(QObject::tr("Loading Module"), QObject::tr("Cancel"), 0, m_settings->m_moduleSettings.size());
    progress.setWindowModality(Qt::WindowModal);
    int moduleID = 0;//Counter for the Module ID

    QStandardItem *parentItem = m_moduleModel->invisibleRootItem();
    QIcon bibleQuoteIcon = QIcon::fromTheme("text-x-generic",QIcon(":/icons/16x16/text-x-generic.png"));
    QStyle *style = QApplication::style();
    QIcon folderIcon;
    folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirClosedIcon), QIcon::Normal, QIcon::Off);
    folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirOpenIcon), QIcon::Normal, QIcon::On);

    QIcon bibleZefaniaIcon =  QIcon::fromTheme("text-xml",QIcon(":/icons/16x16/text-xml.png"));;

    for (int i = 0; i < m_settings->m_moduleSettings.size(); ++i) { //real all modules
        if (progress.wasCanceled())
            break;
        if (m_settings->m_moduleSettings.at(i).isDir == true) {

            QStandardItem *top = new QStandardItem;

            top->setIcon(folderIcon);
            top->setText(m_settings->m_moduleSettings.at(i).moduleName);
            top->setData("-1");

            parentItem->appendRow(top);

            //search for bible in the dir
            QString rpath = m_settings->m_moduleSettings.at(i).modulePath + "/";
            int bibletype = Bible::None;
            QDir dir(rpath);
            dir.setFilter(QDir::Dirs);
            QFileInfoList list = dir.entryInfoList();

            for (int fileCounter = 0; fileCounter < list.size(); ++fileCounter) { //Alle Ordner auslesen
                QFileInfo fileInfo = list.at(fileCounter);
                QString dirname = fileInfo.fileName();
                if (dirname != "." && dirname != "..") {
                    QFile file;
                    QString rfile;
                    bibletype = Bible::None;
                    file.setFileName(rpath + dirname + "/" + "BIBLEQT.INI");
                    if (file.exists()) {
                        rfile = file.fileName();
                        bibletype = Bible::BibleQuoteModule;
                    }
                    file.setFileName(rpath + dirname + "/" + "BIBLEQT.ini");
                    if (bibletype == 0 && file.exists()) {
                        rfile = file.fileName();
                        bibletype = Bible::BibleQuoteModule;
                    }
                    file.setFileName(rpath + dirname + "/" + "bibleqt.ini");
                    if (bibletype == 0 && file.exists()) {
                        rfile = file.fileName();
                        bibletype = Bible::BibleQuoteModule;
                    }
                    file.setFileName(rpath + dirname + "/" + dirname + ".xml");
                    if (bibletype == 0 && file.exists()) {
                        rfile = file.fileName();
                        bibletype = Bible::ZefaniaBibleModule;
                    }
                    file.setFileName(rfile);
                    //todo: add a folder module and it should be the parent of all it childrens
                    if (bibletype != 0 && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                        QString bname;
                        switch (bibletype) {
                        case Bible::BibleQuoteModule: {
                            bname = bq.readInfo(file);
                            if (bname.size() > 0) {
                                Module *module = new Module();
                                module->m_path = file.fileName();
                                module->m_moduleClass = Module::BibleModule;
                                module->m_moduleType = Module::BibleQuoteModule;
                                module->m_title = bname;
                                module->m_id = moduleID;
                                m_moduleMap->m_map.insert(moduleID,module);

                                QStandardItem *bibleItem = new QStandardItem;
                                bibleItem->setText(bname);
                                bibleItem->setData(QString::number(moduleID));
                                bibleItem->setToolTip(QObject::tr("BibleQuote Module") + " - " + module->m_path + " ("+QString::number(module->m_id)+")");

                                bibleItem->setIcon(bibleQuoteIcon);
                                top->appendRow(bibleItem);

                                moduleID++;

                            }
                            break;
                        }
                        case Bible::ZefaniaBibleModule: {
                            //ZenfaniaXML-Bible
                            bname = zef.readInfo(file);
                            if (bname.size() > 0) {

                                Module *module = new Module();
                                module->m_path = file.fileName();
                                module->m_moduleClass = Module::BibleModule;
                                module->m_moduleType = Module::ZefaniaBibleModule;
                                module->m_title = bname;
                                module->m_id = moduleID;
                                m_moduleMap->m_map.insert(moduleID,module);

                                QStandardItem *bibleItem = new QStandardItem;
                                bibleItem->setText(bname);
                                bibleItem->setData(QString::number(moduleID));
                                bibleItem->setToolTip(QObject::tr("Zefania XML Module") + " - " + module->m_path + " ("+QString::number(module->m_id)+")");

                                bibleItem->setIcon(bibleZefaniaIcon);
                                top->appendRow(bibleItem);
                                moduleID++;
                            }
                            break;
                        }
                        }

                    }
                }
            }
        } else {
            //load module
            int bibletype = m_settings->m_moduleSettings.at(i).moduleType.toInt();
            if (bibletype != 0) {
                switch (bibletype) {
                case Module::BibleQuoteModule: {
                    //BibleQuote
                    Module *module = new Module();
                    module->m_path = m_settings->m_moduleSettings.at(i).modulePath;
                    module->m_moduleClass = Module::BibleModule;
                    module->m_moduleType = Module::BibleQuoteModule;
                    module->m_title = m_settings->m_moduleSettings.at(i).moduleName;
                    module->m_id = moduleID;

                    m_moduleMap->m_map.insert(moduleID,module);

                    QStandardItem *bibleItem = new QStandardItem;
                    bibleItem->setText(m_settings->m_moduleSettings.at(i).moduleName);
                    bibleItem->setData(QString::number(moduleID));
                    bibleItem->setToolTip(QObject::tr("BibleQuote Module") + " - " + module->m_path + " ("+QString::number(module->m_id)+")");

                    bibleItem->setIcon(bibleQuoteIcon);
                    parentItem->appendRow(bibleItem);

                    moduleID++;
                    break;
                }
                case Module::ZefaniaBibleModule: {
                    //ZenfaniaXML
                    Module *module = new Module();
                    module->m_path = m_settings->m_moduleSettings.at(i).modulePath;
                    module->m_moduleClass = Module::BibleModule;
                    module->m_moduleType = Module::ZefaniaBibleModule;
                    module->m_title = m_settings->m_moduleSettings.at(i).moduleName;
                    module->m_id = moduleID;

                    m_moduleMap->m_map.insert(moduleID,module);

                    QStandardItem *bibleItem = new QStandardItem;
                    bibleItem->setText(m_settings->m_moduleSettings.at(i).moduleName);
                    bibleItem->setData(QString::number(moduleID));
                    bibleItem->setToolTip(QObject::tr("Zefania XML Module") + " - " + module->m_path + " ("+QString::number(module->m_id)+")");

                    bibleItem->setIcon(bibleZefaniaIcon);
                    parentItem->appendRow(bibleItem);
                    moduleID++;
                    break;
                }
                case Module::ZefaniaStrongModule: {

                    Module *module = new Module();
                    module->m_path = m_settings->m_moduleSettings.at(i).modulePath;
                    module->m_moduleClass = Module::StrongModule;
                    module->m_moduleType = Module::ZefaniaStrongModule;
                    module->m_title = m_settings->m_moduleSettings.at(i).moduleName;
                    module->m_id = moduleID;

                    m_moduleMap->m_map.insert(moduleID,module);
                    moduleID++;
                    break;
                }

                }
            }
        }
    }
    return 0;
}
void ModuleManager::initBible()
{
    m_bible->setSettings(m_settings);
    m_bible->setNotes(m_notes);
    m_bible->setBiblesRootPath(m_iniPath);
    m_bible->setModuleMap(m_moduleMap);
    m_bible->setBibleDisplaySettings(m_bibleDisplaySettings);
}

/**
  Returns true, if a bible is loaded.
  */
bool ModuleManager::bibleLoaded()
{
    if (m_moduleMap->m_map.contains(m_bible->moduleID()) && m_bible->moduleID() >= 0)
        return true;
    return false;
}
/**
  Returns true, if a strong module is loaded.
  */
bool ModuleManager::strongLoaded()
{
    if (m_moduleMap->m_map.contains(m_strong.m_strongModuleID)  &&  m_strong.m_strongModuleID >= 0)
        return true;
    return false;
}
bool ModuleManager::contains(const int &moduleID)
{
    return m_moduleMap->m_map.contains(moduleID);
}

Bible * ModuleManager::bible()
{
    //m_bible->setModuleMap(m_map);//todo: avoid this
    return m_bible;
}
BibleList * ModuleManager::bibleList()
{
    return m_bibleList;
}
Module * ModuleManager::getModule(const int &moduleID)
{
    return m_moduleMap->m_map.value(moduleID);
}

/**
  Converts a PersistentUrl to a link.
  */
QString ModuleManager::notePos2Link(const QString &pos)
{
    //DEBUG_FUNC_NAME
    QString string = "";
    UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::InterfaceUrl, pos);
    urlConverter.m_biblesRootPath = m_bible->biblesRootPath();
    urlConverter.pharse();
    QString link = urlConverter.convert();

    string =  urlConverter.m_bookName + " " + QString::number(urlConverter.m_chapterID + 1) + "," + QString::number(urlConverter.m_verseID + 1);
    return  "<a href=\"" + link + "\" >" + string + "</a>";
}
QString ModuleManager::notePos2Text(const QString &pos)
{
    //DEBUG_FUNC_NAME
    QString string = "";
    UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::InterfaceUrl, pos);
    urlConverter.m_biblesRootPath = m_bible->biblesRootPath();
    urlConverter.pharse();
    QString link = urlConverter.convert();

    string =  urlConverter.m_bookName + " " + QString::number(urlConverter.m_chapterID + 1) + "," + QString::number(urlConverter.m_verseID + 1);
    return string;
}
