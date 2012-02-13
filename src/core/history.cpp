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
#include "src/core/history.h"
#include "src/core/dbghelper.h"
History::History()
{
    m_goF = false;
    m_lock = false;
}
/*!
    Return the next url in the history.
  */
QString History::forward()
{
    if(m_lock) return "";
    if(!m_forwardItems.isEmpty()) {
        m_goF = true;
        return m_forwardItems.takeLast();
    } else
        return QString();
}
/*!
    Return the previous url in the history.
  */
QString History::backward()
{
    if(m_lock) return "";
    if(m_backwardItems.size() > 1) {
        m_goF = true;
        m_forwardItems.append(m_backwardItems.takeLast());
        return m_backwardItems.takeLast();
    } else
        return QString();
}
/*!
    Check if a next url is available.
  */
bool History::forwardAvailable() const
{
    return !m_forwardItems.isEmpty();
}
/*!
    Check if a previous url is available.
  */
bool History::backwardAvailable() const
{
    return m_backwardItems.size() > 1;
}
/*!
    Add a new url to the history.
    \param url the new url
  */
void History::setCurrent(const QString &url)
{
    if(m_lock) return;
    if(m_goF == true) {
        m_goF = false;
    } else {
        m_forwardItems.clear();
    }
    m_backwardItems.append(url);
}

void History::setData1(const QVariant &d)
{
    m_forwardItems = d.toStringList();

}
void History::setData2(const QVariant &d)
{
    m_backwardItems = d.toStringList();

}
void History::setData3(const QVariant &d)
{
    m_goF = d.toBool();

}
QVariant History::data1() const
{
    return QVariant(m_forwardItems);
}
QVariant History::data2() const
{
    return QVariant(m_backwardItems);
}
QVariant History::data3() const
{
    return QVariant(m_goF);
}
void History::lock()
{
    m_lock = true;
}
void History::unlock()
{
    m_lock = false;
}
