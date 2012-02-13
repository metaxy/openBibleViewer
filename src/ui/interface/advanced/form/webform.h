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
#ifndef WEBFORM_H
#define WEBFORM_H

#include <QWidget>
#include <QtGui/QLineEdit>
#include "webviewform.h"
#include "src/module/webpage.h"
namespace Ui
{
class WebForm;
}

class WebForm : public WebViewForm
{
    Q_OBJECT
    Q_DISABLE_COPY(WebForm)
public:
    explicit WebForm(QWidget *parent = 0);
    ~WebForm();
    void init();
    static QUrl guessUrlFromString(const QString &string);

    void restore(const QString &key);
    void save();

    void pharseUrl(QString url);
    void pharseWebUrl(const QString &webUrl);

    Form::FormType type() const;

    void openModule(const int moduleID);

private:
    Ui::WebForm *m_ui;
    QLineEdit *m_locationEdit;

    WebPage *m_page;
public slots:
    void activated();
private slots:
    void changeLocation();
    void adjustLocation();

    void loadStarted();


};

#endif // WEBFORM_H
