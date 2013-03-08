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
