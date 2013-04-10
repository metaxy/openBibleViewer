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
#include "versetableitemdelegate.h"
#include <QHBoxLayout>
#include <QGroupBox>
VerseTableItemDelegate::VerseTableItemDelegate(QObject *parent)
    : QItemDelegate(parent)
{
    m_parent = parent;
}
void VerseTableItemDelegate::init()
{
    m_names << tr("None");
    m_id << -1;

    QMapIterator<int, ModuleSettings *> i(m_settings->m_moduleSettings);
    while(i.hasNext()) {
        i.next();
        if(!m_moduleManager->contains(i.value()->moduleID))
            continue;
        if(m_moduleManager->getModule(i.value()->moduleID)->moduleClass() == ModuleTools::BibleModuleClass) {
            m_names.append(i.value()->name(true));
            m_id.append(i.value()->moduleID);
        }
    }
    myDebug() << m_id << m_names;
}

QWidget * VerseTableItemDelegate::createEditor(QWidget *parent,
        const QStyleOptionViewItem & option ,
        const QModelIndex & index) const
{
    Q_UNUSED(option);


    QComboBox *combo = new QComboBox(parent);
    combo->insertItems(0, m_names);
    myDebug() << index.data(Qt::UserRole + 2).toInt()  << index.data(Qt::UserRole + 3).toInt();

    if(index.data(Qt::UserRole + 3).toInt() > 0) {
        combo->setCurrentIndex(index.data(Qt::UserRole + 3).toInt());
    } else {
        combo->setCurrentIndex(0);
    }

    return combo;
}

void VerseTableItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    DEBUG_FUNC_NAME
    QComboBox *combo = qobject_cast<QComboBox *>(editor);
    if(!combo) {
        QItemDelegate::setEditorData(editor, index);
        return;
    }

    combo->setCurrentIndex(index.model()->data(index, Qt::UserRole + 3).toInt());
}
void VerseTableItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    DEBUG_FUNC_NAME
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
    myDebug() << combo->currentIndex();

    if(combo->currentIndex() > 0)
        model->setData(index, m_id.at(combo->currentIndex()), Qt::UserRole + 2);
    else
        model->setData(index, -1, Qt::UserRole + 2);
}
void VerseTableItemDelegate::updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
