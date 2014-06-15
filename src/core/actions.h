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
#ifndef ACTIONS_H
#define ACTIONS_H

#include <QObject>
#include <QString>
#include <QUrl>
#include "src/core/link/verseurl.h"
#include "src/core/verse/textranges.h"
#include "src/core/verse/versification.h"
#include "src/core/search/searchresult.h"
class Actions : public QObject
{
    Q_OBJECT
public:
    explicit Actions(QObject *parent = 0);
    /**
     * @brief Modifes the way parseUrl will treat the link.
     * Attention: If NoModifer is set, the link may open in a different type Form, then
     */
    enum OpenLinkModifiers {
        NoModifer,
        OpenInNewWindow,
        ForceOpenInThisWindow,
        OpenInOtherWindow,
        OpenParallelH,
        OpenParallelV
    };
    enum OpenLinkType {
        NoType,
        BibleType,
        CommentaryType

    };


    QString name() const;
signals:
    void _previousChapter();
    void _nextChapter();


    /**
      By default the Interface parse all urls.
      */
    void _get(const QString &url, const Actions::OpenLinkModifiers mod);
    void _get(const VerseUrl &url, const Actions::OpenLinkModifiers mod);

    void _getIn(const VerseUrl &url, const Actions::OpenLinkType t);

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

    void _reloadCurrentRange(bool full);
    void _reloadIf(const VerseUrl &url);


    void _searchInText(SearchResult *result);
    void _searchInText();

    void _setActiveItem(const int verseTableID);

    void _moduleChanged(const int moduleID);
    void _setFullScreen(bool enable);
    void _toggleFullScreen();
    void _removeModuleFromVerseTable(const int itemID);
public slots:
    void debug(const QString &s);
    void previousChapter();
    void nextChapter();


    void get(const QString &url);
    void get(const QUrl &url);
    void get(const VerseUrl &url);


    void get(const QString &url, const Actions::OpenLinkModifiers mod);
    void get(const QUrl &url, const Actions::OpenLinkModifiers mod);
    void get(const VerseUrl &url, const Actions ::OpenLinkModifiers mod);

    void getIn(const VerseUrl &url, const Actions::OpenLinkType t);

    void newGet(const QUrl &url);

    void setCurrentBook(const QSet<int> &bookID);
    void setCurrentChapter(const QSet<int> &chapterID);

    void setCurrentBook(const int bookID);
    void setCurrentChapter(const int chapterID);

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



    void reloadCurrentRange();
    void reloadCurrentRange(bool full);
    void reloadIf(const VerseUrl &url);

    void moduleChanged(const int moduleID);


    void searchInText(SearchResult *result);
    void searchInText();

    void setActiveItem(const int verseTableID);
    // updates the ui if full screen was enabled/disabled
    void setFullScreen(bool enable);
    // tiggers full screen or not
    void toggleFullScreen();

    void removeModuleFromVerseTable(const int itemID);

};

#endif // ACTIONS_H
