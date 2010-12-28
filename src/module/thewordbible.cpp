#include "thewordbible.h"
#include "src/core/bible/versification/versification_kjv.h"
#include "src/core/dbghelper.h"
TheWordBible::TheWordBible()
{
    m_versification = new Versification_KJV();
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
    int book = 0;
    int chapter = 0;
    int verse = 0;
    Book *currentBook = new Book();
    currentBook->setID(book);
    Chapter *currentChapter = new Chapter();
    currentChapter->setChapterID(chapter);
    const int linesToSkip = 31102;//see spec

    for(int lineCount = 0; !in.atEnd(); lineCount++) {
        const QString line = in.readLine();
        if(lineCount >= linesToSkip) {
            if(line.startsWith("title")) {
                const QStringList list = line.split("=");
                m_moduleName = list.last();
            }
        } else {
            Verse v(verse,line);
            currentChapter->addVerse(verse, v);
            if(verse < m_versification->maxVerse(Versification::ReturnAll).at(book).at(chapter)) {
                verse++;
            } else {
                if(chapter < m_versification->maxChapter(Versification::ReturnAll).at(book)) {
                    currentBook->addChapter(chapter, *currentChapter);
                    currentChapter = new Chapter();
                    chapter++;
                    currentChapter->setChapterID(chapter);
                    verse = 0;
                } else {
                    //todo: it should never be more
                    m_books.insert(book, *currentBook);
                    currentBook = new Book();
                    currentChapter = new Chapter();
                    book++;
                    currentBook->setID(book);
                    chapter = 0;
                    verse = 0;
                    currentChapter->setChapterID(chapter);
                }
            }
        }
    }
    m_bookNames = m_versification->toBookNames();

}
int TheWordBible::readBook(const int &id)
{
    m_currentBookID = id;
    return 0;
}

QString TheWordBible::readInfo(QFile &file)
{
    const int linesToSkip = 31102;//see spec
    QTextStream in(&file);
    for(int lineCount = 0; !in.atEnd(); lineCount++) {
        if(lineCount >= linesToSkip) {
            const QString line = in.readLine();
            myDebug() << line;
            if(line.startsWith("title")) {
                const QStringList list = line.split("=");
                return list.last();
            }
        } else {
            in.readLine();
        }
    }
    return "";
}
QString TheWordBible::readInfo(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
        return "";
    return readInfo(file);
}

void TheWordBible::search(const SearchQuery &query, SearchResult *res) const
{
}
bool TheWordBible::hasIndex() const
{
    return false;
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
    return m_modulePath;
}
QString TheWordBible::moduleName(bool preferShortName) const
{
    return m_moduleName;
}
QMap<int, int> TheWordBible::bookCount()
{
    return QMap<int,int>();
}
BookNames TheWordBible::getBookNames()
{
    return m_bookNames;
}

Book TheWordBible::book() const
{
    return m_books.value(m_currentBookID);
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
