#ifndef TESTTOOLS_H
#define TESTTOOLS_H
#include "src/core/verse/versification.h"
class TestTools
{

public:
    TestTools();

    static Versification* createTestV11n();
    static QString bookName1();
    static QString bookName2();
    
    static QStringList bookShortNames1();
    static QStringList bookShortNames2();
    
    static int bookID1();
    static int bookID2();
    
    static int maxChapter1();
    static int maxChapter2();
    
    static QList<int> maxVerse1();
    static QList<int> maxVerse2();
};

#endif // TESTTOOLS_H
