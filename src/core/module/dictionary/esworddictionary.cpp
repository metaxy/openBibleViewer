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
#include "esworddictionary.h"
#include <QtSql/QSqlQuery>
#include "src/core/module/response/htmlresponse.h"
#include "src/extern/rtf-qt/rtfreader.h"
#include "src/extern/rtf-qt/ESwordRtfOutput.h"
#include "src/core/rtftools.h"
#include <QTemporaryFile>


ESwordDictionary::ESwordDictionary()
{

}
Response* ESwordDictionary::getEntry(const QString &entry)
{
    if(this->failed()) {
        return nullptr;
    }

    if(!m_loaded) {
        loadModuleData(m_moduleID);
    }
    QTextDocument *rtfDocument = new QTextDocument( NULL );
    QSqlQuery query("select Definition FROM Dictionary WHERE Topic = '"+entry+"'", m_db);
    while (query.next()) {
        const QString text = query.value(0).toString();

        QTemporaryFile file;
        if (file.open()) {
            QTextStream out(&file);
            out << RtfTools::toValidRTF(text);

            file.close();
            RtfReader::Reader *reader = new RtfReader::Reader( NULL );
            bool result = reader->open(file.fileName());
            if(result) {
                //RefText ref(m_settings);
                RtfReader::ESwordRtfOutput *output = new RtfReader::ESwordRtfOutput( rtfDocument/*, ref.toString(-1, bookID, chapterID, startVerse, endVerse,-1)*/);
                output->setSettings(m_settings);
                reader->parseTo( output );
            }
            delete reader;
        }

    }
    HtmlResponse *res = new HtmlResponse(rtfDocument->toHtml());
    delete rtfDocument;
    return res;
}

QStringList ESwordDictionary::getAllKeys()
{
    QStringList ret;
    if(this->failed()) {
        return ret;
    }
    if(!m_loaded) {
        loadModuleData(m_moduleID);
    }

    QSqlQuery query("select Topic FROM Dictionary", m_db);
    while (query.next()) {
        const QString subject = query.value(0).toString();
        ret << subject;
    }
    return ret;
}

Response::ResponseType ESwordDictionary::responseType() const
{
    return Response::HtmlResponse;
}


int ESwordDictionary::loadModuleData(const int moduleID)
{
    DEBUG_FUNC_NAME;
    m_loaded = false;

    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(m_settings->getModuleSettings(moduleID)->modulePath);
    if (!m_db.open()) {
        this->fail(QObject::tr("Cloud not open file"), QObject::tr("Cloud not open file: %1").arg(m_settings->getModuleSettings(moduleID)->modulePath));
        myWarning() << "could not open database " << m_settings->getModuleSettings(moduleID)->modulePath;
        return 1;
    }
    QSqlQuery query ("select name,value from config", m_db);
    while (query.next()) {
        const QString name = query.value(0).toString();
        const QString value = query.value(1).toString();

    }
    m_loaded = true;
    return 0;
}
MetaInfo ESwordDictionary::readInfo(const QString &name)
{
    DEBUG_FUNC_NAME;
    MetaInfo ret;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(name);
    if (!db.open()) {
        myWarning() << "could not open database " << name;
        return MetaInfo();
    }
    QSqlQuery query ("select Description,Abbreviation,Comments,Version,Strong from Details", db);
    while (query.next()) {
        const QString name = query.value(0).toString();
        const QString shortName = query.value(1).toString();
        const QString comments = query.value(2).toString();
        const int version = query.value(3).toInt();
        const bool strong = query.value(4).toBool();

        ret.setName(shortName);
        ret.setShortName(shortName);
        ret.description = comments;

    }
    db.close();
    return ret;
}
