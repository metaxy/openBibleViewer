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
#ifndef INSERTLINKDIALOG_H
#define INSERTLINKDIALOG_H

#include <QDialog>
#include "src/core/basicclass.h"
#include <QtGui/QTreeView>
#include <QtGui/QSortFilterProxyModel>
#include "src/ui/biblepassageframe.h"

namespace Ui
{
class InsertLinkDialog;
}
/**
 * InsertLinkDialog represents a dialog where one can create a link to a note, module or something else.
 * It is used in NotesEditor.
 */

class InsertLinkDialog : public QDialog, public BasicClass
{
    Q_OBJECT
private slots:
    void save();
signals:
    void newLink(QString link);
public:
    explicit InsertLinkDialog(QWidget *parent = 0);
    ~InsertLinkDialog();
    void init();
    void setCurrent(const VerseUrl &url);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::InsertLinkDialog *ui;

    QStandardItemModel *m_itemModel;
    QTreeView *m_treeView;
    QSortFilterProxyModel *m_proxyModel;
    QItemSelectionModel *m_selectionModel;

    BiblePassageFrame *m_frame;
};

#endif // INSERTLINKDIALOG_H
