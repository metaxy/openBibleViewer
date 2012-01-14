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
#include "strongurl.h"
#include <QtCore/QStringList>
#include "src/module/moduletools.h"
StrongUrl::StrongUrl()
{
}

void StrongUrl::addStrongNumber(const int number)
{
    m_numbers.append(number);
}

void StrongUrl::setStrongNumbers(const QList<int> &numbers)
{
    m_numbers = numbers;
}

void StrongUrl::setPrefix(StrongPrefix prefix)
{
    m_prefix = prefix;
}
//todo: move out to an extra class
bool StrongUrl::fromText(QString text)
{
    if(text.isEmpty())
        return false;
    m_numbers.clear();
    text.trimmed();
    if(text.at(0).toLower() == 'g') {
        m_prefix = StrongUrl::G;
    } else if(text.at(0).toLower() == 'h') {
        m_prefix = StrongUrl::H;
    } else {
        return false;
    }
    text.remove(0, 1);
    if(text.contains(" ")) {
        const QStringList parts = text.split(" ");
        foreach(const QString & part, parts) {
            m_numbers.append(part.toInt());
        }
    } else if(text.contains(",")) {
        const QStringList parts = text.split(",");
        foreach(const QString & part, parts) {
            m_numbers.append(part.toInt());
        }
    } else {
        m_numbers.append(text.toInt());
    }
    return true;
}

bool StrongUrl::fromString(QString strong)
{
    //exp:
    //strong:/G1
    //strong:/H10
    //strong:/H10,20
    m_numbers.clear();
    if(!strong.startsWith(ModuleTools::strongScheme))
        return false;
    strong.remove(0, ModuleTools::strongScheme.size());
    strong.trimmed();
    if(strong.at(0).toLower() == 'g') {
        m_prefix = StrongUrl::G;
    } else if(strong.at(0).toLower() == 'h') {
        m_prefix = StrongUrl::H;
    } else {
        return false;
    }
    strong.remove(0, 1);
    if(strong.contains(" ")) {
        const QStringList parts = strong.split(" ");
        foreach(const QString & part, parts) {
            m_numbers.append(part.toInt());
        }
    } else if(strong.contains(",")) {
        const QStringList parts = strong.split(",");
        foreach(const QString & part, parts) {
            m_numbers.append(part.toInt());
        }
    }else {
        m_numbers.append(strong.toInt());
    }
    return true;
}

QString StrongUrl::toString()
{
    QString ret;
    ret = ModuleTools::strongScheme;
    if(m_prefix == StrongUrl::G) {
        ret += "G";
    } else {
        ret += "H";
    }
    bool beginn = true;
    foreach(const int & i, m_numbers) {
        if(!beginn) {
            ret += ",";
        } else {
            beginn = false;
        }
        ret += QString::number(i);
    }
    return ret;
}
QString StrongUrl::toText()
{
    QString ret;
    if(m_prefix == StrongUrl::G) {
        ret += "G";
    } else {
        ret += "H";
    }
    bool beginn = true;
    foreach(const int & i, m_numbers) {
        if(!beginn) {
            ret += " ";
        } else {
            beginn = false;
        }
        ret += QString::number(i);
    }
    return ret;
}
QString StrongUrl::toKey()
{
    QString ret;
    QString pre;
    if(m_prefix == StrongUrl::G) {
        pre += "G";
    } else {
        pre += "H";
    }
    bool beginn = true;
    foreach(const int & i, m_numbers) {
        if(!beginn) {
            ret += " ";
        } else {
            beginn = false;
        }
        ret += pre + QString::number(i);
    }
    return ret;
}
QList<int> StrongUrl::numbers() const
{
    return m_numbers;
}
StrongUrl::StrongPrefix StrongUrl::prefix() const
{
    return m_prefix;
}
