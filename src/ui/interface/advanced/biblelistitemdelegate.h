#ifndef BIBLELISTITEMDELEGATE_H
#define BIBLELISTITEMDELEGATE_H

#include <QItemDelegate>
#include <QtCore/QStringList>
#include "src/core/basicclass.h"

class BibleListItemDelegate : public QItemDelegate , public BasicClass
{
    Q_OBJECT
;
public:
    BibleListItemDelegate(QObject *parent = 0);
    void init();
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &index) const;
private:
    QObject *m_parent;
    QStringList m_names;
    QList<int> m_id;
};

#endif // BIBLELISTITEMDELEGATE_H
