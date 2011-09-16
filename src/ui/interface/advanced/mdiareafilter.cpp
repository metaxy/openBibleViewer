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
#include "mdiareafilter.h"
MdiAreaFilter::MdiAreaFilter(QObject *parent) :
    QObject(parent)
{
}
bool MdiAreaFilter::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::Resize) {
        emit resized();
        return QObject::eventFilter(obj, event);
    }
    return QObject::eventFilter(obj, event);
}
