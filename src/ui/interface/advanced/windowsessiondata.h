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
#ifndef WINDOWSESSIONDATA_H
#define WINDOWSESSIONDATA_H
#include <QtCore/QString>
#include <QtCore/QRect>
#include <QtCore/QVariant>
#include "src/core/settings/settings.h"
#include "src/core/dbghelper.h"
class WindowSessionData
{
public:
    WindowSessionData();

    void clear();
    void setSettings(Settings *settings);
    void setWindowID(const int &windowID);

    void setUrl(const QStringList &url);
    void setUrl(const QStringList &url, const int &windowID);
    void setBiblePoint(const QList<QPoint> &url);
    void setBiblePoint(const QList<QPoint> &url, const int &windowID);

    void setScrollPosition(const QPoint &point);
    void setScrollPosition(const QPoint &point, const int &windowID);

    void setZoom(qreal zoom);
    void setZoom(qreal zoom, const int &windowID);

    void setGeo(const QRect &rect);
    void setGeo(const QRect &rect, const int &windowID);

    void read();
    void write();

    QStringList url(const int &windowID);
    QStringList url();
    QList<QPoint> biblePoint(const int &windowID);
    QList<QPoint> biblePoint();

    QPoint scrollPosition(const int &windowID);
    QPoint scrollPosition();

    qreal zoom(const int &windowID);
    qreal zoom();

    QRect geo(const int &windowID);
    QRect geo();

    int size();

private:
    QMap<int, QVariant> m_url;
    QMap<int, QVariant> m_scrollPosition;
    QMap<int, QVariant> m_zoom;
    QMap<int, QVariant> m_geo;
    QMap<int, QVariant> m_biblePoints;

    Settings *m_settings;
    int m_windowID;

    QMap<int, QVariant> getProp(const int &i) const;
    QString getPropName(const int &i) const;
    QMap<int, QVariant>* propToPointer(const int &i);
    int propSize() const;
};

#endif // WINDOWSESSIONDATA_H
