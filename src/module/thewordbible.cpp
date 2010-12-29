#include "thewordbible.h"
#include "src/core/bible/versification/versification_kjv.h"
#include "src/core/dbghelper.h"
TheWordBible::TheWordBible()
{
    m_versification = new Versification_KJV();
}
TheWordBible::~TheWordBible()
{
    delete m_versification;
}

void TheWordBible::setSettings(Settings *set)
{
    m_settings = set;
}
/**
  * Loads the Data.
  */
void TheWordBible::loadBibleData(const int &id, const QString &path)
{
    DEBUG_FUNC_NAME
    m_moduleID = id;
    m_modulePath = path;
    QString dataFilePath = path;
    QFile file(dataFilePath);
    Versification::VersificationFilterFlags flags;
    if(path.endsWith(".nt")) {
        flags = Versification::ReturnNT;
    }
    else if(path.endsWith(".ot")) {
        flags = Versification::ReturnOT;
    }
    else if(path.endsWith(".ont")) {
        flags = Versification::ReturnAll;
    }

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
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
    bool readingVerse = true;
    for(int lineCount = 0; !in.atEnd(); lineCount++) {
        const QString line = in.readLine();
        if(lineCount >= linesToSkip || line == "") {
            readingVerse = false;
        }
        if(readingVerse) {
            Verse v(verse, line);
            currentChapter->addVerse(verse, v);
            if(verse + 1 < m_versification->maxVerse(flags).at(book).at(chapter)) {
                verse++;
            } else {
                //myDebug() << " book = " << book << " maxChapter = " << m_versification->maxChapter(flags).at(book);
                if(chapter + 1 < m_versification->maxChapter(flags).at(book)) {
                    //myDebug() << "next chapter = " << chapter;
                    currentBook->addChapter(chapter, *currentChapter);
                    chapter++;
                    verse = 0;

                    currentChapter = new Chapter();
                    currentChapter->setChapterID(chapter);
                } else {
                    currentBook->addChapter(chapter, *currentChapter);
                    m_books.insert(book, *currentBook);

                    book++;
                    chapter = 0;
                    verse = 0;

                    currentBook = new Book();
                    currentBook->setID(book);

                    currentChapter = new Chapter();
                    currentChapter->setChapterID(chapter);
                }
            }
        } else {
            //info
            if(line.startsWith("title")) {
                const QStringList list = line.split("=");
                m_moduleName = list.last();
            } else if(line.startsWith("short.title")) {
                const QStringList list = line.split("=");
                m_shortModuleName = list.last();
            } else if(line.startsWith("id")) {
                const QStringList list = line.split("=");
                m_uID = list.last();
            }
        }

    }
    m_bookNames = m_versification->toBookNames(flags);
    m_bookCount = m_versification->toBookCount(flags);

}
int TheWordBible::readBook(const int &id)
{
    m_currentBookID = id;
    return 0;
}

QString TheWordBible::readInfo(QFile &file)
{
    bool skipping = true;
    QTextStream in(&file);
    const int linesToSkip = 31102;//see spec
    for(int lineCount = 0; !in.atEnd(); lineCount++) {
        const QString line = in.readLine();
        if(skipping) {
            myDebug() << line;
            if(line.startsWith("title")) {
                const QStringList list = line.split("=");
                return list.last();
            }
        }
        if(line == "" || lineCount >= linesToSkip)
            skipping = false;

    }
    return "";
}
QString TheWordBible::readInfo(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
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
    if(preferShortName) {
        if(!m_shortModuleName.isEmpty()) {
            return m_shortModuleName;
        } else {
            return m_moduleName;
        }
    } else {
        if(!m_moduleName.isEmpty()) {
            return m_moduleName;
        } else {
            return m_shortModuleName;
        }
    }
}
QMap<int, int> TheWordBible::bookCount() const
{
    return m_bookCount;
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
