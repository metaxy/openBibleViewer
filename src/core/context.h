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
#ifndef CONTEXT_H
#define CONTEXT_H
#include <QtCore/QSettings>
#include <QtCore/QString>
#include <QtCore/QTranslator>

#include "src/core/basicclass.h"

class Context : public QObject, public BasicClass
{
    Q_OBJECT
public:
    explicit  Context(QObject *parent);
    ~Context();

    void init();

    void setSettings(QSettings *settings);
    void setHomePath(const QString &homePath);
    void setTranslator(QTranslator *my, QTranslator *qt);

    void showWindow();

    void writeSettings();

private:
    void loadDefaultSettings();
    void loadSettings();

private:
    QSettings *m_settingsFile;
    QString m_homeDataPath;
    QTranslator *m_myappTranslator;
    QTranslator *m_qtTranslator;
    QSettings *m_sessionFile;

    bool m_firstStart;
};

#endif // CONTEXT_H
