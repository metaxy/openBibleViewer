/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009 Paul Walger
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
#ifndef MDIFORM_H
#define MDIFORM_H

#include <QtGui/QWidget>
#include <QtGui/QCloseEvent>
#include "ui_mdiform.h"
#include "src/core/history.h"
namespace Ui
{
class MdiForm;
}

class MdiForm : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(MdiForm)
public:
    explicit MdiForm(QWidget *parent = 0);
    virtual ~MdiForm();
    Ui::MdiForm *m_ui;
signals:
    void onClose();
    void historyGo(QString);
    void previousChapter();
    void nextChapter();
public slots:
    void historyGetUrl(QString url);
    void backward();
    void forward();

protected:
    virtual void changeEvent(QEvent *e);
    bool eventFilter(QObject *obj, QEvent *ev);
private:
    History browserHistory;
    void setButtons();


};

#endif // MDIFORM_H
