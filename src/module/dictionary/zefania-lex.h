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
#ifndef ZefaniaLex_H
#define ZefaniaLex_H
#include <QtCore/QStringList>
#include <QtCore/QFile>
#include <QtCore/QVariantList>
#include <QtCore/QFile>
#include <QtCore/QDataStream>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtGui/QMessageBox>
#include <QtGui/QProgressDialog>
#include <QtXml/QDomNode>

#include "src/core/settings/settings.h"
#include "src/core/KoXmlReader.h"
#include "src/core/dbghelper.h"
#include "src/core/verse/bibleurl.h"

#include "CLucene.h"
#include "CLucene/_clucene-config.h"
/**
 * ZefaniaLex represents a zefaniaXML(lex) module
 */
class ZefaniaLex
{

public:
    ZefaniaLex();
    void setSettings(Settings *settings);
    void setID(int moduleID, const QString &path);
    QString buildIndexFromData(const QString &fileData, const QString &fileName);
    QString buildIndexFromFile(const QString &fileName);

    QString getEntry(const QString &entry);
    QStringList getAllKeys();
    bool hasIndex();
    int buildIndex();
    QString buildIndexFromXmlDoc(KoXmlDocument *xmldoc);
private:
    Settings *m_settings;
    QStringList m_id;
    QStringList m_title;
    QStringList m_trans;
    QStringList m_pron;
    QStringList m_desc;
    QString indexPath() const;
    int m_moduleID;
    QString m_modulePath;
};

#endif // ZefaniaLex_H
