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
#ifndef VERSETABLEITEMDELEGATE_H
#define VERSETABLEITEMDELEGATE_H

#include <QItemDelegate>
#include <QStringList>
#include <QTimer>
#include <QComboBox>

#include "src/core/basicclass.h"
#include "src/core/dbghelper.h"


class VerseTableItemDelegate : public QItemDelegate , public BasicClass
{
    Q_OBJECT
public:
    VerseTableItemDelegate(QObject *parent = 0);
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

#endif // VERSETABLEITEMDELEGATE_H
