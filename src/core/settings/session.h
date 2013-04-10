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
#ifndef SESSION_H
#define SESSION_H
#include <QVariant>
#include <QSettings>
class Session
{
public:
    Session();
    void setID(const QString &id);
    QString id() const;

    void setFile(QSettings *file);
    QSettings * file();

    void setData(const QString &key, const QVariant &value);
    QVariant getData(const QString &key, const QVariant &defaultValue = QVariant());
    bool isSet(const QString &key);

    void clearGroup(const QString &key);

private:
    QSettings *m_sessionFile;
    QString m_id;

};

#endif // SESSION_H
