/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger <metaxy@walger.name>
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
#ifndef SUBWINDOWFILTER_H
#define SUBWINDOWFILTER_H

#include <QObject>
#include <QtCore/QEvent>

class SubWindowFilter : public QObject
{
        Q_OBJECT
public:
    explicit SubWindowFilter(QObject *parent = 0);
signals:
    void close();
protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // SUBWINDOWFILTER_H
