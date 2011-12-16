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
#include "simplenotes.h"
#include <QtCore/QPointer>

#include <QtCore/QDateTime>
#include <QtGui/QClipboard>
#include <QtGui/QColorDialog>
#include <QtGui/QMessageBox>
#include "src/core/dbghelper.h"
#include "src/core/link/urlconverter2.h"
#include "src/ui/dialog/biblepassagedialog.h"
SimpleNotes::SimpleNotes()
{
}

void SimpleNotes::setDataWidget(QTextBrowser *data)
{
    m_textEdit_note = data;
    m_loadTextBrowser = true;
}
void SimpleNotes::setFrameWidget(QWebFrame *frame)
{
    m_frame = frame;
    m_loadTextBrowser = false;
}
void SimpleNotes::setTitleWidget(QLineEdit *title)
{
    m_lineEdit_title = title;
    connect(title, SIGNAL(editingFinished()), this, SLOT(updateTitle()));
}
void SimpleNotes::setViewWidget(QTreeView *treeView)
{
    m_view = new NotesItemView(m_notes, treeView);
}
void SimpleNotes::setLinkWidget(QLabel *link)
{
    m_label_link = link;
}
void SimpleNotes::setLinkButtonWidget(QPushButton *button)
{
    m_pushButton_link = button;
}


void SimpleNotes::init()
{
    if(m_loadTextBrowser) {
        connect(m_textEdit_note, SIGNAL(undoAvailable(bool)), this, SLOT(fastSave()));
        connect(m_textEdit_note, SIGNAL(redoAvailable(bool)), this, SLOT(fastSave()));
    }
    connect(m_pushButton_link, SIGNAL(clicked()), this, SLOT(editNoteLink()));
    connect(m_label_link, SIGNAL(linkActivated(QString)), this, SLOT(open(QString)));

    if(!m_notes->isLoaded()) {
        m_notes->init(m_settings->homePath + "notes.xml");
        m_notes->loadNotes();
        m_notes->readNotes();
    }

    m_notes->loadingNewInstance();
    connect(m_notes, SIGNAL(saveAll()), this, SLOT(saveNote()));
    connect(m_notes, SIGNAL(titleChanged(QString, QString)), this, SLOT(changeTitle(QString, QString)));
    connect(m_notes, SIGNAL(dataChanged(QString, QString)), this, SLOT(changeData(QString, QString)));
    connect(m_notes, SIGNAL(refChanged(QString, QMap<QString, QString>)), this, SLOT(changeRef(QString, QMap<QString, QString>)));
    connect(m_notes, SIGNAL(noteAdded(QString)), this, SLOT(addNote(QString)));
    connect(m_notes, SIGNAL(noteRemoved(QString, QMap<QString, QString>)), this, SLOT(removeNote(QString)));

    m_view->init();
    connect(m_view, SIGNAL(copyNotes(QStringList)), this, SLOT(copyNote(QStringList)));
    connect(m_view, SIGNAL(innerAddNewTextNote(QString)), this , SLOT(innerAddNewTextNote(QString)));
    connect(m_view, SIGNAL(innerAddNewFolderNote(QString)), this , SLOT(innerAddNewFolderNote(QString)));
    connect(m_view, SIGNAL(innerRemoveNotes()), this, SLOT(innerRemoveNotes()));
    connect(m_view, SIGNAL(showNote(QString)), this, SLOT(showNote(QString)));
    connect(m_view, SIGNAL(pasteNote(QString)), this, SLOT(pasteNote(QString)));
    m_noteID = "";


}
void SimpleNotes::showNote(const QString &noteID, bool selectNote)
{
    //save current notes
    if(selectNote)
        select(noteID);
    fastSave();
    aktNote();
    //load new notes
    m_noteID = noteID;
    m_noteRef = m_notes->getRef(m_noteID);

    setTitle(m_notes->getTitle(m_noteID));
    setData(m_notes->getData(m_noteID));
    setRef(m_noteRef);
    emit contentChanged();
}
void SimpleNotes::setTitle(QString title)
{
    m_lineEdit_title->setText(title);
}
void SimpleNotes::setData(QString data)
{
    if(m_loadTextBrowser) {
        if(m_notes->type() == Notes::TextNotes) {
            m_textEdit_note->setText(data);
        } else if(m_notes->type() == Notes::HtmlNotes) {
            m_textEdit_note->setHtml(data);
        }
    } else {
        m_frame->setHtml(data);
    }
}
void SimpleNotes::setRef(QMap<QString, QString> ref)
{
    if(!ref["link"].isEmpty()) {
        m_label_link->setText(m_moduleManager->notePos2Link(ref["link"]));
        m_pushButton_link->setEnabled(true);
    } else {
        m_label_link->setText("");
        m_pushButton_link->setEnabled(false);
    }
}
void SimpleNotes::editNoteLink()
{
    const QString link = m_noteRef.value("link");
    UrlConverter2 urlConverter(UrlConverter::InterfaceUrl, UrlConverter::PersistentUrl, link);
    urlConverter.setSM(m_settings, m_moduleManager->m_moduleMap.data());
    urlConverter.convert();
    if(urlConverter.url().isValid()) {
        QPointer<BiblePassageDialog> passageDialog = new BiblePassageDialog();
        connect(passageDialog, SIGNAL(updated(VerseUrl)), this, SLOT(updateNoteLink(VerseUrl)));
        passageDialog->setSettings(m_settings);
        passageDialog->setModuleManager(m_moduleManager);
        passageDialog->init();
        passageDialog->frame()->setVerseUrl(urlConverter.url());
        passageDialog->exec();
        delete passageDialog;
    }

}
void SimpleNotes::updateNoteLink(const VerseUrl &link)
{
    m_noteRef["link"] = link.toString();
    m_notes->setRef(m_noteID, m_noteRef);
    setRef(m_noteRef);
}
void SimpleNotes::changeData(const QString &id, const QString &data)
{
    if(m_noteID == id) {
        setData(data);
    }
}
void SimpleNotes::changeTitle(const QString &id, const QString &title)
{
    //DEBUG_FUNC_NAME
    if(m_noteID == id) {
        setTitle(title);
    }
    m_view->setTitle(id, title);

}
void SimpleNotes::updateTitle()
{
    disconnect(m_notes, SIGNAL(titleChanged(QString, QString)), this, SLOT(changeTitle(QString, QString)));

    const QString title = m_lineEdit_title->text();
    m_notes->setTitle(m_noteID, title);
    m_view->setTitle(m_noteID, title);

    connect(m_notes, SIGNAL(refChanged(QString, QMap<QString, QString>)), this, SLOT(changeRef(QString, QMap<QString, QString>)));
}

