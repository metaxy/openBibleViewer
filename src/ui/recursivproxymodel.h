#ifndef RECURSIVPROXYMODEL_H
#define RECURSIVPROXYMODEL_H

#include <QSortFilterProxyModel>

class RecursivProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit RecursivProxyModel(QObject *parent = 0);
    QModelIndexList match(const QModelIndex &start, int role, const QVariant &value, int hits = -1, Qt::MatchFlags flags = Qt::MatchExactly) const;
signals:
protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;


public slots:

};

#endif // RECURSIVPROXYMODEL_H
