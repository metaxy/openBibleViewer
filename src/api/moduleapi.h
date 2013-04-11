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
#ifndef BIBLEAPI_H
#define BIBLEAPI_H
#include "src/core/basicclass.h"
#include <QtWebKit/QWebFrame>
/**
  ModuleAPI is a API to access to the bible and modulemanager
  */
class ModuleApi : public QObject, public BasicClass
{
    Q_OBJECT
public slots:
    void activateModule(const int verseTableID);
    void deleteModule(const int verseTableID);

    int getModuleIdByName(const QString &name);
signals:
    void setActiveItem(const int verseTableID);
public:
    explicit ModuleApi(QObject *parent = 0);
    virtual ~ModuleApi();
    void setFrame(QWebFrame *frame);

    QString name() const;
private:
    QWebFrame *m_frame;
};

#endif // BIBLEAPI_H
