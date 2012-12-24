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
#ifndef VERSE_H
#define VERSE_H
#include <QtCore/QString>
class Verse
{
public:
    Verse();
    Verse(const int verseID, const QString &t);
    QString text;

    int verseID() const;
    QString data() const;

    QString title;

    void append(const QString &str);
    void prepend(const QString &str);
    void insert(const int position, const QString & str);

    void setLayoutDirection(const Qt::LayoutDirection layout);

    Qt::LayoutDirection layoutDirection() const;
private:
    int m_verseID;
    Qt::LayoutDirection m_layout;
};

#endif // VERSE_H
