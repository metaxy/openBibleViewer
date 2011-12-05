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
#include "biblemodule.h"
BibleModule::BibleModule()
{
}
BibleModule::~BibleModule()
{
    DEBUG_FUNC_NAME;
    m_versification.clear();
}

void BibleModule::setSettings(Settings *settings)
{
    m_settings = settings;
}
QSharedPointer<Versification> BibleModule::versification() const
{
    return m_versification;
}

int BibleModule::loadBibleData(const int /*bibleID*/, const QString &/*path*/)
{
    myWarning() << "calling BibleModule";
    return -1;
}

MetaInfo BibleModule::readInfo(QFile &/*file*/)
{
    myWarning() << "calling BibleModule";
    return MetaInfo();
}

void BibleModule::search(const SearchQuery &/*query*/, SearchResult */*res*/) const
{
    myWarning() << "calling BibleModule";
}

bool BibleModule::hasIndex() const
{
    myWarning() << "calling BibleModule";
    return false;
}
void BibleModule::buildIndex()
{
    myWarning() << "calling BibleModule";
}
int BibleModule::moduleID() const
{
    myWarning() << "calling BibleModule";
    return 0;
}
QString BibleModule::modulePath() const
{
    myWarning() << "calling BibleModule";
    return "";
}
QString BibleModule::moduleName(bool /*preferShortName*/) const
{
    myWarning() << "calling BibleModule";
    return "";
}

QString BibleModule::uid() const
{
    return QString();
}
TextRange BibleModule::rawTextRange(int /*bookID*/, int /*chapterID*/, int /*startVerse*/, int /*endVerse*/)
{
    myWarning() << "calling BibleModule";
    return TextRange();
}

std::pair<int, int> BibleModule::minMaxVerse(int /*bookID*/, int /*chapterID*/)
{
    myWarning() << "calling BibleModule";
    return std::pair<int, int>();
}

void BibleModule::clear()
{
    myWarning() << "calling BibleModule";
}
void BibleModule::clearData()
{
    myWarning() << "calling BibleModule";
}
