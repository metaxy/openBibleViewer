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
#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "src/module/dictionary/biblequote-dict.h"
#include "src/module/dictionary/zefania-lex.h"
#include "src/module/bible/zefania-bible.h"
#include "src/module/bible/biblequote.h"
#include "src/module/bible/thewordbible.h"
#include "src/module/webpage.h"
#include "src/module/dictionary/webdictionary.h"
#include "src/module/commentary/webcommentary.h"
#include <QtCore/QFSFileEngine>
#include <QtCore/QPointer>
#include "src/core/qzipreader_p.h"
#ifdef BUILD_WITH_SWORD
#include <stdlib.h>
#include <swmgr.h>
#include <swmodule.h>
#include <markupfiltmgr.h>

using namespace::sword;
#endif
SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent), m_ui(new Ui::SettingsDialog)
{
    m_ui->setupUi(this);
    m_modifedModuleSettings = false;

    connect(m_ui->pushButton_downloadModule, SIGNAL(clicked()), this, SLOT(downloadModule()));
    connect(m_ui->pushButton_addVFolder, SIGNAL(clicked()), this, SLOT(addVirtualFolder()));
    connect(m_ui->pushButton_addFile, SIGNAL(clicked()), this, SLOT(addModuleFile()));
    connect(m_ui->pushButton_addDir, SIGNAL(clicked()), this, SLOT(addModuleDir()));
    connect(m_ui->pushButton_remove_module, SIGNAL(clicked()), this, SLOT(removeModule()));
    connect(m_ui->pushButton_ok, SIGNAL(clicked()), this, SLOT(save()));
    connect(m_ui->pushButton_cancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(m_ui->pushButton_reset, SIGNAL(clicked()), this, SLOT(reset()));
    connect(m_ui->pushButton_moduleEdit, SIGNAL(clicked()), this, SLOT(editModule()));

    connect(m_ui->checkBox_showStrong, SIGNAL(clicked(bool)), m_ui->checkBox_showStrongInline, SLOT(setEnabled(bool)));

#ifdef BUILD_WITH_SWORD
    QPushButton *button = new QPushButton(this);
    button->setText(tr("Import Sword Modules"));
    m_ui->horizontalLayout_2->addWidget(button);
    connect(button, SIGNAL(clicked()), this, SLOT(importSwordModules()));
#endif
}

SettingsDialog::~SettingsDialog()
{
    delete m_ui;
}

void SettingsDialog::reset()
{
    m_modifedModuleSettings = false;
    m_set = m_backupSet;
    setSettings(m_set);
}

QStringList SettingsDialog::scan(const QString &path, const int level = 0)
{
    QStringList ret;
    QDir dir(path);
    const QFileInfoList list = dir.entryInfoList();
    foreach(const QFileInfo & info, list) {
        if(info.fileName() != ".." && info.fileName() != ".") {
            if(info.isDir()) {
                //if(level <= 2)//i think this is ok
                ret.append(scan(info.absoluteFilePath(), level + 1));
            } else {
                ret.append(info.absoluteFilePath());
            }
        }
    }
    return ret;
}

int SettingsDialog::setSettings(Settings settings)
{
    m_set = settings;
    m_backupSet = settings;

    m_model = new ModuleModel(this);
    m_model->setSettings(&m_set);


    m_proxyModel = new RecursivProxyModel(this);
    m_proxyModel->setSourceModel(m_model->itemModel());
    m_proxyModel->setHeaderData(0, Qt::Horizontal, tr("Module"));
    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    m_selectionModel = new QItemSelectionModel(m_proxyModel);

    m_ui->treeView->setSortingEnabled(true);
    m_ui->treeView->setModel(m_proxyModel);
    m_ui->treeView->setSelectionModel(m_selectionModel);

    m_proxyModel->sort(0);

    m_encodings.clear();
    m_langCode.clear();
    //General
    ////Encoding
    m_encodings << "Apple Roman" << "Big5" << "Big5-HKSCS" << "EUC-JP" << "EUC-KR" << "GB18030-0" << "IBM 850"
                << "IBM 866" << "IBM 874" << "ISO 2022-JP" << "ISO 8859-1" << "ISO 8859-2" << "ISO 8859-3" << "ISO 8859-4"
                << "ISO 8859-5" << "ISO 8859-6" << "ISO 8859-7" << "ISO 8859-8" << "ISO 8859-9" << "ISO 8859-10"
                << "ISO 8859-13" << "ISO 8859-14" << "ISO 8859-15" << "ISO 8859-16" << "Iscii-Bng" << "Dev" << "Gjr"
                << "Knd" << "Mlm" << "Ori" << "Pnj" << "Tlg" << "Tml" << "JIS X 0201" << "JIS X 0208" << "KOI8-R"
                << "KOI8-U" << "MuleLao-1" << "ROMAN8" << "Shift-JIS" << "TIS-620" << "TSCII" << "UTF-8" << "UTF-16"
                << "UTF-16BE" << "UTF-16LE" << "UTF-32" << "UTF-32BE" << "UTF-32LE" << "Windows-1250" << "Windows-1251" << "Windows-1252"
                << "Windows-1253" << "Windows-1254" << "Windows-1255" << "Windows-1256" << "Windows-1257" << "Windows-1258" << "WINSAMI2";
    m_ui->comboBox_encoding->clear();
    m_ui->comboBox_encoding->insertItems(0, m_encodings);
    m_ui->comboBox_encoding->setCurrentIndex(m_encodings.lastIndexOf(m_set.encoding));
    //Language
    QStringList langs;
    QString name(_AV_LANG_NAME);
    QStringList names = name.split(";");
    QString nativeName = QString::fromLocal8Bit(_AV_LANG_NAME_NATIVE);
    QStringList nativeNames = nativeName.split(";");
    for(int i = 0; i < names.size(); i++) {
        langs << tr(names.at(i).toStdString().c_str()) + "( " + nativeNames.at(i) + " )";
    }

    QString av(_AV_LANG);
    m_langCode << av.split(";");
    //myDebug() << "set.language = " << m_set.language;
    m_ui->comboBox_language->clear();
    m_ui->comboBox_language->insertItems(0, langs);
    int code;
    code = m_langCode.lastIndexOf(m_set.language);
    if(code == -1) {
        QString lang = m_set.language;
        lang.remove(lang.lastIndexOf("_"), lang.size());
        code = m_langCode.lastIndexOf(lang);
    }
    //myDebug() << "code = " << code;
    if(code != -1) {
        m_ui->comboBox_language->setCurrentIndex(code);
    } else { // no lang code was written in the config file
        m_ui->comboBox_language->setCurrentIndex(0);
    }
    //Module
    generateModuleTree();

    QStringList autoLayout;
    autoLayout << tr("None") << tr("Tile") << tr("Vertical tile") << tr("Horizontal tile") << tr("Cascade");
    m_ui->comboBox_autoLayout->clear();
    m_ui->comboBox_autoLayout->insertItems(0, autoLayout);
    m_ui->comboBox_autoLayout->setCurrentIndex(m_set.autoLayout);

    QStringList interface;
    interface << tr("Simple") << tr("Advanced");
    m_ui->comboBox_interface->clear();
    m_ui->comboBox_interface->insertItems(0, interface);
    int currentInterface = 0;
    const QString i = m_set.session.getData("interface", "advanced").toString();

    if(i == "advanced") {
        currentInterface = 1;
    } else if(i == "simple") {
        currentInterface = 0;
    }

    m_ui->comboBox_interface->setCurrentIndex(currentInterface);

   m_ui->checkBox_checkForUpdates->setChecked(m_set.checkForUpdates);

    ModuleSettings *config = settings.getModuleSettings(-1);
    m_ui->checkBox_showBottomToolbar->setChecked(config->displaySettings()->showBottomToolBar());
    m_ui->checkBox_showMarks->setChecked(config->displaySettings()->showMarks());
    m_ui->checkBox_showNotes->setChecked(config->displaySettings()->showNotes());
    m_ui->checkBox_showRefLinks->setChecked(config->displaySettings()->showRefLinks());
    m_ui->checkBox_showStrong->setChecked(config->displaySettings()->showStrong());
    m_ui->checkBox_showStudyNotes->setChecked(config->displaySettings()->showStudyNotes());
    m_ui->checkBox_showRMac->setChecked(config->displaySettings()->showRMAC());
    m_ui->checkBox_showCaptions->setChecked(config->displaySettings()->showCaptions());
    m_ui->checkBox_showProlog->setChecked(config->displaySettings()->showProlog());
    m_ui->checkBox_showStrongInline->setChecked(config->displaySettings()->showStrongInline());
    m_ui->checkBox_showStrongInline->setEnabled(config->displaySettings()->showStrong());

    return 0;
}

void SettingsDialog::generateModuleTree()
{
    DEBUG_FUNC_NAME;
    m_model->clear();
    m_model->generate();
    m_proxyModel->setHeaderData(0, Qt::Horizontal, tr("Module"));
}
QModelIndex SettingsDialog::findItem(const int moduleID)
{
    const QModelIndexList list = m_proxyModel->match(m_model->itemModel()->invisibleRootItem()->index(), Qt::UserRole + 1, QString::number(moduleID));
    myDebug()<< moduleID << list.size();
    if(list.isEmpty())
        return QModelIndex();
    return list.first();
}

void SettingsDialog::addModuleFile(void)
{
    m_modifedModuleSettings = true;
    QPointer<QFileDialog> dialog = new QFileDialog(this);
    dialog->setFileMode(QFileDialog::ExistingFiles);
    dialog->setDirectory(m_set.session.getData("addModuleFile_Dir", QFSFileEngine::homePath()).toString());

    if(dialog->exec()) {
        const QStringList fileName = dialog->selectedFiles();
        addModules(fileName, QStringList());
        m_set.session.setData("addModuleFile_Dir", dialog->directory().absolutePath());
    }
    delete dialog;
}
void SettingsDialog::addVirtualFolder()
{
    ModuleSettings *m = new ModuleSettings();
    m->moduleID = m_set.newModuleID();
    m->moduleName = tr("New Folder");
    m->moduleType = ModuleTools::FolderModule;

    m->encoding = "Default";
    m->parentID = -1;

    m_set.getModuleSettings(m->parentID)->appendChild(m);
    m_set.m_moduleSettings.insert(m->moduleID, m);

    generateModuleTree();

    QModelIndex index = findItem(m->moduleID);
    if(index.isValid()) {
        m_selectionModel->clearSelection();
        m_selectionModel->setCurrentIndex(m_proxyModel->mapFromSource(index), QItemSelectionModel::Select);
    }

}

void SettingsDialog::addModuleDir(void)
{
    m_modifedModuleSettings = true;
    QPointer<QFileDialog> dialog = new QFileDialog(this);

    dialog->setFileMode(QFileDialog::Directory);
    dialog->setOption(QFileDialog::ShowDirsOnly, true);
    dialog->setDirectory(m_set.session.getData("addModuleDir_Dir", QFSFileEngine::homePath()).toString());

    if(dialog->exec()) {
        const QStringList fileName = dialog->selectedFiles();
        m_set.session.setData("addModuleDir_Dir", dialog->directory().absolutePath());

        if(fileName.size() > 0) {
            QProgressDialog progress(QObject::tr("Adding Modules"), QObject::tr("Cancel"), 0, fileName.size());
            progress.setWindowModality(Qt::WindowModal);
            progress.show();
            for(int i = 0; i < fileName.size(); i++) {
                progress.setValue(i);
                if(progress.wasCanceled()) {
                    delete dialog;
                    return;
                }
                const QString f = fileName.at(i);
                ModuleSettings *m = new ModuleSettings();
                m->moduleID = m_set.newModuleID();
                QFileInfo fileInfo(f);
                if(fileInfo.isDir()) {
                    QString f = fileName.at(i);
                    if(f.endsWith("/")) {
                        f.remove(f.size() - 1, 10);
                    }
                    m->modulePath = f;
                    QStringList ldictname = (f + "/").split("/");
                    QString dictname;
                    if(ldictname.size() > 0) {
                        dictname = ldictname[ldictname.size()-2];
                    } else {
                        dictname = "(" + QString::number(i) + ")";
                    }
                    m->moduleName = dictname;
                    m->moduleType = ModuleTools::FolderModule;
                } else {
                    QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("It is not a folder."));
                    delete dialog;
                    return;
                }

                // standard config
                m->biblequote_removeHtml = m_set.removeHtml;
                m->zefbible_hardCache = m_set.zefaniaBible_hardCache;
                m->zefbible_softCache = m_set.zefaniaBible_softCache;
                //m->zefbible_textFormatting = m_set.textFormatting;
                m->encoding = "Default";//no translating
                m->parentID = -1;
                m_set.m_moduleSettings.insert(m->moduleID, m);
                m_set.getModuleSettings(m->parentID)->appendChild(m);

                ModuleSettings *parent = m_set.getModuleSettings(m->parentID);
                m->setParent(parent);
                m->setDisplaySettings(parent->displaySettings());

                const QStringList scanned = scan(f);
                foreach(const QString & file, scanned) {
                    if(ModuleTools::recognizeModuleType(file) != ModuleTools::NoneType) {//that is faster than check in quitAddModule
                        quiteAddModule(file, m->moduleID);
                    }
                }
            }
            generateModuleTree();
            progress.close();

        }

    }
    delete dialog;
}

