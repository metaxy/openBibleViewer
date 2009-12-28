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
#include "moduleconfigdialog.h"
#include "src/core/dbghelper.h"
#include "src/module/zefania-bible.h"
#include "src/module/bible.h"
#include "ui_moduleconfigdialog.h"
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
}

ModuleConfigDialog::~ModuleConfigDialog()
{
    delete m_ui;
}
void ModuleConfigDialog::setModule(ModuleSettings config)
{
    DEBUG_FUNC_NAME
    c = config;
    m_ui->lineEdit_name->setText(config.moduleName);
    m_ui->lineEdit_path->setText(config.modulePath);
    m_ui->comboBox_type->setCurrentIndex(config.moduleType.toInt());
    m_ui->comboBox_textFromatting->setCurrentIndex(config.zefbible_textFormatting);
    if (config.biblequote_removeHtml == true)
        m_ui->checkBox_removeHtml->setChecked(true);
    else
        m_ui->checkBox_removeHtml->setChecked(false);

    if (config.zefbible_hardCache == true)
        m_ui->checkBox_hardCache->setChecked(true);
    else
        m_ui->checkBox_hardCache->setChecked(false);

    if (config.zefbible_softCache == true)
        m_ui->checkBox_softCache->setChecked(true);
    else
        m_ui->checkBox_softCache->setChecked(false);

    if (config.zefbible_showStrong == true)
        m_ui->checkBox_showStrong->setChecked(true);
    else
        m_ui->checkBox_showStrong->setChecked(false);

    if (config.zefbible_showStudyNote == true)
        m_ui->checkBox_showStudyNote->setChecked(true);
    else
        m_ui->checkBox_showStudyNote->setChecked(false);
    encodings << "Default" << "Apple Roman" << "Big5" << "Big5-HKSCS" << "EUC-JP" << "EUC-KR" << "GB18030-0" << "IBM 850"
    << "IBM 866" << "IBM 874" << "ISO 2022-JP" << "ISO 8859-1" << "ISO 8859-2" << "ISO 8859-3" << "ISO 8859-4"
    << "ISO 8859-5" << "ISO 8859-6" << "ISO 8859-7" << "ISO 8859-8" << "ISO 8859-9" << "ISO 8859-10"
    << "ISO 8859-13" << "ISO 8859-14" << "ISO 8859-15" << "ISO 8859-16" << "Iscii-Bng" << "Dev" << "Gjr"
    << "Knd" << "Mlm" << "Ori" << "Pnj" << "Tlg" << "Tml" << "JIS X 0201" << "JIS X 0208" << "KOI8-R"
    << "KOI8-U" << "MuleLao-1" << "ROMAN8" << "Shift-JIS" << "TIS-620" << "TSCII" << "UTF-8" << "UTF-16"
    << "UTF-16BE" << "UTF-16LE" << "UTF-32" << "UTF-32BE" << "UTF-32LE" << "Windows-1250" << "Windows-1251" << "Windows-1252"
    << "Windows-1253" << "Windows-1254" << "Windows-1255" << "Windows-1256" << "Windows-1257" << "Windows-1258" << "WINSAMI2";
    m_ui->comboBox_encoding->clear();
    m_ui->comboBox_encoding->insertItems(0, encodings);
    if (encodings.lastIndexOf(config.encoding) != -1) {
        m_ui->comboBox_encoding->setCurrentIndex(encodings.lastIndexOf(config.encoding));
    } else {
        m_ui->comboBox_encoding->setCurrentIndex(0);
    }
}
void ModuleConfigDialog::bsave()
{
    DEBUG_FUNC_NAME

    if (c.moduleType.toInt() == Bible::ZefaniaBibleModule &&
            (c.encoding != encodings.at(m_ui->comboBox_encoding->currentIndex()) ||
             c.moduleType != QString::number(m_ui->comboBox_type->currentIndex()) ||
             c.modulePath != m_ui->lineEdit_path->text())) {
        myDebug() << "clear hard in zefania cache";
        ZefaniaBible zef;
        zef.removeHardCache(m_ui->lineEdit_path->text());
        if (c.modulePath != m_ui->lineEdit_path->text()) {
            zef.removeHardCache(c.modulePath);
        }
    }
    c.moduleName = m_ui->lineEdit_name->text();
    c.modulePath = m_ui->lineEdit_path->text();
    c.moduleType = QString::number(m_ui->comboBox_type->currentIndex());
    c.zefbible_textFormatting = m_ui->comboBox_textFromatting->currentIndex();
    c.biblequote_removeHtml = m_ui->checkBox_removeHtml->isChecked();
    c.zefbible_hardCache =  m_ui->checkBox_hardCache->isChecked();
    c.zefbible_softCache =  m_ui->checkBox_softCache->isChecked();
    c.zefbible_showStrong =  m_ui->checkBox_showStrong->isChecked();
    c.zefbible_showStudyNote =  m_ui->checkBox_showStudyNote->isChecked();
    c.encoding = encodings.at(m_ui->comboBox_encoding->currentIndex());
    //todo:if path type or encoding changed clear cache
    emit save(c);
}
void ModuleConfigDialog::moduleTypeChanged(int id)
{
    m_ui->groupBox_bq->setVisible(false);
    m_ui->groupBox_zefBible->setVisible(false);
    id = m_ui->comboBox_type->currentIndex();
    switch (id) {
    case Bible::BibleQuoteModule:
        m_ui->groupBox_bq->setVisible(true);
        break;
    case Bible::ZefaniaBibleModule:
        m_ui->groupBox_zefBible->setVisible(true);
        break;

    }
}
void  ModuleConfigDialog::fileSelect()
{
    if (c.isDir) {
        QFileDialog dialog(this);

        dialog.setFileMode(QFileDialog::Directory);
#if QT_VERSION >= 0x040500
        dialog.setOption(QFileDialog::ShowDirsOnly, true);
#endif
        if (dialog.exec()) {
            QStringList fileName = dialog.selectedFiles();
            if (fileName.size() > 0) {
                m_ui->lineEdit_path->setText(fileName.at(0));
            }
        }
    } else {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open Bible"), c.modulePath, tr("Bibles (*.ini *.xml *.*)"));
        if (fileName != "") {
            m_ui->lineEdit_path->setText(fileName);
        }
    }
    return;
}
void ModuleConfigDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
