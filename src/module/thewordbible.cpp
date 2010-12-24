#include "thewordbible.h"

TheWordBible::TheWordBible()
{
}
void TheWordBible::setSettings(Settings *set)
{
    m_settings = set;
}
/**
  * Loads the Data.
  * path is e.g /home/paul/bible/akjv . The real files are then
  * /home/paul/bible/akjv.ont or /home/paul/bible/akjv.nt
  * or /home/paul/bible/akjv.ot and /home/paul/bible/akjv.nt
  */
void TheWordBible::loadBibleData(const int &id, const QString &path)
{
    m_moduleID = id;
    m_modulePath = path;
    QString dataFilePath = path;
    QFile file(dataFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    for(int lineCount = 0; !in.atEnd(); lineCount++) {
        const QString line = in.readLine();
    }

}
int TheWordBible::readBook(const int &id)
{
}

QString TheWordBible::readInfo(QFile &file)
{
}

void TheWordBible::search(const SearchQuery &query, SearchResult *res) const
{
}
bool TheWordBible::hasIndex() const
{
}
void TheWordBible::buildIndex()
{
}

int TheWordBible::moduleID() const
{
    return m_moduleID;
}

QString TheWordBible::modulePath() const
{
}
QString TheWordBible::moduleName(bool preferShortName = false) const
{
}
QMap<int, int> TheWordBible::bookCount()
{
}
BookNames TheWordBible::getBookNames()
{

}

Book TheWordBible::book() const
{

}
bool TheWordBible::hasNT() const
{
    QFileInfo f(m_modulePath + ".nt");
    return f.exists();
}
bool TheWordBible::hasOT() const
{
    QFileInfo f(m_modulePath + ".ot");
    return f.exists();
}
bool TheWordBible::hasONT() const
{
    QFileInfo f(m_modulePath + ".ont");
    return f.exists();
}
