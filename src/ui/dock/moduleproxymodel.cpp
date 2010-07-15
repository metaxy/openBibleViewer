#include "moduleproxymodel.h"
#include <QModelIndex>

ModuleProxyModel::ModuleProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}
/**
  Filter rows, but show alway folders.
  */
bool ModuleProxyModel::filterAcceptsRow(int sourceRow,
                                        const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    return (index.data(Qt::UserRole + 1).toString() == "-1" || index.data(Qt::DisplayRole).toString().contains(filterRegExp()));
}
