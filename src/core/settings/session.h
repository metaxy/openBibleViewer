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
#ifndef SESSION_H
#define SESSION_H
#include <QtCore/QVariant>
class Session
{
public:
    Session();
    void setData(QString key, QVariant value);
    QVariant getData(QString key, QVariant defaultValue) const;
    QVariant getData(QString key) const;
    bool isSet(QString key) const;
    QMapIterator<QString, QVariant> getInterator() const;
private:
    QMap<QString, QVariant> m_data;


};

#endif // SESSION_H
