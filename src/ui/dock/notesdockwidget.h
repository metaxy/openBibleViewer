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
#ifndef NOTESDOCKWIDGET_H
#define NOTESDOCKWIDGET_H

#include <QDockWidget>
#include <QStringList>
#include "dockwidget.h"
#include "src/core/verseselection.h"
#include "src/core/link/urlconverter.h"
#include "src/core/notes/simplenotes.h"
namespace Ui
{
class NotesDockWidget;
}
/**
  NotesDockWidget represents a dock widget to view and edit your notes.
 */
class NotesDockWidget : public DockWidget
{
    Q_OBJECT
private slots:
    void noteSetTextBold();
    void noteSetTextItalic();
    void noteSetTextUnderline();
    void noteSetTextColor();
    void noteUndo();
    void noteRedo();

    void changeRef(QString id, QMap<QString, QString> ref);
    void removeNote(QString id , QMap<QString, QString> ref);
public slots:
    void newNote();
    void newNoteWithLink(VerseSelection selection, QSharedPointer<Versification> v11n);

    void newMark(VerseSelection selection, QColor color, QSharedPointer<Versification> v11n);
    void newStyleMark(VerseSelection, QString style, QSharedPointer<Versification> v11n);
    void showNote(const QString &noteID);
    void removeMark(VerseSelection selection, QSharedPointer<Versification> v11n);
    void saveNote();
public:
    NotesDockWidget(QWidget *parent = 0);
    ~NotesDockWidget();
    void init();

protected:
    void changeEvent(QEvent *e);
private:
    Ui::NotesDockWidget *ui;
    SimpleNotes *m_simpleNotes;
};

#endif // NOTESDOCKWIDGET_H
