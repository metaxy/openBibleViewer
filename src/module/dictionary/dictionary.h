/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger
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
#ifndef DICTIONARY_H
#define DICTIONARY_H
#include "src/module/simplemoduleclass.h"
#include "src/core/dbghelper.h"
/**
 * Dictionary represents a dictionary module. Like ZefaniaLex or BibleQuoteDict.
 * It generalise the access to such dictionary modules.
 */
class Dictionary : public SimpleModuleClass
{
public:
    Dictionary();
    /**
     * Loads the module with the given id.
     * @returns 0 If everything went fine.
     * @returns 1 If there was an error.
     */
    int loadModuleData(const int id);
    /**
     * Search for the entry and returns them fully formated.
     * @return Fullyformated entry.
     * @returns QString() if not found.
     */
    QString getEntry(const QString &key) const;
    /**
     * @return All keys.
     * @returns QStringList() if failed.
     */
    QStringList getAllKeys() const;
private:
    ZefaniaLex *m_zefaniaLex;
    BibleQuoteDict *m_bibleQuoteDict;
};

#endif // DICTIONARY_H
