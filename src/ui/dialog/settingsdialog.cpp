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
#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "src/core/settings.h"
#include "src/core/modulesettings.h"
#include "src/core/dbghelper.h"
#include "src/module/zefania-bible.h"
#include "src/module/zefania-lex.h"
#include "src/module/biblequote.h"
#include "src/module/bible.h"
#include "src/module/module.h"
#include "moduleconfigdialog.h"
#include "moduledownloaddialog.h"
#include "config.h"
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QStringList>
#include <QtGui/QProgressDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>

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
    //myDebug() << " currentInterface = " << currentInterface;
    m_ui->comboBox_interface->setCurrentIndex(currentInterface);

    return 0;

}
void SettingsDialog::generateModuleTree()
{
    m_ui->treeWidget_module->clear();
    QList<QTreeWidgetItem *> items;
    for(int i = 0; i < m_set.m_moduleSettings.size(); i++) {
        QTreeWidgetItem *ibible = new QTreeWidgetItem(m_ui->treeWidget_module);
        ibible->setText(0, m_set.m_moduleSettings.at(i).moduleName);
        ibible->setText(1, m_set.m_moduleSettings.at(i).modulePath);
        QString moduleType;
        if(m_set.m_moduleSettings.at(i).isDir) {
            moduleType = QObject::tr("Folder");
        } else {
            switch(m_set.m_moduleSettings.at(i).moduleType.toInt()) {
            case Module::BibleQuoteModule:
                moduleType = QObject::tr("Bible Quote");
                break;
            case Module::ZefaniaBibleModule:
                moduleType = QObject::tr("Zefania XML");;
                break;
            case Module::ZefaniaLexModule:
                moduleType = QObject::tr("Zefania XML Strong");;
                break;

            }
        }
        ibible->setText(2, moduleType);
        items << ibible;
    }
    m_ui->treeWidget_module->insertTopLevelItems(0, items);

}

