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
#include "src/core/bible/textranges.h"
class Actions : public QObject
{
    Q_OBJECT
public:
    explicit Actions(QObject *parent = 0);

signals:
    void _showChapter(const int &moduleID, const int &bookID, const int &chapterID);
    void _previousChapter();
    void _nextChapter();
    /**
      By default the Interface pharse all urls.
      */
    void _get(const QString &url);
    void _get(const BibleUrl &url);

    void _showNote(const QString &nodeID);

    void _setCurrentBook(const QSet<int> &bookID);
    void _setCurrentChapter(const QSet<int> &chapterID);

    void _reloadActive();

    void _setTitle(const QString &title);
    void _updateChapters(const QStringList &chapters);
    void _clearChapters();
    void _updateBooks(const QHash<int, QString> &books, QList<int> ids);
    void _clearBooks();

    void _setCurrentModule(const int &moduleID);

    void _setTabbedView();
    void _setSubWindowView();

    /**
      I will remove it soon.
      */
    void _newBookmark();

    void _historySetUrl(const QString &url);
    void _showTextRanges(const QString &html, const TextRanges &range, const BibleUrl &url);

    void _loadBibleList(bool hadBible);
    void _reloadChapter(bool full);

    void _reloadBible();
    void _reshowCurrentRange();

public slots:
    /**
      * Show a chapter in current SubWindow.
      */
    void showChapter(const int &moduleID, const int &bookID, const int &chapterID);
    void previousChapter();
    void nextChapter();

    void get(const QString &url);
    void get(const QUrl &url);
    void get(const BibleUrl &url);

    void showNote(const QString &noteID);

    void setCurrentBook(const QSet<int> &bookID);
    void setCurrentChapter(const QSet<int> &chapterID);

    void reloadActive();
    /**
      * Same as reloadChapter(false)
      */
    void reloadChapter();
    void reloadChapter(bool full);

    void setTitle(const QString &title);

    void updateChapters(const QStringList &chapters);
    void clearChapters();
    void updateBooks(const QHash<int, QString> &books, QList<int> ids);
    void clearBooks();

    void setCurrentModule(const int &moduleID);

    void setTabbedView();
    void setSubWindowView();

    void historySetUrl(const QString &url);
    /**
      * Using showTextRanges there is no need to call historySetUrl seperatly.
      */
    void showTextRanges(const QString &html, const TextRanges &range, const BibleUrl &url);

    void loadBibleList(bool hadBible);

    void reloadBible();
    void reshowCurrentRange();



};

#endif // ACTIONS_H
