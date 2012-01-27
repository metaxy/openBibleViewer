#include "txtbook.h"

TxtBook::TxtBook()
{
}
TxtBook::~TxtBook()
{

}

MetaInfo TxtBook::readInfo(QFile &file)
{
    return MetaInfo();
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
QString TxtBook::readAll()
{
    QFile file(m_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "";

    QTextStream in(&file);

    QString out = "<html><head></head><body><div class='book'>";
    int line = 0;
    while (!in.atEnd()) {
        out += "<span line='"+QString::number(line) +"' class='line'>" + in.readLine() + "</span><br >";
        line++;
    }
    out += "</div></body></html>";
    return out;
}
