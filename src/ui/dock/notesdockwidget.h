#ifndef NOTESDOCKWIDGET_H
#define NOTESDOCKWIDGET_H

#include <QDockWidget>
#include <QtCore/QStringList>
#include "dockwidget.h"
#include "src/core/verseselection.h"
#include "src/core/urlconverter.h"
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
    void showNote(QListWidgetItem * item);
    void saveNote();
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
public slots:
    void newNote();
    void newNoteWithLink(VerseSelection selection);

    void newMark(VerseSelection selection,QColor color);
    void showNote(const QString &noteID);
    void removeMark(VerseSelection selection);
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
    QStringList m_textNotesID;
    QString m_noteID;
    QMap<QString, QString> currentNoteRef;
    QString notePos2Text(const QString &pos);
};

#endif // NOTESDOCKWIDGET_H
