#include "eswordtopic.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include "src/module/response/stringresponse.h"
#include "src/extern/rtf-qt/rtfreader.h"
#include "src/extern/rtf-qt/TextDocumentRtfOutput.h"
#include "src/core/verse/reftext.h"
#include <QtCore/QTemporaryFile>
ESwordTopic::ESwordTopic() : m_bookTree(NULL)
{
}
ESwordTopic::~ESwordTopic()
{
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
    while (query.next()) {
        QTemporaryFile file;
        if (file.open()) {
            QTextStream out(&file);
            out << toValidRTF(query.value(1).toString());
            file.close();
            RtfReader::Reader *reader = new RtfReader::Reader( NULL );
            bool result = reader->open(file.fileName());
            if(result) {
                RefText ref;
                ref.setSettings(m_settings);
                RtfReader::TextDocumentRtfOutput *output = new RtfReader::TextDocumentRtfOutput( rtfDocument );
                reader->parseTo( output );
            }
            delete reader;
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
QString ESwordTopic::toValidRTF(QString data)
{
    if(!data.startsWith("{\\rtf")) {
        data.prepend("{\\rtf1");
        data.append("}");
    }
    return data;
}
