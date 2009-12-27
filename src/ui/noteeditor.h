#ifndef NOTEEDITOR_H
#define NOTEEDITOR_H

#include <QDialog>

namespace Ui {
    class NoteEditor;
}

class NoteEditor : public QDialog {
    Q_OBJECT
public:
    NoteEditor(QWidget *parent = 0);
    ~NoteEditor();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::NoteEditor *ui;
};

#endif // NOTEEDITOR_H
