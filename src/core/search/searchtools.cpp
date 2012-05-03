#include "searchtools.h"
#include "config.h"
#include "src/core/dbghelper.h"
#include <iostream>


SearchTools::SearchTools()
{
}

QString SearchTools::toQString(const TCHAR* string)
{

#ifdef OBV_USE_WSTRING
    return QString::fromWCharArray(string);
#else
    return QString::fromUtf16((const ushort*) string);
#endif
}
const TCHAR* SearchTools::toTCHAR(const QString& string)
{

#ifdef OBV_USE_WSTRING
    wchar_t *we = new TCHAR[string.size()+1];
    string.toWCharArray(we);
    we[string.size()] = 0;
    return we;
#else
    return reinterpret_cast<const TCHAR *>(string.utf16());
#endif

}
const TCHAR* SearchTools::toTCHAR(const QString& string, wchar_t *ret)
{

#ifdef OBV_USE_WSTRING
    string.toWCharArray(ret);
    ret[string.size()] = 0;
    return ret;
#else
    ret = reinterpret_cast<const TCHAR *>(string.utf16());
    return ret;
#endif

}

TCHAR* SearchTools::createBuffer()
{
    return new TCHAR[SearchTools::MAX_LUCENE_FIELD_LENGTH];
}

