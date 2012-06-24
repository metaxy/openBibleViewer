#include "testtools.h"
#include "src/core/verse/versification/versification_cache.h"
TestTools::TestTools()
{

}


Versification* TestTools::createTestV11n()
{
    QMap<int, BookV11N> d;
    
    {
        BookV11N b1;
        b1.bookID = bookID1();
        b1.name = bookName1();
        b1.shortNames = bookShortNames1();
        b1.maxChapter = maxChapter1();
        b1.maxVerse = maxVerse1();
        d[b1.bookID] = b1;
    }
    {
        BookV11N b1;
        b1.bookID = bookID2();
        b1.name = bookName2();
        b1.shortNames = bookShortNames2();
        b1.maxChapter = maxChapter2();
        b1.maxVerse = maxVerse2();
        d[b1.bookID] = b1;
    }
    return new Versification_Cache(d);
    
}
QString TestTools::bookName1()
{
    return "Book 1";
}
QString TestTools::bookName2()
{
    return "Book 2";
}
    
QStringList TestTools::bookShortNames1()
{
    QStringList l;
    l << "1B" << "1.B" << "1 B";
    return l;
}
QStringList TestTools::bookShortNames2()
{
    QStringList l;
    l << "2B" << "2.B" << "2 B";
    return l;
}
    
int TestTools::bookID1()
{
    return 0;
}
int TestTools::bookID2()
{
    return 40;
}
    
int TestTools::maxChapter1()
{
    return 1;
}
int TestTools::maxChapter2()
{
    return 1;
}
    
QList<int> TestTools::maxVerse1()
{
    QList<int> maxVerse;
    maxVerse << 10;
    return maxVerse;
}
QList<int> TestTools::maxVerse2()
{
    QList<int> maxVerse;
    maxVerse << 20;
    return maxVerse;
}