void SettingsDialog::removeModule()
{
    //DEBUG_FUNC_NAME;
    if(m_ui->treeView->selectionModel()->selectedIndexes().isEmpty())
        return;
    m_modifedModuleSettings = true;
    QModelIndexList l = m_ui->treeView->selectionModel()->selectedIndexes();
    QListIterator<QModelIndex> i(l);
    i.toBack();
    while (i.hasPrevious()) {
        QModelIndex index = i.previous();
        bool ok;
        const int moduleID = index.data(Qt::UserRole + 1).toInt(&ok);
        if(ok) {
            //myDebug() << "moduleID  = " << moduleID;
            ModuleSettings *child = m_set.getModuleSettings(moduleID);
            m_set.getModuleSettings(child->parentID)->removeChild(child);
            m_set.m_moduleSettings.remove(moduleID);
        }
        m_ui->treeView->model()->removeRow(index.row(), index.parent());
    }
}

void SettingsDialog::editModule()
{

    if(m_ui->treeView->selectionModel()->selectedIndexes().isEmpty())
        return;
    bool ok;
    const int moduleID = m_ui->treeView->selectionModel()->selectedIndexes().first().data(Qt::UserRole + 1).toInt(&ok);
    if(moduleID >= 0 && ok) {
        QPointer<ModuleConfigDialog> mDialog = new ModuleConfigDialog(this);
        mDialog->setModule(m_set.getModuleSettings(moduleID));
        connect(mDialog, SIGNAL(save(int)), this, SLOT(update(int)));
        mDialog->exec();
        delete mDialog;
    }

}
void SettingsDialog::update(int moduleID)
{
    DEBUG_FUNC_NAME
    m_modifedModuleSettings = true;
    myDebug() << moduleID;
    QModelIndex index = findItem(moduleID);
    if(index.isValid()) {
        m_proxyModel->setData(m_proxyModel->mapFromSource(index), m_set.getModuleSettings(moduleID)->moduleName);
    }
}

