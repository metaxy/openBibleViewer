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
#ifndef ACTIONS_H
#define ACTIONS_H

#include <QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include "src/core/verse/verseurl.h"
#include "src/core/verse/textranges.h"
#include "src/core/verse/versification.h"
#include "src/core/search/searchresult.h"
class Actions : public QObject
{
    Q_OBJECT
public:
    explicit Actions(QObject *parent = 0);

signals:
    void _previousChapter();
    void _nextChapter();


    /**
      By default the Interface pharse all urls.
      */
    void _get(const QString &url);
    void _get(const VerseUrl &url);


    void _setCurrentBook(const QSet<int> &bookID);
    void _setCurrentChapter(const QSet<int> &chapterID);

    void _reloadActive();

    void _setTitle(const QString &title);
    void _updateChapters(int bookID, Versification *v11n);
    void _clearChapters();
    void _updateBooks(Versification *v11n);
    void _clearBooks();

    void _setCurrentModule(const int moduleID);

    void _setTabbedView();
    void _setSubWindowView();

    void _showTextRanges(const QString &html, const TextRanges &range, const VerseUrl &url);
    void _showHtml(const QString &html);

    void _loadVerseTable(bool hadModule);
    void _reloadChapter(bool full);

    void _reloadBible();
    void _reShowCurrentRange();
    void _newSubWindowIfEmpty();

    void _needDictionaryWindow();
    void _needBibleWindow();
    void _needWebWindow();


    void _searchInText(SearchResult *result);
    void _searchInText();

    void _showDictEntry(const QString &key, int moduleID);

public slots:

    void previousChapter();
    void nextChapter();


    void get(const QString &url);
    void get(const QUrl &url);
    void get(const VerseUrl &url);


    void setCurrentBook(const QSet<int> &bookID);
    void setCurrentChapter(const QSet<int> &chapterID);

    void reloadActive();
    /**
      * Same as reloadChapter(false)
      */
    void reloadChapter();
    void reloadChapter(bool full);

    void setTitle(const QString &title);

    void updateChapters(int bookID, Versification *v11n);
    void clearChapters();
    void updateBooks(Versification *v11n);
    void clearBooks();

    void setCurrentModule(const int moduleID);

    void setTabbedView();
    void setSubWindowView();
    void newSubWindowIfEmpty();

    void needDictionaryWindow();
    void needBibleWindow();
    void needWebWindow();

    /**
      * Using showTextRanges there is no need to call historySetUrl seperatly.
      */
    void showTextRanges(const QString &html, const TextRanges &range, const VerseUrl &url);
    void showDictEntry(const QString &key, int moduleID=-1);

    void showHtml(const QString &html);

    void loadVerseTable(bool hadModule);

    void reloadBible();
    void reShowCurrentRange();

    void searchInText(SearchResult *result);
    void searchInText();


};

#endif // ACTIONS_H
