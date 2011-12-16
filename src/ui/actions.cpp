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

void Actions::get(const QString &url)
{
    emit _get(url);
}

void Actions::get(const QUrl &url)
{
    emit _get(url.toString());
}

void Actions::get(const VerseUrl &url)
{
    emit _get(url);
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

void Actions::showTextRanges(const QString &html, const TextRanges &range, const VerseUrl &url)
{
    emit _showTextRanges(html, range, url);
}

void Actions::showHtml(const QString &html)
{
    emit _showHtml(html);
}

void Actions::loadVerseTable(bool hadModule)
{
    emit _loadVerseTable(hadModule);
}

void Actions::reloadCurrentRange()
{
    reloadCurrentRange(false);
}
void Actions::reloadCurrentRange(bool full)
{
    emit _reloadCurrentRange(full);
}

void Actions::newSubWindowIfEmpty()
{
    emit _newSubWindowIfEmpty();
}
void Actions::searchInText(SearchResult *result)
{
    emit _searchInText(result);
}

void Actions::searchInText()
{
    emit _searchInText();
}

void Actions::needBibleWindow()
{
    emit _needBibleWindow();
}

void Actions::needDictionaryWindow()
{
    emit _needDictionaryWindow();
}

void Actions::needWebWindow()
{
    emit _needWebWindow();
}

void Actions::showDictEntry(const QString &key, int moduleID)
{
    emit _showDictEntry(key, moduleID);
}
void Actions::setCurrentVerseTableID(const int verseTableID)
{
    emit _setCurrentVerseTableID(verseTableID);
}
void Actions::reloadIf(const VerseUrl &url)
{
    emit _reloadIf(url);
}
