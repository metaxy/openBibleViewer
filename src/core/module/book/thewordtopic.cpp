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
#include "thewordtopic.h"
#include <QtSql/QSqlQuery>
#include "src/core/module/response/htmlresponse.h"
#include "src/extern/rtf-qt/rtfreader.h"
#include "src/extern/rtf-qt/TheWordRtfOutput.h"
#include <QTemporaryFile>
TheWordTopic::TheWordTopic() : m_bookTree(nullptr), m_compressed(false), m_contentType(RtfTools::RTFContent)
{
}
TheWordTopic::~TheWordTopic()
{
    if(m_bookTree != nullptr) {
        delete m_bookTree;
        m_bookTree = nullptr;
    }
}

MetaInfo TheWordTopic::readInfo(const QString &name)
{
    DEBUG_FUNC_NAME;
    MetaInfo ret;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(name);
    if (!db.open()) {
        myWarning() << "could not open database " << name;
        return MetaInfo();
    }
    QSqlQuery query1 ("select name,value from config", db);
    while (query1.next()) {
        const QString name = query1.value(0).toString();
        const QString value = query1.value(1).toString();
        myDebug() << name << value;
        if(name == "title") {
            ret.setName(value);
        } else if(name == "abbrev") {
            ret.setShortName(value);
        }
    }
    db.close();
    return ret;
}

void TheWordTopic::search(const SearchQuery &query, SearchResult *res) const
{
}

bool TheWordTopic::hasIndex() const
{
    return false;
}
void TheWordTopic::buildIndex()
{

}

int TheWordTopic::moduleID() const
{
    return m_moduleID;
}
QString TheWordTopic::modulePath() const
{
    return m_path;
}
QString TheWordTopic::moduleName(bool preferShortName) const
{
    return m_settings->getModuleSettings(m_moduleID)->name(preferShortName);
}

int TheWordTopic::loadModuleData(const int moduleID, const QString &path)
{
    DEBUG_FUNC_NAME

    myDebug() << moduleID << path;
    m_loaded = false;

    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);
    if (!m_db.open()) {
        myWarning() << "could not open database " << path;
        return 1;
    }
    QSqlQuery query1 ("select name,value from config", m_db);
    while (query1.next()) {
        const QString name = query1.value(0).toString();
        const QString value = query1.value(1).toString();
        if(name == "content.type") {
            if(value == "rvf") {
                m_contentType = RtfTools::RVFContent;
            } else if(value == "rtf"){
                m_contentType = RtfTools::RTFContent;
            }
        } else if(name == "compressed") {
            if(value == "1") {
                m_compressed = true;
            }
        }
    }

    if(m_bookTree != nullptr) {
        delete m_bookTree;
        m_bookTree = nullptr;
    }
    m_bookTree = new BookTree();
    QHash<int, BookTree*> map;
    map.insert(0, m_bookTree);
    QSqlQuery query("select id,pid,subject from topics", m_db);
    while (query.next()) {
        const int id = query.value(0).toInt();
        const int pid = query.value(1).toInt();
        const QString subject = query.value(2).toString();
        BookTree *parent = map.value(pid);
        if(parent != nullptr) {
            map.insert(id, parent->addChildren(id, subject));
        } else {
            map.insert(id, m_bookTree->addChildren(id, subject));
        }

    }
    m_loaded = true;
    return 0;
}

Response* TheWordTopic::readChapter(const int chapterID)
{
    DEBUG_FUNC_NAME
    bool haveRtf = false;
    bool haveRvf = false;
    QString ret;
    QTextDocument *rtfDocument = new QTextDocument( nullptr );
    QSqlQuery query("select topic_id,data from content where topic_id = "+ QString::number(chapterID), m_db);
    while (query.next()) {
        if(m_contentType == RtfTools::RTFContent) {
            if(RtfTools::isRvf(query.value(1).toByteArray())) { //sometimes we have rvf content in rtf modules
                readRvf(query.value(1), &ret);
                haveRvf = true;
            } else {
                readRtf(query.value(1), rtfDocument);
                haveRtf = true;
            }
        } else {
            readRvf(query.value(1), &ret);
            haveRvf = true;
        }
    }
    Response *res = nullptr;
    if(haveRvf && haveRtf) {
        myWarning() << "we cannot have both";
    } else if(haveRtf) {
        res = new HtmlResponse(rtfDocument->toHtml());
        myDebug() << "html response";
    } else if(haveRvf){
        res = new StringResponse(ret);
        myDebug() << "string response";
    } else {
        res = new StringResponse("");
    }
    delete rtfDocument;
    return res;
}
void TheWordTopic::readRtf(const QVariant &value, QTextDocument *rtfDocument)
{
    DEBUG_FUNC_NAME
    QTemporaryFile file;
    if (file.open()) {
        QTextStream out(&file);
        if(m_compressed) {
            out << RtfTools::toValidRTF(RtfTools::gUncompress(value.toByteArray()));
        } else {
            out << RtfTools::toValidRTF(value.toString());
        }
        file.close();
        RtfReader::Reader *reader = new RtfReader::Reader( nullptr );
        bool result = reader->open(file.fileName());
        if(result) {
            RtfReader::TheWordRtfOutput *output = new RtfReader::TheWordRtfOutput( rtfDocument, "" );
            output->setSettings(m_settings);
            reader->parseTo( output );
        }
        delete reader;
    }
}

void TheWordTopic::readRvf(const QVariant &value, QString *ret)
{
    DEBUG_FUNC_NAME
    if(m_compressed) {
        ret->append(RtfTools::fromRVF(RtfTools::gUncompress(value.toByteArray())));
    } else {
        ret->append(RtfTools::fromRVF(value.toByteArray()));
    }
}
BookTree * TheWordTopic::bookTree()
{
    DEBUG_FUNC_NAME
    return m_bookTree;
}

