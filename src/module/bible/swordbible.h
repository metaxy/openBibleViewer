/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger
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
#ifndef SWORDBIBLE_H
#define SWORDBIBLE_H
#include "src/core/dbghelper.h"
#include "src/module/bible/biblemodule.h"

#ifdef BUILD_WITH_SWORD
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <swmgr.h>
#include <swmodule.h>
#include <markupfiltmgr.h>
#include <versekey.h>
using namespace::sword;

#endif
class SwordBible : public BibleModule
{
public:
    SwordBible();

    int loadBibleData(const int moduleID, const QString &path);
    void setSettings(Settings *settings);
    QString readInfo(QFile &file);
    int readBook(const int id);

    TextRange rawTextRange(int bookID, int chapterID, int startVerse, int endVerse);
    std::pair<int, int> minMaxVerse(int bookID, int chapterID);

    void search(const SearchQuery &query, SearchResult *res) const;
    bool hasIndex() const;
    void buildIndex();

    int moduleID() const;
    QString modulePath() const;
    QString moduleName(bool preferShortName = false) const;

    Versification *versification() const;
    QString uid() const;
private:
    int m_moduleID;
    QString m_modulePath;
    QString indexPath() const;

    Versification *m_v11n;
#ifdef BUILD_WITH_SWORD
    SWModule *m_target;
    SWMgr *m_library;
#endif
};

#endif // SWORDBIBLE_H
