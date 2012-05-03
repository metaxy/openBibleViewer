#ifndef SEARCHTOOLS_H
#define SEARCHTOOLS_H
#include <QtCore/QString>
#include "CLucene/clucene-config.h"
class SearchTools
{
public:
    SearchTools();

    static QString toQString(const TCHAR* string);
    static const TCHAR* toTCHAR(const QString& string);
};

#endif // SEARCHTOOLS_H
