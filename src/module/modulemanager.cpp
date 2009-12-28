#include "src/module/modulemanager.h"
#include <QtGui/QProgressDialog>
#include <QtGui/QApplication>
#include <QtCore/QDir>
#include <QtCore/QObject>
#include "src/module/bible.h"
#include "src/module/strong.h"
#include "src/module/module.h"
#include "src/core/dbghelper.h"
ModuleManager::ModuleManager()
{
}
void ModuleManager::setSettings(Settings *settings)
{
    m_settings = settings;
}

int ModuleManager::loadAllModules()
{
    Bible bible;
    bible.setSettings(m_settings);
    m_moduleList.clear();
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
            m_items.append(top);

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
                    //todo: current only bibles loaded
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
                                module.m_iniPath = file.fileName();
                                module.m_moduleType = Module::BibleModule;
                                module.m_moduleClass = Module::BibleQuoteModule;
                                module.m_name = bname;
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
                                module.m_iniPath = file.fileName();
                                module.m_moduleType = Module::BibleModule;
                                module.m_moduleClass = Module::ZefaniaBibleModule;
                                module.m_name = bname;
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
            file.setFileName(m_settings->module.at(i).modulePath);
            if (bibletype != 0 && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                switch (bibletype) {
                case Bible::BibleQuoteModule: {
                    //BibleQuote
                    Module module;
                    module.m_iniPath = file.fileName();
                    module.m_moduleType = Module::BibleModule;
                    module.m_moduleClass = Module::BibleQuoteModule;
                    module.m_name = m_settings->module.at(i).moduleName;
                    module.m_id = rcount;
                    m_moduleList << module;
                    m_settings->moduleID.insert(rcount, i);

                    QTreeWidgetItem *bibleItem = new QTreeWidgetItem;
                    bibleItem->setText(0, m_settings->module.at(i).moduleName);
                    bibleItem->setText(1, QString::number(rcount));
                    qDebug() << "MainWindow::loadModules()m_settings->moduleID rcount = " << rcount << " i = " << i;


                    QIcon bibleIcon;
                    bibleIcon.addPixmap(QPixmap(":/icons/16x16/text-x-generic.png"), QIcon::Normal, QIcon::Off);
                    bibleItem->setIcon(0, bibleIcon);
                    m_items.append(bibleItem);
                    rcount++;
                    break;
                }
                case Bible::ZefaniaBibleModule: {
                    //ZenfaniaXML
                    Module module;
                    module.m_iniPath = file.fileName();
                    module.m_moduleType = Module::BibleModule;
                    module.m_moduleClass = Module::ZefaniaBibleModule;
                    module.m_name = m_settings->module.at(i).moduleName;
                    module.m_id = rcount;
                    m_moduleList << module;
                    m_settings->moduleID.insert(rcount, i);

                    QTreeWidgetItem *bibleItem = new QTreeWidgetItem;
                    bibleItem->setText(0, m_settings->module.at(i).moduleName);
                    bibleItem->setText(1, QString::number(rcount));
                    qDebug() << "MainWindow::loadModules()m_settings->moduleID rcount = " << rcount << " i = " << i;

                    QIcon bibleIcon;
                    bibleIcon.addPixmap(QPixmap(":/icons/16x16/text-xml.png"), QIcon::Normal, QIcon::Off);
                    bibleItem->setIcon(0, bibleIcon);
                    m_items.append(bibleItem);
                    rcount++;
                    break;
                }
                /*case 3: {
                    rcount++;
                }*/

                }
            }
        }
    }
    return 0;
}
