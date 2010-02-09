#ifndef NOTESEDITOR_H
#define NOTESEDITOR_H

#include <QDialog>
#include "src/core/settings.h"
#include "src/core/notes.h"
#include "src/module/modulemanager.h"
#include "src/core/verseselection.h"
namespace Ui
{
class NotesEditor;
}

class NotesEditor : public QDialog
{
    Q_OBJECT
private slots:
    void showNote(QListWidgetItem * item);

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

    void newMark(VerseSelection selection, QColor color);
    void showNote(const QString &noteID);
    void removeMark(VerseSelection selection);
    void saveNote();
signals:
    void get(QString url);
    void reloadChapter();
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
    QStringList m_textNotesID;
    QString m_noteID;
    QMap<QString, QString> currentNoteRef;
};

#endif // NOTESEDITOR_H
