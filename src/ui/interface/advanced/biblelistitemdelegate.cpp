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
#include "biblelistitemdelegate.h"
BibleListItemDelegate::BibleListItemDelegate(QObject *parent)
    : QItemDelegate(parent)
{
    m_parent = parent;
}
void BibleListItemDelegate::init()
{
    m_names << tr("None") << m_moduleManager->getBibleTitles();
    m_id << -1 << m_moduleManager->getBibleIDs();
}

QWidget * BibleListItemDelegate::createEditor(QWidget *parent,
        const QStyleOptionViewItem & option ,
        const QModelIndex & index) const
{
    Q_UNUSED(option);
    QComboBox *combo = new QComboBox(parent);
    combo->insertItems(0, m_names);
    if(index.data(Qt::UserRole + 3).toInt() > 0) {
        combo->setCurrentIndex(index.data(Qt::UserRole + 3).toInt());
    } else {
        combo->setCurrentIndex(0);
    }

    return combo;
}

void BibleListItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    //DEBUG_FUNC_NAME
    QComboBox *combo = qobject_cast<QComboBox *>(editor);
    if(!combo) {
        QItemDelegate::setEditorData(editor, index);
        return;
    }

    combo->setCurrentIndex(index.model()->data(index, Qt::UserRole + 3).toInt());
}
void BibleListItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    //DEBUG_FUNC_NAME
    QComboBox *combo = qobject_cast<QComboBox *>(editor);
    if(!combo) {
        QItemDelegate::setModelData(editor, model, index);
        return;
    }
    model->setData(index, combo->currentIndex(), Qt::UserRole + 3);
    if(combo->currentIndex() > 0)
        model->setData(index, combo->currentText());
    else
        model->setData(index, "");

    if(combo->currentIndex() > 0)
        model->setData(index, m_id.at(combo->currentIndex()), Qt::UserRole + 2);
    else
        model->setData(index, -1, Qt::UserRole + 2);
}
void BibleListItemDelegate::updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
