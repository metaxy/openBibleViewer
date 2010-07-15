/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2010 Paul Walger
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
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
private:
    QObject *m_parent;
    QStringList m_names;
    QList<int> m_id;
};

#endif // BIBLELISTITEMDELEGATE_H
