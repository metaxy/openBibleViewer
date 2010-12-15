#ifndef WINDOWSESSIONDATA_H
#define WINDOWSESSIONDATA_H
#include <QtCore/QString>
#include <QtCore/QRect>
#include <QtCore/QVariant>
#include "src/core/settings/settings.h"
class WindowSessionData
{
public:
    WindowSessionData();

    void clear();
    void setSettings(Settings *settings);
    void setWindowID(const int &windowID);

    void setUrl(const QString &url);
    void setUrl(const QString &url, const int &windowID);

    void setScrollPosition(const QPoint &point);
    void setScrollPosition(const QPoint &point, const int &windowID);

    void setZoom(qreal zoom);
    void setZoom(qreal zoom, const int &windowID);

    void setGeo(const QRect &rect);
    void setGeo(const QRect &rect, const int &windowID);

    void read();
    void write();

    QString url(const int &windowID);
    QString url();
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

    Settings *m_settings;
    int m_windowID;

    QMap<int, QVariant> getProp(const int &i) const;
    QString getPropName(const int &i) const;
    QMap<int, QVariant>* propToPointer(const int &i);
    int propSize() const;
};

#endif // WINDOWSESSIONDATA_H
