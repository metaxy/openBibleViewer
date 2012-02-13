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
#ifndef RECURSIVPROXYMODEL_H
#define RECURSIVPROXYMODEL_H

#include <QSortFilterProxyModel>

class RecursivProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit RecursivProxyModel(QObject *parent = 0);
    /**
      * Reimplemented match ( without the Qt::MatchFlags and hits ) to search recursive.
      */
    QModelIndexList match(const QModelIndex &start, int role, const QVariant &value, int hits = -1, Qt::MatchFlags flags = Qt::MatchExactly) const;
signals:
protected:
    /**
      * Filter rows, but don't hide items if they have children who aren't hidden.
      */
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;


public slots:

};

#endif // RECURSIVPROXYMODEL_H
