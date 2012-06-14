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
#ifndef COMMENTARY_H
#define COMMENTARY_H
#include "src/module/versemodule.h"
class Commentary : public VerseModule
{
public:
    Commentary();
    Response* readRanges(const Ranges &ranges, bool ignoreModuleID = false);
    void search(SearchQuery query, SearchResult *result);
    void clearData();
    bool loaded();
    QSharedPointer<CommentaryModule> m_commentaryModule;
private:
    int currentBook();
    int currentChapter();
    std::pair<int, int> minMaxVerse(const int bookID, const int chapterID);



    int loadModuleData(const int moduleID);

    bool m_loaded;
    int m_loadedModuleID;

};

#endif // COMMENTARY_H
