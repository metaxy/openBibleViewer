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
    return -2;
}
QString TxtBook::modulePath() const
{
    return "";
}
QString TxtBook::moduleName(bool preferShortName) const
{
    return "";
}
