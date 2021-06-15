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
#include <QNetworkReply>
#include <QNetworkDiskCache>
WebView::WebView(QWidget *parent) :
    QWebEngineView(parent), m_doBlocking(false)
{
    //TODO: Add network mananger
    //m_networManager = new NetworkAccessManager(this);
    //this->page()->setNetworkAccessManager(m_networManager);

    this->page()->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, false);

}
void WebView::contextMenuEvent(QContextMenuEvent * ev)
{
    emit contextMenuRequested(ev);
}

void WebView::scrollToAnchor(const QString &anchor)
{
    page()->runJavaScript("window.location.href = '" + anchor + "';");
}

void WebView::scrollTo(int x, int y)
{
    page()->runJavaScript(QString("window.scrollTo(%1, %2);").arg(x).arg(y));
}
void WebView::scrollTo(float x, float y)
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


void WebView::mouseReleaseEvent(QMouseEvent *event)
{
   if (mouseReleased(event->pos())) {
        event->accept();
        return;
    }
    QWebEngineView::mouseReleaseEvent(event);
}
bool WebView::mouseReleased(const QPoint &pos)
{
    //const QWebHitTestResult hitTest = page()->mainFrame()->hitTestContent(pos);
    //const QString url = hitTest.element().attribute("href");
/*
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
    }*/
    return false;
}
void WebView::mousePressEvent(QMouseEvent *event)
{
     m_pressedButtons = event->buttons();
     m_keyboardModifiers = event->modifiers();
     QWebEngineView::mousePressEvent(event);
}
bool WebView::hasSelection() const
{
    #if QT_VERSION >= 0x40800
      return page()->hasSelection();
    #else
      return true;
    #endif
}
void WebView::setBlockRules(const BlockRules &rules)
{
    DEBUG_FUNC_NAME
    //m_networManager->setBlockRules(rules);
    //currently block only mainFrame
    //connect(this->page()->mainFrame(), SIGNAL(loadFinished(bool)), this, SLOT(applyHidingRules(bool)));
}

void WebView::applyHidingRules(bool ok)
{
    /*
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
    }*/
}

void WebView::load(const QUrl &url)
{
   /* NetworkAccessManager *sep = new NetworkAccessManager(this);
    connect(sep, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(filter(QNetworkReply*)));
    QNetworkRequest req(url);
    req.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
    sep->get(req);*/
}
/**
 * @brief WebView::filter filters the html from unneed elems
 * @param r
 */
void WebView::filter(QNetworkReply *r)
{
    /*setContent(r->readAll(),"text/html",r->url());
    applyHidingRules(true);
    sender()->deleteLater();*/
}

