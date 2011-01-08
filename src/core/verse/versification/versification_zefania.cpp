#include "versification_zefania.h"

Versification_Zefania::Versification_Zefania(const QStringList &fullNames, const QStringList &shortNames, const QStringList &bookIDs)
{
    for(int i = 0; i < fullNames.size(); i++) {
        BookV11N book;
        book.name = fullNames.at(i);
        book.shortNames = (QStringList() << shortNames.at(i));
        book.bookID = bookIDs.at(i).toInt();
        m_books[book.bookID] = book;
    }
    setFlags(Versification::ReturnAll);
}
bool Versification_Zefania::filter(const int &bookID, VersificationFilterFlags flags) const
{
    if(flags.testFlag(Versification::ReturnAll) || (flags.testFlag(Versification::ReturnOT) && flags.testFlag(Versification::ReturnNT)))
        return true;
    else if(flags.testFlag(Versification::ReturnOT)) {
        return (bookID >= 0 && bookID <= 38);
    } else if(flags.testFlag(Versification::ReturnNT)) {
         return (bookID >= 39);
    }
}
void Versification_Zefania::setMaxChapter(const int &bookID, const int &maxChapter)
{
    BookV11N book = m_books.value(bookID);
    book.maxChapter = maxChapter;
    m_books[bookID] = book;
}
