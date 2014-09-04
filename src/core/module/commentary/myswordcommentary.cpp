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
#include "myswordcommentary.h"
#include <QtSql/QSqlQuery>
#include "src/core/module/response/htmlresponse.h"
#include "src/core/verse/reftext.h"

#include <QTemporaryFile>
#include <QtSql/QSqlError>
MySwordCommentary::MySwordCommentary()
{
}
MetaInfo MySwordCommentary::readInfo(const QString &name)
{
    DEBUG_FUNC_NAME;
    MetaInfo ret;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(name);
    if (!db.open()) {
        myWarning() << "could not open database " << name;
        return MetaInfo();
    }
    QSqlQuery query ("select description,abbreviation from Details", db);
    query.next();
    const QString desc = query.value(0).toString();
    const QString abbr = query.value(1).toString();
    ret.setName(desc);
    ret.setShortName(abbr);

    ret.setContent(ModuleTools::CommentaryContent);
    db.close();
    return ret;
}

int MySwordCommentary::loadModuleData(const int moduleID, const QString &fileName)
{
    DEBUG_FUNC_NAME;
    myDebug() << moduleID << fileName;
    m_loaded = false;

    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(fileName);
    if (!m_db.open()) {
        myWarning() << "could not open database " << fileName;
        return 1;
    }
    if(!m_settings->getModuleSettings(moduleID)->versificationName.isEmpty()) {
        QSqlQuery query("select book from commentary", m_db);
        QSet<int> books;
        while(query.next()) {
            const int book = query.value(0).toInt() - 1;
            books.insert(book);
        }
        if(books.size() == 27 && books.toList().first() == 39 && books.toList().last() == 65) {
            m_settings->getModuleSettings(moduleID)->versificationName = "kjv-nt";
            m_versification = QSharedPointer<Versification>(new Versification_KJV());
            m_versification->setFlags(Versification::ReturnNT);
        } else if(books.size() == 39 && books.toList().first() == 0 && books.toList().last() == 38) {
            m_settings->getModuleSettings(moduleID)->versificationName = "kjv-ot";
            m_versification = QSharedPointer<Versification>(new Versification_KJV());
            m_versification->setFlags(Versification::ReturnOT);
        } else {
            m_settings->getModuleSettings(moduleID)->versificationName = "kjv";
            m_versification = QSharedPointer<Versification>(new Versification_KJV());
        }

    } else {
        m_versification = m_settings->getV11N(moduleID);
    }
    m_moduleID = moduleID;
    m_loaded = true;
    return 0;
}
Response * MySwordCommentary::readStart()
{
   return new StringResponse(m_settings->getModuleSettings(m_moduleID)->name());
}
Response * MySwordCommentary::readBook(const int bookID)
{
    DEBUG_FUNC_NAME
    m_book = bookID;
    m_chapter = 0;
    QSqlQuery query("select data "
                    "from commentary where Book = "+ QString::number(bookID+1), m_db);
    query.next();
    myDebug() << "error =" << query.lastError().text();
    const QString data = query.value(0).toString();
    Response *res = new HtmlResponse(data);
    return res;
}
Response * MySwordCommentary::readChapter(const int bookID, const int chapterID)
{
    DEBUG_FUNC_NAME
    m_book = bookID;
    m_chapter = chapterID;
    QSqlQuery query(m_db);
    query.prepare("SELECT data, fromverse, toverse FROM commentary WHERE book = :book and chapter = :chapter ORDER BY fromVerse");
    query.bindValue(":book", bookID+1);
    query.bindValue(":chapter", chapterID+1);
    query.exec();
    QString ret = "";
    myDebug() << "query.size() = " << query.size();
    myDebug() << "error =" << query.lastError().text();
    myDebug() << query.executedQuery();
    while(query.next()) {
        const QString data = query.value(0).toString();
        const int fromVerse = query.value(1).toInt() - 1;
        const int toVerse = query.value(2).toInt() - 1;

        ret.append("<h2>V."+QString::number(fromVerse+1)+" - " + QString::number(toVerse+1)+"</h2>");
        ret.append(data);
    }

    Response *res = new HtmlResponse(ret);
    return res;
}
Response * MySwordCommentary::readVerseRange(const int bookID,const int chapterID, const int mStartVerse, const int mEndVerse)
{
    DEBUG_FUNC_NAME
    m_book = bookID;
    m_chapter = chapterID;
    bool hasAnything = false;
    QSqlQuery query(m_db);
    query.prepare("SELECT data, fromverse, toverse FROM commentary where book = :book and chapter = :chapter ORDER BY fromVerse");
    query.bindValue(":book", bookID+1);
    query.bindValue(":chapter", chapterID+1);
    query.exec();
    myDebug() << "query.size() = " << query.size();
    myDebug() << "error =" << query.lastError().text();
    QString ret = "";
    while(query.next()) {
        const QString data = query.value(0).toString();
        const int fromVerse = query.value(1).toInt() - 1;
        const int toVerse = query.value(2).toInt() - 1;
        //two intervals a.b have not an element in common, iff the end of a does not reaches b, or the end of b does not reaches a
        if(!(mEndVerse < fromVerse || toVerse < mStartVerse)) {
            ret.append("<h2>V."+QString::number(fromVerse+1)+" - " + QString::number(toVerse+1)+"</h2>");
            ret.append(data);
            hasAnything = true;
        }
    }

    if(hasAnything) {
        Response *res = new HtmlResponse(ret);
        return res;
    } else {
        return readChapter(bookID, chapterID);
    }
}

void MySwordCommentary::search(SearchQuery query, SearchResult *result)
{

}

int MySwordCommentary::currentBook()
{
    return m_book;
}

int MySwordCommentary::currentChapter()
{
    return m_chapter;
}

std::pair<int, int> MySwordCommentary::minMaxVerse(const int bookID, const int chapterID)
{
    std::pair<int, int> ret;
    ret.first = 0;
    if(m_versification->containsChapter(bookID, chapterID)) {//is in bounds
        ret.second = m_versification->maxVerse().value(bookID).at(chapterID);
    } else {
        ret.second = 0;
    }
    return ret;
}
CommentaryModule::LinkPolicy MySwordCommentary::linkPolicy() const
{
    return CommentaryModule::NoLinkPolicy;
}
