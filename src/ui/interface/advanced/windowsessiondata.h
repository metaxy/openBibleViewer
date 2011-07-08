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
#ifndef WINDOWSESSIONDATA_H
#define WINDOWSESSIONDATA_H
#include <QtCore/QString>
#include <QtCore/QRect>
#include <QtCore/QVariant>
#include <QtGui/QWidget>
#include "src/core/settings/settings.h"
#include "src/core/dbghelper.h"

class WindowSessionData
{
public:
    WindowSessionData();

    void setWindowID(const int windowID);

    void setData(QMap<QString, QVariant> d);
    QMap<QString, QVariant> getData();

    void setType(OBVCore::FormType t);
    OBVCore::FormType getType();


private:
    int m_windowID;
    QMap<int, QMap<QString, QVariant> > m_data;
    QMap<int, OBVCore::FormType> m_type;
};

#endif // WINDOWSESSIONDATA_H
