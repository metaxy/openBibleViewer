/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2012 Paul Walger <metaxy@walger.name>
This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3 of the License, or (at your option)
any later version.
This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with
this program; if not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/
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

#ifdef OBV_USE_WSTRING
    const TCHAR* SearchTools::toTCHAR(const QString& string)
    {
        wchar_t *we = new TCHAR[string.size()+1];
        string.toWCharArray(we);
        we[string.size()] = 0;
        return we;
    }
    const TCHAR* SearchTools::toTCHAR(const QString& string, wchar_t *ret)
    {
        string.toWCharArray(ret);
        ret[string.size()] = 0;
        return ret;
    }
#else
    const TCHAR* SearchTools::toTCHAR(const QString& string)
    {
        return reinterpret_cast<const TCHAR *>(string.utf16());
    }
    const TCHAR* SearchTools::toTCHAR(const QString& string, const TCHAR* ret)
    {
        ret = reinterpret_cast<const TCHAR *>(string.utf16());
        return ret;
    }
#endif


TCHAR* SearchTools::createBuffer()
{
    return new TCHAR[SearchTools::MAX_LUCENE_FIELD_LENGTH];
}

