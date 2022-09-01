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
#include "webview.h"
#include <QtWebEngineWidgets/QtWebEngineWidgets>
#include "src/core/dbghelper.h"

WebView::WebView(QWidget *parent) :
    QWebEngineView(parent)
{
    this->page()->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
}

void WebView::contextMenuEvent(QContextMenuEvent * ev)
{
    emit contextMenuRequested(ev);
}
void WebView::setContentEditable(bool& editable)
{
    page()->runJavaScript(QString("document.documentElement.contentEditable = %1;").arg(editable));
}

void WebView::scrollToAnchor(const QString &anchor)
{
    page()->runJavaScript("document.querySelector('."+anchor+"').scrollIntoView({behavior: 'smooth' });");
}

void WebView::scrollTo(int x, int y)
{
    page()->runJavaScript(QString("window.scrollTo(%1, %2);").arg(x).arg(y));
}

void WebView::scrollTo(qreal x, qreal y)
{
    page()->runJavaScript(QString("window.scrollTo(%1, %2);").arg(x).arg(y));
}

void WebView::scrollTo(QPoint point)
{
    scrollTo(point.x(), point.y());
}
void WebView::scrollTo(QPointF point)
{
    scrollTo(point.x(), point.y());
}
void WebView::insertStyleSheet(const QString &name, const QString &source, bool immediately)
{
    QFile file(source);
    file.open(QIODevice::ReadOnly);
    if(!file.open(QIODevice::ReadOnly)) {
        myDebug() << "error: " << file.errorString();
    }

    QTextStream in(&file);
    QString cssContent = in.readAll();
    QWebEngineScript script;
    QString s = QString::fromLatin1("(function() {"\
                                    "    css = document.createElement('style');"\
                                    "    css.type = 'text/css';"\
                                    "    css.id = '%1';"\
                                    "    document.head.appendChild(css);"\
                                    "    css.innerText = '%2';"\
                                    "})()").arg(name).arg(cssContent.simplified());
    if (immediately)
        page()->runJavaScript(s, QWebEngineScript::ApplicationWorld);

    script.setName(name);
    script.setSourceCode(s);
    script.setInjectionPoint(QWebEngineScript::DocumentReady);
    script.setRunsOnSubFrames(true);
    script.setWorldId(QWebEngineScript::ApplicationWorld);
    page()->scripts().insert(script);
    file.close();
}

void WebView::removeStyleSheet(const QString &name, bool immediately)
{
    QString s = QString::fromLatin1("(function() {"\
                                    "    var element = document.getElementById('%1');"\
                                    "    element.outerHTML = '';"\
                                    "    delete element;"\
                                    "})()").arg(name);
    if (immediately)
        page()->runJavaScript(s, QWebEngineScript::ApplicationWorld);

    QWebEngineScript script = page()->scripts().findScript(name);
    page()->scripts().remove(script);
}


bool WebView::hasSelection() const
{
    return page()->hasSelection();
}

void WebView::load(const QUrl &url)
{
    page()->load(url);
}
