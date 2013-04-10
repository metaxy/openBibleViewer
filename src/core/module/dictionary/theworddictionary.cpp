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
#include "theworddictionary.h"
#include <QtSql/QSqlQuery>
#include "src/core/module/response/htmlresponse.h"
#include "src/extern/rtf-qt/rtfreader.h"
#include "src/extern/rtf-qt/TheWordRtfOutput.h"
#include "src/core/rtftools.h"
#include <QTemporaryFile>
TheWordDictionary::TheWordDictionary() : m_compressed(false), m_secure(false), m_doEncrypt(false)
{
}
Response* TheWordDictionary::getEntry(const QString &entry)
{
    if(!m_loaded) {
        loadModuleData(m_moduleID);
    }
    if(m_secure || m_doEncrypt) {
        return new StringResponse(QObject::tr("This module is encrypted."));
    }
    QTextDocument *rtfDocument = new QTextDocument( NULL );
    QSqlQuery query("select id,subject FROM topics WHERE subject = '"+entry+"'", m_db);
    while (query.next()) {
        //const int id = query.value(0).toInt();
        const QString subject = query.value(1).toString();

        QSqlQuery query2("select data from content where topic_id = '" + query.value(0).toString() + "'", m_db);

        while(query2.next()) {
            QTemporaryFile file;
            if (file.open()) {
                QTextStream out(&file);
                if(m_compressed) {
                    out << RtfTools::toValidRTF(RtfTools::gUncompress(query2.value(0).toByteArray()));
                } else {
                    out << RtfTools::toValidRTF(query2.value(0).toString());
                }
                file.close();
                RtfReader::Reader *reader = new RtfReader::Reader( NULL );
                bool result = reader->open(file.fileName());
                if(result) {
                    RtfReader::TheWordRtfOutput *output = new RtfReader::TheWordRtfOutput( rtfDocument, subject);
                    output->setSettings(m_settings);
                    reader->parseTo( output );
                }
                delete reader;
            }
        }
    }
    HtmlResponse *res = new HtmlResponse(rtfDocument->toHtml());
    delete rtfDocument;
    return res;

}

QStringList TheWordDictionary::getAllKeys()
{
    if(!m_loaded) {
        loadModuleData(m_moduleID);
    }
    QStringList ret;
    QSqlQuery query("select id,subject FROM topics", m_db);
    while (query.next()) {
        const QString subject = query.value(1).toString();
        ret << subject;
    }
    return ret;
}

Response::ResponseType TheWordDictionary::responseType() const
{
    return Response::HtmlResponse;
}


int TheWordDictionary::loadModuleData(const int moduleID)
{
    DEBUG_FUNC_NAME;
    m_loaded = false;

    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(m_settings->getModuleSettings(moduleID)->modulePath);
    if (!m_db.open()) {
        myWarning() << "could not open database " << m_settings->getModuleSettings(moduleID)->modulePath;
        return 1;
    }
    QSqlQuery query ("select name,value from config", m_db);
    while (query.next()) {
        const QString name = query.value(0).toString();
        const QString value = query.value(1).toString();
        if(name == "compressed") {
            if(value == "1") {
                m_compressed = true;
            } else if(value == "0") {
                m_compressed = false;
            }
        }
        if(name == "secure" && value == "1") {
            m_secure = true;
        }
        if(name == "do.encrypt") {
            m_doEncrypt = true;
        }
    }
    m_loaded = true;
    return 0;
}
MetaInfo TheWordDictionary::readInfo(const QString &name)
{
    DEBUG_FUNC_NAME;
    MetaInfo ret;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(name);
    if (!db.open()) {
        myWarning() << "could not open database " << name;
        return MetaInfo();
    }
    QSqlQuery query ("select name,value from config", db);
    while (query.next()) {
        const QString name = query.value(0).toString();
        const QString value = query.value(1).toString();
        myDebug() << name << value;
        if(name == "title") {
            ret.setName(value);
        } else if(name == "abbrev") {
            ret.setShortName(value);
        } else if(name == "description") {
            ret.description = value;
        }
    }
    db.close();
    return ret;
}
