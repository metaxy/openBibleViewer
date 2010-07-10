#include "webview.h"
#include <QtGui/QMenu>
#include <QtGui/QAction>
#include <QContextMenuEvent>
WebView::WebView(QWidget *parent) :
    QWebView(parent)
{
}
void WebView::contextMenuEvent ( QContextMenuEvent * ev )
{
    emit contextMenuRequested(ev);
}
