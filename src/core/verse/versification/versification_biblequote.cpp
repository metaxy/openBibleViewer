#include "versification_biblequote.h"

Versification_BibleQuote::Versification_BibleQuote(const QStringList &fullNames, const QList<QStringList> &shortNames, const QMap<int, int> &bookCount)
{
    for(int i = 0; i < fullNames.size(); i++) {
        BookV11N book;
        book.name = fullNames.at(i);
        if(i < shortNames.size())
            book.shortNames = shortNames.at(i);
        else
            book.shortNames = QStringList();
        book.bookID = i;
        book.maxChapter = bookCount.value(i);
        m_books[book.bookID] = book;
    }
    setFlags(Versification::ReturnAll);
}
bool Versification_BibleQuote::filter(const int &bookID, VersificationFilterFlags flags) const
{
    return true;
}
