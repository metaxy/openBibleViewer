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
#include "zefania-strong.h"

#include <QtXml/QDomNode>
#include "src/core/KoXmlReader.h"
#include "src/core/dbghelper.h"
#include <QtCore/QFile>
#include <QtCore/QVariantList>
#include <QtCore/QFile>
#include <QtCore/QDataStream>
#include <QtCore/QCryptographicHash>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtGui/QMessageBox>
#include <QtGui/QProgressDialog>
#include <QtXml/QDomElement>
ZefaniaStrong::ZefaniaStrong()
{
    m_settings = new Settings();
}
void ZefaniaStrong::setSettings(Settings *settings)
{
    m_settings = settings;
}
/**
  Load a Zefania XML Lex file the first time. Generate a cache file, for fast access.
  */
QString ZefaniaStrong::loadFile(QString fileData, QString fileName)
{
    KoXmlDocument doc;
    if (!doc.setContent(fileData)) {
        QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("The file is not valid"));
        myDebug() << "the file isnt valid";
        return QString();
    }
    QString fileTitle = "";
    QStringList l_id = QStringList();
    QStringList l_title = QStringList();
    QStringList l_trans = QStringList();
    QStringList l_pron = QStringList();
    QStringList l_desc = QStringList();

    KoXmlNode item = doc.documentElement().firstChild();
    for (int c = 0; !item.isNull();) {
        QString id = "";
        QString title = "";
        QString trans = "";
        QString pron = "";
        QString desc = "";
        KoXmlElement e = item.toElement();
        if (e.tagName() == "INFORMATION") {
            KoXmlNode titel = item.namedItem("title");
            fileTitle = titel.toElement().text();
        } else if (e.tagName() == "item") {
            id = e.attribute("id");
            KoXmlNode details = item.firstChild();
            while (!details.isNull()) {
                KoXmlElement edetails = details.toElement();
                if (edetails.tagName() == "title") {
                    title = edetails.text();
                } else if (edetails.tagName() == "transliteration") {
                    trans =  edetails.text();
                } else if (edetails.tagName() == "pronunciation") {
                    KoXmlNode em = details.firstChild();
                    while (!em.isNull()) {
                        if (em.toElement().tagName() == "em")
                            pron = "<em>" + em.toElement().text() + "</em>";
                        em = em.nextSibling();
                    }
                } else if (edetails.tagName() == "description") {
                    KoXmlNode descNode = details.firstChild();
                    while (!descNode.isNull()) {
                        if (descNode.nodeType() == 2) {
                            desc += descNode.toText().data();
                        } else if (descNode.nodeType() == 1) {
                            KoXmlElement descElement  = descNode.toElement();
                            if (descElement.tagName() == "reflink") {
                                QString mscope = descElement.attribute("mscope", ";;;");
                                QStringList list = mscope.split(";");
                                int bookID = list.at(0).toInt() - 1;
                                int chapterID = list.at(1).toInt() - 1;
                                int verseID = list.at(2).toInt() - 1;
                                QString url = "bible://current/" + QString::number(bookID) + "," + QString::number(chapterID) + "," + QString::number(verseID) + ",";
                                QString name = "";

                                if (bookID < m_settings->bookNames.size()) {
                                    name = m_settings->bookNames.at(bookID) + " " + list.at(1) + "," + list.at(2);
                                } else {
                                    name = list.at(0) + " " + list.at(1) + "," + list.at(2);
                                }
                                desc += " <a href=\"" + url + "\">" + name + "</a> ";
                            }
                        }
                        descNode = descNode.nextSibling();
                    }
                    desc += "<hr />";
                }
                details = details.nextSibling();
            }
            l_id << id;
            l_title << title;
            l_trans << trans;
            l_pron << pron;
            l_desc << desc;
        }
        item = item.nextSibling();
        c++;
    }
    doc.clear();
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(fileName.toLocal8Bit());
    QString path = m_settings->homePath + "cache/" + hash.result().toBase64() + ".strong";
    //if cache folder exist then remove it
    QFileInfo info(path);
    if (info.exists()) {
        QFile::remove(path);
    }
    //make a new cache folder
    QDir dir;
    dir.mkpath(m_settings->homePath + "cache/");
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("Can not open cache file."));
        return QString();
    }
    //write data
    QDataStream out(&file);
    out << l_id;
    out << l_title;
    out << l_trans;
    out << l_pron;
    out << l_desc;
    file.close();
    return fileTitle;
}
/**
  Load the cache file into memor
  \fileName Location of the cache file.
  */
bool ZefaniaStrong::loadDataBase(QString fileName)
{
    myDebug() << "fileName = " << fileName;
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(fileName.toLocal8Bit());
    QFile file(m_settings->homePath + "cache/" + hash.result().toBase64() + ".strong");
    myDebug() << "fileName3 = " << m_settings->homePath + "cache/" + hash.result().toBase64() + ".strong";
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("Can not open cache file."));
        return false;
    }
    m_id.clear();
    m_title.clear();
    m_trans.clear();
    m_pron.clear();
    m_desc.clear();
    QDataStream in(&file);
    in >> m_id;
    in >> m_title;
    in >> m_trans;
    in >> m_pron;
    in >> m_desc;
    myDebug() << "fileName2 = " << file.fileName() << " m_id.size() = "  << m_id.size();
    file.close();
    return true;
}
/**
  Returns a Strong.
  \strongID The strongID
  */
QString ZefaniaStrong::getStrong(QString strongID)
{
    myDebug() << "strongID = " << strongID;
    for (int i = 0; i < m_id.size(); ++i) {
        QString id = m_id.at(i);
        if (id.trimmed().toLower() == strongID.trimmed().toLower()) {
            QString ret = "<h3>" + m_id.at(i) + " - " + m_title.at(i) + "</h3>";
            if (m_trans.at(i) != "") {
                ret += " (" + m_trans.at(i) + ") ";
            }
            if (m_pron.at(i) != "") {
                ret += " [" + m_pron.at(i) + "] ";
            }
            ret += "<br />" + m_desc.at(i);
            return ret;
        }
    }
    return QString();
}

