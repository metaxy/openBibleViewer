/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger
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
#include "src/core/settings/settings.h"
#include "src/core/basicclass.h"
#include "src/ui/biblepassageframe.h"
#include <QtGui/QDialog>

namespace Ui
{
class BiblePassageDialog;
}
/**
 * BiblePassageDialog represents a dialog to select or change a bible passage using BiblePassageFrame.
 */
class BiblePassageDialog : public QDialog, public BasicClass
{
    Q_OBJECT
public:
    explicit BiblePassageDialog(QWidget *parent = 0);
    virtual ~BiblePassageDialog();
    void init();
    BiblePassageFrame *frame();

protected:
    virtual void changeEvent(QEvent *e);
public slots:
    void save();
signals:
    void updated(VerseUrl url);
private:
    Ui::BiblePassageDialog *m_ui;
    BiblePassageFrame *m_frame;
};

#endif // BIBLEPASSAGEDIALOG_H
