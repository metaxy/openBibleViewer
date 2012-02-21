#include "theworddictionary.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include "src/module/response/htmlresponse.h"
#include "src/extern/rtf-qt/rtfreader.h"
#include "src/extern/rtf-qt/TheWordRtfOutput.h"
#include "src/core/verse/reftext.h"
#include "src/core/rtftools.h"
#include <QtCore/QTemporaryFile>
TheWordDictionary::TheWordDictionary()
{
}
Response* TheWordDictionary::getEntry(const QString &entry)
{
    if(!m_loaded) {
        loadModuleData(m_moduleID);
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

                out << RtfTools::toValidRTF(query2.value(0).toString());
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
    QSqlQuery query1 ("select name,value from config", db);
    while (query1.next()) {
        const QString name = query1.value(0).toString();
        const QString value = query1.value(1).toString();
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
