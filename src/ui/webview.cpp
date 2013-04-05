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
#include <QtWebKit/QWebFrame>
#include "src/core/dbghelper.h"
#include <QWebElement>
#include <QNetworkReply>
WebView::WebView(QWidget *parent) :
    QWebView(parent), m_doBlocking(false)
{
    m_networManager = new NetworkAccessManager(this);
    this->page()->setNetworkAccessManager(m_networManager);


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

void WebView::mouseReleaseEvent(QMouseEvent *event)
{
   if (mouseReleased(event->pos())) {
        event->accept();
        return;
    }
    QWebView::mouseReleaseEvent(event);
}
bool WebView::mouseReleased(const QPoint &pos)
{
    const QWebHitTestResult hitTest = page()->mainFrame()->hitTestContent(pos);
    const QString url = hitTest.element().attribute("href");

    if (!url.isEmpty()) {
        if ((m_pressedButtons & Qt::MidButton) ||
            ((m_pressedButtons & Qt::LeftButton) && (m_keyboardModifiers & Qt::ControlModifier))) {
            emit linkMiddleOrCtrlClicked(url);
            return true;
        }

        if ((m_pressedButtons & Qt::LeftButton) && (m_keyboardModifiers & Qt::ShiftModifier)) {
            emit linkShiftClicked(url);
            return true;

        }
    }
    return false;
}
void WebView::mousePressEvent(QMouseEvent *event)
{
     m_pressedButtons = event->buttons();
     m_keyboardModifiers = event->modifiers();
     QWebView::mousePressEvent(event);
}
bool WebView::hasSelection() const
{
    #if QT_VERSION >= 0x40800
      return QWebView::hasSelection();
    #else
      return true;
    #endif
}
void WebView::setBlockRules(const BlockRules &rules)
{
    DEBUG_FUNC_NAME
    m_networManager->setBlockRules(rules);
    //currently block only mainFrame
    connect(this->page()->mainFrame(), SIGNAL(loadFinished(bool)), this, SLOT(applyHidingRules(bool)));
}

void WebView::applyHidingRules(bool ok)
{
    DEBUG_FUNC_NAME
    if (!ok)
        return;
    if(!m_networManager->m_doBlock) {
        return;
    }
    QWebFrame * frame = this->page()->mainFrame();


    QWebElement document = frame->documentElement();

    // HIDE RULES
    Q_FOREACH(const QString & filter, m_networManager->m_blockRules.m_filterBySelector)
    {
        QWebElementCollection elements = document.findAll(filter);

        Q_FOREACH(QWebElement el, elements)
        {
            if (el.isNull())
                continue;
            myDebug() << "Hide element: " << el.localName();
            el.removeFromDocument();
        }
    }
}

void WebView::load(const QUrl &url)
{
    NetworkAccessManager *sep = new NetworkAccessManager(this);
    connect(sep, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(f(QNetworkReply*)));

    sep->get(QNetworkRequest(url));
}
/**
 * @brief WebView::fn filters the html from unneed elems
 * @param r
 */
void WebView::f(QNetworkReply *r)
{
    setContent(r->readAll(),"text/html",r->url());
    applyHidingRules(true);
    sender()->deleteLater();
}
