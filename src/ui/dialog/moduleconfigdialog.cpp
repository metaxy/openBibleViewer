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
#include "moduleconfigdialog.h"
#include "ui_moduleconfigdialog.h"
#include "src/module/bible/zefania-bible.h"
#include "src/module/module.h"
#include "src/core/dbghelper.h"


#include <QtGui/QFileDialog>

ModuleConfigDialog::ModuleConfigDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::ModuleConfigDialog)
{
    m_ui->setupUi(this);
    connect(m_ui->pushButton_ok, SIGNAL(clicked()), this, SLOT(bsave()));
    connect(m_ui->pushButton_cancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(m_ui->toolButton_file, SIGNAL(clicked()), this, SLOT(fileSelect()));
    connect(m_ui->comboBox_type, SIGNAL(currentIndexChanged(int)), this, SLOT(moduleTypeChanged(int)));
    connect(m_ui->checkBox_useParentSettings, SIGNAL(stateChanged(int)), this, SLOT(useParentsSettingsChanged(int)));
    connect(m_ui->checkBox_showStrong, SIGNAL(clicked(bool)), m_ui->checkBox_showStrongsInline, SLOT(setEnabled(bool)));

    m_ui->comboBox_type->insertItems(0, ModuleTools::moduleTypeNames());

    QStringList defaultModules;


    defaultModules << tr("Not a Default Module") << tr("Default Bible")
                   << tr("Default Strong Dictionary") << tr("Default Grammar Dictionary")
                   << tr("Default Dictionary") << tr("Default RMAC Module");
    m_ui->comboBox_defaultModule->insertItems(0, defaultModules);

    QStringList contentTypes;
    contentTypes << tr("Unkown") << tr("Bible")
                   << tr("Bible OT") << tr("Bible NT")
                   << tr("Strongs") << tr("Strongs Greek") << tr("Strongs Hebrew")
                   << tr("RMAC")
                   << tr("Word Dictionary")
                   << tr("Dictionary")
                   << tr("Gram Dictionary");
    m_ui->comboBox_contentType->insertItems(0, contentTypes);


    m_encodings << "Default" << "Apple Roman" << "Big5" << "Big5-HKSCS" << "EUC-JP" << "EUC-KR" << "GB18030-0" << "IBM 850"
                << "IBM 866" << "IBM 874" << "ISO 2022-JP" << "ISO 8859-1" << "ISO 8859-2" << "ISO 8859-3" << "ISO 8859-4"
                << "ISO 8859-5" << "ISO 8859-6" << "ISO 8859-7" << "ISO 8859-8" << "ISO 8859-9" << "ISO 8859-10"
                << "ISO 8859-13" << "ISO 8859-14" << "ISO 8859-15" << "ISO 8859-16" << "Iscii-Bng" << "Dev" << "Gjr"
                << "Knd" << "Mlm" << "Ori" << "Pnj" << "Tlg" << "Tml" << "JIS X 0201" << "JIS X 0208" << "KOI8-R"
                << "KOI8-U" << "MuleLao-1" << "ROMAN8" << "Shift-JIS" << "TIS-620" << "TSCII" << "UTF-8" << "UTF-16"
                << "UTF-16BE" << "UTF-16LE" << "UTF-32" << "UTF-32BE" << "UTF-32LE" << "Windows-1250" << "Windows-1251" << "Windows-1252"
                << "Windows-1253" << "Windows-1254" << "Windows-1255" << "Windows-1256" << "Windows-1257" << "Windows-1258" << "WINSAMI2";
    m_ui->comboBox_encoding->clear();
    m_ui->comboBox_encoding->insertItems(0, m_encodings);
}

ModuleConfigDialog::~ModuleConfigDialog()
{
    delete m_ui;
}

void ModuleConfigDialog::setModule(ModuleSettings *config)
{
    m_moduleSettings = config;

    m_ui->lineEdit_name->setText(config->moduleName);
    m_ui->lineEdit_path->setText(config->modulePath);
    m_ui->lineEdit_shortName->setText(config->moduleShortName);
    m_ui->comboBox_type->setCurrentIndex(config->moduleType);
    m_ui->comboBox_textFromatting->setCurrentIndex(config->zefbible_textFormatting);
    if(config->biblequote_removeHtml == true)
        m_ui->checkBox_removeHtml->setChecked(true);
    else
        m_ui->checkBox_removeHtml->setChecked(false);

    if(config->zefbible_hardCache == true)
        m_ui->checkBox_hardCache->setChecked(true);
    else
        m_ui->checkBox_hardCache->setChecked(false);

    if(config->zefbible_softCache == true)
        m_ui->checkBox_softCache->setChecked(true);
    else
        m_ui->checkBox_softCache->setChecked(false);


    if(m_encodings.lastIndexOf(config->encoding) != -1) {
        m_ui->comboBox_encoding->setCurrentIndex(m_encodings.lastIndexOf(config->encoding));
    } else {
        m_ui->comboBox_encoding->setCurrentIndex(0);
    }
    if(config->useParentSettings) {
        m_ui->checkBox_useParentSettings->setCheckState(Qt::Checked);
    } else {
        m_ui->checkBox_useParentSettings->setCheckState(Qt::Unchecked);
    }
    useParentsSettingsChanged(m_ui->checkBox_useParentSettings->checkState());

    ModuleDisplaySettings *dis = NULL;
    if(!config->displaySettings().isNull()) {
        dis = config->displaySettings().data();

    } else if(config->parent()) {
        dis = config->parent()->displaySettings().data();
    }
    if(dis != NULL) {
        m_ui->checkBox_showBottomToolbar->setChecked(dis->showBottomToolBar());
        m_ui->checkBox_showMarks->setChecked(dis->showMarks());
        m_ui->checkBox_showNotes->setChecked(dis->showNotes());
        m_ui->checkBox_showRefLinks->setChecked(dis->showRefLinks());
        m_ui->checkBox_showStrong->setChecked(dis->showStrong());
        m_ui->checkBox_showStudyNotes->setChecked(dis->showStudyNotes());
        m_ui->checkBox_showRMac->setChecked(dis->showRMAC());
        m_ui->checkBox_showCaptions->setChecked(dis->showCaptions());
        m_ui->checkBox_showProlog->setChecked(dis->showProlog());
        m_ui->checkBox_showStrongsInline->setChecked(dis->showStrongInline());
        m_ui->checkBox_showStrongsInline->setEnabled(dis->showStrong());
    }

    m_ui->lineEdit_styleSheet->setText(config->styleSheet);
    m_ui->lineEdit_language->setText(config->moduleLanguage);
    m_ui->comboBox_defaultModule->setCurrentIndex((int) config->defaultModule);
    m_ui->comboBox_contentType->setCurrentIndex((int)config->contentType);
}
void ModuleConfigDialog::bsave()
{
    //remove hard cache of zefania Bible Modules
    if(m_moduleSettings->moduleType == ModuleTools::ZefaniaBibleModule &&
            (m_moduleSettings->encoding != m_encodings.at(m_ui->comboBox_encoding->currentIndex()) ||
             (int)m_moduleSettings->moduleType != m_ui->comboBox_type->currentIndex() ||
             m_moduleSettings->modulePath != m_ui->lineEdit_path->text())) {

        ZefaniaBible zef;
        zef.removeHardCache(m_ui->lineEdit_path->text());
        if(m_moduleSettings->modulePath != m_ui->lineEdit_path->text()) {
            zef.removeHardCache(m_moduleSettings->modulePath);
        }
    }
    m_moduleSettings->moduleName = m_ui->lineEdit_name->text();
    m_moduleSettings->modulePath = m_ui->lineEdit_path->text();
    m_moduleSettings->moduleShortName = m_ui->lineEdit_shortName->text();
    m_moduleSettings->moduleType = (ModuleTools::ModuleType) m_ui->comboBox_type->currentIndex();
    m_moduleSettings->zefbible_textFormatting = (ModuleSettings::ZefBible_TextFormating) m_ui->comboBox_textFromatting->currentIndex();
    m_moduleSettings->biblequote_removeHtml = m_ui->checkBox_removeHtml->isChecked();
    m_moduleSettings->zefbible_hardCache = m_ui->checkBox_hardCache->isChecked();
    m_moduleSettings->zefbible_softCache = m_ui->checkBox_softCache->isChecked();
    m_moduleSettings->encoding = m_encodings.at(m_ui->comboBox_encoding->currentIndex());
    m_moduleSettings->styleSheet = m_ui->lineEdit_styleSheet->text();
    m_moduleSettings->moduleLanguage = m_ui->lineEdit_language->text();

    m_moduleSettings->defaultModule = (ModuleTools::DefaultModule) m_ui->comboBox_defaultModule->currentIndex();
    m_moduleSettings->contentType = (ModuleTools::ContentType) m_ui->comboBox_contentType->currentIndex();

    m_moduleSettings->useParentSettings = m_ui->checkBox_useParentSettings->isChecked();
    if(!m_moduleSettings->useParentSettings) {
        m_moduleSettings->displaySettings()->setShowBottomToolBar(m_ui->checkBox_showBottomToolbar->isChecked());
        m_moduleSettings->displaySettings()->setShowMarks(m_ui->checkBox_showMarks->isChecked());
        m_moduleSettings->displaySettings()->setShowNotes(m_ui->checkBox_showNotes->isChecked());
        m_moduleSettings->displaySettings()->setShowRefLinks(m_ui->checkBox_showRefLinks->isChecked());
        m_moduleSettings->displaySettings()->setShowStrong(m_ui->checkBox_showStrong->isChecked());
        m_moduleSettings->displaySettings()->setShowStudyNotes(m_ui->checkBox_showStudyNotes->isChecked());
        m_moduleSettings->displaySettings()->setShowRMAC(m_ui->checkBox_showRMac->isChecked());
        m_moduleSettings->displaySettings()->setShowCaptions(m_ui->checkBox_showCaptions->isChecked());
        m_moduleSettings->displaySettings()->setShowProlog(m_ui->checkBox_showProlog->isChecked());
        m_moduleSettings->displaySettings()->setShowStrongInline(m_ui->checkBox_showStrongsInline->isChecked());

    }
    emit save(m_moduleSettings->moduleID);
    close();
}
void ModuleConfigDialog::moduleTypeChanged(int id)
{
    m_ui->groupBox_bq->setVisible(false);
    m_ui->groupBox_zefBible->setVisible(false);
    id = m_ui->comboBox_type->currentIndex();
    switch(id) {
    case ModuleTools::BibleQuoteModule:
        m_ui->groupBox_bq->setVisible(true);
        break;
    case ModuleTools::ZefaniaBibleModule:
        m_ui->groupBox_zefBible->setVisible(true);
        break;

    }
}
void ModuleConfigDialog::fileSelect()
{
    const QString fileName = QFileDialog::getOpenFileName(this, tr("Open Bible"), m_moduleSettings->modulePath, tr("Modules (*.ini *.xml *.*)"));
    if(!fileName.isEmpty()) {
        m_ui->lineEdit_path->setText(fileName);
    }
    return;
}
void ModuleConfigDialog::useParentsSettingsChanged(int newState)
{
    if(newState == Qt::Unchecked) {
        m_ui->frame_display->setDisabled(false);
    } else {
        m_ui->frame_display->setDisabled(true);
    }
}

void ModuleConfigDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch(e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
