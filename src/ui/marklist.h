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
#ifndef MARKLIST_H
#define MARKLIST_H

#include <QtGui/QDialog>
#include <QtGui/QSortFilterProxyModel>
#include <QtGui/QItemSelectionModel>
#include <QtGui/QTreeView>
#include "src/core/basicclass.h"
namespace Ui
{
class MarkList;
}

class MarkList : public QDialog, public BasicClass
{
    Q_OBJECT
private slots:
    void load(QModelIndex index);
    void showContextMenu(QPoint point);
    void deleteMarks();
    void addNote(const QString &id);
    void removeNote(const QString &id);
public:
    MarkList(QWidget *parent = 0);
    ~MarkList();
    void init();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MarkList *ui;
    QStandardItemModel *m_itemModel;
    QSortFilterProxyModel *m_proxyModel;
    QItemSelectionModel *m_selectionModel;
    QPoint m_currentPoint;
    void addMark(const int &row, const QString &id);
};

#endif // MARKLIST_H
