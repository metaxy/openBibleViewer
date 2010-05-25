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
#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "src/core/settings.h"
#include "src/core/modulesettings.h"
#include "src/core/dbghelper.h"
#include "src/module/zefania-bible.h"
#include "src/module/zefania-strong.h"
#include "src/module/biblequote.h"
#include "src/module/bible.h"
#include "src/module/module.h"
#include "moduleconfigdialog.h"
#include "moduledownloaddialog.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QtDebug>
#include <QtCore/QStringList>
#include <QtGui/QProgressDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent), m_ui(new Ui::SettingsDialog)
{
    m_ui->setupUi(this);

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
    langs <<  tr("English") +"( English )" << tr("German") + "( Deutsch )" << tr("Russian") + QString::fromLocal8Bit("( русском )") << tr("Czech") + QString::fromLocal8Bit("( čeština )");
    m_langCode << "en" << "de" << "ru" << "cs";
    myDebug() << "set.language = " << m_set.language;
    m_ui->comboBox_language->clear();
    m_ui->comboBox_language->insertItems(0, langs);
    int code;
    code = m_langCode.lastIndexOf(m_set.language);
    if (code == -1) {
        QString lang = m_set.language;
        QString onlyLang = lang.remove(lang.lastIndexOf("_"), lang.size());
        code = m_langCode.lastIndexOf(onlyLang);
    }
    myDebug() << "code = " << code;
    if (code != -1) {
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
    int currentInterface;
    QString i = m_set.session.getData("interface", "advanced").toString();
    qDebug() << "interface = " << i;
    if (i == "advanced") {
        currentInterface = 1;
    } else if (i == "simple") {
        currentInterface = 0;
    }
    myDebug() << " currentInterface = " << currentInterface;
    m_ui->comboBox_interface->setCurrentIndex(currentInterface);

    return 0;

}
void SettingsDialog::generateModuleTree()
{
    m_ui->treeWidget_module->clear();
    QList<QTreeWidgetItem *> items;
    for (int i = 0; i < m_set.module.size(); i++) {
        QTreeWidgetItem *ibible = new QTreeWidgetItem(m_ui->treeWidget_module);
        ibible->setText(0, m_set.module.at(i).moduleName);
        ibible->setText(1, m_set.module.at(i).modulePath);
        QString moduleType;
        if (m_set.module.at(i).isDir) {
            moduleType = QObject::tr("Folder");
        } else {
            switch (m_set.module.at(i).moduleType.toInt()) {
            case Module::BibleQuoteModule:
                moduleType = QObject::tr("Bible Quote");
                break;
            case Module::ZefaniaBibleModule:
                moduleType = QObject::tr("Zefania XML");;
                break;
            case Module::ZefaniaStrongModule:
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
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);

    if (dialog.exec()) {
        QStringList fileName = dialog.selectedFiles();
        addModules(fileName, QStringList());
    }
    return;
}
void SettingsDialog::addModuleDir(void)
{
    QFileDialog dialog(this);

    dialog.setFileMode(QFileDialog::Directory);
#if QT_VERSION >= 0x040500
    dialog.setOption(QFileDialog::ShowDirsOnly, true);
#endif

    if (dialog.exec()) {
        QStringList fileName = dialog.selectedFiles();
        QList<QTreeWidgetItem *> items;
        if (fileName.size() > 0) {
            QProgressDialog progress(QObject::tr("Adding Modules"), QObject::tr("Cancel"), 0, fileName.size());
            progress.setWindowModality(Qt::WindowModal);
            for (int i = 0; i < fileName.size(); i++) {
                progress.setValue(i);
                if (progress.wasCanceled())
                    return;
                QString f = fileName.at(i);
                QString bibleName;
                QString moduleType;
                ModuleSettings m;
                QFileInfo fileInfo(f);
                if (fileInfo.isDir()) {
                    moduleType = QObject::tr("Folder");
                    QString f = fileName.at(i);
                    if (f.endsWith("/")) {
                        f.remove(f.size() - 1, 10);
                    }
                    m.modulePath = f;
                    QStringList ldictname = (f + "/").split("/");
                    QString dictname;
                    if (ldictname.size() > 0) {
                        dictname = ldictname[ldictname.size()-2];
                    } else {
                        QString spath_count;
                        spath_count.setNum(i, 10);
                        dictname = "(" + spath_count + ")";
                    }
                    m.moduleName = dictname;
                    m.moduleType = QString::number(Bible::None);
                    m.isDir = true;
                } else {
                    QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("The file is not valid"));
                    return;
                }


                myDebug() << "new Module file" << f << " moduleName" << bibleName << " moduleType" << moduleType;
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
                m_set.module << m;
            }
            progress.close();
            m_ui->treeWidget_module->insertTopLevelItems(0, items);
        }

    }
    return;
}
void SettingsDialog::removeModule()
{
    int row = m_ui->treeWidget_module->indexOfTopLevelItem(m_ui->treeWidget_module->currentItem());
    //remove from listWidget
    QTreeWidgetItem * token = m_ui->treeWidget_module->currentItem();
    delete token;
    //remove from settings
    m_set.module.removeAt(row);
    return;
}
void SettingsDialog::editModule()
{
    //DEBUG_FUNC_NAME
    int row = m_ui->treeWidget_module->indexOfTopLevelItem(m_ui->treeWidget_module->currentItem());
    if (row >= 0) {
        ModuleConfigDialog *mDialog = new ModuleConfigDialog(this);
        mDialog->setModule(m_set.module.at(row));
        connect(mDialog, SIGNAL(save(ModuleSettings)), this, SLOT(saveModule(ModuleSettings)));
        connect(mDialog, SIGNAL(save(ModuleSettings)), mDialog, SLOT(close()));
        mDialog->show();
        mDialog->exec();
    }

}
void SettingsDialog::saveModule(ModuleSettings c)
{
    int row = m_ui->treeWidget_module->indexOfTopLevelItem(m_ui->treeWidget_module->currentItem());
    m_set.module.replace(row, c);
    generateModuleTree();
}
void SettingsDialog::save(void)
{
    //Informationen aus dem Dialog auslesen
    m_set.encoding = m_encodings.at(m_ui->comboBox_encoding->currentIndex());
    m_set.language = m_langCode.at(m_ui->comboBox_language->currentIndex());
    m_set.autoLayout = m_ui->comboBox_autoLayout->currentIndex();
    int currentInterface = m_ui->comboBox_interface->currentIndex();
    if (currentInterface == 0) {
        m_set.session.setData("interface", "simple");
    } else if (currentInterface == 1) {
        m_set.session.setData("interface", "advanced");
    }
    //Alles andere, wie z.b die Module sind schon gespeichert
    emit settingsChanged(m_set);//Speichern
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
    if (fileName.size() > 0) {
        QProgressDialog progress(QObject::tr("Adding Modules"), QObject::tr("Cancel"), 0, fileName.size() + 1);
        progress.setWindowModality(Qt::WindowModal);
        progress.show();
        progress.setValue(1);
        for (int i = 0; i < fileName.size(); i++) {
            progress.setValue(i + 2);
            if (progress.wasCanceled()) {
                progress.close();
                generateModuleTree();
                return;
            }
            QString f = fileName.at(i);
            QString fileData;
            QString bibleName;
            int imoduleType = 0;
            QString moduleType;
            BibleQuote bq;
            ZefaniaBible zef;
            ZefaniaStrong zefStrong;
            ModuleSettings m;
            zefStrong.setSettings(&m_set);

            QFileInfo fileInfo(f);
            if (fileInfo.isFile()) {
                if (f.endsWith(".zip")) {
                    return;
                    //todo: unzip first
                }
                //open file
                QFile file(f);
                if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("Can not read the file"));
                    progress.close();
                    generateModuleTree();
                    return;
                }
                QTextStream in(&file);
                fileData = in.readAll();

                if (fileData.contains("BookQty", Qt::CaseInsensitive)) {
                    imoduleType = 1;// BibleQuote
                } else if (fileData.contains("XMLBIBLE", Qt::CaseInsensitive)) {
                    imoduleType = 2;// Zefania Bible
                } else if (fileData.contains("<dictionary type=\"x-strong\"", Qt::CaseInsensitive)) {
                    imoduleType = 3;// Zefania Strong
                } else {
                    QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("The file is not valid"));
                    progress.close();
                    generateModuleTree();
                    return;
                }

                switch (imoduleType) {
                case Module::BibleQuoteModule:
                    bibleName = bq.readInfo(file);
                    moduleType = QObject::tr("Bible Quote");
                    break;
                case Module::ZefaniaBibleModule:
                    bibleName = zef.readInfo(fileData);
                    moduleType = QObject::tr("Zefania XML");
                    break;
                case Module::ZefaniaStrongModule:
                    bibleName = zefStrong.loadFile(fileData, f);
                    moduleType = QObject::tr("Zefania XML Strong");
                    break;

                }
                m.modulePath = f;
                if (names.size() > 0) {
                    m.moduleName = names.at(i);
                } else {
                    m.moduleName = bibleName;
                }
                m.moduleType = QString::number(imoduleType);
                m.isDir = false;

            } else {
                QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("The file is not valid"));
                progress.close();
                generateModuleTree();
                return;
            }


            myDebug() << "new Module file" << f << " moduleName" << bibleName << " moduleType" << moduleType;
            // standard config
            m.biblequote_removeHtml = m_set.removeHtml;
            m.zefbible_hardCache = m_set.zefaniaBible_hardCache;
            m.zefbible_softCache = m_set.zefaniaBible_softCache;
            m.zefbible_textFormatting = m_set.textFormatting;
            m.zefbible_showStrong = true;
            m.zefbible_showStudyNote = true;
            m.encoding = "Default";//no translating
            m_set.module << m;
        }
        progress.close();
        generateModuleTree();
    }
    myDebug() << "files = " << fileName;
}

void SettingsDialog::setCurrentTab(int tabID)
{
    m_ui->tabWidget->setCurrentIndex(tabID);
}

void SettingsDialog::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
