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
#include "thewordcommentary.h"
#include <QtSql/QSqlQuery>
#include "src/core/module/response/htmlresponse.h"
#include "src/extern/rtf-qt/rtfreader.h"
#include "src/extern/rtf-qt/TheWordRtfOutput.h"
#include "src/core/verse/reftext.h"

#include <QtCore/QTemporaryFile>
TheWordCommentary::TheWordCommentary() : m_compressed(false), m_contentType(RtfTools::RTFContent)
{
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
    ret.setContent(ModuleTools::CommentaryContent);
    db.close();
    return ret;
}
Response * TheWordCommentary::readVerseRange(const int bookID,const int chapterID, const int startVerseID, const int endVerseID)
{
    m_book = bookID;
    m_chapter = chapterID;
    bool hasSomeThing = false;
    QTextDocument *rtfDocument = new QTextDocument( NULL );
    QString ret;
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
            hasSomeThing = true;
            if(m_contentType == RtfTools::RTFContent) {
                readRtf(query2.value(0), rtfDocument,bi,ci,fvi,tvi);
            } else {
                readRvf(query2.value(0), &ret);
            }
        }
    }
    Response *res;
    if(m_contentType == RtfTools::RTFContent) {
        res = new HtmlResponse(rtfDocument->toHtml());
    } else {
        res = new StringResponse(ret);
    }
    delete rtfDocument;

    if(!hasSomeThing) {
        delete res;
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
    QString ret;
    QSqlQuery query("select topic_id,bi,ci,fvi,tvi from bible_refs where bi = "+ QString::number(bookID+1)+" and ci = " + QString::number(chapterID+1) +" ORDER BY fvi DESC", m_db);

    while (query.next()) {
        const int bi = query.value(1).toInt();
        const int ci = query.value(2).toInt();
        const int fvi = query.value(3).toInt();
        const int tvi = query.value(4).toInt();

        QSqlQuery query2("select data from content where topic_id =" + query.value(0).toString(), m_db);

        while(query2.next()) {
            if(m_contentType == RtfTools::RTFContent) {
                readRtf(query2.value(0), rtfDocument,bi,ci,fvi,tvi);
            } else {
                readRvf(query2.value(0), &ret);
            }
        }
    }
    Response *res;
    if(m_contentType == RtfTools::RTFContent) {
        res = new HtmlResponse(rtfDocument->toHtml());
    } else {
        res = new StringResponse(ret);
    }
    delete rtfDocument;
    return res;
}

Response * TheWordCommentary::readBook(const int bookID)
{
    m_book = bookID;
    m_chapter = 0;
    QString ret;
    QTextDocument *rtfDocument = new QTextDocument( NULL );
    QSqlQuery query("select topic_id,bi,ci,fvi,tvi from bible_refs where bi = "+ QString::number(bookID+1)+" ORDER BY ci,fvi DESC", m_db);

    while (query.next()) {
        const int bi = query.value(1).toInt();
        const int ci = query.value(2).toInt();
        const int fvi = query.value(3).toInt();
        const int tvi = query.value(4).toInt();

        QSqlQuery query2("select data from content where topic_id =" + query.value(0).toString(), m_db);

        while(query2.next()) {
            if(m_contentType == RtfTools::RTFContent) {
                readRtf(query2.value(0), rtfDocument,bi,ci,fvi,tvi);
            } else {
                readRvf(query2.value(0), &ret);
            }

        }
    }
    Response *res;
    if(m_contentType == RtfTools::RTFContent) {
        res = new HtmlResponse(rtfDocument->toHtml());
    } else {
        res = new StringResponse(ret);
    }
    delete rtfDocument;
    return res;
}
void TheWordCommentary::readRtf(const QVariant &value, QTextDocument *rtfDocument, const int bi, const int ci, const int fvi, const int tvi)
{
    QTemporaryFile file;
    if (file.open()) {
        QTextStream out(&file);
        if(m_compressed) {
            out << RtfTools::toValidRTF(RtfTools::gUncompress(value.toByteArray()));
        } else {
            out << RtfTools::toValidRTF(value.toString());
        }
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

void TheWordCommentary::readRvf(const QVariant &value, QString *ret)
{
    if(m_compressed) {
        ret->append(RtfTools::fromRVF(RtfTools::gUncompress(value.toByteArray())));
    } else {
        ret->append(RtfTools::fromRVF(value.toByteArray()));
    }
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
    QSqlQuery query1 ("select name,value from config", m_db);
    while (query1.next()) {
        const QString name = query1.value(0).toString();
        const QString value = query1.value(1).toString();
        if(name == QLatin1String("content.type")) {
            if(value == QLatin1String("rvf")) {
                m_contentType = RtfTools::RVFContent;
            } else if(value == QLatin1String("rtf")){
                m_contentType = RtfTools::RTFContent;
            }
        } else if(name == QLatin1String("compressed") && value == QLatin1String("1")) {
            m_compressed = true;

        }
    }
    m_loaded = true;
    return 0;
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
