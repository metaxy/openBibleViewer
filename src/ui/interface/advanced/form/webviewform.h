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
#ifndef WEBVIEWFORM_H
#define WEBVIEWFORM_H
#include "form.h"
#include "src/ui/webview.h"
class WebViewForm : public Form
{
    Q_OBJECT
public:
    explicit WebViewForm(QWidget *parent = 0);

    WebView *m_view;
signals:
    
public slots:
    virtual void copy();
    virtual void selectAll();
    virtual void print();
    virtual void printPreview();
    virtual void saveFile();
    virtual QString selectedText();

    virtual void zoomIn();
    virtual void zoomOut();

protected:

};

#endif // WEBVIEWFORM_H
