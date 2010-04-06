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
#include "noteseditor.h"
#include "ui_noteseditor.h"
#include "src/core/dbghelper.h"
#include <QtGui/QClipboard>
#include <QtGui/QMenu>
#include <QtGui/QColorDialog>
#include <QtGui/QToolBar>
#include "src/ui/dialog/biblepassagedialog.h"
#include "src/core/urlconverter.h"
NotesEditor::NotesEditor(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::NotesEditor)
{
    ui->setupUi(this);
    m_simpleNotes = new SimpleNotes();

    QToolBar *bar = new QToolBar(this->parentWidget());
    bar->setIconSize(QSize(32, 32));
#if QT_VERSION >= 0x040600
    bar->setToolButtonStyle(Qt::ToolButtonFollowStyle);
#else
    bar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
#endif
    bar->setObjectName("toolBar");
    bar->setWindowTitle(tr("ToolBar"));

    QAction *actionNew = new QAction(QIcon(":/icons/32x32/list-add.png"), tr("Add note"), bar);
    connect(actionNew, SIGNAL(triggered()), m_simpleNotes, SLOT(newNote()));
    QAction *actionSave = new QAction(QIcon(":/icons/32x32/document-save.png"), tr("Save"), bar);
    connect(actionSave, SIGNAL(triggered()), m_simpleNotes, SLOT(save()));


    bar->addAction(actionNew);
    bar->addAction(actionSave);
    addToolBar(bar);

}

NotesEditor::~NotesEditor()
{
    delete ui;
}
void NotesEditor::init()
{


    m_simpleNotes->setModuleManager(m_moduleManager);
    m_simpleNotes->setNotes(m_notes);
    m_simpleNotes->setSettings(m_settings);
    m_simpleNotes->setBibleDisplay(m_bibleDisplay);

    m_simpleNotes->setDataWidget(ui->textEdit_note);
    m_simpleNotes->setViewWidget(ui->treeView);
    m_simpleNotes->setTitleWidget(ui->lineEdit_noteTitle);
    m_simpleNotes->setLinkButtonWidget(ui->pushButton_editNoteLink);
    m_simpleNotes->setLinkWidget(ui->label_noteLink);
    m_simpleNotes->init();

    connect(ui->toolButton_noteBold, SIGNAL(clicked()), this, SLOT(noteSetTextBold()));
    connect(ui->toolButton_noteItalic, SIGNAL(clicked()), this, SLOT(noteSetTextItalic()));
    connect(ui->toolButton_noteUnderline, SIGNAL(clicked()), this, SLOT(noteSetTextUnderline()));
    connect(ui->toolButton_noteFontColor, SIGNAL(clicked()), this, SLOT(noteSetTextColor()));
    connect(ui->toolButton_noteUndo, SIGNAL(clicked()), this, SLOT(noteUndo()));
    connect(ui->toolButton_noteRedo, SIGNAL(clicked()), this, SLOT(noteRedo()));

    connect(ui->textEdit_note, SIGNAL(undoAvailable(bool)), ui->toolButton_noteUndo, SLOT(setEnabled(bool)));
    connect(ui->textEdit_note, SIGNAL(redoAvailable(bool)), ui->toolButton_noteRedo, SLOT(setEnabled(bool)));
   // connect(ui->textEdit_note, SIGNAL(undoAvailable(bool)), m_simpleNotes, SLOT(fastSave()));
  //  connect(ui->textEdit_note, SIGNAL(redoAvailable(bool)), m_simpleNotes, SLOT(fastSave()));

}

void NotesEditor::noteSetTextBold(void)
{
    if (ui->textEdit_note->fontWeight() == QFont::Bold) {
        ui->textEdit_note->setFontWeight(QFont::Normal);
    } else {
        ui->textEdit_note->setFontWeight(QFont::Bold);
    }
}
void NotesEditor::noteSetTextItalic(void)
{
    if (ui->textEdit_note->fontItalic()) {
        ui->textEdit_note->setFontItalic(false);
    } else {
        ui->textEdit_note->setFontItalic(true);
    }
}
void NotesEditor::noteSetTextUnderline(void)
{
    if (ui->textEdit_note->fontUnderline()) {
        ui->textEdit_note->setFontUnderline(false);
    } else {
        ui->textEdit_note->setFontUnderline(true);
    }
}
void NotesEditor::noteSetTextColor(void)
{
    QColor color = QColorDialog::getColor(Qt::green, this);
    if (color.isValid()) {
        ui->textEdit_note->setTextColor(color);
    }
}
void NotesEditor::noteUndo()
{
    ui->textEdit_note->undo();
}
void NotesEditor::noteRedo()
{
    ui->textEdit_note->redo();
}


void NotesEditor::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
