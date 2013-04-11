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
#include "pdfbook.h"
#include <QFileInfo>
#include "src/core/module/response/htmlresponse.h"
#include "src/core/module/response/urlresponse.h"
#include <QUrl>
PDFBook::PDFBook()
{
}

MetaInfo PDFBook::readInfo(const QString &fileName)
{
    MetaInfo ret;
    QFileInfo info(fileName);
    ret.setName(info.baseName());
    ret.setContent(ModuleTools::BookContent);
    return ret;
}

void PDFBook::search(const SearchQuery &query, SearchResult *res) const
{

}
bool PDFBook::hasIndex() const
{
    return false;
}
void PDFBook::buildIndex()
{

}

int PDFBook::moduleID() const
{
    return m_moduleID;
}
QString PDFBook::modulePath() const
{
    return m_path;
}
QString PDFBook::moduleName(bool preferShortName) const
{
    return m_settings->getModuleSettings(m_moduleID)->moduleName;
}
int PDFBook::loadModuleData(const int moduleID, const QString &path)
{
    m_path = path;
    m_moduleID = moduleID;
    return 0;
}
Response* PDFBook::readAll()
{
    return new UrlResponse("qrc:/data/pdf.js/viewer.html?file=" + QUrl::fromLocalFile(m_path).toString());
}
