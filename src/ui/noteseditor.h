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

class NotesEditor : public QMainWindow, public BasicClass
{
    Q_OBJECT
private slots:
    void noteSetTextBold();
    void noteSetTextItalic();
    void noteSetTextUnderline();
    void noteSetTextColor();
    void noteUndo();
    void noteRedo();

public slots:

signals:

public:
    explicit NotesEditor(QWidget *parent = 0);
    ~NotesEditor();

    void init();
protected:
    void changeEvent(QEvent *e);

private:
    Ui::NotesEditor *ui;
    SimpleNotes *m_simpleNotes;

};

#endif // NOTESEDITOR_H
