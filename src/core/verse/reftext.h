#ifndef REFTEXT_H
#define REFTEXT_H
#include "src/core/basicclass.h"
#include "src/core/verse/verseurl.h"
class RefText : public BasicClass
{
public:
    RefText();
    void setShowModuleName(bool show);
    QString toString(const VerseUrl &url);


    QString toString(const Ranges &ranges);
private:
    bool m_showModuleName;
    QString toString(int moduleID, int bookID, int chapterID, int startVerseID, int endVerseID);
};

#endif // REFTEXT_H
