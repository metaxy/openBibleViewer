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
#ifndef NOTES_H
#define NOTES_H
#include <QtXml/QDomDocument>
#include "src/core/search/searchresult.h"
/**
 Notes represents a note system
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
    Notes() {}
    enum NotesTextType {
        HtmlNotes,
        TextNotes
    };
    virtual Notes::NotesTextType type() const = 0;
    /**
      Init the notes;
      \param fileName The path to the notes file or folder.
      */
    virtual void init(const QString &fileName) = 0;
    /**
      Load notes data.
      */
    virtual int loadNotes() = 0;
    /**
      Pharse note data.
      */
    virtual int readNotes() = 0;
    /**
      Save the data.
      */
    virtual int saveNotes() = 0;
    /**
      * Clear all data
      */
    virtual void clearAll() = 0;
    /*!
      Get the type of a note(eg. text, mark, tag)
      \param id Note ID
      */
    virtual QString getType(const QString &id) const = 0;
    /*!
      Get the note title
      \param id Note ID
      */
    virtual QString getTitle(const QString &id) const = 0;
    /*!
      Get the note data
      \param id Note ID
      */
    virtual QString getData(const QString &id) const = 0;
    /*!
      Get the note reference
      \param id note ID
      \param refID reference ID
      */
    virtual QString getRef(const QString &id, const QString &refID) const = 0;
    /**
      Get all references of a note
      \param id note ID
      */
    virtual QMap<QString, QString> getRef(const QString &id) const = 0;
    /**
      Set note type
      \param id note ID
      \param type note type
      */
    virtual void setType(const QString &id, const QString &type) = 0;
    /**
      Set note title
      \param id note ID
      \param title note title
      */
    virtual void setTitle(const QString &id, const QString &title) = 0;
    /**
      Set note data
      \param id note ID
      \param data note data
      */
    virtual void setData(const QString &id, const QString &data) = 0;
    /**
      Set note reference
      \param id note ID
      \param ref note reference
      */
    virtual void setRef(const QString &id, const QMap<QString, QString>  &ref) = 0;
    /**
      Set note reference
      \param id The note ID.
      \param key The key.
      \param value The new value.
      */
    virtual void setRef(const QString &id, const QString &key, const QString &value) = 0;
    /**
        Generate a new note ID, without colliding with other IDs
     */
    virtual QString generateNewID() const = 0;
    /*!
     Insert a new ID, and to add a new note you have to set title for this id
     \param id the new note id
     */
    virtual void insertID(const QString &id) = 0;
    /*!
     Remove Note
     \param id note id
      */
    virtual void removeNote(const QString &id) = 0;

    virtual  QStringList getIDList() const = 0;
    /**
      Get all notes IDs with a given type.
      \param type The needed type.
      */
    virtual QStringList getIDList(const QString &type) const = 0;
    /**
      Returns true if in this instance the notes are loaded.
      */
    virtual bool isLoaded() const = 0;
    /**
      Emits a signal to save data in all other instances.
      */
    virtual void loadingNewInstance() = 0;
    /**
      Search in Notes.
      \param query The Search Query
      \param res The Results.
      */
    virtual void search(SearchQuery query, SearchResult *result) const = 0;


};

#endif // NOTES_H
