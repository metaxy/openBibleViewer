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
#ifndef STRONG_H
#define STRONG_H
#include "src/module/simplemoduleclass.h"
#include "src/module/zefania-strong.h"
class Strong : public SimpleModuleClass
{
public:
    Strong();
    QString loadStrong(QString strongID);
    void loadStrongModule(const int &id, QString path);
    int m_strongModuleID;
    QString m_strongString;
private:
    ZefaniaStrong m_zefStrong;
};

#endif // STRONG_H
