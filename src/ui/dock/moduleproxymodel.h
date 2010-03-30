#ifndef MODULEPROXYMODEL_H
#define MODULEPROXYMODEL_H

#include <QSortFilterProxyModel>

class ModuleProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit ModuleProxyModel(QObject *parent = 0);

signals:
protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
public slots:

};

#endif // MODULEPROXYMODEL_H
