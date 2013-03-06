#include "pdfbook.h"
#include <QtCore/QFileInfo>
#include "src/core/module/response/htmlresponse.h"
PDFBook::PDFBook()
{
    m_pdfjsPath = "file:///home/paul/c++/openBibleViewer/src/data/js/pdf.js";
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
            "<script type='text/javascript'' src='"+m_pdfjsPath+"'></script>"
            "<script type='text/javascript'>PDFJS.workerSrc = '"+m_pdfjsPath+"';</script>"
            "<script type='text/javascript'>"
            "PDFJS.getDocument('file://"+m_path+"').then(function(pdf) {"
                "pdf.getPage(1).then(function(page) {"
                    "var scale = 1.5;"
                    "var viewport = page.getViewport(scale);"
                    "var canvas = document.getElementById('the-canvas');"
                    "var context = canvas.getContext('2d');"
                    "canvas.height = viewport.height;"
                    "canvas.width = viewport.width;"
                    "var renderContext = {"
                        "canvasContext: context,"
                        "viewport: viewport"
                    "};"
                    "page.render(renderContext);"
                "});"
             "});"

           " </script>"
            "</head><body>"
            "<canvas id='the-canvas'' style='border:1px solid black;'/>"
            "</body></html>";
    return new HtmlResponse(out);
}
