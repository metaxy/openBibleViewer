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
