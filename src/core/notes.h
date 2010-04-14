/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009 Paul Walger
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
#ifndef NOTES_H
#define NOTES_H
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QMap>
#include <QtXml/QDomDocument>
/**
 Notes represents a note system

 @author Paul Walger <metaxy@walger.name>
*/
class Notes : public QObject
{
    Q_OBJECT
signals:
    void saveAll();
    void titleChanged(QString id, QString newTitle);
    void dataChanged(QString id, QString newData);
    void refChanged(QString id, QMap<QString, QString> newRef);
    void noteRemoved(QString id, const QMap<QString, QString>  &ref);
    void noteAdded(QString id);
public:
    Notes();
    void init(const QString &fileName);
    int loadNotes();
    int readNotes();
    int saveNotes();
    void clearAll();
    QString getType(const QString &id);
    QString getTitle(const QString &id);
    QString getData(const QString &id);
    QString getRef(const QString &id, const QString &refID);
    QMap<QString, QString> getRef(const QString &id);
    void setType(const QString &id, const QString &type);
    void setTitle(const QString &id, const QString &title);
    void setData(const QString &id, const QString &data);
    void setRef(const QString &id, const QMap<QString, QString>  &ref);
    QString generateNewID();
    void insertID(const QString &id);
    void removeNote(const QString &id);
    QStringList getIDList();
    bool isLoaded();
    void loadingNewInstance();


private:
    QString m_fileName;
    QStringList notesID;
    QMap<QString, QString> notesData;
    QMap<QString, QString> notesTitle;
    QMap<QString, QMap<QString, QString> > notesRef;
    QMap<QString, QString> notesType;
    QDomDocument doc;
    QString m_version;
    bool m_isLoaded;
    bool m_isInited;
};

#endif // NOTES_H
