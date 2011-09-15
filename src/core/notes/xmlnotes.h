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
#ifndef XMLNOTES_H
#define XMLNOTES_H
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QMap>
#include <QtXml/QDomDocument>
#include "src/core/search/searchresult.h"
#include "src/core/search/searchquery.h"
#include "src/core/notes/notes.h"
/**
 Notes represents a note system
*/
class XmlNotes : public Notes
{
    Q_OBJECT
public:
    XmlNotes();
    /**
      Init the notes;
      \param fileName The path to the notes xml file.
      */
    void init(const QString &fileName);
    /**
      Load notes data from an xml-file
      */
    int loadNotes();
    /*!
      Pharse note data from the xml-file
      */
    int readNotes();
    /*!
      Save the data in a xml file
      */
    int saveNotes();
    /**
      * Clear all data
      */
    void clearAll();
    /*!
      Get the type of a note(eg. text, mark, tag)
      \param id Note ID
      */
    QString getType(const QString &id) const;
    /*!
      Get the note title
      \param id Note ID
      */
    QString getTitle(const QString &id) const;
    /*!
      Get the note data
      \param id Note ID
      */
    QString getData(const QString &id) const;
    /*!
      Get the note reference
      \param id note ID
      \param refID reference ID
      */
    QString getRef(const QString &id, const QString &refID) const;
    /**
      Get all references of a note
      \param id note ID
      */
    QMap<QString, QString> getRef(const QString &id) const;
    /**
      Set note type
      \param id note ID
      \param type note type
      */
    void setType(const QString &id, const QString &type);
    /**
      Set note title
      \param id note ID
      \param title note title
      */
    void setTitle(const QString &id, const QString &title);
    /**
      Set note data
      \param id note ID
      \param data note data
      */
    void setData(const QString &id, const QString &data);
    /**
      Set note reference
      \param id note ID
      \param ref note reference
      */
    void setRef(const QString &id, const QMap<QString, QString>  &ref);
    /**
      Set note reference
      \param id The note ID.
      \param key The key.
      \param value The new value.
      */
    void setRef(const QString &id, const QString &key, const QString &value);
    /**
        Generate a new note ID, without colliding with other IDs
     */
    QString generateNewID() const;
    /*!
     Insert a new ID, and to add a new note you have to set title for this id
     \param id the new note id
     */
    void insertID(const QString &id);
    /*!
     Remove Note
     \param id note id
      */
    void removeNote(const QString &id);

    QStringList getIDList() const;
    /**
      Get all notes IDs with a given type.
      \param type The needed type.
      */
    QStringList getIDList(const QString &type) const;
    /**
      Returns true if in this instance the notes are loaded.
      */
    bool isLoaded() const;
    /**
      Emits a signal to save data in all other instances.
      */
    void loadingNewInstance();
    /**
      Search in Notes.
      \param query The Search Query
      \param res The Results.
      */
    void search(SearchQuery query, SearchResult *result) const;
    Notes::NotesTextType type() const;

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

    QString m_oldVersion;
};

#endif // XMLNOTES_H
