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
#ifndef COMMENTARYMODULE_H
#define COMMENTARYMODULE_H
#include "src/module/response/response.h"
#include "src/module/simplemodule.h"
#include "src/module/searchablemodule.h"
class CommentaryModule : public SimpleModule
{
public:
    enum LinkPolicy {
        NoLinkPolicy,
        OpenWebLinksHere

    };

    CommentaryModule();
    QSharedPointer<Versification> versification() const;
    virtual Response * readVerseRange(const int bookID,const int chapterID, const int startVerseID, const int endVerseID) = 0;
    virtual Response * readChapter(const int bookID, const int chapterID) = 0;
    virtual Response * readBook(const int bookID) = 0;

    virtual int loadModuleData(const int moduleID, const QString &fileName) = 0;

    virtual void search(SearchQuery query, SearchResult *result) = 0;

    virtual int currentBook() = 0;
    virtual int currentChapter() = 0;
    virtual std::pair<int, int> minMaxVerse(const int bookID, const int chapterID) = 0;

    virtual CommentaryModule::LinkPolicy linkPolicy() const = 0;
protected:
    QSharedPointer<Versification> m_versification;
};

#endif // COMMENTARYMODULE_H
