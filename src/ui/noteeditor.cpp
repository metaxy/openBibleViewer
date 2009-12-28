#include "noteeditor.h"
#include "ui_noteeditor.h"

NoteEditor::NoteEditor(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::NoteEditor)
{
    ui->setupUi(this);
}

NoteEditor::~NoteEditor()
{
    delete ui;
}

void NoteEditor::changeEvent(QEvent *e)
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