void SettingsDialog::save(void)
{
    //save settings
    m_set.encoding = m_encodings.at(m_ui->comboBox_encoding->currentIndex());
    m_set.language = m_langCode.at(m_ui->comboBox_language->currentIndex());
    m_set.autoLayout = (Settings::LayoutEnum) m_ui->comboBox_autoLayout->currentIndex();

    m_set.checkForUpdates = m_ui->checkBox_checkForUpdates->isChecked();

    int currentInterface = m_ui->comboBox_interface->currentIndex();
    if(currentInterface == 0) {
        m_set.session.setData("interface", "simple");
    } else if(currentInterface == 1) {
        m_set.session.setData("interface", "advanced");
    }
    ModuleSettings *config = m_set.getModuleSettings(-1);
    config->displaySettings()->setShowBottomToolBar(m_ui->checkBox_showBottomToolbar->isChecked());
    config->displaySettings()->setShowMarks(m_ui->checkBox_showMarks->isChecked());
    config->displaySettings()->setShowNotes(m_ui->checkBox_showNotes->isChecked());
    config->displaySettings()->setShowRefLinks(m_ui->checkBox_showRefLinks->isChecked());
    config->displaySettings()->setShowStrong(m_ui->checkBox_showStrong->isChecked());
    config->displaySettings()->setShowStudyNotes(m_ui->checkBox_showStudyNotes->isChecked());
    config->displaySettings()->setShowRMAC(m_ui->checkBox_showRMac->isChecked());
    config->displaySettings()->setShowCaptions(m_ui->checkBox_showCaptions->isChecked());
    config->displaySettings()->setShowProlog(m_ui->checkBox_showProlog->isChecked());
    config->displaySettings()->setShowStrongInline(m_ui->checkBox_showStrongInline->isChecked());


    QMap<int, int> struc;
    foreach(ModuleSettings * set, m_set.m_moduleSettings) {
        struc.insert(set->moduleID, set->parentID);
    }
    foreach(ModuleSettings * set, m_set.m_moduleSettings) {
        set->clearChildren();
    }

    QModelIndex rootIndex = m_ui->treeView->rootIndex();
    saveModule(rootIndex, m_set.getModuleSettings(-1));

    QMap<int, int> struc2;
    foreach(ModuleSettings * set, m_set.m_moduleSettings) {
        struc2.insert(set->moduleID, set->parentID);
    }

    if(struc != struc2) {
        myDebug() << "modified struct";
        m_modifedModuleSettings = true;
    }
    emit settingsChanged(m_set, m_modifedModuleSettings); //Speichern
    close();
}

