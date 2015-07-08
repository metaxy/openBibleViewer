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
#include "mysworddictionary.h"
#include <QtSql/QSqlQuery>
#include "src/core/module/response/htmlresponse.h"
#include "src/extern/rtf-qt/rtfreader.h"
#include "src/extern/rtf-qt/ESwordRtfOutput.h"
#include "src/core/rtftools.h"
#include <QTemporaryFile>
#include <QtSql/QSqlError>
MySwordDictionary::MySwordDictionary()
{

}
Response* MySwordDictionary::getEntry(const QString &entry)
{
    if(this->failed()) return nullptr;
    if(!this->loaded()) {
        loadModuleData(m_moduleID);
    }
    QSqlQuery query("select data FROM dictionary WHERE word = '"+entry+"'", m_db);
    QString ret;
    while (query.next()) {
        const QString text = query.value(0).toString();
        ret += text;
    }
    if(ret.isEmpty()) {
        myDebug() << "ret is empty";
        QSqlQuery query2("select data,word FROM dictionary WHERE word LIKE '"+entry+"%'", m_db);
        myDebug() << "error =" << query2.lastError().text();

        while (query2.next()) {
            const QString text = query2.value(0).toString();
            const QString word = query2.value(1).toString();
            myDebug() << text;
            ret += "<h2> "+ word +"</h2>";
            ret += text;
        }

    }
    HtmlResponse *res = new HtmlResponse(ret);
    return res;
}

QStringList MySwordDictionary::getAllKeys()
{
    QStringList ret;
    if(this->failed()) return ret;
    if(!m_loaded) {
        loadModuleData(m_moduleID);
    }

    QSqlQuery query("select word FROM dictionary", m_db);
    while (query.next()) {
        const QString subject = query.value(0).toString();
        ret << subject;
    }
    return ret;
}

Response::ResponseType MySwordDictionary::responseType() const
{
    return Response::HtmlResponse;
}


int MySwordDictionary::loadModuleData(const int moduleID)
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
    m_loaded = true;
    return 0;
}
MetaInfo MySwordDictionary::readInfo(const QString &name)
{
    DEBUG_FUNC_NAME;
    MetaInfo ret;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(name);
    if (!db.open()) {
        myWarning() << "could not open database " << name;
        return MetaInfo();
    }
    QSqlQuery query ("select title,abbreviation,comments,version,strong from details", db);
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
