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
#ifndef STUDYINTERFACE_H
#define STUDYINTERFACE_H

#include <QWidget>
#include "src/ui/interface/interface.h"
namespace Ui
{
class StudyInterface;
}

class StudyInterface : public Interface
{
    Q_OBJECT
public:
    StudyInterface(QWidget *parent = 0);
    ~StudyInterface();
    void init();

    bool hasMenuBar();
    QMenuBar* menuBar();

    bool hasToolBar();
    QList<QToolBar*> toolBars();

    QHash<DockWidget *, Qt::DockWidgetArea> docks();

    void createDocks();
    void createToolBars();
    void createMenu();

    QString name() const;

protected:
    void changeEvent(QEvent *e);

private:
    Ui::StudyInterface *ui;
};

#endif // STUDYINTERFACE_H
