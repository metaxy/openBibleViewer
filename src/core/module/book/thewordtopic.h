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
#ifndef THEWORDTOPIC_H
#define THEWORDTOPIC_H
#include "treebookmodule.h"
#include <QtSql/QSqlDatabase>
#include <QTextDocument>
#include "src/core/rtftools.h"
class TheWordTopic : public TreeBookModule
{
public:
    TheWordTopic();
    ~TheWordTopic();

    MetaInfo readInfo(const QString &name);

    void search(const SearchQuery &query, SearchResult *res) const;
    bool hasIndex() const;
    void buildIndex();

    int moduleID() const;
    QString modulePath() const;
    QString moduleName(bool preferShortName = false) const;

    int loadModuleData(const int moduleID, const QString &path);


    Response* readChapter(const int chapterID);
    BookTree * bookTree();
private:
    QSqlDatabase m_db;
    BookTree *m_bookTree;

    void readRtf(const QVariant &value, QTextDocument *rtfDocument);
    void readRvf(const QVariant &value, QString *ret);

    bool m_compressed;
    RtfTools::TheWordContentType m_contentType;

};

#endif // THEWORDTOPIC_H
