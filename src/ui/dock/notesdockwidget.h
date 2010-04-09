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
#ifndef NOTESDOCKWIDGET_H
#define NOTESDOCKWIDGET_H

#include <QDockWidget>
#include <QtCore/QStringList>
#include "dockwidget.h"
#include "src/core/verseselection.h"
#include "src/core/urlconverter.h"
#include "src/core/simplenotes.h"
namespace Ui
{
class NotesDockWidget;
}
/*!
 NotesDockWidget represents a dock widget to view and edit your notes

*/
class NotesDockWidget : public DockWidget
{
    Q_OBJECT
private slots:
    /*void showNote(QListWidgetItem * item);

    void notesContextMenu();
    void removeNote();
    void reloadNotes();
    void copyNote();
    void editNoteLink();*/
    void noteSetTextBold();
    void noteSetTextItalic();
    void noteSetTextUnderline();
    void noteSetTextColor();
    void noteUndo();
    void noteRedo();

    void changeRef(QString id, QMap<QString, QString> ref);
    /*void updateNote(QString pos);*/
public slots:
    void newNote();
    void newNoteWithLink(VerseSelection selection);

    void newMark(VerseSelection selection, QColor color);
    void showNote(const QString &noteID);
    void removeMark(VerseSelection selection);
    void saveNote();
public:
    NotesDockWidget(QWidget *parent = 0);
    ~NotesDockWidget();
    void init();

protected:
    void changeEvent(QEvent *e);
signals:
    void get(QString url);
    void reloadChapter();
private:
    Ui::NotesDockWidget *ui;
    SimpleNotes *m_simpleNotes;
    /*  QStringList m_textNotesID;
      QString m_noteID;
      QMap<QString, QString> currentNoteRef;*/

};

#endif // NOTESDOCKWIDGET_H
