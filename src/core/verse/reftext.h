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
#ifndef REFTEXT_H
#define REFTEXT_H
#include "src/core/settings/settings.h"
#include "src/core/verse/ranges.h"
#include "src/core/link/verseurl.h"
class RefText
{
public:
    RefText(Settings *settings);
    void setSettings(Settings *settings);
    void setShowModuleName(bool show);
    QString toString(const VerseUrl &url) const;
    QString toString(const Ranges &ranges) const;
    QString toString(int moduleID, int bookID, int chapterID, int startVerseID, int endVerseID, int prevBook) const;
private:
    bool m_showModuleName;
    Settings *m_settings;
};

#endif // REFTEXT_H
