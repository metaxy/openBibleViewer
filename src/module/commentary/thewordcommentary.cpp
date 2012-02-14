#include "thewordcommentary.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include "src/module/response/stringresponse.h"
#include "src/extern/rtf-qt/rtfreader.h"
#include "src/extern/rtf-qt/TextDocumentRtfOutput.h"
#include <QtCore/QTemporaryFile>
TheWordCommentary::TheWordCommentary()
{
}

Response * TheWordCommentary::readVerseRange(const int bookID,const int chapterID, const int startVerseID, const int endVerseID)
{
    QSqlQuery query("select topic_id from bible_refs where bi = "+ QString::number(bookID)+" and ci = " + QString::number(chapterID), m_db);
    QString ret = "";
    while (query.next()) {
        QSqlQuery query2("select data from content where topic_id =" + query.value(0).toString(), m_db);
        while(query2.next()) {
            ret += query2.value(0).toString();
        }
    }
    myDebug() << ret;
    QTemporaryFile file;
    if (file.open()) {
        QTextStream out(&file);
        out << ret;
        file.close();
        RtfReader::Reader *reader = new RtfReader::Reader( NULL );
        bool result = reader->open(file.fileName());

        QTextDocument *rtfDocument = new QTextDocument( NULL );
        RtfReader::TextDocumentRtfOutput *output = new RtfReader::TextDocumentRtfOutput( rtfDocument );
        reader->parseTo( output );
        return new StringResponse(rtfDocument->toHtml());

    }
   return NULL;
}

Response * TheWordCommentary::readChapter(const int bookID, const int chapterID)
{
    return NULL;
}

Response * TheWordCommentary::readBook(const int bookID)
{
    return NULL;
}

int TheWordCommentary::loadModuleData(const int moduleID, const QString &fileName)
{
    DEBUG_FUNC_NAME;
    myDebug() << moduleID << fileName;
    m_loaded = false;
    m_settings->getModuleSettings(moduleID)->versificationName = "kjv";
    m_versification = QSharedPointer<Versification>(new Versification_KJV());

    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(fileName);
    if (!m_db.open()) {
        myWarning() << "could not open database " << fileName;
        return 1;
    }
    m_loaded = true;
    return 0;
}
MetaInfo TheWordCommentary::readInfo(const QString &name)
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
    return ret;
}
void TheWordCommentary::search(SearchQuery query, SearchResult *result)
{

}

int TheWordCommentary::currentBook()
{
    return 0;
}

int TheWordCommentary::currentChapter()
{
    return 0;
}

std::pair<int, int> TheWordCommentary::minMaxVerse(const int bookID, const int chapterID)
{
    std::pair<int, int> ret;
    ret.first = 0;
    ret.second = m_versification->maxVerse().value(bookID).at(chapterID);
    return ret;
}
