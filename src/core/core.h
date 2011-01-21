/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2010 Paul Walger
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
#ifndef CORE_H
#define CORE_H
namespace CORE {
enum ModuleClass {
    NoneClass = 0,
    BibleModuleClass = 1,
    DictionaryModuleClass = 2,
    FolderClass = 3
};

enum ModuleType {
    NoneType = 0,
    BibleQuoteModule = 1,
    ZefaniaBibleModule = 2,
    ZefaniaLexModule = 3,
    BibleQuoteDictModule = 4,
    TheWordBibleModule = 5
};

}
#endif //CORE_H
