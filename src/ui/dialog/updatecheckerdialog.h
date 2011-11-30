/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger <metaxy@walger.name>
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
#ifndef UPDATECHECKERDIALOG_H
#define UPDATECHECKERDIALOG_H

#include <QDialog>
#include "src/core/basicclass.h"

namespace Ui {
    class UpdateCheckerDialog;
}

class UpdateCheckerDialog : public QDialog, public BasicClass
{
    Q_OBJECT

public:
    explicit UpdateCheckerDialog(QWidget *parent = 0);
    ~UpdateCheckerDialog();

    void setText(const QString &text);
    void setUrl(const QString &url);
    void setVersion(const QString &version);

private slots:
    void skip();
    void later();
    void download();
private:
    Ui::UpdateCheckerDialog *ui;
    QString m_url;
    QString m_version;
};

#endif // UPDATECHECKERDIALOG_H
