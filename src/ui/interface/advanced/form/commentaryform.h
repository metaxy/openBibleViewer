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
#ifndef COMMENTARYFORM_H
#define COMMENTARYFORM_H

#include <QWidget>
#include "webviewform.h"
#include "src/core/history.h"
#include "src/core/module/commentary/commentary.h"
namespace Ui {
class CommentaryForm;
}

class CommentaryForm : public WebViewForm
{
    Q_OBJECT
    
public:
    explicit CommentaryForm(QWidget *parent = 0);
    ~CommentaryForm();

    void init();

    void restore(const QString &key);
    void save();

    void parseUrl(QString string);
    void parseUrl(VerseUrl url);

    Form::FormType type() const;

    ModuleID moduleID() const;
    void activated();

    void showRanges(Ranges ranges, const VerseUrl &source);
private:
    bool loaded();
private slots:
    void changeLocation();
    void backward();
    void forward();
    void showContextMenu(QContextMenuEvent* ev);

    void get(QUrl url);
    void newGet(QUrl url);

    void openInBrowser();
    void openInBible();
    void showUrlResponse(UrlResponse *res);
private:
    Ui::CommentaryForm *ui;
    Commentary *m_com;
    VerseUrl m_url;
    History m_browserHistory;
    void setButtons();
    void historySetUrl(QString url);
    void actTitle();

    QString transformUrl(const QString &url);
};

#endif // COMMENTARYFORM_H
