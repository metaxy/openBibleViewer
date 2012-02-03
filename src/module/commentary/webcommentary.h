#ifndef WEBCOMMENTARY_H
#define WEBCOMMENTARY_H
#include "src/module/versemodule.h"
class Webcommentary : public VerseModule
{
public:
    Webcommentary();
    Response* readRanges(const Ranges &ranges, bool ignoreModuleID = false);

};

#endif // WEBCOMMENTARY_H
