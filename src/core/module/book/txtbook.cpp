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
#include "txtbook.h"
#include <QtCore/QFileInfo>
#include "src/core/module/response/htmlresponse.h"
TxtBook::TxtBook()
{
}
TxtBook::~TxtBook()
{

}

MetaInfo TxtBook::readInfo(const QString &fileName)
{
    MetaInfo ret;
    QFileInfo info(fileName);
    ret.setName(info.baseName());
    ret.setContent(ModuleTools::BookContent);
    return ret;
}

void TxtBook::search(const SearchQuery &query, SearchResult *res) const
{

}
bool TxtBook::hasIndex() const
{
    return false;
}
void TxtBook::buildIndex()
{

}

int TxtBook::moduleID() const
{
    return m_moduleID;
}
QString TxtBook::modulePath() const
{
    return m_path;
}
QString TxtBook::moduleName(bool preferShortName) const
{
    return m_settings->getModuleSettings(m_moduleID)->moduleName;
}
int TxtBook::loadModuleData(const int moduleID, const QString &path)
{
    m_path = path;
    m_moduleID = moduleID;
    return 0;
}
Response* TxtBook::readAll()
{
    QFile file(m_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return new HtmlResponse("File not found");

    QTextStream in(&file);

    QString out = "<html><head></head><body><div class='book'>";
    int line = 0;
    while (!in.atEnd()) {
        out += "<span line='"+QString::number(line) +"' class='line'>" + in.readLine() + "</span><br >";
        line++;
    }
    out += "</div></body></html>";
    return new HtmlResponse(out);
}
