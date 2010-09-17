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
#include "moduledownloaddialog.h"
#include "ui_moduledownloaddialog.h"
#include "src/core/dbghelper.h"
#include <QtCore/QList>
#include <QtCore/QDir>
#include <QtCore/QUrl>
#include <QtCore/QFile>
#include <QtGui/QMessageBox>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QStyle>
#include <QtGui/QIcon>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <QtCore/QSet>
#include "src/core/moduledownloader.h"
ModuleDownloadDialog::ModuleDownloadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModuleDownloadDialog)
{
    ui->setupUi(this);
    connect(ui->pushButton_download, SIGNAL(clicked()), this, SLOT(download()));//set httpRequestAborted = false
    connect(ui->pushButton_cancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(item(QTreeWidgetItem*)));

}
/**
  Read the xml file withe downloadable modules and generate the tree.
  */
void ModuleDownloadDialog::readModules()
{
    //DEBUG_FUNC_NAME
    QDomDocument doc;
    QFile file(":/data/modules.xml");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        myWarning() << "can't read the file";
        return;
    }
    if(!doc.setContent(&file)) {
        myWarning() << "the file isn't valid";
        return;
    }
    QList<QTreeWidgetItem *> items;

    QDomNode id = doc.documentElement().firstChild();
    for(; !id.isNull();) {
        QMap<QString, QStringList> map2;
        QMap<QString, QStringList> map;
        QDomNode item = id.firstChild();
        for(; !item.isNull();) {
            QDomElement e = item.toElement();
            map[e.attribute("lang", "unkown")].append(e.firstChild().toText().data());
            map2[e.attribute("lang", "unkown")].append(e.attribute("link", ""));
            item = item.nextSibling();
        }
        QTreeWidgetItem *top = new QTreeWidgetItem(ui->treeWidget);

        QStyle *style = ui->treeWidget->style();
        QIcon folderIcon;
        folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirClosedIcon), QIcon::Normal, QIcon::Off);
        folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirOpenIcon), QIcon::Normal, QIcon::On);
        top->setIcon(0, folderIcon);
        QDomElement e = id.toElement();
        QString typ = e.attribute("typ", "bible");
        if(typ == "bible") {
            top->setText(0, tr("Bibles"));
        } else if(typ == "strong") {
            top->setText(0, tr("Dictionarys"));
        }
        items.append(top);

        QMapIterator<QString, QStringList> i(map);
        while(i.hasNext()) {
            i.next();
            if(i.value().size() > 0) {
                QTreeWidgetItem *langItem = new QTreeWidgetItem();
                langItem->setText(0, i.key());
                langItem->setCheckState(0, Qt::Unchecked);
                langItem->setData(1, 0, "lang");
                top->addChild(langItem);
                for(int a = 0; a < i.value().size(); a++) {
                    QTreeWidgetItem *bibleItem = new QTreeWidgetItem();
                    bibleItem->setData(1, 0, "module");
                    bibleItem->setData(2, 0, map2[i.key()].at(a));
                    bibleItem->setData(2, 1, map[i.key()].at(a));
                    bibleItem->setCheckState(0, Qt::Unchecked);
                    bibleItem->setText(0, i.value().at(a));
                    langItem->addChild(bibleItem);
                }
            }
        }
        top->setExpanded(true);
        id = id.nextSibling();
    }
}
void ModuleDownloadDialog::item(QTreeWidgetItem* i)
{
    //DEBUG_FUNC_NAME
    //myDebug() << "i = " << i->data(1, 0) << " downloadList = " << m_downloadList;
    if(i->data(1, 0) == "lang") {
        if(i->checkState(0) == Qt::Checked) {
            for(int c = 0; c < i->childCount(); c++) {
                i->child(c)->setCheckState(0, Qt::Checked);
                item(i->child(c));
            }
            //check also all childrem of this
        } else if(i->checkState(0) == Qt::Unchecked) {
            for(int c = 0; c < i->childCount(); c++) {
                i->child(c)->setCheckState(0, Qt::Unchecked);
                item(i->child(c));
            }
            //uncheck all children
        }
    } else if(i->data(1, 0) == "module") {

        if(i->checkState(0) == Qt::Checked) {
            m_downloadList.append(i->data(2, 0).toString());
            m_names[i->data(2, 0).toString()] = i->data(2, 1).toString();
            // add to download list
        } else if(i->checkState(0) == Qt::Unchecked) {
            m_downloadList.removeOne(i->data(2, 0).toString());
            m_names[i->data(2, 0).toString()] = "";
        }
    }
}
void ModuleDownloadDialog::setSettings(Settings settings)
{
    m_set = settings;
}
void ModuleDownloadDialog::download()
{
    DEBUG_FUNC_NAME
    ModuleDownloader *m = new ModuleDownloader(this, m_downloadList, m_names);
    connect(m, SIGNAL(downloaded(QStringList, QStringList)), this, SIGNAL(downloaded(QStringList, QStringList)));
    m->setSettings(&m_set);
    m->start();
}

ModuleDownloadDialog::~ModuleDownloadDialog()
{
    delete ui;
}

void ModuleDownloadDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch(e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
