/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2010 Paul Walger
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
#ifndef URLCONVERTER2_H
#define URLCONVERTER2_H
#include "urlconverter.h"
class UrlConverter2
{
public:
    UrlConverter2(UrlConverter::UrlType from, UrlConverter::UrlType to, BibleUrl url);
    UrlConverter2(UrlConverter::UrlType from, UrlConverter::UrlType to, QString url);
    void setFromTo(UrlConverter::UrlType from, UrlConverter::UrlType to);
    void setSM(Settings *settings, ModuleMap *map);
    void setV11n(Versification *versification);
    void setUrl(BibleUrl url);
    void convert();

    bool contains(const int &moduleID, const int &bookID, const int &chapterID, const int &verseID) const;
    bool contains(const int &moduleID, const int &bookID, const int &chapterID) const;

    QString bookName() const;
    int moduleID() const;
    int bookID() const;
    int chapterID() const;
    int verseID() const;
    BibleUrl url() const;
private:
    UrlConverter m_urlConverter;
    BibleUrl m_url;
    BibleUrl m_newUrl;

};

#endif // URLCONVERTER2_H
