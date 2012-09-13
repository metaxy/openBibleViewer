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
#ifndef ZefaniaLex_H
#define ZefaniaLex_H
#include <QtCore/QDir>
#include <QtGui/QMessageBox>
#include <QtXml/QXmlStreamReader>

#include "src/core/link/strongurl.h"

#include "src/module/dictionary/dictionarymodule.h"
#include "src/core/verse/reftext.h"

/**
 * ZefaniaLex represents a zefaniaXML(lex) module. The file suffix is *.xml.
 */
class ZefaniaLex : public DictionaryModule
{

public:
    ZefaniaLex();
    MetaInfo readInfo(const QString &name);

    Response *getEntry(const QString &entry);
    QStringList getAllKeys();

    bool hasIndex();
    int buildIndex();

    Response::ResponseType responseType() const;
private:
    QString indexPath() const;
    QStringList m_entryList;
    QXmlStreamReader *m_xml;
    bool cmp(const QStringRef &r, const QString &s);

    QString parseTitle();
    QString parseTrans();
    QString parseEm();

    QString parsePron();
    QString parseDesc();
    QString parseReflink();
    QString parseSee();

    MetaInfo readMetaInfo(MetaInfo ret);

    RefText m_refText;
};

#endif // ZefaniaLex_H
