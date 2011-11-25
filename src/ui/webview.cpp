/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger
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
#include <QtWebKit/QWebFrame>
WebView::WebView(QWidget *parent) :
    QWebView(parent)
{
    this->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);
}
void WebView::contextMenuEvent(QContextMenuEvent * ev)
{
    emit contextMenuRequested(ev);
}

void WebView::scrollToAnchor(const QString &anchor)
{
#if QT_VERSION >= 0x040700
    page()->mainFrame()->scrollToAnchor(anchor);
#else
    page()->mainFrame()->evaluateJavaScript("window.location.href = '" + anchor + "';");
#endif

}
