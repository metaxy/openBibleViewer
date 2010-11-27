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
#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QtGui/QDialog>
#include <QtCore/QString>

namespace Ui
{
class AboutDialog;
}
/*!
 AboutDialog represents a dialog with informationg about the software version number etc.

 @author Paul Walger <metaxy@walger.name>
*/
class AboutDialog : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(AboutDialog)
public:
    explicit AboutDialog(QWidget *parent = 0);
    virtual ~AboutDialog();
    void setText(const QString &text);

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::AboutDialog *m_ui;
};

#endif // ABOUTDIALOG_H
