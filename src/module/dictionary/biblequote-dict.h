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

#include "src/core/settings/settings.h"
#include "src/core/dbghelper.h"
/**
  * BibleQuoteDict is a dictionary module for BibelQuote Dictionaries. The file suffix is *.idx.
  */
class BibleQuoteDict
{
public:
    BibleQuoteDict();
    void setSettings(Settings *settings);
    QString readInfo(QFile &file);
    QString readInfo(const QString &fileName);
    int buildIndex();
    bool hasIndex();
    QString getEntry(const QString &id);
    void setID(const int id, const QString &path);
    QStringList getAllKeys();
private:
    Settings *m_settings;
    QString m_modulePath;
    int m_moduleID;
    QString indexPath() const;
};

#endif // BIBLEQUOTEDICT_H
