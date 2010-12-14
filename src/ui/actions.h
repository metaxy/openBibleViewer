/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2010 Paul Walger
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
#ifndef ACTIONS_H
#define ACTIONS_H

#include <QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include "src/core/bible/bibleurl.h"
class Actions : public QObject
{
    Q_OBJECT
public:
    explicit Actions(QObject *parent = 0);

signals:
    void _showChapter(const int &moduleID, const int &bookID, const int &chapterID);
    /**
      By default the Interface pharse all urls.
      */
    void _get(const QString &url);
    void _get(const BibleUrl &url);

    void _showNote(const QString &nodeID);

    void _setCurrentBook(const QSet<int> &bookID);
    void _setCurrentChapter(const QSet<int> &chapterID);

    void _reloadActiveBible();

    void _setTitle(const QString &title);
    void _updateChapters(const QStringList &chapters);
    void _updateBooks(const QHash<int, QString> &books, QList<int> ids);

    void _setCurrentModule(const int &moduleID);

    void _setTabbedView();
    void _setSubWindowView();
public slots:
    /**
      Show a chapter in current SubWindow.
      */
    void showChapter(const int &moduleID, const int &bookID, const int &chapterID);

    void get(const QString &url);
    void get(const QUrl &url);
    void get(const BibleUrl &url);

    void showNote(const QString &noteID);

    void setCurrentBook(const QSet<int> &bookID);
    void setCurrentChapter(const QSet<int> &chapterID);

    void reloadActiveBible();

    void setTitle(const QString &title);
    void updateChapters(const QStringList &chapters);
    void updateBooks(const QHash<int, QString> &books, QList<int> ids);

    void setCurrentModule(const int &moduleID);

    void setTabbedView();
    void setSubWindowView();

};

#endif // ACTIONS_H
