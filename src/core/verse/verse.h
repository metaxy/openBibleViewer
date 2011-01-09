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
#ifndef VERSE_H
#define VERSE_H
#include <QtCore/QString>
class Verse
{
public:
    Verse();
    Verse(const int &verseID, const QString &text);
    int verseID() const;
    QString data() const;

    void append(const QString &str);
    void prepend(const QString &str);
    void insert(int position, const QString & str);
private:
    int m_verseID;
    QString m_text;
};

#endif // VERSE_H