void SettingsDialog::saveModule(QModelIndex parentIndex, ModuleSettings *parentSettings)
{
    for(int i = 0; i < m_ui->treeView->model()->rowCount(parentIndex); ++i) {
        const QModelIndex index =  m_ui->treeView->model()->index(i, 0, parentIndex);
        if(!index.isValid())
            continue;
        int moduleID = index.data(Qt::UserRole + 1).toInt();
        ModuleSettings *settings = m_set.getModuleSettings(moduleID);
        parentSettings->appendChild(settings);
        settings->setParent(parentSettings);
        settings->parentID = parentSettings->moduleID;

        if(m_ui->treeView->model()->hasChildren(index))
            saveModule(index, settings);
    }
}

void SettingsDialog::downloadModule()
{
    QPointer<ModuleDownloadDialog> mDialog = new ModuleDownloadDialog(this);
    mDialog->setSettings(m_set);
    mDialog->readModules();
    connect(mDialog, SIGNAL(downloaded(QMap<QString, QString>)), this, SLOT(addModules(QMap<QString, QString>)));

    mDialog->exec();
    delete mDialog;
}

void SettingsDialog::addModules(QMap<QString, QString> data)
{
    addModules(data.keys(), data.values());
}

void SettingsDialog::addModules(QStringList fileName, QStringList names, int parentID)
{
    DEBUG_FUNC_NAME
    if(!fileName.isEmpty()) {
        QProgressDialog progress(QObject::tr("Adding Modules"), QObject::tr("Cancel"), 0, fileName.size() + 2);
        progress.setWindowModality(Qt::WindowModal);
        progress.show();
        progress.setValue(1);
        for(int i = 0; i < fileName.size(); i++) {
            progress.setValue(i + 2);
            if(progress.wasCanceled()) {
                progress.close();
                generateModuleTree();
                return;
            }
            const QString f = fileName.at(i);
            if(i < names.size()) {
                quiteAddModule(f, parentID, names.at(i));
            } else {
                quiteAddModule(f, parentID);
            }

        }
        generateModuleTree();
        progress.close();
    }
}

