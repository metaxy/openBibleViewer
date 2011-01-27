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
    defaultVersification = 0;
}
Settings::~Settings()
{
    //todo: memory leak
    /*if(defaultVersification != 0)
        delete defaultVersification;*/
}


ModuleSettings * Settings::getModuleSettings(int moduleID) const
{
    if(m_moduleSettings.contains(moduleID)) {
        return m_moduleSettings.value(moduleID);
    } else {
        return NULL;
    }
}
/**
  Replace homePath and settingsPath to make it more portable.
  */
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
/**
  Recover Urls which were saved with savableUrl().
  */
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
/**
 * Generate a hash from a string
 */
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
