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
#include "src/core/module/response/htmlresponse.h"
#include "src/core/module/response/stringresponse.h"
#include "src/core/module/response/urlresponse.h"
#include "src/core/module/response/textrangesresponse.h"
#include <QWebInspector>
class WebViewForm : public Form
{
    Q_OBJECT
public:
    explicit WebViewForm(QWidget *parent = 0);
    virtual ~WebViewForm();
    //hack needed by biblequote scrool to in advancedinterface.cpp
    //todo: make proctected
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
    virtual void debugger();
    virtual void openInNewTab();
    virtual void openHere();
    virtual void copyLinkText();
    virtual void copyLinkUrl();
    /**
     * @brief opens m_contextMenuUrl in the module specified in the sender
     */
    virtual void openIn();
    virtual void openInNew();

    virtual void openIn(QString url, const int moduleID, const Actions::OpenLinkModifiers mod);

    /**
     * @brief Calls openIn() with m_contextMenuUrl as url
     * @param moduleID
     */
    virtual void openInM(const int moduleID);
    virtual void openInNewM(const int moduleID);

    /**
     * @brief Show the ModuleSelectDialog. If a module is selected it calls openInM(moduleID)
     */
    virtual void openInS();
    virtual void openInNewS();
    virtual bool showResponse(Response *res);
    virtual void showStringResponse(StringResponse *res);
    virtual void showUrlResponse(UrlResponse *res);
    virtual void showHtmlResponse(HtmlResponse *res);
    virtual void showTextRangesResponse(TextRangesResponse *res);

    virtual void loadStyleSheet(QString url = "");
    virtual QString getStyleSheetUrl();


    virtual ModuleID moduleID() const;


protected:

    virtual void addJS(const QString &url);

    template <typename T> void addApi(T *t)
    {
        m_view->page()->mainFrame()->addToJavaScriptWindowObject(t->name(), t);
    }

    QString m_contextMenuUrl;
    QString m_contextMenuText;

    QString m_styleSheet;
    QUrl m_lastUrl;

    QWebInspector * m_inspector;

};

#endif // WEBVIEWFORM_H
