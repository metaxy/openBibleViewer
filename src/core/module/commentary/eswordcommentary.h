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
#ifndef ESWORDCOMMENTARY_H
#define ESWORDCOMMENTARY_H
#include "src/core/module/commentary/commentarymodule.h"
#include <QtSql/QSqlDatabase>
#include "src/core/rtftools.h"
#include <QTextDocument>
class ESwordCommentary : public CommentaryModule
{
public:
    ESwordCommentary();

    Response * readVerseRange(const int bookID,const int chapterID, const int startVerseID, const int endVerseID);
    Response * readChapter(const int bookID, const int chapterID);
    Response * readBook(const int bookID);
    Response * readStart();

    int loadModuleData(const int moduleID, const QString &fileName);

    void search(SearchQuery query, SearchResult *result);

    int currentBook();
    int currentChapter();
    std::pair<int, int> minMaxVerse(const int bookID, const int chapterID);

    MetaInfo readInfo(const QString &name);

    CommentaryModule::LinkPolicy linkPolicy() const;

    void readRtf(const QVariant &value, QTextDocument *rtfDocument, const int bi, const int ci, const int fvi, const int tvi);

private:
    QSqlDatabase m_db;
    int m_moduleID;
    int m_book;
    int m_chapter;
};

#endif // ESWORDCOMMENTARY_H
