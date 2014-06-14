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
#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "src/core/module/dictionary/biblequote-dict.h"
#include "src/core/module/dictionary/zefania-lex.h"
#include "src/core/module/bible/zefania-bible.h"
#include "src/core/module/bible/biblequote.h"
#include "src/core/module/bible/thewordbible.h"
#include "src/core/module/webpage.h"
#include "src/core/module/dictionary/webdictionary.h"
#include "src/core/module/commentary/webcommentary.h"
#include "src/core/module/commentary/thewordcommentary.h"
#include "src/core/module/book/thewordtopic.h"
#include "src/core/module/book/eswordtopic.h"
#include "src/core/module/dictionary/theworddictionary.h"
#include <QFSFileEngine>
#include <QPointer>
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
    m_encodings = ModuleTools::encodings();
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
    m_ui->checkBox_compactUI->setChecked(m_set.moreCompactUI);

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
    ModuleSettings *m = m_set.newVirtualFolder(-1);

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

                ModuleSettings *m = m_set.newModuleSettings(-1);
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


                ModuleSettings *parent = m_set.getModuleSettings(m->parentID);
                m->setParent(parent);
                m->setDisplaySettings(parent->displaySettings());

                const QStringList scanned = ModuleTools::scan(f);
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
/**
 * It removes all selected Modules
 */
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
            m_set.removeModule(moduleID);
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
        mDialog->setSettings(&m_set);
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
    m_set.moreCompactUI = m_ui->checkBox_compactUI->isChecked();

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


    QMap<int, int> struc;// the structure (parent,child)
    foreach(ModuleSettings * set, m_set.m_moduleSettings) {
        struc.insert(set->moduleID, set->parentID);
    }
    //remove all children, but not deleting them
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
/**
 * Saves the Structure of the tree in ModuleSettings
 */
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
    this->activateWindow();
}

void SettingsDialog::addModules(const QMap<QString, QString> &data)
{
    addModules(data.keys(), data.values());
}

void SettingsDialog::addModules(const QStringList &fileName, const QStringList &names, int parentID)
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
    myDebug()  << f << parentID << name;
    QFileInfo fileInfo(f);
    if(fileInfo.suffix() == "zip") {
        QStringList files = ModuleTools::unzip(f, m_set.homePath + "modules/");
        //remove file
        QFile a(f);
        a.remove();

        myDebug() << files;
        if(files.size() == 1) {
            quiteAddModule(files.first(), parentID, name);
        } else {
            foreach(const QString &f, files) {
                quiteAddModule(f, parentID);
            }
        }

        return 0;
    }
    ModuleTools::ModuleType moduleType = ModuleTools::NoneType;

    ModuleSettings *m = m_set.newModuleSettings(parentID);

    if(fileInfo.isFile()) {
        moduleType = ModuleTools::recognizeModuleType(f);
        if(moduleType == ModuleTools::NoneType) {
            //QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("Cannot determine the module type."));
            myWarning() << "cannot determine module type of " << f;
            return 4;
        }
        MetaInfo info;
        SimpleModule *module = Module::newSimpleModule(moduleType);
        if(!module) {
            myWarning() << "not a module";
            //todo: remove it
            return 1;
        }
        module->setSettings(&m_set);
        info = module->readInfo(f);
        delete module;

        if(name.isEmpty()) {
            myDebug() << "info.name" << info.name();
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
        myWarning() << "cannot open the file " << f;
        //QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("Cannot open the file."));
        return 1;
    }

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
            ModuleSettings *m = m_set.newModuleSettings(-1)
            m->moduleName = desc;
            m->modulePath = name;
            m->moduleType = ModuleTools::SwordBibleModule;
        } else if(type     m_ui->checkBox_checkForUpdates->setChecked(m_set.checkForUpdates);
== "Lexicons / Dictionaries") {
            ModuleSettings *m = m_set.newModuleSettings(-1)
            m->moduleName = desc;
            m->modulePath = name;
            m->moduleType = ModuleTools::SwordLexiconModule;
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