void SimpleNotes::changeRef(const QString &id, const QMap<QString, QString> &ref)
{
    if(m_noteID == id) {
        setRef(ref);
    }
}

void SimpleNotes::copyNote(const QStringList &ids)
{
    QClipboard *clipboard = QApplication::clipboard();
    QString text;
    for(int i = 0; i < ids.size(); i++) {
        const QString id = ids.at(i);
        QTextDocument doc;
        doc.setHtml(m_notes->getData(id));
        if(i != 0)
            text += "\n";
        text += doc.toPlainText();
    }
    if(!text.isEmpty()) {
        clipboard->setText(text);
    }
}
void SimpleNotes::pasteNote(const QString &parentID)
{
     QClipboard *clipboard = QApplication::clipboard();

     disconnect(m_notes, SIGNAL(noteAdded(QString)), this, SLOT(addNote(QString)));

     aktNote();
     fastSave();

     const QString title = tr("(unnamed)");
     const QString type = "text";
     const QString newID = m_notes->generateNewID();
     m_notes->setData(newID, clipboard->text());
     createNew(newID, title, type, parentID);
     m_view->addNote(newID, title, parentID);

     connect(m_notes, SIGNAL(noteAdded(QString)), this, SLOT(addNote(QString)));
}

void SimpleNotes::saveNote(void)
{
    //DEBUG_FUNC_NAME
    aktNote();
    fastSave();

    //save structure
    disconnect(m_notes, SIGNAL(refChanged(QString, QMap<QString, QString>)), this, SLOT(changeRef(QString, QMap<QString, QString>)));
    m_view->save();
    connect(m_notes, SIGNAL(refChanged(QString, QMap<QString, QString>)), this, SLOT(changeRef(QString, QMap<QString, QString>)));

    m_notes->saveNotes();
}
void SimpleNotes::fastSave(void)
{
    //DEBUG_FUNC_NAME
    disconnect(m_notes, SIGNAL(titleChanged(QString, QString)), this, SLOT(changeTitle(QString, QString)));
    disconnect(m_notes, SIGNAL(dataChanged(QString, QString)), this, SLOT(changeData(QString, QString)));
    disconnect(m_notes, SIGNAL(refChanged(QString, QMap<QString, QString>)), this, SLOT(changeRef(QString, QMap<QString, QString>)));

    if(m_loadTextBrowser)
        m_notes->setData(m_noteID, m_textEdit_note->toHtml());
    else
        m_notes->setData(m_noteID, m_frame->toHtml());

    m_notes->setTitle(m_noteID, m_lineEdit_title->text());
    m_notes->setRef(m_noteID, m_noteRef);


    connect(m_notes, SIGNAL(titleChanged(QString, QString)), this, SLOT(changeTitle(QString, QString)));
    connect(m_notes, SIGNAL(dataChanged(QString, QString)), this, SLOT(changeData(QString, QString)));
    connect(m_notes, SIGNAL(refChanged(QString, QMap<QString, QString>)), this, SLOT(changeRef(QString, QMap<QString, QString>)));
}
void SimpleNotes::aktNote()
{
    //DEBUG_FUNC_NAME
    if(m_noteID.isEmpty())
        return;
    const QString title = m_lineEdit_title->text();
    m_notes->setTitle(m_noteID, title);
    m_view->aktNote(m_noteID, title);
}
void SimpleNotes::select(const QString &noteID)
{
    //DEBUG_FUNC_NAME
    m_view->select(noteID);
}
void SimpleNotes::innerAddNewTextNote(const QString &parent)
{
    //DEBUG_FUNC_NAME
    aktNote();
    fastSave();
    const QString title = tr("(unnamed)");
    const QString type = "text";
    const QString newID = m_notes->generateNewID();

    createNew(newID, title, type, parent);
    m_view->addNote(newID, title, parent);
}