int SettingsDialog::quiteAddModule(const QString &f, int parentID, const QString &name)
{
    DEBUG_FUNC_NAME
    QFileInfo fileInfo(f);
    if(fileInfo.suffix() == "zip") {

        QZipReader reader(f);
        QString path;

        if(fileInfo.absoluteDir().entryList(QDir::Files | QDir::NoDotAndDotDot).size() > 1) {
            //mkpath
            QDir d(fileInfo.absoluteDir());
            d.mkdir(fileInfo.baseName());
            path = fileInfo.absoluteDir().path() + "/" + fileInfo.baseName();
        } else {
            path = fileInfo.absoluteDir().path();
        }

        reader.extractAll(path);
        reader.close();


        QStringList l;
        QDir pathDir(path);
        foreach(QFileInfo info, pathDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot)) {
            if(info.suffix() == "zip")
                continue;
            l << info.absoluteFilePath();
        }
        foreach(const QString &p, l) {
            quiteAddModule(p, parentID, name);
        }

        return 0;
    }
    ModuleTools::ModuleType moduleType = ModuleTools::NoneType;

    ModuleSettings *m = new ModuleSettings();
    m->moduleID = m_set.newModuleID();

    if(fileInfo.isFile()) {
        moduleType = ModuleTools::recognizeModuleType(f);
        if(moduleType == ModuleTools::NoneType) {
            //QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("Cannot determine the module type."));
            myWarning() << "cannot determine module type of " << f;
            return 4;
        }
        MetaInfo info;
        if(moduleType == ModuleTools::BibleQuoteModule) {
            BibleQuote bq;
            bq.setSettings(&m_set);
            info = bq.readInfo(f);
        } else if(moduleType == ModuleTools::ZefaniaBibleModule) {
            ZefaniaBible zef;
            zef.setSettings(&m_set);
            info = zef.readInfo(f);
        } else if(moduleType == ModuleTools::ZefaniaLexModule) {
            ZefaniaLex zefLex;
            zefLex.setSettings(&m_set);
            zefLex.setID(0, f);
            info = zefLex.buildIndexFromFile(f);
        } else if(moduleType == ModuleTools::BibleQuoteDictModule) {
            BibleQuoteDict bibleQuoteDict;
            bibleQuoteDict.setSettings(&m_set);
            bibleQuoteDict.setID(0, f);
            info = bibleQuoteDict.readInfo(f);
            //bibleQuoteDict.buildIndex();
        } else if(moduleType == ModuleTools::TheWordBibleModule) {
            TheWordBible theWordBible;
            theWordBible.setSettings(&m_set);
            info = theWordBible.readInfo(f);
        } else if(moduleType == ModuleTools::WebPageModule) {
            WebPage webPage;
            webPage.setSettings(&m_set);
            info = webPage.readInfo(f);
        } else if(moduleType == ModuleTools::WebDictionaryModule) {
            WebDictionary webDict;
            webDict.setSettings(&m_set);
            info = webDict.readInfo(f);
        } else if(moduleType == ModuleTools::WebCommentaryModule) {
            WebCommentary webComm;
            webComm.setSettings(&m_set);
            info = webComm.readInfo(f);
        }else if(moduleType == ModuleTools::TxtBookModule) {
            info = MetaInfo();
            info.setName(fileInfo.baseName());
            info.setContent(ModuleTools::BookContent);
        }
        if(name.isEmpty()) {
            m->moduleName = info.name();
        } else {
            m->moduleName = name;
        }
        m->moduleShortName = info.shortName();


        bool setDefault = true;
        //if there is already a default module, don't overwrite
        foreach(const ModuleSettings * s, m_set.m_moduleSettings) {
            if(s->defaultModule == info.defaultModule()) {
                setDefault = false;
            }
        }
        if(setDefault) {
            m->defaultModule = info.defaultModule();
        }
        m->contentType = info.content();

        m->modulePath = f;
        m->moduleType = moduleType;
        m->moduleLanguage = info.language;


    } else {
        //QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("Cannot open the file."));
        return 1;
    }

    // standard config
    m->biblequote_removeHtml = m_set.removeHtml;
    m->zefbible_hardCache = m_set.zefaniaBible_hardCache;
    m->zefbible_softCache = m_set.zefaniaBible_softCache;
    m->encoding = "Default";
    m->parentID = parentID;


    m_set.getModuleSettings(m->parentID)->appendChild(m);
    m_set.m_moduleSettings.insert(m->moduleID, m);

    return 0;
}

