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
#ifndef NOTESEDITOR_H
#define NOTESEDITOR_H

#include <QMainWindow>
#include "src/core/settings.h"
#include "src/core/notes.h"
#include "src/module/modulemanager.h"
#include "src/core/verseselection.h"
#include "src/core/simplenotes.h"
namespace Ui
{
class NotesEditor;
}

class NotesEditor : public QMainWindow
{
    Q_OBJECT
private slots:
    /*void showNote(QListWidgetItem * item);

    void notesContextMenu();
    void removeNote();
    void reloadNotes();
    void copyNote();
    void editNoteLink();
    void noteSetTextBold();
    void noteSetTextItalic();
    void noteSetTextUnderline();
    void noteSetTextColor();
    void noteUndo();
    void noteRedo();
    void updateNote(QString pos);
    void fastSave();

    void changeData(QString id, QString data);
    void changeTitle(QString id, QString title);*/
public slots:
   /* void newNote();
    void newNoteWithLink(VerseSelection selection);
    void showNote(const QString &noteID);
    void saveNote();*/
signals:
    /*void get(QString url);
    void reloadChapter();*/
public:
    explicit NotesEditor(QWidget *parent = 0);
    ~NotesEditor();
    void setSettings(Settings *settings);
    void setNotes(Notes *notes);
    void setModuleManager(ModuleManager *moduleManager);
    void init();
protected:
    void changeEvent(QEvent *e);

private:
    Ui::NotesEditor *ui;
    Settings *m_settings;
    Notes *m_notes;
    ModuleManager *m_moduleManager;
    /*QStringList m_textNotesID;
    QString m_noteID;
    QMap<QString, QString> currentNoteRef;
    void setTitle(QString title);
    void setData(QString data);
    void setRef(QMap<QString, QString> ref);*/
    SimpleNotes *m_simpleNotes;

};

#endif // NOTESEDITOR_H
