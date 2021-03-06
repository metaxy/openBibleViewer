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
#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QWebView>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include "src/ui/web/networkaccessmanager.h"
#include "src/core/blockrules.h"

class WebView : public QWebView
{
    Q_OBJECT
public:
    explicit WebView(QWidget *parent = 0);

    void scrollToAnchor(const QString &anchor);
    bool hasSelection() const;

    void setBlockRules(const BlockRules &rules);

    void load(const QUrl &url);
protected:
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent * ev);
signals:
    void contextMenuRequested(QContextMenuEvent * ev);
    void linkMiddleOrCtrlClicked(const QUrl &url);
    void linkShiftClicked(const QUrl &url);
private slots:
    void applyHidingRules(bool ok);

    void filter(QNetworkReply* r);
private:
    bool mouseReleased(const QPoint &pos);
    Qt::KeyboardModifiers m_keyboardModifiers;
    Qt::MouseButtons m_pressedButtons;

    NetworkAccessManager *m_networManager;
    bool m_doBlocking;
};

#endif // WEBVIEW_H
