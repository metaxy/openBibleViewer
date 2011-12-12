/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger <metaxy@walger.name>
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
#ifndef STRONGURL_H
#define STRONGURL_H
#include "src/core/link/url.h"
#include <QtCore/QList>
class StrongUrl : public Url
{
public:
    StrongUrl();
    enum StrongPrefix {
        H,
        G
    };
    void addStrongNumber(const int number);
    void setStrongNumbers(const QList<int> &numbers);
    void setPrefix(StrongPrefix prefix);
    bool fromText(QString text);
    bool fromString(QString strong);
    QString toString();
    QString toText();

    QString toKey();
    QList<int> numbers() const;
    StrongPrefix prefix() const;
private:
    StrongPrefix m_prefix;
    QList<int> m_numbers;

};
#endif // STRONGURL_H
