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
#ifndef MDISUBWINDOW_H
#define MDISUBWINDOW_H

#include <QtGui/QMdiSubWindow>

class MdiSubWindow : public QMdiSubWindow
{
    Q_OBJECT
public:
    explicit MdiSubWindow(QWidget *parent = 0);
    void closeEvent(QCloseEvent * closeEvent);
signals:
    void close();
public slots:
    
};

#endif // MDISUBWINDOW_H
