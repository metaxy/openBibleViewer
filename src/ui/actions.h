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
#include "src/core/link/verseurl.h"
#include "src/core/verse/textranges.h"
#include "src/core/verse/versification.h"
#include "src/core/search/searchresult.h"
class Actions : public QObject
{
    Q_OBJECT
public:
    explicit Actions(QObject *parent = 0);
    enum OpenLinkModifiers {
        NoModifer,
        OpenInNewWindow,
        ForceOpenInThisWindow
    };

signals:
    void _previousChapter();
    void _nextChapter();


    /**
      By default the Interface pharse all urls.
      */
    void _get(const QString &url, const Actions::OpenLinkModifiers mod);
    void _get(const VerseUrl &url, const Actions::OpenLinkModifiers mod);


    void _setCurrentBook(const QSet<int> &bookID);
    void _setCurrentChapter(const QSet<int> &chapterID);

    void _reloadActive();

    void _setTitle(const QString &title);
    void _updateChapters(int bookID, QSharedPointer<Versification> v11n);
    void _updateBooks(QSharedPointer<Versification> v11n);

    void _clearChapters();
    void _clearBooks();
    void _clear();


    void _setCurrentModule(const int moduleID);

    void _setTabbedView();
    void _setSubWindowView();

    void _loadVerseTable(bool hadModule);
    void _reloadCurrentRange(bool full);
    void _reloadIf(const VerseUrl &url);




    void _searchInText(SearchResult *result);
    void _searchInText();

    void _setCurrentVerseTableID(const int verseTableID);

    void _moduleChanged(const int moduleID);

public slots:

    void previousChapter();
    void nextChapter();


    void get(const QString &url);
    void get(const QUrl &url);
    void get(const VerseUrl &url);


    void get(const QString &url, const Actions::OpenLinkModifiers mod);
    void get(const QUrl &url, const Actions::OpenLinkModifiers mod);
    void get(const VerseUrl &url, const Actions ::OpenLinkModifiers mod);

    void newGet(const QUrl &url);

    void setCurrentBook(const QSet<int> &bookID);
    void setCurrentChapter(const QSet<int> &chapterID);

    void reloadActive();

    void setTitle(const QString &title);

    void updateChapters(int bookID, QSharedPointer<Versification>v11n);
    void updateBooks(QSharedPointer<Versification>v11n);

    void clearBooks();
    void clearChapters();
    void clear();

    void setCurrentModule(const int moduleID);

    void setTabbedView();
    void setSubWindowView();


    void loadVerseTable(bool hadModule);

    void reloadCurrentRange();
    void reloadCurrentRange(bool full);

    void searchInText(SearchResult *result);
    void searchInText();

    void setCurrentVerseTableID(const int verseTableID);

    void reloadIf(const VerseUrl &url);

    void moduleChanged(const int moduleID);
};

#endif // ACTIONS_H
