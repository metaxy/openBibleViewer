/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009 Paul Walger
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
#include "src/core/bibledisplay.h"
#include "src/core/settings.h"
#include "src/core/notes.h"
class Interface : public QWidget
{
    Q_OBJECT
public slots:
    void closing();

public:
    Interface(QWidget *parent);
    ModuleManager *m_moduleManager;
    BibleDisplay *m_bibleDisplay;
    Settings *m_settings;
    Notes *m_notes;

    void setModuleManager(ModuleManager *manager);
    void setBibleDisplay(BibleDisplay *bibledisplay);
    void setSettings(Settings *settings);
    void setNotes(Notes *notes);

    bool hasMenuBar();
    QMenuBar* menuBar();

    bool hasToolBar();
    QToolBar* toolBar();

};

#endif // INTERFACE_H
