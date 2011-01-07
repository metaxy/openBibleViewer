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
#include "windowsessiondata.h"
#include "src/core/dbghelper.h"
WindowSessionData::WindowSessionData()
{
}
QMap<int, QVariant> WindowSessionData::getProp(const int &i) const
{
    if(i == 0)
        return m_geo;
    else if(i == 1)
        return m_scrollPosition;
    else if(i == 2)
        return m_url;
    else if(i == 3)
        return m_zoom;
    else if(i == 4)
        return m_biblePoints;
    else
        return QMap<int, QVariant>();
}
QString WindowSessionData::getPropName(const int &i) const
{
    if(i == 0)
        return "windowGeo";
    else if(i == 1)
        return "scrollPos";
    else if(i == 2)
        return "windowUrls";
    else if(i == 3)
        return "zoom";
    else if(i == 4)
        return "biblePoint";
    else
        return "";
}
QMap<int, QVariant>* WindowSessionData::propToPointer(const int &i)
{
    if(i == 0)
        return &m_geo;
    else if(i == 1)
        return &m_scrollPosition;
    else if(i == 2)
        return &m_url;
    else if(i == 3)
        return &m_zoom;
    else if(i == 4)
        return &m_biblePoints;
    else
        return 0;
}
int WindowSessionData::propSize() const
{
    return 5;
}
void WindowSessionData::clear()
{
    m_url.clear();
    m_scrollPosition.clear();
    m_geo.clear();
    m_zoom.clear();
}

void WindowSessionData::read()
{
    //DEBUG_FUNC_NAME
    for(int i = 0; i < propSize(); ++i) {
        QList<QVariant> list = m_settings->session.getData(getPropName(i)).toList();
        //myDebug() << "name = " << getPropName(i) << "list = " << list;
        QMap<int, QVariant> *d = propToPointer(i);
        for(int j = 0; j < list.size(); j++) {
            d->insert(j, list.at(j));
        }
    }
}

void WindowSessionData::write()
{
    for(int i = 0; i < propSize(); ++i) {
        QMap<int, QVariant> d = getProp(i);
        QList<QVariant> list;
        foreach(QVariant v, d) {
            list << v;
        }
        //myDebug() << "name = " << getPropName(i) << "list = " << list;
        m_settings->session.setData(getPropName(i), list);
    }
}

void WindowSessionData::setSettings(Settings *settings)
{
    m_settings = settings;
}

void WindowSessionData::setWindowID(const int &windowID)
{
    m_windowID = windowID;
}

void WindowSessionData::setUrl(const QStringList &url)
{
    setUrl(url, m_windowID);
}

void WindowSessionData::setUrl(const QStringList &url, const int &windowID)
{
    m_url.insert(windowID, url);
}
void WindowSessionData::setBiblePoint(const QList<QPoint> &p)
{
    setBiblePoint(p, m_windowID);
}

void WindowSessionData::setBiblePoint(const QList<QPoint> &p, const int &windowID)
{
    QStringList save;
    foreach(const QPoint &point, p) {
        save << QString::number(point.x()) + ";" + QString::number(point.y());
    }
    m_biblePoints.insert(windowID, save);
}

void WindowSessionData::setScrollPosition(const QPoint &point)
{
    setScrollPosition(point, m_windowID);
}

void WindowSessionData::setScrollPosition(const QPoint &point, const int &windowID)
{
    m_scrollPosition.insert(windowID, point);
}

void WindowSessionData::setZoom(qreal zoom)
{
    setZoom(zoom, m_windowID);
}

void WindowSessionData::setZoom(qreal zoom, const int &windowID)
{
    m_zoom.insert(windowID, zoom);
}

void WindowSessionData::setGeo(const QRect &rect)
{
    setGeo(rect, m_windowID);
}

void WindowSessionData::setGeo(const QRect &rect, const int &windowID)
{
    m_geo.insert(windowID, rect);
}


QStringList WindowSessionData::url(const int &windowID)
{
    return m_url.value(windowID).toStringList();
}

QStringList WindowSessionData::url()
{
    return url(m_windowID);
}
QList<QPoint> WindowSessionData::biblePoint(const int &windowID)
{
    const QStringList l = m_biblePoints.value(windowID).toStringList();
    QList<QPoint> ret;
    foreach(const QString &s, l) {
        const QStringList split = s.split(";");
        ret << QPoint(split.first().toInt(), split.last().toInt());
    }
    return ret;
}

QList<QPoint> WindowSessionData::biblePoint()
{
    return biblePoint(m_windowID);
}
QPoint WindowSessionData::scrollPosition(const int &windowID)
{
    return m_scrollPosition.value(windowID).toPoint();
}

QPoint WindowSessionData::scrollPosition()
{
    return scrollPosition(m_windowID);
}

qreal WindowSessionData::zoom(const int &windowID)
{
    return m_zoom.value(windowID).toReal();
}

qreal WindowSessionData::zoom()
{
    return zoom(m_windowID);
}

QRect WindowSessionData::geo(const int &windowID)
{
    return m_geo.value(windowID).toRect();
}

QRect WindowSessionData::geo()
{
    return geo(m_windowID);
}
int WindowSessionData::size()
{
    return m_url.size();
}
