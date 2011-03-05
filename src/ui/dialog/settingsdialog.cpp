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


SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent), m_ui(new Ui::SettingsDialog)
{
    m_ui->setupUi(this);
    m_modifedModuleSettings = false;

    connect(m_ui->pushButton_downloadModule, SIGNAL(clicked()), this, SLOT(downloadModule()));
    connect(m_ui->pushButton_addFile, SIGNAL(clicked()), this, SLOT(addModuleFile()));
    connect(m_ui->pushButton_addDir, SIGNAL(clicked()), this, SLOT(addModuleDir()));
    connect(m_ui->pushButton_remove_module, SIGNAL(clicked()), this, SLOT(removeModule()));
    connect(m_ui->pushButton_ok, SIGNAL(clicked()), this, SLOT(save()));
    connect(m_ui->pushButton_cancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(m_ui->pushButton_reset, SIGNAL(clicked()), this, SLOT(reset()));
    connect(m_ui->pushButton_moduleEdit, SIGNAL(clicked()), this, SLOT(editModule()));
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
    m_encodings.clear();
    m_langCode.clear();
    //Allgemeine
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
        const QString onlyLang = lang.remove(lang.lastIndexOf("_"), lang.size());
        code = m_langCode.lastIndexOf(onlyLang);
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

    ModuleSettings *config = settings.getModuleSettings(-1);
    m_ui->checkBox_showBottomToolbar->setChecked(config->displaySettings()->showBottomToolBar());
    m_ui->checkBox_showMarks->setChecked(config->displaySettings()->showMarks());
    m_ui->checkBox_showNotes->setChecked(config->displaySettings()->showNotes());
    m_ui->checkBox_showRefLinks->setChecked(config->displaySettings()->showRefLinks());
    m_ui->checkBox_showStrong->setChecked(config->displaySettings()->showStrong());
    m_ui->checkBox_showStudyNotes->setChecked(config->displaySettings()->showStudyNotes());

    return 0;

}
void SettingsDialog::generateModuleTree()
{
    DEBUG_FUNC_NAME;
    ModuleModel model(this);
    model.setSettings(&m_set);
    model.generate();
    m_ui->treeView->setModel(model.itemModel());
    m_ui->treeView->model()->setHeaderData(0, Qt::Horizontal, tr("Module"));
}

void SettingsDialog::addModuleFile(void)
{
    m_modifedModuleSettings = true;
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setDirectory(m_set.session.getData("addModuleFile_Dir", QFSFileEngine::homePath()).toString());

    if(dialog.exec()) {
        const QStringList fileName = dialog.selectedFiles();
        addModules(fileName, QStringList());
        m_set.session.setData("addModuleFile_Dir", dialog.directory().absolutePath());
    }
    return;
}
void SettingsDialog::addModuleDir(void)
{
    m_modifedModuleSettings = true;
    QFileDialog dialog(this);

    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly, true);
    dialog.setDirectory(m_set.session.getData("addModuleDir_Dir", QFSFileEngine::homePath()).toString());

    if(dialog.exec()) {
        const QStringList fileName = dialog.selectedFiles();
        m_set.session.setData("addModuleDir_Dir", dialog.directory().absolutePath());

        if(fileName.size() > 0) {
            QProgressDialog progress(QObject::tr("Adding Modules"), QObject::tr("Cancel"), 0, fileName.size());
            progress.setWindowModality(Qt::WindowModal);
            for(int i = 0; i < fileName.size(); i++) {
                progress.setValue(i);
                if(progress.wasCanceled())
                    return;
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
                    m->moduleType = OBVCore::FolderModule;
                    //m.isDir = true;
                } else {
                    QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("It is not a folder."));
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
                const QStringList scanned = scan(f);
                foreach(const QString & file, scanned) {
                    if(ModuleManager::recognizeModuleType(file) != OBVCore::NoneType) {//that is faster than check in quitAddModule
                        quiteAddModule(file, m->moduleID);
                    }
                }
            }
            progress.close();
            generateModuleTree();
        }

    }
}
void SettingsDialog::removeModule()
{
    //DEBUG_FUNC_NAME;
    if(m_ui->treeView->selectionModel()->selectedIndexes().isEmpty())
        return;
    m_modifedModuleSettings = true;

    foreach(QModelIndex index, m_ui->treeView->selectionModel()->selectedIndexes()) {
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
    //DEBUG_FUNC_NAME
    m_modifedModuleSettings = true;
    if(m_ui->treeView->selectionModel()->selectedIndexes().isEmpty())
        return;
    bool ok;
    const int moduleID = m_ui->treeView->selectionModel()->selectedIndexes().first().data(Qt::UserRole + 1).toInt(&ok);
    myDebug() << "moduleID = " << moduleID;
    if(moduleID >= 0 && ok) {
        ModuleConfigDialog *mDialog = new ModuleConfigDialog(this);
        mDialog->setModule(m_set.getModuleSettings(moduleID));
        connect(mDialog, SIGNAL(save(ModuleSettings)), mDialog, SLOT(close()));
        mDialog->show();
        mDialog->exec();
    }

}

void SettingsDialog::save(void)
{
    //save settings
    m_set.encoding = m_encodings.at(m_ui->comboBox_encoding->currentIndex());
    m_set.language = m_langCode.at(m_ui->comboBox_language->currentIndex());
    m_set.autoLayout = (Settings::LayoutEnum) m_ui->comboBox_autoLayout->currentIndex();
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
    ModuleDownloadDialog *mDialog = new ModuleDownloadDialog(this);
    mDialog->setSettings(m_set);
    mDialog->readModules();
    connect(mDialog, SIGNAL(downloaded(QStringList, QStringList)), this, SLOT(addModules(QStringList, QStringList)));
    connect(mDialog, SIGNAL(downloaded(QStringList, QStringList)), mDialog, SLOT(close()));
    mDialog->show();
    mDialog->exec();
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
            if(i < names.size())
                quiteAddModule(f, parentID, names.at(i));
            else
                quiteAddModule(f, parentID);

        }
        generateModuleTree();
        progress.close();
    }
}
int SettingsDialog::quiteAddModule(const QString &f, int parentID, const QString &name)
{
    QString moduleName;
    OBVCore::ModuleType moduleType = OBVCore::NoneType;

    ModuleSettings *m = new ModuleSettings();
    m->moduleID = m_set.newModuleID();

    QFileInfo fileInfo(f);
    if(fileInfo.isFile()) {

        if(f.endsWith(".zip")) {
            //todo: unzip first
            //QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("Cannot open zipped files."));
            return 3;

        }
        moduleType = ModuleManager::recognizeModuleType(f);

        if(moduleType == OBVCore::NoneType) {
            //QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("Cannot determine the module type."));
            myWarning() << "cannot determine module type";
            return 4;
        }
        if(name.isEmpty()) {
            if(moduleType == OBVCore::BibleQuoteModule) {
                BibleQuote bq;
                bq.setSettings(&m_set);
                m->moduleName = bq.readInfo(f);
            } else if(moduleType == OBVCore::ZefaniaBibleModule) {
                ZefaniaBible zef;
                zef.setSettings(&m_set);
                m->moduleName = zef.readInfo(f);
            } else if(moduleType ==  OBVCore::ZefaniaLexModule) {
                ZefaniaLex zefLex;
                zefLex.setSettings(&m_set);
                m->moduleName = zefLex.buildIndexFromFile(f);
            } else if(moduleType == OBVCore::BibleQuoteDictModule) {
                BibleQuoteDict bibleQuoteDict;
                bibleQuoteDict.setSettings(&m_set);
                m->moduleName = bibleQuoteDict.readInfo(f);
                bibleQuoteDict.buildIndex();
            } else if(moduleType == OBVCore::TheWordBibleModule) {
                TheWordBible theWordBible;
                theWordBible.setSettings(&m_set);
                m->moduleName = theWordBible.readInfo(f);
            }
        } else {
            m->moduleName = name;
        }
        m->modulePath = f;
        m->moduleType = moduleType;

    } else {
        //QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("Cannot open the file."));
        return 1;
    }

    // standard config
    m->biblequote_removeHtml = m_set.removeHtml;
    m->zefbible_hardCache = m_set.zefaniaBible_hardCache;
    m->zefbible_softCache = m_set.zefaniaBible_softCache;
    //m->zefbible_textFormatting = m_set.textFormatting;
    m->encoding = "Default";
    m->parentID = parentID;

    m_set.getModuleSettings(m->parentID)->appendChild(m);
    m_set.m_moduleSettings.insert(m->moduleID, m);

    return 0;
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
