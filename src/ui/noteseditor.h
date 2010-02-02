#ifndef NOTESEDITOR_H
#define NOTESEDITOR_H

#include <QDialog>
#include "src/core/settings.h"
#include "src/core/notes.h"
namespace Ui {
    class NotesEditor;
}

class NotesEditor : public QDialog {
    Q_OBJECT
public:
    explicit NotesEditor(QWidget *parent = 0);
    ~NotesEditor();
    void setSettings(Settings *settings);
    void setNotes(Notes *notes);
    void init();
protected:
    void changeEvent(QEvent *e);

private:
    Ui::NotesEditor *ui;
    Settings *m_settings;
    Notes *m_notes;
};

#endif // NOTESEDITOR_H
