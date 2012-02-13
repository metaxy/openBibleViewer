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
#include "urlconverter2.h"

UrlConverter2::UrlConverter2(UrlConverter::UrlType from, UrlConverter::UrlType to, VerseUrl url)
{
    m_urlConverter.setFrom(from);
    m_urlConverter.setTo(to);
    m_urlConverter.setUrl(url);
}
UrlConverter2::UrlConverter2(UrlConverter::UrlType from, UrlConverter::UrlType to, QString url)
{
    m_urlConverter.setFrom(from);
    m_urlConverter.setTo(to);
    VerseUrl u;
    u.fromStringUrl(url);
    m_urlConverter.setUrl(u);
}
void UrlConverter2::setFromTo(UrlConverter::UrlType from, UrlConverter::UrlType to)
{
    m_urlConverter.setFrom(from);
    m_urlConverter.setTo(to);
}

void UrlConverter2::setSM(Settings *settings, ModuleMap *map)
{
    m_urlConverter.setSettings(settings);
    m_urlConverter.setModuleMap(map);
}

void UrlConverter2::setV11n(QSharedPointer<Versification> v11n)
{
    m_urlConverter.setV11n(v11n);
}

void UrlConverter2::setUrl(VerseUrl url)
{
    m_urlConverter.setUrl(url);
}

void UrlConverter2::convert()
{
    m_newUrl = m_urlConverter.convert();
}

bool UrlConverter2::contains(const int moduleID, const int bookID, const int chapterID, const int verseID) const
{
    return m_newUrl.contains(moduleID, bookID, chapterID, verseID);
}

bool UrlConverter2::contains(const int moduleID, const int bookID, const int chapterID) const
{
    return m_newUrl.contains(moduleID, bookID, chapterID);
}

QString UrlConverter2::bookName() const
{
    return m_url.getParam("b0");
}

int UrlConverter2::moduleID() const
{
    if(m_newUrl.ranges().isEmpty())
        return -1;
    return m_newUrl.ranges().first().moduleID();
}

int UrlConverter2::bookID() const
{
    if(m_newUrl.ranges().isEmpty())
        return -1;
    return m_newUrl.ranges().first().bookID();
}

int UrlConverter2::chapterID() const
{
    if(m_newUrl.ranges().isEmpty())
        return -1;
    return m_newUrl.ranges().first().chapterID();
}

int UrlConverter2::verseID() const
{
    if(m_newUrl.ranges().isEmpty())
        return -1;
    return m_newUrl.ranges().first().activeVerseID();
}
VerseUrl UrlConverter2::url() const
{
    return m_newUrl;
}