void SimpleNotes::newTextNote()
{
    //DEBUG_FUNC_NAME
    disconnect(m_notes, SIGNAL(noteAdded(QString)), this, SLOT(addNote(QString)));

    aktNote();
    fastSave();

    const QString title = tr("(unnamed)");
    const QString type = "text";
    const QString newID = m_notes->generateNewID();

    createNew(newID, title, type, "");
    m_view->addNote(newID, title, "");

    connect(m_notes, SIGNAL(noteAdded(QString)), this, SLOT(addNote(QString)));

}
void SimpleNotes::innerAddNewFolderNote(const QString &parent)
{
    //DEBUG_FUNC_NAME
    myDebug() << parent;
    aktNote();
    fastSave();
    const QString title = tr("(unnamed)");
    const QString type = "folder";
    const QString newID = m_notes->generateNewID();

    createNew(newID, title, type, parent);
    m_view->addFolder(newID, title, parent);
}
void SimpleNotes::createNew(const QString &noteID, const QString &title, const QString &type, const QString &parentID)
{
    //DEBUG_FUNC_NAME
    m_notes->setTitle(noteID, title);
    m_notes->setType(noteID, type);

    m_noteRef = QMap<QString, QString>();
    const QDateTime t = QDateTime::currentDateTime();

    m_noteRef["created"] = t.toString(Qt::ISODate);
    m_noteRef["parent"] = parentID;
    m_notes->setRef(noteID, m_noteRef);
    m_noteID = noteID;
    m_notes->insertID(noteID);
    //maybe show Note?
    setTitle(title);
    setData(m_notes->getData(noteID));
    setRef(m_noteRef);

}

void SimpleNotes::newFolder()
{
    //DEBUG_FUNC_NAME
    disconnect(m_notes, SIGNAL(noteAdded(QString)), this, SLOT(addNote(QString)));

    aktNote();
    fastSave();

    const QString title = tr("(unnamed)");
    const QString type = "folder";
    const QString newID = m_notes->generateNewID();

    createNew(newID, title, type, "");
    m_view->addFolder(newID, title, "");
    connect(m_notes, SIGNAL(noteAdded(QString)), this, SLOT(addNote(QString)));
}

void SimpleNotes::addNote(const QString &id)
{
    if(m_noteID == id)
        return;
    if(m_notes->getType(id) == "text") {
        m_view->addNote(id, m_notes->getTitle(id), m_notes->getRef(id).value("parent"));
    } else if(m_notes->getType(id) == "folder") {
        m_view->addFolder(id, m_notes->getTitle(id), m_notes->getRef(id).value("parent"));
    }
}

