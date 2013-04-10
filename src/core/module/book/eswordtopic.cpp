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
#include "eswordtopic.h"
#include <QtSql/QSqlQuery>
#include "src/core/module/response/stringresponse.h"
#include "src/extern/rtf-qt/rtfreader.h"
#include "src/extern/rtf-qt/ESwordRtfOutput.h"
#include "src/core/rtftools.h"
#include <QTemporaryFile>
ESwordTopic::ESwordTopic() : m_bookTree(NULL)
{
}
ESwordTopic::~ESwordTopic()
{
    m_db.close();
    if(m_bookTree != NULL) {
        delete m_bookTree;
        m_bookTree = NULL;
    }
}
MetaInfo ESwordTopic::readInfo(const QString &name)
{
    DEBUG_FUNC_NAME;
    MetaInfo ret;
  /*  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
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
    }*/
    ret.setName(name);
    return ret;
}

void ESwordTopic::search(const SearchQuery &query, SearchResult *res) const
{
}

bool ESwordTopic::hasIndex() const
{
    return false;
}
void ESwordTopic::buildIndex()
{

}

int ESwordTopic::moduleID() const
{
    return m_moduleID;
}
QString ESwordTopic::modulePath() const
{
    return m_path;
}
QString ESwordTopic::moduleName(bool preferShortName) const
{
    return "";
}

int ESwordTopic::loadModuleData(const int moduleID, const QString &path)
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
    int counter = 0;
    if(m_bookTree != NULL) {
        delete m_bookTree;
        m_bookTree = NULL;
    }
    m_bookTree = new BookTree();
    QSqlQuery query("select Title from Topics", m_db);
    while (query.next()) {
        m_chapterData.insert(counter, query.value(0).toString());
        m_bookTree->addChildren(counter, query.value(0).toString());
        counter++;
    }
    m_loaded = true;
    return 0;
}

Response* ESwordTopic::readChapter(const int chapterID)
{
    DEBUG_FUNC_NAME
    QTextDocument *rtfDocument = new QTextDocument( NULL );
    myDebug() << chapterID << m_chapterData.value(chapterID);
    QSqlQuery query("select Title,Notes from Topics where Title = '"+ m_chapterData.value(chapterID)+ "'", m_db);

    bool hasAny = false;
    while (query.next()) {
        QTemporaryFile file;
        if (file.open()) {
            QTextStream out(&file);
            myDebug() << query.value(1).toString();
            out << RtfTools::toValidRTF(query.value(1).toString());
            file.close();
            RtfReader::Reader *reader = new RtfReader::Reader( NULL );
            bool result = reader->open(file.fileName());
            if(result) {
                RtfReader::ESwordRtfOutput *output = new RtfReader::ESwordRtfOutput( rtfDocument );
                output->setSettings(m_settings);
                reader->parseTo( output );

                hasAny = true;
            }
            delete reader;
        }

    }
    if(!hasAny) {
        QSqlQuery query("select Title,Notes from Topics", m_db);
        int counter = 0;
        while (query.next()) {
            if(counter == chapterID) {
                QTemporaryFile file;
                if (file.open()) {
                    QTextStream out(&file);
                    out << RtfTools::toValidRTF(query.value(1).toString());
                    file.close();
                    RtfReader::Reader *reader = new RtfReader::Reader( NULL );
                    bool result = reader->open(file.fileName());
                    if(result) {
                        RtfReader::ESwordRtfOutput *output = new RtfReader::ESwordRtfOutput( rtfDocument );
                        output->setSettings(m_settings);
                        reader->parseTo( output );
                    }
                    delete reader;
                }
            }
            counter++;

        }
    }

    StringResponse *res = new StringResponse(rtfDocument->toHtml());
    delete rtfDocument;
    return res;
}

BookTree * ESwordTopic::bookTree()
{
    DEBUG_FUNC_NAME
    return m_bookTree;
}
