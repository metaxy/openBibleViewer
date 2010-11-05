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
#ifndef BIBLEDISPLAY_H
#define BIBLEDISPLAY_H
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>
class BibleDisplay : public QObject
{
    Q_OBJECT
public:
    BibleDisplay();
    /**
     * Set the Displaytext to text
     */
    void setHtml(const QString &text);
    /**
     * emits get(const QString &url)
     */
    void emitGet(const QString &url);
Q_SIGNALS:
    /**
     * Emits signal if newHtml was called
     */
    void newHtml(const QString &html);
    /**
     * Is connect to the get in the Interface
     */
    void get(const QString &url);
    /**
     * Is connect to the get in the Interface
     */
    void get(const QUrl &url);
private:
    //QString m_text;
};

#endif // BIBLEDISPLAY_H
