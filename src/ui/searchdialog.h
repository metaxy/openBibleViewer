/***************************************************************************
openBibleViewer - Free Bibel Viewer
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
#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
        class searchDialog;
}

class searchDialog : public QDialog {
        Q_OBJECT
private slots:
        int search( void );
        void showMore( void);
signals:
        void searched(QString string,bool regexp,bool whole,bool casesen );
public:
        explicit searchDialog(QWidget *parent = 0);
        int setText(QString text);
        virtual ~searchDialog();

protected:
        virtual void changeEvent(QEvent *e);

private:
        Ui::searchDialog *m_ui;
};

#endif // SEARCHDIALOG_H
