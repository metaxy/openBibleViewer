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
#ifndef BIBLEPASSAGEDIALOG_H
#define BIBLEPASSAGEDIALOG_H
#include "src/core/settings.h"
#include "src/core/basicclass.h"
#include <QtGui/QDialog>

namespace Ui
{
class BiblePassageDialog;
}
/*!
 BiblePassageDialog represents a dialog select or change a bible passage

*/
class BiblePassageDialog : public QDialog, public BasicClass
{
    Q_OBJECT
public:
    explicit BiblePassageDialog(QWidget *parent = 0);
    virtual ~BiblePassageDialog();
    void setCurrent(const int &bible, const QString &path, const int &book, const int &chapter, const int &verse);

protected:
    virtual void changeEvent(QEvent *e);
public slots:
    void save();

    void indexChanged(int index);
signals:
    void updated(QString pos);
private:
    Ui::BiblePassageDialog *m_ui;
    QStringList m_bibles;
    int m_bibleID, m_bookID, m_chapterID, m_verseID;
    QString m_path;
};

#endif // BIBLEPASSAGEDIALOG_H