void SimpleNotes::newTextNoteWithLink(VerseSelection selection, QSharedPointer<Versification> v11n)
{
    //DEBUG_FUNC_NAME
    disconnect(m_notes, SIGNAL(noteAdded(QString)), this, SLOT(addNote(QString)));
    aktNote();
    fastSave();

    VerseUrlRange range;
    range.setModule(selection.moduleID);
    range.setBook(selection.bookID);
    range.setChapter(selection.startChapterID);
    range.setStartVerse(selection.startVerse);
    VerseUrl url(range);
    UrlConverter2 urlConverter(UrlConverter::InterfaceUrl, UrlConverter::PersistentUrl, url);
    urlConverter.setSM(m_settings, m_moduleManager->m_moduleMap.data());
    urlConverter.setV11n(v11n);
    urlConverter.convert();
    const VerseUrl url1 = urlConverter.url();
    const QString link = url1.toString();
    myDebug() << "link = " << link;
    const QString newID = m_notes->generateNewID();
    m_notes->setData(newID, "");
    m_notes->setTitle(newID, tr("(unnamed)"));
    m_notes->setType(newID, "text");

    QMap<QString, QString> ref;
    QDateTime t = QDateTime::currentDateTime();
    ref["created"] = t.toString(Qt::ISODate);
    ref["link"] = link;
    m_noteRef = ref;
    m_notes->setRef(newID, m_noteRef);

    m_notes->insertID(newID);
    m_noteID = newID;

    m_view->addNote(m_noteID, m_notes->getTitle(m_noteID), "");
    m_view->select(m_noteID);

    connect(m_notes, SIGNAL(noteAdded(QString)), this, SLOT(addNote(QString)));
    setTitle(tr("(unnamed)"));
    setData("");
    setRef(m_noteRef);

    m_actions->reloadIf(url1);
}
void SimpleNotes::newStyleMark(VerseSelection selection, const QString &style, QSharedPointer<Versification> v11n)
{
    if(!selection.canBeUsedForMarks()) {
        myWarning() << "cannot create mark";
        QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("Cannot create mark."));
        return;
    }
    disconnect(m_notes, SIGNAL(noteAdded(QString)), this, SLOT(addNote(QString)));
    aktNote();
    fastSave();

    VerseUrlRange range;
    range.setModule(selection.moduleID);
    range.setBook(selection.bookID);
    range.setChapter(selection.startChapterID);
    range.setStartVerse(selection.startVerse);
    VerseUrl url(range);
    UrlConverter2 urlConverter(UrlConverter::InterfaceUrl, UrlConverter::PersistentUrl, url);
    urlConverter.setSM(m_settings, m_moduleManager->m_moduleMap.data());
    urlConverter.setV11n(v11n);
    urlConverter.convert();
    const QString link = urlConverter.url().toString();

    //reloadNotes();
    const QString newID = m_notes->generateNewID();
    m_notes->setData(newID, "");
    m_notes->setTitle(newID, tr("(unnamed)"));
    m_notes->setType(newID, "mark");

    QMap<QString, QString> ref;
    ref["link"] = link;
    ref["start"] = QString::number(selection.startVerse);
    ref["end"] = QString::number(selection.endVerse);
    ref["selection_pos_type"] = selection.typeToString();

    if(selection.type == VerseSelection::RepeatOfLongestString) {
        myDebug() << "using repeat of longest string";
        ref["repeat"] = QString::number(selection.repeat);
        ref["longest_string"] = selection.longestString;
    } else if(selection.type == VerseSelection::ShortestString) {
        ref["startString"] = selection.shortestStringInStartVerse;
        ref["endString"] = selection.shortestStringInEndVerse;
    }

    ref["style"] = style;
    m_notes->setRef(newID, ref);
    m_notes->insertID(newID);
    connect(m_notes, SIGNAL(noteAdded(QString)), this, SLOT(addNote(QString)));
    m_actions->reloadIf(urlConverter.url());
}

void SimpleNotes::removeNotesFromData(const QStringList &ids)
{
    disconnect(m_notes, SIGNAL(noteRemoved(QString,QMap<QString, QString>)), this, SLOT(removeNote(QString)));

    foreach(const QString &id, ids) {
        if(id == m_noteID) {
            setTitle("");
            setData("");
            setRef(QMap<QString, QString>());
        }
        m_notes->removeNote(id);
    }
    connect(m_notes, SIGNAL(noteRemoved(QString,QMap<QString, QString>)), this, SLOT(removeNote(QString)));
}
void SimpleNotes::innerRemoveNotes()
{
    const QStringList removed = m_view->removeSelectedNotesFromView();
    removeNotesFromData(removed);
}

void SimpleNotes::removeNote(const QString &id)
{
    //DEBUG_FUNC_NAME
    disconnect(m_notes, SIGNAL(noteRemoved(QString,QMap<QString, QString>)), this, SLOT(removeNote(QString)));
    if(id == m_noteID) {
        setTitle("");
        setData("");
        setRef(QMap<QString, QString>());
    }
    m_view->removeNote(id);
    m_notes->removeNote(id);
    connect(m_notes, SIGNAL(noteRemoved(QString,QMap<QString, QString>)), this, SLOT(removeNote(QString)));
}
void SimpleNotes::open(const QString &link)
{
    VerseUrl url;
    url.fromString(link);

    UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::InterfaceUrl, url);
    urlConverter.setSettings(m_settings);
    urlConverter.setModuleMap(m_moduleManager->m_moduleMap.data());
    VerseUrl newUrl = urlConverter.convert();
    newUrl.setOpenToTransformation(true);

    m_actions->get(newUrl);
}
