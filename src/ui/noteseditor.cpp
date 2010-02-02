#include "noteseditor.h"
#include "ui_noteseditor.h"

NotesEditor::NotesEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NotesEditor)
{
    ui->setupUi(this);
}

NotesEditor::~NotesEditor()
{
    delete ui;
}
void NotesEditor::setSettings(Settings *settings)
{
    m_settings = settings;
}
void NotesEditor::setNotes(Notes *notes)
{
    m_notes = notes;
}
void NotesEditor::init()
{
}

void NotesEditor::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
