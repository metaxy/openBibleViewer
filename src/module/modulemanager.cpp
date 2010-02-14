/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009 Paul Walger
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
#include "src/module/bible.h"
#include "src/module/strong.h"
#include "src/module/module.h"
#include "src/core/dbghelper.h"
#include "src/core/urlconverter.h"
ModuleManager::ModuleManager()
{
}
void ModuleManager::setSettings(Settings *settings)
{
    //DEBUG_FUNC_NAME
    m_settings = settings;
}
/**
  Load all Modules.
  */
int ModuleManager::loadAllModules()
{
    //DEBUG_FUNC_NAME
    Bible bible;
    bible.setSettings(m_settings);
    m_moduleList.clear();
    m_bibleItems.clear();
    QProgressDialog progress(QObject::tr("Loading Module"), QObject::tr("Cancel"), 0, m_settings->module.size());
    progress.setWindowModality(Qt::WindowModal);
    int rcount = 0;//Counter for the Bible ID
    for (int i = 0; i < m_settings->module.size(); ++i) { //real all modules
        if (progress.wasCanceled())
            break;
        if (m_settings->module.at(i).isDir == true) {
            int uModuleCount = 0;
            QTreeWidgetItem *top = new QTreeWidgetItem;
            QStyle *style = QApplication::style();
            QIcon folderIcon;
            folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirClosedIcon), QIcon::Normal, QIcon::Off);
            folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirOpenIcon), QIcon::Normal, QIcon::On);
            top->setIcon(0, folderIcon);
            top->setText(0, m_settings->module.at(i).moduleName);
            top->setText(1, "-1");
            m_bibleItems.append(top);

            //search for bible in the dir
            QString rpath = m_settings->module.at(i).modulePath + "/";
            int bibletype;
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
                    if (bibletype != 0 && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                        QString bname;
                        switch (bibletype) {
                        case Bible::BibleQuoteModule: {
                            bname = bible.bq.readInfo(file);
                            if (bname.size() > 0) {
                                Module module;
                                module.m_path = file.fileName();
                                module.m_moduleClass = Module::BibleModule;
                                module.m_moduleType = Module::BibleQuoteModule;
                                module.m_title = bname;
                                module.m_id = rcount;
                                m_moduleList << module;

                                QTreeWidgetItem *ibible = new QTreeWidgetItem();
                                ibible->setText(0, bname);
                                ibible->setText(1, QString::number(rcount));
                                QIcon bibleIcon;
                                bibleIcon.addPixmap(QPixmap(":/icons/16x16/text-x-generic.png"), QIcon::Normal, QIcon::Off);
                                ibible->setIcon(0, bibleIcon);
                                top->addChild(ibible);

                                m_settings->moduleID.insert(rcount, i);

                                rcount++;
                                uModuleCount++;
                            }
                            break;
                        }
                        case Bible::ZefaniaBibleModule: {
                            //ZenfaniaXML-Bible
                            bname = bible.zef.readInfo(file);
                            if (bname.size() > 0) {

                                Module module;
                                module.m_path = file.fileName();
                                module.m_moduleClass = Module::BibleModule;
                                module.m_moduleType = Module::ZefaniaBibleModule;
                                module.m_title = bname;
                                module.m_id = rcount;
                                m_moduleList << module;

                                QTreeWidgetItem *bibleItem = new QTreeWidgetItem();
                                bibleItem->setText(0, bname);
                                bibleItem->setText(1, QString::number(rcount));


                                QIcon bibleIcon;
                                bibleIcon.addPixmap(QPixmap(":/icons/16x16/text-xml.png"), QIcon::Normal, QIcon::Off);
                                bibleItem->setIcon(0, bibleIcon);
                                top->addChild(bibleItem);
                                m_settings->moduleID.insert(rcount, i);
                                rcount++;
                                uModuleCount++;
                            }
                            break;
                        }
                        }

                    }
                }
            }
            if (m_settings->module.at(i).uModuleCount != uModuleCount) {
                m_settings->clearNamesCache(i); //ModuleID
            }
            ModuleSettings m = m_settings->module.at(i);
            m.uModuleCount = uModuleCount;
            m_settings->module.replace(i, m);
        } else {
            //load module
            QFile file;
            int bibletype = m_settings->module.at(i).moduleType.toInt();
            if (bibletype != 0) {
                switch (bibletype) {
                case Module::BibleQuoteModule: {
                    //BibleQuote
                    Module module;
                    module.m_path = m_settings->module.at(i).modulePath;
                    module.m_moduleClass = Module::BibleModule;
                    module.m_moduleType = Module::BibleQuoteModule;
                    module.m_title = m_settings->module.at(i).moduleName;
                    module.m_id = rcount;
                    m_moduleList << module;
                    m_settings->moduleID.insert(rcount, i);

                    QTreeWidgetItem *bibleItem = new QTreeWidgetItem;
                    bibleItem->setText(0, m_settings->module.at(i).moduleName);
                    bibleItem->setText(1, QString::number(rcount));
                    myDebug() << "m_settings->moduleID rcount = " << rcount << " i = " << i;


                    QIcon bibleIcon;
                    bibleIcon.addPixmap(QPixmap(":/icons/16x16/text-x-generic.png"), QIcon::Normal, QIcon::Off);
                    bibleItem->setIcon(0, bibleIcon);
                    m_bibleItems.append(bibleItem);
                    rcount++;
                    break;
                }
                case Module::ZefaniaBibleModule: {
                    //ZenfaniaXML
                    Module module;
                    module.m_path = m_settings->module.at(i).modulePath;
                    module.m_moduleClass = Module::BibleModule;
                    module.m_moduleType = Module::ZefaniaBibleModule;
                    module.m_title = m_settings->module.at(i).moduleName;
                    module.m_id = rcount;
                    m_moduleList << module;
                    m_settings->moduleID.insert(rcount, i);

                    QTreeWidgetItem *bibleItem = new QTreeWidgetItem;
                    bibleItem->setText(0, m_settings->module.at(i).moduleName);
                    bibleItem->setText(1, QString::number(rcount));
                    myDebug() << "MainWindow::loadModules()m_settings->moduleID rcount = " << rcount << " i = " << i;

                    QIcon bibleIcon;
                    bibleIcon.addPixmap(QPixmap(":/icons/16x16/text-xml.png"), QIcon::Normal, QIcon::Off);
                    bibleItem->setIcon(0, bibleIcon);
                    m_bibleItems.append(bibleItem);
                    rcount++;
                    break;
                }
                case Module::ZefaniaStrongModule: {
                    myDebug() << "Strong added";
                    Module module;
                    module.m_path = m_settings->module.at(i).modulePath;
                    module.m_moduleClass = Module::StrongModule;
                    module.m_moduleType = Module::ZefaniaStrongModule;
                    module.m_title = m_settings->module.at(i).moduleName;
                    module.m_id = rcount;
                    m_moduleList << module;
                    m_settings->moduleID.insert(rcount, i);
                    rcount++;
                    break;
                }

                }
            }
        }
    }

    //todo: its ugly
    QStringList iniPath;
    for (int i = 0; i < m_moduleList.size(); ++i) {
        //if(m_moduleList.at(i).m_moduleClass == Module::BibleModule)
        iniPath << m_moduleList.at(i).m_path;
    }
    m_bible.biblesIniPath = iniPath;
    return 0;
}
bool ModuleManager::bibleLoaded()
{
    if (m_moduleList.size() > m_bible.bibleID() && m_bible.bibleID() >= 0)
        return true;
    return false;
}
bool ModuleManager::strongLoaded()
{
    if (m_moduleList.size() > m_strong.m_strongModuleID &&  m_strong.m_strongModuleID >= 0)
        return true;
    return false;
}
QString ModuleManager::notePos2Text(const QString &pos)
{
    //DEBUG_FUNC_NAME

    myDebug() << "start pos = " << pos;
    QString string = "";
    UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::InterfaceUrl, pos);
    urlConverter.m_biblesIniPath = m_bible.biblesIniPath;
    urlConverter.pharse();
    QString link = urlConverter.convert();

    string =  urlConverter.m_bookName + " " + QString::number(urlConverter.m_chapterID + 1) + "," + QString::number(urlConverter.m_verseID + 1);
    return  "<a href=\"" + link + "\" > " + string + "</a>";
}
