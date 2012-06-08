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
#include "rtfbook.h"
#include "src/extern/rtf-qt/rtfreader.h"
#include "src/extern/rtf-qt/TextDocumentRtfOutput.h"
#include <QtCore/QFileInfo>
RTFBook::RTFBook()
{
}
MetaInfo RTFBook::readInfo(const QString &name)
{
    MetaInfo ret;
    QFileInfo info(name);
    ret.setName(info.baseName());
    ret.setContent(ModuleTools::BookContent);
    return ret;
}

void RTFBook::search(const SearchQuery &query, SearchResult *res) const
{

}
bool RTFBook::hasIndex() const
{
    return false;
}
void RTFBook::buildIndex()
{
    QTextDocument rtfDocument;
    RtfReader::Reader *reader = new RtfReader::Reader( NULL );
    bool result = reader->open(m_path);
    RtfReader::TextDocumentRtfOutput *output = new RtfReader::TextDocumentRtfOutput( &rtfDocument );
    reader->parseTo( output );
    delete reader;

}

int RTFBook::moduleID() const
{
    return m_moduleID;
}
QString RTFBook::modulePath() const
{
    return m_path;
}
QString RTFBook::moduleName(bool preferShortName) const
{
    return m_settings->getModuleSettings(m_moduleID)->moduleName;
}
int RTFBook::loadModuleData(const int moduleID, const QString &path)
{
    m_path = path;
    m_moduleID = moduleID;
    return 0;
}
QString RTFBook::readAll()
{
    QTextDocument rtfDocument;
    RtfReader::Reader *reader = new RtfReader::Reader( NULL );
    bool result = reader->open(m_path);
    RtfReader::TextDocumentRtfOutput *output = new RtfReader::TextDocumentRtfOutput( &rtfDocument );
    reader->parseTo( output );
    delete reader;

    return rtfDocument.toHtml();
}
