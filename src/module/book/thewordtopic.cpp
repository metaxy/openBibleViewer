#include "thewordtopic.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include "src/module/response/stringresponse.h"
#include "src/extern/rtf-qt/rtfreader.h"
#include "src/extern/rtf-qt/TheWordRtfOutput.h"
#include "src/core/verse/reftext.h"
#include <QtCore/QTemporaryFile>
#include "src/core/rtftools.h"
TheWordTopic::TheWordTopic() : m_bookTree(NULL)
{
}
TheWordTopic::~TheWordTopic()
{
    if(m_bookTree != NULL) {
        delete m_bookTree;
        m_bookTree = NULL;
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
    if(m_bookTree != NULL) {
        delete m_bookTree;
        m_bookTree = NULL;
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
        if(parent != NULL) {
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
    QString ret;
    QTextDocument *rtfDocument = new QTextDocument( NULL );
    QSqlQuery query("select topic_id,data from content where topic_id = "+ QString::number(chapterID), m_db);
    while (query.next()) {
        QTemporaryFile file;
        if (file.open()) {
            QTextStream out(&file);
            out << RtfTools::toValidRTF(query.value(1).toString());
            file.close();
            RtfReader::Reader *reader = new RtfReader::Reader( NULL );
            bool result = reader->open(file.fileName());
            if(result) {
                RefText ref;
                ref.setSettings(m_settings);
                RtfReader::TheWordRtfOutput *output = new RtfReader::TheWordRtfOutput( rtfDocument, "" );
                output->setSettings(m_settings);
                reader->parseTo( output );
            }
            delete reader;
        }

    }
    StringResponse *res = new StringResponse(rtfDocument->toHtml());
    delete rtfDocument;
    return res;
}

BookTree * TheWordTopic::bookTree()
{
    DEBUG_FUNC_NAME
    return m_bookTree;
}

