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
#include "../core/config.h"
#include "../core/moduleconfig.h"
#include "../module/zefania-bible.h"
#include "../module/zefania-strong.h"
#include "../module/biblequote.h"
#include "moduleconfigdialog.h"
#include "moduledownloaddialog.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QtDebug>
#include <QtCore/QStringList>
#include <QtGui/QProgressDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>

settingsDialog::settingsDialog(QWidget *parent) : QDialog(parent), m_ui(new Ui::settingsDialog)
{
    m_ui->setupUi(this);
    //connect( m_ui->pushButton_remove_path, SIGNAL( clicked() ), this, SLOT( removePath( ) ) );
    //connect( m_ui->pushButton_add_path, SIGNAL( clicked() ), this, SLOT( addPath( ) ) );
    connect(m_ui->pushButton_downloadModule, SIGNAL(clicked()), this, SLOT(downloadModule()));
    connect(m_ui->pushButton_addFile, SIGNAL(clicked()), this, SLOT(addModuleFile()));
    connect(m_ui->pushButton_addDir, SIGNAL(clicked()), this, SLOT(addModuleDir()));
    connect(m_ui->pushButton_remove_module, SIGNAL(clicked()), this, SLOT(removeModule()));
    connect(m_ui->pushButton_ok, SIGNAL(clicked()), this, SLOT(bsave()));
    connect(m_ui->pushButton_cancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(m_ui->pushButton_reset, SIGNAL(clicked()), this, SLOT(reset()));
    connect(m_ui->pushButton_moduleEdit, SIGNAL(clicked()), this, SLOT(editModule()));
}

settingsDialog::~settingsDialog()
{
    delete m_ui;
}
void settingsDialog::reset()
{
    set = backupSet;
    setSettings(set);
}
int settingsDialog::setSettings(settings_s settings)
{
    set = settings;
    backupSet = set;
    encodings.clear();
    langCode.clear();
    //Allgemeine
    ////Encoding
    encodings << "Apple Roman" << "Big5" << "Big5-HKSCS" << "EUC-JP" << "EUC-KR" << "GB18030-0" << "IBM 850"
    << "IBM 866" << "IBM 874" << "ISO 2022-JP" << "ISO 8859-1" << "ISO 8859-2" << "ISO 8859-3" << "ISO 8859-4"
    << "ISO 8859-5" << "ISO 8859-6" << "ISO 8859-7" << "ISO 8859-8" << "ISO 8859-9" << "ISO 8859-10"
    << "ISO 8859-13" << "ISO 8859-14" << "ISO 8859-15" << "ISO 8859-16" << "Iscii-Bng" << "Dev" << "Gjr"
    << "Knd" << "Mlm" << "Ori" << "Pnj" << "Tlg" << "Tml" << "JIS X 0201" << "JIS X 0208" << "KOI8-R"
    << "KOI8-U" << "MuleLao-1" << "ROMAN8" << "Shift-JIS" << "TIS-620" << "TSCII" << "UTF-8" << "UTF-16"
    << "UTF-16BE" << "UTF-16LE" << "UTF-32" << "UTF-32BE" << "UTF-32LE" << "Windows-1250" << "Windows-1251" << "Windows-1252"
    << "Windows-1253" << "Windows-1254" << "Windows-1255" << "Windows-1256" << "Windows-1257" << "Windows-1258" << "WINSAMI2";
    m_ui->comboBox_encoding->clear();
    m_ui->comboBox_encoding->insertItems(0, encodings);
    m_ui->comboBox_encoding->setCurrentIndex(encodings.lastIndexOf(set.encoding));
    //Language
    QStringList langs;
    langs <<  "English" << "German ( Deutsch )" << QString::fromLocal8Bit("Russian ( русском )");
    langCode << "en" << "de" << "ru";
    qDebug() << "settingsDialog::setSettings() set.language = " << set.language;
    m_ui->comboBox_language->clear();
    m_ui->comboBox_language->insertItems(0, langs);
    int code;
    code = langCode.lastIndexOf(set.language);
    if (code == -1) {
        QString lang = set.language;
        QString onlyLang = lang.remove(lang.lastIndexOf("_"), lang.size());
        code = langCode.lastIndexOf(onlyLang);
    }
    qDebug() << "settingsDialog::setSettings() code = " << code;
    if (code != -1) {
        m_ui->comboBox_language->setCurrentIndex(code);
    } else { // no lang code was written in the config file
        m_ui->comboBox_language->setCurrentIndex(0);
    }
    //Module
    generateModuleTree();


    return 0;

}
void settingsDialog::generateModuleTree()
{
    m_ui->treeWidget_module->clear();
    QList<QTreeWidgetItem *> items;
    for (int i = 0; i < set.module.size(); i++) {
        QTreeWidgetItem *ibible = new QTreeWidgetItem(m_ui->treeWidget_module);
        ibible->setText(0, set.module.at(i).moduleName);
        ibible->setText(1, set.module.at(i).modulePath);
        QString moduleType;
        if (set.module.at(i).isDir) {
            moduleType = QObject::tr("Folder");
        } else {
            switch (set.module.at(i).moduleType.toInt()) {
            case 1:
                moduleType = QObject::tr("Bible Quote");
                break;
            case 2:
                moduleType = QObject::tr("Zefania XML");;
                break;
            case 3:
                moduleType = QObject::tr("Zefania XML Strong");;
                break;

            }
        }
        ibible->setText(2, moduleType);
        items << ibible;
    }
    m_ui->treeWidget_module->insertTopLevelItems(0, items);

}
/*void settingsDialog::addStrong( void )
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::DirectoryOnly);
    if(dialog.exec())
    {
        QStringList fileName = dialog.selectedFiles();
        if(fileName.size() > 0)
        {
            QStringList fileName2;
            for(int i = 0;i<fileName.size();i++)
            {
                QString f = fileName.at(i);
                if(f.endsWith("/"))
                {
                    f.remove(f.size()-1,10);
                }
                fileName2 << f;
            }
            set.path << fileName2;
            m_ui->listWidget_path->clear();
            m_ui->listWidget_path->insertItems(0,set.path);
        }
    }
    return;
}
void settingsDialog::removeStrong()
{
    int row = m_ui->listWidget_path->currentRow();
    //remove from listWidget
    QListWidgetItem * token = m_ui->listWidget_path->takeItem(row);
    delete token;
    //remove from set
    set.path.removeAt(row);
    return;
}*/
void settingsDialog::addModuleFile(void)
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);

    if (dialog.exec()) {
        QStringList fileName = dialog.selectedFiles();
        addModules(fileName, QStringList());
    }
    return;
}
void settingsDialog::addModuleDir(void)
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
                QString fileData;
                QString bibleName;
                QString moduleType;
                biblequote bq;
                zefaniaBible zef;
                moduleConfig m;
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
                    m.moduleType = "0";//mean: dont know
                    m.isDir = true;
                } else {
                    QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("The file is not valid"));
                    return;
                }


                qDebug() << "settingsDialog::addModule() new Module file" << f << " moduleName" << bibleName << " moduleType" << moduleType;
                /*if (progress.wasCanceled())
                    return;*/
                // standard config
                m.biblequote_removeHtml = set.removeHtml;
                m.zefbible_hardCache = set.zefaniaBible_hardCache;
                m.zefbible_textFormatting = set.textFormatting;


                QTreeWidgetItem * ibible = new QTreeWidgetItem(m_ui->treeWidget_module);
                ibible->setText(0, m.moduleName);
                ibible->setText(1, m.modulePath);
                ibible->setText(2, moduleType);
                items << ibible;
                set.module << m;
            }
            progress.close();
            m_ui->treeWidget_module->insertTopLevelItems(0, items);
        }

    }
    return;
}
void settingsDialog::removeModule()
{
    int row = m_ui->treeWidget_module->indexOfTopLevelItem(m_ui->treeWidget_module->currentItem());
    //remove from listWidget
    QTreeWidgetItem * token = m_ui->treeWidget_module->currentItem();
    delete token;
    //remove from settings
    set.module.removeAt(row);
    return;
}
void settingsDialog::editModule()
{
    int row = m_ui->treeWidget_module->indexOfTopLevelItem(m_ui->treeWidget_module->currentItem());
    if (row >= 0) {
        moduleConfigDialog *mDialog = new moduleConfigDialog(this);
        mDialog->setModule(set.module.at(row));
        connect(mDialog, SIGNAL(save(struct moduleConfig)), this, SLOT(saveModule(struct moduleConfig)));
        connect(mDialog, SIGNAL(save(struct moduleConfig)), mDialog, SLOT(close()));
        mDialog->show();
    }

}
void settingsDialog::saveModule(struct moduleConfig c)
{
    int row = m_ui->treeWidget_module->indexOfTopLevelItem(m_ui->treeWidget_module->currentItem());
    set.module.removeAt(row);
    set.module.insert(row, c);
    generateModuleTree();
}
int settingsDialog::bsave(void)
{
    //Informationen aus dem Dialog auslesen
    set.encoding = encodings.at(m_ui->comboBox_encoding->currentIndex());
    set.language = langCode.at(m_ui->comboBox_language->currentIndex());
    emit save(set);//Speichern
    return 0;
}
void settingsDialog::downloadModule()
{
    moduleDownloadDialog *mDialog = new moduleDownloadDialog(this);
    mDialog->setSettings(set);
    mDialog->readModules();
    connect(mDialog, SIGNAL(downloaded(QStringList, QStringList)), this, SLOT(addModules(QStringList, QStringList)));
    connect(mDialog, SIGNAL(downloaded(QStringList, QStringList)), mDialog, SLOT(close()));
    mDialog->show();
}
void settingsDialog::addModules(QStringList fileName, QStringList names)
{
    if (fileName.size() > 0) {
        QProgressDialog progress(QObject::tr("Adding Modules"), QObject::tr("Cancel"), 0, fileName.size());
        progress.setWindowModality(Qt::WindowModal);
        for (int i = 0; i < fileName.size(); i++) {
            progress.setValue(i);
            if (progress.wasCanceled())
                return;
            QString f = fileName.at(i);
            QString fileData;
            QString bibleName;
            int imoduleType = 0;
            QString moduleType;
            biblequote bq;
            zefaniaBible zef;
            zefaniaStrong zefStrong;
            moduleConfig m;
            zefStrong.setSettings(set, m);

            QFileInfo fileInfo(f);
            if (fileInfo.isFile()) {
                //open file
                QProgressDialog progress(QObject::tr("Open File"), QObject::tr("Cancel"), 0, 0);
                progress.setWindowModality(Qt::WindowModal);
                progress.setValue(1);
                QFile file(f);
                if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("Can not read the file"));
                    return;
                }
                QTextStream in(&file);
                progress.setValue(2);
                while (!in.atEnd()) {
                    QString line = in.readLine();
                    fileData += line;
                }
                if (fileData.contains("BookQty", Qt::CaseInsensitive)) {
                    imoduleType = 1;//BibleQuote
                } else if (fileData.contains("XMLBIBLE", Qt::CaseInsensitive)) {
                    imoduleType = 2;//Zefania
                } else if (fileData.contains("<dictionary type=\"x-strong\"", Qt::CaseInsensitive)) {
                    imoduleType = 3;//Zefania Strong
                }
                /*else if(f.endsWith(".xml"))
                {
                    imoduleType = 2;
                }*/
                else {
                    QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("The file is not valid"));
                    return;
                }

                switch (imoduleType) {
                case 1:
                    bibleName = bq.readInfo(file);
                    moduleType = "Bible Quote";
                    break;
                case 2:
                    bibleName = zef.readInfo(fileData);
                    moduleType = "Zefania XML";
                    break;
                case 3:
                    bibleName = zefStrong.loadFile(fileData, f);
                    moduleType = "Zefania XML Strong";
                    break;

                }
                progress.setValue(3);
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
                return;
            }


            qDebug() << "settingsDialog::addModule() new Module file" << f << " moduleName" << bibleName << " moduleType" << moduleType;
            /*if (progress.wasCanceled())
                return;*/
            // standard config
            m.biblequote_removeHtml = set.removeHtml;
            m.zefbible_hardCache = set.zefaniaBible_hardCache;
            m.zefbible_softCache = set.zefaniaBible_softCache;
            m.zefbible_textFormatting = set.textFormatting;
            m.zefbible_showStrong = true;
            m.zefbible_showStudyNote = true;
            set.module << m;
        }
        progress.close();
        generateModuleTree();
    }
    qDebug() << "settingsDialog::addModules files = " << fileName;
}
void settingsDialog::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
