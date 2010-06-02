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
#include <QtCore/QtDebug>
#define myDebug() qDebug() << Q_FUNC_INFO << " "

#ifndef QT_NO_DEBUG
#define DEBUG_FUNC_NAME DbgHelper dbgHelper(Q_FUNC_INFO);
#else
#define DEBUG_FUNC_NAME DbgHelper dbgHelper(Q_FUNC_INFO);
#endif
#ifndef DBGHELPER_H
#define DBGHELPER_H
#include <QtCore/QString>
/*!
 DbgHelper is a debug helper, it show the debug ouput more clearly

 @author Paul Walger <metaxy@walger.name>
*/

class DbgHelper
{
public:
    DbgHelper(const QString &t);
    ~DbgHelper();
private:
    QString txt;
    static int indent;
    static int colorIndex;
    int myColor;
};
#endif
