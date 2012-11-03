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
#include "eswordcommentary.h"
#include <QtSql/QSqlQuery>
#include "src/core/module/response/htmlresponse.h"
#include "src/extern/rtf-qt/rtfreader.h"
#include "src/extern/rtf-qt/ESwordRtfOutput.h"
#include "src/core/verse/reftext.h"

#include <QtCore/QTemporaryFile>
#include <QtSql/QSqlError>
ESwordCommentary::ESwordCommentary()
{
}
MetaInfo ESwordCommentary::readInfo(const QString &name)
{
    DEBUG_FUNC_NAME;
    MetaInfo ret;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(name);
    if (!db.open()) {
        myWarning() << "could not open database " << name;
        return MetaInfo();
    }
    QSqlQuery query ("select Description,Abbreviation,Version from Details", db);
    query.next();
    const QString desc = query.value(0).toString();
    const QString abbr = query.value(1).toString();
    //const int version = query1.value(2).toInt();//todo: use version
    ret.setName(desc);
    ret.setShortName(abbr);

    ret.setContent(ModuleTools::CommentaryContent);
    db.close();
    return ret;
}

int ESwordCommentary::loadModuleData(const int moduleID, const QString &fileName)
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
    //todo: look wich books we really have
    m_loaded = true;
    return 0;
}

Response * ESwordCommentary::readBook(const int bookID)
{
    DEBUG_FUNC_NAME
    m_book = bookID;
    m_chapter = 0;
    QTextDocument *rtfDocument = new QTextDocument( NULL );
    QSqlQuery query("select Comments "
                    "from Books where Book = "+ QString::number(bookID+1), m_db);
    query.next();
    myDebug() << "error =" << query.lastError().text();
    const QString comment = query.value(0).toString();
    readRtf(comment, rtfDocument,bookID,-1,-1,-1);

    Response *res = new HtmlResponse(rtfDocument->toHtml());
    delete rtfDocument;
    return res;
}
Response * ESwordCommentary::readChapter(const int bookID, const int chapterID)
{
    DEBUG_FUNC_NAME
    m_book = bookID;
    m_chapter = chapterID;
    QTextDocument *rtfDocument = new QTextDocument( NULL );
    QSqlQuery query("select Comments "
                    "from Chapters "
                    "where Book = "+ QString::number(bookID+1) + " " +
                    "and Chapter = " + QString::number(chapterID+1), m_db);

    while(query.next()) {
        const QString comment = query.value(0).toString();
        readRtf(comment, rtfDocument,bookID, chapterID,-1,-1);
    }

    Response *res = new HtmlResponse(rtfDocument->toHtml());
    delete rtfDocument;
    return res;
}
Response * ESwordCommentary::readVerseRange(const int bookID,const int chapterID, const int mStartVerse, const int mEndVerse)
{
    DEBUG_FUNC_NAME
    m_book = bookID;
    m_chapter = chapterID;
    bool hasAnything = false;
    QTextDocument *rtfDocument = new QTextDocument( NULL );
    QSqlQuery query("select Comments,VerseBegin,VerseEnd "
                    "from Verses "
                    "where Book = "+ QString::number(bookID+1) + " " +
                    "and ChapterBegin >= " + QString::number(chapterID+1) + " " +
                    "and ChapterEnd  <= " + QString::number(chapterID+1), m_db);

    while(query.next()) {
        myDebug() << "read verse range";
        const QString comment = query.value(0).toString();
        const int startVerse = query.value(1).toInt() - 1;
        const int endVerse = query.value(2).toInt() - 1;
        myDebug() << "[" << startVerse << "," << endVerse << "]";
        myDebug() << "[" << mStartVerse << "," << mEndVerse << "]";
        //two intervals a.b have not an element in common, iff the end of a does not reaches b, or the end of b does not reaches a
        if(!(mEndVerse < startVerse || endVerse < mStartVerse)) {
            myDebug() << "yup";
            readRtf(comment, rtfDocument, bookID, chapterID, startVerse, endVerse);
            hasAnything = true;
        }
    }


    if(hasAnything) {
        Response *res = new HtmlResponse(rtfDocument->toHtml());
        delete rtfDocument;
        return res;
    } else {
        delete rtfDocument;
        return readChapter(bookID, chapterID);
    }
}



void ESwordCommentary::readRtf(const QVariant &value, QTextDocument *rtfDocument, const int bookID, const int chapterID, const int startVerse, const int endVerse)
{
    Q_UNUSED(bookID);
    Q_UNUSED(chapterID);
    Q_UNUSED(startVerse);
    Q_UNUSED(endVerse);

    QTemporaryFile file;
    if (file.open()) {
        QTextStream out(&file);
        out << RtfTools::toValidRTF(value.toString());

        file.close();
        RtfReader::Reader *reader = new RtfReader::Reader( NULL );
        bool result = reader->open(file.fileName());
        if(result) {
            RefText ref;
            ref.setSettings(m_settings);
            RtfReader::ESwordRtfOutput *output = new RtfReader::ESwordRtfOutput( rtfDocument/*, ref.toString(-1, bookID, chapterID, startVerse, endVerse,-1)*/);
            //output->setSettings(m_settings);
            reader->parseTo( output );
        }
        delete reader;
    }
}



void ESwordCommentary::search(SearchQuery query, SearchResult *result)
{

}

int ESwordCommentary::currentBook()
{
    return m_book;
}

int ESwordCommentary::currentChapter()
{
    return m_chapter;
}

std::pair<int, int> ESwordCommentary::minMaxVerse(const int bookID, const int chapterID)
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
CommentaryModule::LinkPolicy ESwordCommentary::linkPolicy() const
{
    return CommentaryModule::NoLinkPolicy;
}
