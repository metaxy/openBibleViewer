#include "pdfbook.h"
#include <QtCore/QFileInfo>
#include "src/core/module/response/htmlresponse.h"
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

    QString out = "<!doctype html><html><head>"
            "<script type=\"text/javascript\" src=\"qrc://pdf.js\"></script>"
            "<script type=\"text/javascript\">PDFJS.workerSrc = 'qrc://pdf.js';</script>"
            "<script type=\"text/javascript\" src=\"qrc://pdfsample.js\"></script>"
            "</head><body>"
            "<canvas id=\"the-canvas\" style=\"border:1px solid black;\"/>"
            "</body></html>";
    return new HtmlResponse(out);
}