void SettingsDialog::addModuleFile(void)
{
    m_modifedModuleSettings = true;
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);

    if(dialog.exec()) {
        const QStringList fileName = dialog.selectedFiles();
        addModules(fileName, QStringList());
    }
    return;
}
void SettingsDialog::addModuleDir(void)
{
    m_modifedModuleSettings = true;
    QFileDialog dialog(this);

    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly, true);

    if(dialog.exec()) {
        const QStringList fileName = dialog.selectedFiles();
        QList<QTreeWidgetItem *> items;
        if(fileName.size() > 0) {
            QProgressDialog progress(QObject::tr("Adding Modules"), QObject::tr("Cancel"), 0, fileName.size());
            progress.setWindowModality(Qt::WindowModal);
            for(int i = 0; i < fileName.size(); i++) {
                progress.setValue(i);
                if(progress.wasCanceled())
                    return;
                const QString f = fileName.at(i);
                QString bibleName;
                QString moduleType;
                ModuleSettings m;
                QFileInfo fileInfo(f);
                if(fileInfo.isDir()) {
                    moduleType = QObject::tr("Folder");
                    QString f = fileName.at(i);
                    if(f.endsWith("/")) {
                        f.remove(f.size() - 1, 10);
                    }
                    m.modulePath = f;
                    QStringList ldictname = (f + "/").split("/");
                    QString dictname;
                    if(ldictname.size() > 0) {
                        dictname = ldictname[ldictname.size()-2];
                    } else {
                        dictname = "(" + QString::number(i) + ")";
                    }
                    m.moduleName = dictname;
                    m.moduleType = QString::number(Module::NoneType);
                    m.isDir = true;
                } else {
                    QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("The file is not valid"));
                    return;
                }


                //myDebug() << "new Module file" << f << " moduleName" << bibleName << " moduleType" << moduleType;
                // standard config
                m.biblequote_removeHtml = m_set.removeHtml;
                m.zefbible_hardCache = m_set.zefaniaBible_hardCache;
                m.zefbible_textFormatting = m_set.textFormatting;
                m.zefbible_showStrong = true;
                m.zefbible_showStudyNote = true;
                m.encoding = "Default";// don't translate


                QTreeWidgetItem * ibible = new QTreeWidgetItem(m_ui->treeWidget_module);
                ibible->setText(0, m.moduleName);
                ibible->setText(1, m.modulePath);
                ibible->setText(2, moduleType);
                items << ibible;
                m_set.m_moduleSettings << m;
            }
            progress.close();
            m_ui->treeWidget_module->insertTopLevelItems(0, items);
        }

    }
    return;
}
void SettingsDialog::removeModule()
{
    m_modifedModuleSettings = true;
    int row = m_ui->treeWidget_module->indexOfTopLevelItem(m_ui->treeWidget_module->currentItem());
    //remove from listWidget
    QTreeWidgetItem * token = m_ui->treeWidget_module->currentItem();
    delete token;
    //remove from settings
    m_set.m_moduleSettings.removeAt(row);
    return;
}
void SettingsDialog::editModule()
{
    //DEBUG_FUNC_NAME
    m_modifedModuleSettings = true;
    int row = m_ui->treeWidget_module->indexOfTopLevelItem(m_ui->treeWidget_module->currentItem());
    if(row >= 0) {
        ModuleConfigDialog *mDialog = new ModuleConfigDialog(this);
        mDialog->setModule(m_set.m_moduleSettings.at(row));
        connect(mDialog, SIGNAL(save(ModuleSettings)), this, SLOT(saveModule(ModuleSettings)));
        connect(mDialog, SIGNAL(save(ModuleSettings)), mDialog, SLOT(close()));
        mDialog->show();
        mDialog->exec();
    }

}
void SettingsDialog::saveModule(ModuleSettings c)
{
    int row = m_ui->treeWidget_module->indexOfTopLevelItem(m_ui->treeWidget_module->currentItem());
    m_set.m_moduleSettings.replace(row, c);
    generateModuleTree();
}
void SettingsDialog::save(void)
{
    //Informationen aus dem Dialog auslesen
    m_set.encoding = m_encodings.at(m_ui->comboBox_encoding->currentIndex());
    m_set.language = m_langCode.at(m_ui->comboBox_language->currentIndex());
    m_set.autoLayout = m_ui->comboBox_autoLayout->currentIndex();
    int currentInterface = m_ui->comboBox_interface->currentIndex();
    if(currentInterface == 0) {
        m_set.session.setData("interface", "simple");
    } else if(currentInterface == 1) {
        m_set.session.setData("interface", "advanced");
    }
    //Alles andere, wie z.b die Module sind schon gespeichert
    emit settingsChanged(m_set,m_modifedModuleSettings);//Speichern
    close();
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
void SettingsDialog::addModules(QStringList fileName, QStringList names)
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
            QString fileData;
            QString moduleName;
            Module::ModuleType moduleType = Module::NoneType;
            QString moduleTypeName = "";
            BibleQuote bq;
            ZefaniaBible zef;
            ZefaniaLex zefLex;
            BibleQuoteDict bibleQuoteDict;
            ModuleSettings m;
            zefLex.setSettings(&m_set);

            QFileInfo fileInfo(f);
            if(fileInfo.isFile()) {
                if(f.endsWith(".zip")) {
                    //todo: unzip first
                    QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("Cannot open zipped files."));
                    return;

                }
                //open file
                QFile file(f);
                if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("Cannot read the file."));
                    progress.close();
                    generateModuleTree();
                    return;
                }
                QTextStream in(&file);
                if(f.endsWith(".xml")) {
                    for(int i = 0; i < 100; i++)
                        fileData += in.readLine(i);
                } else {
                    fileData = in.readAll();
                }
                if(fileInfo.suffix() == "idx") {
                    moduleType = Module::BibleQuoteDictModule;
                } else {
                    //todo: find something better
                    if(fileData.contains("BookQty", Qt::CaseInsensitive)) {
                        moduleType = Module::BibleQuoteModule;// BibleQuote
                    } else if(fileData.contains("XMLBIBLE", Qt::CaseInsensitive) && !(fileData.contains("x-quran", Qt::CaseInsensitive) || // i cannot allow this
                              fileData.contains("x-cult", Qt::CaseInsensitive) ||
                              fileData.contains("x-mormon", Qt::CaseInsensitive))) {
                        moduleType = Module::ZefaniaBibleModule;// Zefania Bible
                    } else if(fileData.contains("<dictionary", Qt::CaseInsensitive)) {
                        moduleType = Module::ZefaniaLexModule;// Zefania Strong
                    } else {
                        QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("Cannot determine the module type."));
                        myWarning() << "cannot detetct module type";
                        progress.close();
                        generateModuleTree();
                        return;
                    }
                }

                if(f.endsWith(".xml")) {
                    fileData += in.readAll();
                }
                switch(moduleType) {
                case Module::BibleQuoteModule:
                    if(names.size() == 0 || i >= names.size()) {
                        moduleName = bq.readInfo(file);
                    }
                    moduleTypeName = QObject::tr("Bible Quote");
                    break;
                case Module::ZefaniaBibleModule:
                    if(names.size() == 0 || i >= names.size()) {
                        moduleName = zef.readInfo(fileData);
                    }
                    moduleTypeName = QObject::tr("Zefania XML");
                    break;
                case Module::ZefaniaLexModule:
                    if(names.size() == 0 || i >= names.size()) {
                        moduleName = zefLex.buildIndexFromData(fileData, f);
                    }
                    moduleTypeName = QObject::tr("Zefania XML Dictionary");
                    break;
                case Module::BibleQuoteDictModule:
                    if(names.size() == 0 || i >= names.size()) {
                        moduleName = bibleQuoteDict.readInfo(file);
                        bibleQuoteDict.buildIndex();
                    }
                    moduleTypeName = QObject::tr("Bible Quote Dictionary");
                case Module::NoneType:
                    QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("Cannot determine the module type."));
                    progress.close();
                    generateModuleTree();
                    return;
                }
                m.modulePath = f;

                if(names.size() > 0 && i < names.size()) {  //if a name is given in the stringlist use it
                    m.moduleName = names.at(i);
                } else {//else use the biblename from the filename
                    m.moduleName = moduleName;
                }
                m.moduleType = QString::number(moduleType);
                m.isDir = false;

            } else {
                QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("Cannot open the file."));
                progress.close();
                generateModuleTree();
                return;
            }

            // standard config
            m.biblequote_removeHtml = m_set.removeHtml;
            m.zefbible_hardCache = m_set.zefaniaBible_hardCache;
            m.zefbible_softCache = m_set.zefaniaBible_softCache;
            m.zefbible_textFormatting = m_set.textFormatting;
            m.zefbible_showStrong = true;
            m.zefbible_showStudyNote = true;
            m.encoding = "Default";//no translating
            m_set.m_moduleSettings << m;
        }
        generateModuleTree();
        progress.close();
    }
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
