/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009 Paul Walger
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
#include <QtCore/QString>
#include <QtCore/QStringList>
/*!
 History is a simple class to get a url history

 @author Paul Walger <metaxy@walger.name>
*/
class History
{
private:
    QStringList forwardItems;
    QStringList backwardItems;
    bool goF;
public:
    History();
    QString forward();
    QString backward();
    bool forwardAvailable();
    bool backwardAvailable();
    void setCurrent(const QString &current);
};

#endif // HISTORY_H
