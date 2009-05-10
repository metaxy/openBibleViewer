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
#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QtGui/QDialog>
#include <QString>
namespace Ui {
    class aboutDialog;
}

class aboutDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(aboutDialog)
public:
    explicit aboutDialog(QWidget *parent = 0);
    virtual ~aboutDialog();
    void setText(QString text);

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::aboutDialog *m_ui;
};

#endif // ABOUTDIALOG_H
