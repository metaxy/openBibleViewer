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
#ifndef HISTORY_H
#define HISTORY_H

#include <QVariant>
#include <QStringList>
/**
 * History represents a url history.
 */
class History
{

public:
    History();
    QString forward();
    QString backward();
    bool forwardAvailable() const;
    bool backwardAvailable() const;
    void setCurrent(const QString &current);


    QVariant data1() const;
    QVariant data2() const;
    QVariant data3() const;

    void setData1(const QVariant &d);
    void setData2(const QVariant &d);
    void setData3(const QVariant &d);

    void lock();
    void unlock();

    QString current() const;

private:
    QStringList m_forwardItems;
    QStringList m_backwardItems;
    bool m_goF;
    bool m_lock;
};

#endif // HISTORY_H
