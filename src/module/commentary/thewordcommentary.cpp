#include "thewordcommentary.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include "src/module/response/stringresponse.h"
#include "src/extern/rtf-qt/rtfreader.h"
#include "src/extern/rtf-qt/TheWordRtfOutput.h"
#include "src/core/verse/reftext.h"
#include "src/core/rtftools.h"
#include <QtCore/QTemporaryFile>
TheWordCommentary::TheWordCommentary()
{
}

Response * TheWordCommentary::readVerseRange(const int bookID,const int chapterID, const int startVerseID, const int endVerseID)
{
    m_book = bookID;
    m_chapter = chapterID;
    bool hasSomeThing = false;
    QTextDocument *rtfDocument = new QTextDocument( NULL );
    QSqlQuery query("select topic_id,bi,ci,fvi,tvi from bible_refs where bi = "+ QString::number(bookID+1)+
                    " and ci = " + QString::number(chapterID+1) +
                    " and fvi >=" + QString::number(startVerseID+1)+
                    " and tvi <=" + QString::number(endVerseID+1) +
                    " ORDER BY fvi DESC", m_db);
    while (query.next()) {
        const int bi = query.value(1).toInt();
        const int ci = query.value(2).toInt();
        const int fvi = query.value(3).toInt();
        const int tvi = query.value(4).toInt();
        QSqlQuery query2("select data from content where topic_id =" + query.value(0).toString(), m_db);

        while(query2.next()) {
            QTemporaryFile file;
            if (file.open()) {
                QTextStream out(&file);
                out << RtfTools::toValidRTF(query2.value(0).toString());
                file.close();
                RtfReader::Reader *reader = new RtfReader::Reader( NULL );
                bool result = reader->open(file.fileName());
                if(result) {
                    RefText ref;
                    ref.setSettings(m_settings);
                    RtfReader::TheWordRtfOutput *output = new RtfReader::TheWordRtfOutput( rtfDocument, ref.toString(-1, bi-1, ci-1, fvi-1, tvi-1,-1) );
                    output->setSettings(m_settings);
                    reader->parseTo( output );
                    hasSomeThing = true;
                }
                delete reader;
            }
        }
    }
    StringResponse *res = new StringResponse(rtfDocument->toHtml());
    delete rtfDocument;
    if(!hasSomeThing) {
        return readChapter(bookID, chapterID);
    } else {
        return res;
    }
}

Response * TheWordCommentary::readChapter(const int bookID, const int chapterID)
{
    DEBUG_FUNC_NAME
    m_book = bookID;
    m_chapter = chapterID;
    QTextDocument *rtfDocument = new QTextDocument( NULL );
    QSqlQuery query("select topic_id,bi,ci,fvi,tvi from bible_refs where bi = "+ QString::number(bookID+1)+" and ci = " + QString::number(chapterID+1) +" ORDER BY fvi DESC", m_db);

    while (query.next()) {
        const int bi = query.value(1).toInt();
        const int ci = query.value(2).toInt();
        const int fvi = query.value(3).toInt();
        const int tvi = query.value(4).toInt();

        QSqlQuery query2("select data from content where topic_id =" + query.value(0).toString(), m_db);

        while(query2.next()) {
            QTemporaryFile file;
            if (file.open()) {
                QTextStream out(&file);
                out << RtfTools::toValidRTF(query2.value(0).toString());
                file.close();
                RtfReader::Reader *reader = new RtfReader::Reader( NULL );
                bool result = reader->open(file.fileName());
                if(result) {
                    RefText ref;
                    ref.setSettings(m_settings);
                    RtfReader::TheWordRtfOutput *output = new RtfReader::TheWordRtfOutput( rtfDocument, ref.toString(-1, bi-1, ci-1, fvi-1, tvi-1,-1) );
                    output->setSettings(m_settings);
                    reader->parseTo( output );
                }
                delete reader;
            }
        }
    }
    StringResponse *res = new StringResponse(rtfDocument->toHtml());
    delete rtfDocument;
    return res;
}

Response * TheWordCommentary::readBook(const int bookID)
{
    m_book = bookID;
    m_chapter = 0;
    QTextDocument *rtfDocument = new QTextDocument( NULL );
    QSqlQuery query("select topic_id,bi,ci,fvi,tvi from bible_refs where bi = "+ QString::number(bookID+1)+" ORDER BY ci,fvi DESC", m_db);

    while (query.next()) {
        const int bi = query.value(1).toInt();
        const int ci = query.value(2).toInt();
        const int fvi = query.value(3).toInt();
        const int tvi = query.value(4).toInt();

        QSqlQuery query2("select data from content where topic_id =" + query.value(0).toString(), m_db);

        while(query2.next()) {
            QTemporaryFile file;
            if (file.open()) {
                QTextStream out(&file);
                out << RtfTools::toValidRTF(query2.value(0).toString());
                file.close();
                RtfReader::Reader *reader = new RtfReader::Reader( NULL );
                bool result = reader->open(file.fileName());
                if(result) {
                    RefText ref;
                    ref.setSettings(m_settings);
                    RtfReader::TheWordRtfOutput *output = new RtfReader::TheWordRtfOutput( rtfDocument, ref.toString(-1, bi-1, ci-1, fvi-1, tvi-1,-1) );
                    output->setSettings(m_settings);
                    reader->parseTo( output );
                }
                delete reader;
            }
        }
    }
    StringResponse *res = new StringResponse(rtfDocument->toHtml());
    delete rtfDocument;
    return res;
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
    return m_book;
}

int TheWordCommentary::currentChapter()
{
    return m_chapter;
}

std::pair<int, int> TheWordCommentary::minMaxVerse(const int bookID, const int chapterID)
{
    std::pair<int, int> ret;
    ret.first = 0;
    if(m_versification->maxVerse().contains(bookID) && chapterID < m_versification->maxVerse().value(bookID).size()) {
        ret.second = m_versification->maxVerse().value(bookID).at(chapterID);
    } else {
        ret.second = 0;
    }
    return ret;
}
CommentaryModule::LinkPolicy TheWordCommentary::linkPolicy() const
{
    return CommentaryModule::NoLinkPolicy;
}
