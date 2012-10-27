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
#include "actions.h"
#include "src/core/dbghelper.h"
Actions::Actions(QObject *parent) :
    QObject(parent)
{
}
void Actions::previousChapter()
{
    emit _previousChapter();
}

void Actions::nextChapter()
{
    emit _nextChapter();
}
void Actions::get(const QString &url, const Actions::OpenLinkModifiers mod)
{
    emit _get(url, mod);
}

void Actions::get(const QUrl &url, const Actions::OpenLinkModifiers mod)
{
    DEBUG_FUNC_NAME
    emit _get(url.toString(), mod);
}

void Actions::get(const VerseUrl &url, const Actions::OpenLinkModifiers mod)
{
    DEBUG_FUNC_NAME
    emit _get(url, mod);
}

void Actions::get(const QString &url)
{
    get(url, Actions::NoModifer);
}

void Actions::get(const QUrl &url)
{
    get(url, Actions::NoModifer);
}

void Actions::get(const VerseUrl &url)
{
    get(url, Actions::NoModifer);
}
void Actions::newGet(const QUrl &url)
{
    DEBUG_FUNC_NAME
    get(url, Actions::OpenInNewWindow);
}



void Actions::setCurrentBook(const QSet<int> &bookID)
{
    emit _setCurrentBook(bookID);
}

void Actions::setCurrentChapter(const QSet<int> &chapterID)
{
    emit _setCurrentChapter(chapterID);
}

void Actions::reloadActive()
{
    emit _reloadActive();
}

void Actions::setTitle(const QString &title)
{
    emit _setTitle(title);
}

void Actions::updateChapters(int bookID, QSharedPointer<Versification> v11n)
{
    if(v11n != NULL)
        emit _updateChapters(bookID, v11n);
}

void Actions::updateBooks(QSharedPointer<Versification> v11n)
{
    if(v11n != NULL)
        emit _updateBooks(v11n);
}

void Actions::clearBooks()
{
    emit _clearBooks();
}

void Actions::clearChapters()
{
    emit _clearChapters();
}
void Actions::clear()
{
    emit _clear();
}

void Actions::setCurrentModule(const int moduleID)
{
    emit _setCurrentModule(moduleID);
}

void Actions::setTabbedView()
{
    emit _setTabbedView();
}

void Actions::setSubWindowView()
{
    emit _setSubWindowView();
}


void Actions::reloadCurrentRange()
{
    reloadCurrentRange(false);
}
void Actions::reloadCurrentRange(bool full)
{
    emit _reloadCurrentRange(full);
}

void Actions::searchInText(SearchResult *result)
{
    emit _searchInText(result);
}

void Actions::searchInText()
{
    emit _searchInText();
}

void Actions::setCurrentVerseTableID(const int verseTableID)
{
    emit _setCurrentVerseTableID(verseTableID);
}

void Actions::reloadIf(const VerseUrl &url)
{
    emit _reloadIf(url);
}

void Actions::moduleChanged(const int moduleID)
{
    emit _moduleChanged(moduleID);
}
