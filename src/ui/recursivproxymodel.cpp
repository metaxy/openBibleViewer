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
#include "recursivproxymodel.h"
#include <QModelIndex>
#include "src/core/dbghelper.h"
RecursivProxyModel::RecursivProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}
/**
  Filter rows, but show alway folders.
  */
bool RecursivProxyModel::filterAcceptsRow(int sourceRow,
        const QModelIndex &sourceParent) const
{
    if(filterRegExp().isEmpty())
        return true;

    QModelIndex current(sourceModel()->index(sourceRow, filterKeyColumn(), sourceParent));

    if(sourceModel()->hasChildren(current)) {
        bool atLeastOneValidChild = false;
        int i = 0;
        while(!atLeastOneValidChild) {
            const QModelIndex child(current.child(i, current.column()));
            if(!child.isValid())
                break;

            atLeastOneValidChild = filterAcceptsRow(i, current);
            i++;
        }
        return atLeastOneValidChild;
    }
    return sourceModel()->data(current).toString().contains(filterRegExp());
}

/**
  Reimplemented match ( without the Qt::MatchFlags and hits ) to search recursiv.
  */
QModelIndexList RecursivProxyModel::match(const QModelIndex &start, int role,
        const QVariant &value, int hits,
        Qt::MatchFlags flags) const
{
    QModelIndexList list;
    for(int i = 0; i < sourceModel()->rowCount(start); ++i) {
        const QModelIndex index = sourceModel()->index(i, 0, start);
        if(!index.isValid())
            continue;
        if(index.data(role) == value) {
            list.append(index);
        }
        if(sourceModel()->hasChildren(index))
            list.append(match(index, role, value, hits, flags));
    }
    return list;
}
