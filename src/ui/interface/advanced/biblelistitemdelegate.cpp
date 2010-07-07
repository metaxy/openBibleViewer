#include "biblelistitemdelegate.h"
#include <QtGui/QComboBox>
#include "src/core/dbghelper.h"
#include <QtCore/QTimer>
BibleListItemDelegate::BibleListItemDelegate(QObject *parent)
    : QItemDelegate(parent)
{
    m_parent = parent;
}
void BibleListItemDelegate::init()
{
    m_names = m_moduleManager->getBibleTitles();
    m_id = m_moduleManager->getBibleIDs();
}

QWidget * BibleListItemDelegate::createEditor(QWidget *parent,
     const QStyleOptionViewItem &/* option */,
     const QModelIndex &/* index */) const
{
    DEBUG_FUNC_NAME
    QComboBox *combo = new QComboBox( parent );
    combo->insertItems(0,m_names);
    return combo;
}

void BibleListItemDelegate::setEditorData(QWidget *editor,const QModelIndex &index) const
{
    QComboBox *combo = qobject_cast<QComboBox *>( editor );
    if(!combo){
        QItemDelegate::setEditorData( editor, index );
        return;
    }
    int pos = combo->findText( index.model()->data( index ).toString(), Qt::MatchExactly );
    combo->setCurrentIndex( pos );
}
void BibleListItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const
{
    QComboBox *combo = qobject_cast<QComboBox *>( editor );
     if ( !combo ) {
       QItemDelegate::setModelData( editor, model, index );
       return;
     }
     model->setData( index, combo -> currentText() );
     model->setData(index,m_id.at(combo->currentIndex()),Qt::UserRole+2);
}
void BibleListItemDelegate::updateEditorGeometry(QWidget *editor,
     const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
