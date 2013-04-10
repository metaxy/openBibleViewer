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
#ifndef BIBLEQUOTEDICT_H
#define BIBLEQUOTEDICT_H
#include <QTextDecoder>
#include <QDir>
#include <QProgressDialog>

#include "src/core/module/dictionary/dictionarymodule.h"
/**
 * BibleQuoteDict is a dictionary module for BibelQuote Dictionaries. The file suffix is *.idx.
 */
class BibleQuoteDict : public DictionaryModule
{
public:
    BibleQuoteDict();

    MetaInfo readInfo(QFile &file);
    MetaInfo readInfo(const QString &fileName);

    Response *getEntry(const QString &id);
    QStringList getAllKeys();

    int buildIndex();
    bool hasIndex();
    Response::ResponseType responseType() const;
private:
    QString indexPath() const;

    QStringList m_entryList;

};

#endif // BIBLEQUOTEDICT_H
