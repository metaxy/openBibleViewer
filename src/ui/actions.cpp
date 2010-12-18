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
#include "actions.h"
#include "src/core/dbghelper.h"
Actions::Actions(QObject *parent) :
    QObject(parent)
{
}
void Actions::showChapter(const int &moduleID, const int &bookID, const int &chapterID)
{
    emit _showChapter(moduleID, bookID, chapterID);
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
void Actions::get(const BibleUrl &url)
{
    emit _get(url);
}
void Actions::showNote(const QString &noteID)
{
    emit _showNote(noteID);
}
void Actions::setCurrentBook(const QSet<int> &bookID)
{
    DEBUG_FUNC_NAME
    emit _setCurrentBook(bookID);
}
void Actions::setCurrentChapter(const QSet<int> &chapterID)
{
    DEBUG_FUNC_NAME
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

void Actions::updateChapters(const QStringList &chapters)
{
    emit _updateChapters(chapters);
}

void Actions::updateBooks(const QHash<int, QString> &books, QList<int> ids)
{
    emit _updateBooks(books, ids);
}
void Actions::setCurrentModule(const int &moduleID)
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
void Actions::historySetUrl(const QString &url)
{
    emit _historySetUrl(url);
}
void Actions::showTextRanges(const QString &html, const TextRanges &range, const BibleUrl &url)
{
    emit _showTextRanges(html, range, url);
}
