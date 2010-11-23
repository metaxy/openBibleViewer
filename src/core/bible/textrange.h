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
#ifndef TEXTRANGE_H
#define TEXTRANGE_H
#include <QtCore/QString>
#include <QtCore/QList>
#include "src/core/bible/verse.h"
class TextRange
{
public:
    TextRange();
    QString join(const QString &seperator);
    void addVerse(const Verse &verse);
    void addVerse(const QList<Verse> &verse);
    void setTitle(const QString &title);
    QList<Verse> verse();
    QString title();

private:
    QList<Verse> m_verse;
    QString m_title;
};

#endif // TEXTRANGE_H
