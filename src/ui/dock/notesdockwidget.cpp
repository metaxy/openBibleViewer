/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger
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
#include "notesdockwidget.h"
#include "ui_notesdockwidget.h"
#include "src/core/dbghelper.h"
#include "src/ui/dialog/biblepassagedialog.h"
#include <QtGui/QClipboard>
#include <QtGui/QMenu>
#include <QtGui/QColorDialog>
#include <QtGui/QMessageBox>
#include "src/core/obvcore.h"
NotesDockWidget::NotesDockWidget(QWidget *parent) :
    DockWidget(parent),
    ui(new Ui::NotesDockWidget)
{
    ui->setupUi(this);
    m_simpleNotes = new SimpleNotes();
}

NotesDockWidget::~NotesDockWidget()
{
    delete m_simpleNotes;
    delete ui;
}
void NotesDockWidget::init()
{
    setAll(m_simpleNotes);


    m_simpleNotes->setDataWidget(ui->textBrowser);
    m_simpleNotes->setViewWidget(ui->treeView);
    m_simpleNotes->setTitleWidget(ui->lineEdit_noteTitle);
    m_simpleNotes->setLinkButtonWidget(ui->pushButton_editNoteLink);
    m_simpleNotes->setLinkWidget(ui->label_noteLink);
    m_simpleNotes->init();


    connect(ui->pushButton_note_save, SIGNAL(clicked()), this, SLOT(saveNote()));
    connect(ui->toolButton_noteBold, SIGNAL(clicked()), this, SLOT(noteSetTextBold()));
    connect(ui->toolButton_noteItalic, SIGNAL(clicked()), this, SLOT(noteSetTextItalic()));
    connect(ui->toolButton_noteUnderline, SIGNAL(clicked()), this, SLOT(noteSetTextUnderline()));
    connect(ui->toolButton_noteColor, SIGNAL(clicked()), this, SLOT(noteSetTextColor()));
    connect(ui->toolButton_noteUndo, SIGNAL(clicked()), this, SLOT(noteUndo()));
    connect(ui->toolButton_noteRedo, SIGNAL(clicked()), this, SLOT(noteRedo()));

    connect(ui->textBrowser, SIGNAL(undoAvailable(bool)), ui->toolButton_noteUndo, SLOT(setEnabled(bool)));
    connect(ui->textBrowser, SIGNAL(redoAvailable(bool)), ui->toolButton_noteRedo, SLOT(setEnabled(bool)));
    connect(ui->textBrowser, SIGNAL(anchorClicked(QUrl)), m_actions, SLOT(get(QUrl)));

    connect(m_notes, SIGNAL(refChanged(QString, QMap<QString, QString>)), this, SLOT(changeRef(QString, QMap<QString, QString>)));
    connect(m_notes, SIGNAL(noteRemoved(QString, QMap<QString, QString>)), this, SLOT(removeNote(QString, QMap<QString, QString>)));
}
void NotesDockWidget::changeRef(QString id, QMap<QString, QString> ref)
{
    Q_UNUSED(id);
    const QString link = ref.value("link");

    VerseUrl url;
    url.fromString(link);

    UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::InterfaceUrl, url);
    urlConverter.setSettings(m_settings);
    urlConverter.setModuleMap(m_moduleManager->m_moduleMap.data());
    VerseUrl newUrl = urlConverter.convert();
    if(newUrl.isValid()) {
        m_actions->reloadIf(newUrl);
    }

}
void NotesDockWidget::removeNote(QString id, QMap<QString, QString>ref)
{
    Q_UNUSED(id);
    const QString link = ref.value("link");

    VerseUrl url;
    url.fromString(link);

    UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::InterfaceUrl, url);
    urlConverter.setSettings(m_settings);
    urlConverter.setModuleMap(m_moduleManager->m_moduleMap.data());
    const VerseUrl newUrl = urlConverter.convert();
    if(newUrl.isValid()) {
        m_actions->reloadIf(newUrl);
    }
}

void NotesDockWidget::showNote(const QString &noteID)
{
    m_simpleNotes->showNote(noteID, true);
}

void NotesDockWidget::saveNote(void)
{
    m_simpleNotes->saveNote();
}
void NotesDockWidget::newNote(void)
{
    m_simpleNotes->newTextNote();
}
void NotesDockWidget::newNoteWithLink(VerseSelection selection, QSharedPointer<Versification> v11n)
{
    m_simpleNotes->newTextNoteWithLink(selection, v11n);
}
void NotesDockWidget::noteSetTextBold(void)
{
    if(ui->textBrowser->fontWeight() == QFont::Bold) {
        ui->textBrowser->setFontWeight(QFont::Normal);
    } else {
        ui->textBrowser->setFontWeight(QFont::Bold);
    }
}
void NotesDockWidget::noteSetTextItalic(void)
{
    if(ui->textBrowser->fontItalic()) {
        ui->textBrowser->setFontItalic(false);
    } else {
        ui->textBrowser->setFontItalic(true);
    }
}
void NotesDockWidget::noteSetTextUnderline(void)
{
    if(ui->textBrowser->fontUnderline()) {
        ui->textBrowser->setFontUnderline(false);
    } else {
        ui->textBrowser->setFontUnderline(true);
    }
}
void NotesDockWidget::noteSetTextColor(void)
{
    const QColor color = QColorDialog::getColor(Qt::green, this);
    if(color.isValid()) {
        ui->textBrowser->setTextColor(color);
    }
}
void NotesDockWidget::noteUndo()
{
    ui->textBrowser->undo();
}
void NotesDockWidget::noteRedo()
{
    ui->textBrowser->redo();
}

void NotesDockWidget::newMark(VerseSelection selection, QColor color, QSharedPointer<Versification> v11n)
{
    newStyleMark(selection, "background-color:" + color.name(), v11n);
}
void NotesDockWidget::newStyleMark(VerseSelection selection, QString style, QSharedPointer<Versification> v11n)
{
    DEBUG_FUNC_NAME
    m_simpleNotes->newStyleMark(selection, style, v11n);

}
void NotesDockWidget::removeMark(VerseSelection selection, QSharedPointer<Versification> v11n)
{
    DEBUG_FUNC_NAME
    bool r = false;
    const QStringList id = m_notes->getIDList();
    for(int i = 0; i < id.size(); i++) {
        if(m_notes->getType(id.at(i)) == "mark") {
            const QString noteID = id.at(i);
            const QString link = m_notes->getRef(noteID, "link");
            VerseUrl url;
            url.fromString(link);

            UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::InterfaceUrl, url);
            urlConverter.setSettings(m_settings);
            urlConverter.setModuleMap(m_moduleManager->m_moduleMap.data());
            VerseUrl newUrl = urlConverter.convert();

            if(newUrl.contains(selection.moduleID, selection.bookID, selection.startChapterID)) {
                int start = selection.startVerse;
                int end;
                if(selection.endVerse != -1) {
                    end = selection.endVerse;
                } else {
                    end = start;
                }
                if(m_notes->getRef(noteID, "start").toInt() <= start && end <= m_notes->getRef(noteID, "end").toInt()) {
                    myDebug() << "remove = " << m_notes->getRef(noteID, "start");
                    //todo: work with positions in text
                    m_notes->removeNote(noteID);
                    m_actions->reloadIf(newUrl);
                }
            }
        }
    }

}
void NotesDockWidget::changeEvent(QEvent *e)
{
    QDockWidget::changeEvent(e);
    switch(e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
