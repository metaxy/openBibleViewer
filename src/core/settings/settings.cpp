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
#include "src/core/settings/settings.h"
#include "src/core/dbghelper.h"
#include <QtCore/QUrl>
#include <QtCore/QDir>
#include <QtCore/QCryptographicHash>
Settings::Settings()
{
    homePath = "";
}
Settings::~Settings()
{
    defaultVersification.clear();
}


ModuleSettings * Settings::getModuleSettings(int moduleID) const
{
    if(m_moduleSettings.contains(moduleID)) {
        return m_moduleSettings.value(moduleID);
    } else {
        myWarning() << "no modulesettings found " << m_moduleSettings.keys() << moduleID;
        return NULL;
    }
}
QSharedPointer<Versification> Settings::getV11N(const int moduleID) const
{
    ModuleSettings *set = getModuleSettings(moduleID);
    if(!set)
        return defaultVersification;
    QSharedPointer<Versification> v11n = set->getV11n();
    if(!v11n)
        return defaultVersification;
    return v11n;
}
QString Settings::savableUrl(QString url) const
{
    if(url.startsWith(homePath)) {
        url.replace(homePath, "?obvHomePath?");
        const QByteArray ba = QUrl::toPercentEncoding(url);
        return QString(ba.constData());
    }
    if(url.startsWith(QDir::homePath())) {
        url.replace(QDir::homePath(), "?homePath?");
        const QByteArray ba = QUrl::toPercentEncoding(url);
        return QString(ba.constData());
    }
    return url;
}

QString Settings::recoverUrl(QString url) const
{
    url = QUrl::fromPercentEncoding(url.toLocal8Bit());
    if(url.startsWith("?obvHomePath?")) {
        url.replace("?obvHomePath?", homePath);
        return url;
    }
    if(url.startsWith("?homePath?")) {
        url.replace("?homePath?", QDir::homePath());
        return url;
    }
    return url;
}

QString Settings::hash(const QString &path) const
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(savableUrl(path).toLocal8Bit());
    return QString(hash.result().toHex());
}

QString Settings::v11nFile(const QString &path) const
{
    return homePath + "v11n/" + hash(path) + "/v11n.ini";
}

int Settings::newModuleID() const
{
    int max = 0;
    QMap<int, ModuleSettings *>::const_iterator i;
    for(i = m_moduleSettings.constBegin(); i != m_moduleSettings.constEnd(); ++i) {
        if(i.key() > max) {
            max = i.key();
        }
    }
    return max + 1;

}