void SettingsDialog::importSwordModules()
{
    DEBUG_FUNC_NAME;
#ifdef BUILD_WITH_SWORD
    SWMgr library(new MarkupFilterMgr(FMT_PLAIN));
    ModMap::iterator it;
    for(it = library.Modules.begin(); it != library.Modules.end(); it++) {
        //todo: deduplication
        const QString name = QString::fromLocal8Bit((*it).second->Name());
        const QString desc = QString::fromLocal8Bit((*it).second->Description());
        const QString type = QString::fromLocal8Bit((*it).second->Type());

        myDebug() << name << desc << type;
        if(type == "Biblical Texts") { //cu
            ModuleSettings *m = new ModuleSettings();
            m->moduleID = m_set.newModuleID();
            m->moduleName = desc;
            m->modulePath = name;
            m->moduleType = ModuleTools::SwordBibleModule;

            m->biblequote_removeHtml = m_set.removeHtml;
            m->zefbible_hardCache = m_set.zefaniaBible_hardCache;
            m->zefbible_softCache = m_set.zefaniaBible_softCache;

            m->encoding = "Default";
            m->parentID = -1;

            m_set.getModuleSettings(m->parentID)->appendChild(m);
            m_set.m_moduleSettings.insert(m->moduleID, m);
        } else if(type == "Lexicons / Dictionaries") {
            ModuleSettings *m = new ModuleSettings();
            m->moduleID = m_set.newModuleID();
            m->moduleName = desc;
            m->modulePath = name;
            m->moduleType = ModuleTools::SwordLexiconModule;

            m->biblequote_removeHtml = m_set.removeHtml;
            m->zefbible_hardCache = m_set.zefaniaBible_hardCache;
            m->zefbible_softCache = m_set.zefaniaBible_softCache;

            m->encoding = "Default";
            m->parentID = -1;

            m_set.getModuleSettings(m->parentID)->appendChild(m);
            m_set.m_moduleSettings.insert(m->moduleID, m);

        }
    }
    generateModuleTree();

#endif
}

void SettingsDialog::setCurrentTab(int tabID)
{
    m_ui->tabWidget->setCurrentIndex(tabID);
}

void SettingsDialog::changeEvent(QEvent *e)
{
    switch(e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
