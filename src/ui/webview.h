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

#include <QtWebEngineWidgets/QtWebEngineWidgets>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>

class WebView : public QWebEngineView
{
    Q_OBJECT
public:
    explicit WebView(QWidget *parent = 0);

    bool hasSelection() const;

    void load(const QUrl &url);

    void insertStyleSheet(const QString &name, const QString &source, bool immediately);
    void removeStyleSheet(const QString &name, bool immediately);

    void scrollToAnchor(const QString &anchor);
    void scrollTo(int x, int y);
    void scrollTo(qreal x, qreal y);
    void scrollTo(QPoint x);
    void scrollTo(QPointF x);

    void setContentEditable(bool &editable);
protected:
    //void mouseReleaseEvent(QMouseEvent *event);
    //void mousePressEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent * ev);
signals:
    void contextMenuRequested(QContextMenuEvent * ev);
    void loadFinished(bool ok);
    
};

#endif // WEBVIEW_H
