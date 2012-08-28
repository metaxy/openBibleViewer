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
#ifndef DICTIONARYMANAGER_H
#define DICTIONARYMANAGER_H
#include "src/core/basicclass.h"
#include "src/ui/dock/dictionarydockwidget.h"
#include "src/ui/interface/advanced/manager/windowmanager.h"

#include <QObject>

class DictionaryManager : public QObject, public BasicClass
{
    Q_OBJECT
public:
    DictionaryManager(QObject *parent, QWidget *p);
    void setWidget(QWidget *p);
    void createDocks();
    QHash<DockWidget *, Qt::DockWidgetArea> docks();
    DictionaryDockWidget *dictionaryDockWidget();

    void parseUrl(QString url, const Actions::OpenLinkModifiers mod);

    void open(const QString &key, ModuleTools::ContentType contentType, const Actions::OpenLinkModifiers mod);

    void setWindowManager(WindowManager *windowManager);
private:
    QWidget *m_p;
    DictionaryDockWidget *m_dictionaryDock;
    WindowManager *m_windowManager;
};

#endif // DICTIONARYMANAGER_H
