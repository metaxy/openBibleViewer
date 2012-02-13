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
#ifndef INTERFACE_H
#define INTERFACE_H

#include <QWidget>
#include <QtGui/QMenuBar>
#include <QtGui/QToolBar>
#include "src/module/modulemanager.h"
#include "src/core/settings/settings.h"
#include "src/core/notes/notes.h"
#include "src/core/basicclass.h"
#include "src/ui/dock/bookdockwidget.h"
#include "src/ui/dock/moduledockwidget.h"
#include "src/ui/dock/advancedsearchresultdockwidget.h"
#include "src/ui/dock/searchresultdockwidget.h"
#include "src/ui/dock/notesdockwidget.h"
#include "src/ui/dock/bookmarksdockwidget.h"
#include "src/ui/dock/dictionarydockwidget.h"
#include "src/ui/dock/quickjumpdockwidget.h"
class Interface : public QWidget, public BasicClass
{
    Q_OBJECT
public slots:
    virtual void closing();

public:
    Interface(QWidget *parent);
    virtual bool hasMenuBar() = 0;
    virtual QMenuBar* menuBar() = 0;

    virtual bool hasToolBar() = 0;
    virtual QList<QToolBar*> toolBars() = 0;

    virtual void init() = 0;
    virtual QHash<DockWidget *, Qt::DockWidgetArea> docks() = 0;
    /**
      * createDocks() is called first on construction. Should depend only on init()
      */
    virtual void createDocks() = 0;
    /**
      * createToolsBars() is called second on construction. Should depend only on init() and createDocks()
      */
    virtual void createToolBars() = 0;
    /**
      * createMenu() is called as last on construction. It can depend on init(), createDocks() and createToolBars()
      */
    virtual void createMenu() = 0;

    virtual QString name() const = 0;




};

#endif // INTERFACE_H
