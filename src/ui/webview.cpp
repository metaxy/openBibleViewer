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
