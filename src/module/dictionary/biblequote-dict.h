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
#ifndef BIBLEQUOTEDICT_H
#define BIBLEQUOTEDICT_H
#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QTextDecoder>
#include <QtCore/QDir>
#include <QtCore/QTextStream>
#include <QtGui/QProgressDialog>

#include "src/module/dictionary/dictionarymodule.h"
#include "src/core/dbghelper.h"
/**
 * BibleQuoteDict is a dictionary module for BibelQuote Dictionaries. The file suffix is *.idx.
 */
class BibleQuoteDict : public DictionaryModule
{
public:
    BibleQuoteDict();

    QString readInfo(QFile &file);
    QString readInfo(const QString &fileName);


    QString getEntry(const QString &id);
    QStringList getAllKeys();

    int buildIndex();
    bool hasIndex();
private:
    QString indexPath() const;

};

#endif // BIBLEQUOTEDICT_H
