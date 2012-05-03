#ifndef SEARCHTOOLS_H
#define SEARCHTOOLS_H
#include <QtCore/QString>
#include "CLucene/clucene-config.h"
class SearchTools
{
public:
    SearchTools();

    static QString toQString(const TCHAR* string);
    static const TCHAR *toTCHAR(const QString& string);
    static const TCHAR* toTCHAR(const QString& string, wchar_t *ret);
    static TCHAR* createBuffer();

    static const unsigned long MAX_LUCENE_FIELD_LENGTH = 1024 * 1024;
};

#endif // SEARCHTOOLS_H
