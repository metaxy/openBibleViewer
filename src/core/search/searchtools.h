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
#ifndef SEARCHTOOLS_H
#define SEARCHTOOLS_H
#include "config.h"
#include <QtCore/QString>
#include "CLucene/clucene-config.h"
#include <tchar.h>
class SearchTools
{
public:
    SearchTools();

    static QString toQString(const TCHAR* string);
#ifdef OBV_USE_WSTRING
    static const TCHAR *toTCHAR(const QString& string);
    static const TCHAR* toTCHAR(const QString& string, wchar_t *ret);
#else
    static const TCHAR *toTCHAR(const QString& string);
    static const TCHAR* toTCHAR(const QString& string, const TCHAR *ret);
#endif

    static TCHAR* createBuffer();

    static const unsigned long MAX_LUCENE_FIELD_LENGTH = 1024 * 1024;
};

#endif // SEARCHTOOLS_H
