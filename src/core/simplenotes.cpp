#include "simplenotes.h"
#include <QSortFilterProxyModel>
#include "src/core/dbghelper.h"
#include <QtGui/QClipboard>
#include <QtGui/QMenu>
#include <QtGui/QColorDialog>
#include <QApplication>
#include "src/ui/dialog/biblepassagedialog.h"
#include "src/core/urlconverter.h"
SimpleNotes::SimpleNotes()
{
}

void SimpleNotes::setDataWidget(QTextEdit *data)
{
    m_textEdit_note = data;
}
void SimpleNotes::setTitleWidget(QLineEdit *title)
{
    m_lineEdit_title = title;
}
void SimpleNotes::setViewWidget(QTreeView *treeView)
{
    m_treeView = treeView;
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
    m_itemModel = new QStandardItemModel(m_treeView);
    connect(m_treeView,SIGNAL(clicked(QModelIndex)),this,SLOT(showNote(QModelIndex)));
    connect(m_treeView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(notesContextMenu()));
    connect(m_textEdit_note, SIGNAL(undoAvailable(bool)), this, SLOT(fastSave()));
    connect(m_textEdit_note, SIGNAL(redoAvailable(bool)), this, SLOT(fastSave()));
    connect(m_pushButton_link, SIGNAL(clicked()), this, SLOT(editNoteLink()));
    connect(m_label_link,SIGNAL(linkActivated(QString)),m_bibleDisplay,SIGNAL(get(QString)));

    if (!m_notes->isLoaded()) {
        m_notes->init(m_settings->homePath + "notes.xml");
        m_notes->loadNotes();
        m_notes->readNotes();
    }

    m_notes->loadingNewInstance();
    connect(m_notes, SIGNAL(saveAll()), this, SLOT(saveNote()));
    connect(m_notes,SIGNAL(titleChanged(QString,QString)), this, SLOT(changeTitle(QString,QString)));
    connect(m_notes,SIGNAL(dataChanged(QString,QString)),this,SLOT(changeData(QString,QString)));
    connect(m_notes,SIGNAL(refChanged(QString,QMap<QString,QString>)),this,SLOT(changeRef(QString,QMap<QString,QString>)));
    connect(m_notes,SIGNAL(noteAdded(QString)),this,SLOT(addNote(QString)));
    connect(m_notes,SIGNAL(noteRemoved(QString)),this,SLOT(removeNote(QString)));

    m_itemModel->clear();
    QStringList id = m_notes->getIDList();

    QStandardItem *parentItem = m_itemModel->invisibleRootItem();
    myDebug() << " id = " << id;
    for (int i = 0; i < id.size(); i++) {
        if (m_notes->getType(id.at(i)) == "text") {

            QStandardItem *noteItem = new QStandardItem;
            noteItem->setText(m_notes->getTitle(id.at(i)));
            noteItem->setData(id.at(i));
            parentItem->appendRow(noteItem);
        }
    }
    m_noteID = "";

    m_proxyModel = new QSortFilterProxyModel(this);
    m_proxyModel->setSourceModel(m_itemModel);
    m_proxyModel->setHeaderData(0,Qt::Horizontal,tr("Note Title"));
    m_selectionModel = new QItemSelectionModel(m_proxyModel);

    m_treeView->setSortingEnabled(true);
    m_treeView->setModel(m_proxyModel);
    m_treeView->setSelectionModel(m_selectionModel);
}
void SimpleNotes::showNote(QModelIndex index)
{
    showNote(index.data(Qt::UserRole + 1).toString());
}
void SimpleNotes::showNote(const QString &noteID)
{
    //DEBUG_FUNC_NAME
    //save current notes
    select(noteID);

    m_notes->setData(m_noteID, m_textEdit_note->toHtml());
    m_notes->setTitle(m_noteID, m_lineEdit_title->text());
    m_notes->setRef(m_noteID, currentNoteRef);


    //load new notes
    m_noteID = noteID;
    currentNoteRef = m_notes->getRef(m_noteID);

    setTitle(m_notes->getTitle(m_noteID));
    setData(m_notes->getData(m_noteID));
    setRef(currentNoteRef);
}
void SimpleNotes::setTitle(QString title)
{
    m_lineEdit_title->setText(title);
}
void SimpleNotes::setData(QString data)
{
     m_textEdit_note->setHtml(data);
}
void SimpleNotes::setRef(QMap<QString, QString> ref)
{
    if(!ref["link"].isEmpty()) {
        m_label_link->setText(m_moduleManager->notePos2Text(ref["link"]));
        m_pushButton_link->setEnabled(true);
    } else {
        m_label_link->setText("");
        m_pushButton_link->setEnabled(false);
    }
}
void SimpleNotes::editNoteLink()
{
    DEBUG_FUNC_NAME
    QString link = currentNoteRef["link"];

    UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::None, link);
    urlConverter.m_biblesIniPath = m_moduleManager->m_bible.biblesIniPath;
    urlConverter.pharse();

    BiblePassageDialog *passageDialog = new  BiblePassageDialog();
    connect(passageDialog, SIGNAL(updated(QString)), this, SLOT(updateNote(QString)));
    passageDialog->setSettings(m_settings);
    passageDialog->setCurrent(urlConverter.m_bibleID.toInt(), urlConverter.m_path, urlConverter.m_bookID, urlConverter.m_chapterID + 1, urlConverter.m_verseID + 1);
    passageDialog->show();
    passageDialog->exec();

}
void SimpleNotes::updateNote(QString link)
{
    currentNoteRef["link"] = link;
    showNote(m_noteID);
    return;
}
void SimpleNotes::changeData(QString id, QString data)
{
    DEBUG_FUNC_NAME
    if(m_noteID == id) {
        setData(data);
    }
}
void SimpleNotes::changeTitle(QString id, QString title)
{
    DEBUG_FUNC_NAME
    if(m_noteID == id) {
         setTitle(title);
    }
    //todo: reload list
}
void SimpleNotes::changeRef(QString id, QMap<QString, QString> ref)
{
    if(m_noteID == id) {
         setRef(ref);
    }
}

void SimpleNotes::copyNote(void)
{
    QModelIndexList list = m_selectionModel->selectedRows(0);
    QClipboard *clipboard = QApplication::clipboard();
    QString text;
    for(int i = 0; i < list.size();i++) {
         QTextDocument doc;
         doc.setHtml(m_notes->getData(list.at(i).data(Qt::UserRole+1).toString()));
         if(i != 0)
             text += "\n";
         text += doc.toPlainText();
    }
    if(!text.isEmpty()) {
        clipboard->setText(text);
    }
}
void SimpleNotes::saveNote(void)
{
    //DEBUG_FUNC_NAME
    aktNote();
    fastSave();
    m_notes->saveNotes();
}
void SimpleNotes::fastSave(void)
{
    m_notes->setData(m_noteID, m_textEdit_note->toHtml());
    m_notes->setTitle(m_noteID, m_lineEdit_title->text());
    m_notes->setRef(m_noteID, currentNoteRef);
}
void SimpleNotes::aktNote()
{
    if(m_noteID == "")
        return;
    QModelIndexList list = m_treeView->model()->match(m_treeView->model()->index(0, 0), Qt::UserRole + 1, m_noteID);
    if (list.size() != 1) {
        myDebug() << "invalid noteID = " << m_noteID;
        return;
    }
    QModelIndex index = list.at(0);
    if(index.data(Qt::DisplayRole) != m_notes->getTitle(m_noteID)) {
        m_treeView->model()->setData(index,m_notes->getTitle(m_noteID),Qt::DisplayRole);
    }
}
void SimpleNotes::select(QString noteID)
{
    QModelIndexList list = m_treeView->model()->match(m_treeView->model()->index(0, 0), Qt::UserRole + 1, noteID);
    if (list.size() != 1) {
        return;
    }
    QModelIndex index = list.at(0);
    m_selectionModel->clearSelection();
    m_selectionModel->setCurrentIndex(index, QItemSelectionModel::Select);
}

void SimpleNotes::newNote(void)
{
    aktNote();
    fastSave();
    m_selectionModel->clearSelection();

    QString newID = m_notes->generateNewID();
    m_notes->setData(newID, "");
    m_notes->setTitle(newID, tr("(unnamed)"));
    m_notes->setType(newID, "text");

    currentNoteRef = QMap<QString, QString>();
    m_notes->setRef(newID, currentNoteRef);
    m_noteID = newID;
    m_notes->insertID(newID);


    QStandardItem *parentItem = m_itemModel->invisibleRootItem();
    QStandardItem *newItem = new QStandardItem;
    newItem->setText(m_notes->getTitle(m_noteID));
    newItem->setData(m_noteID);
    parentItem->appendRow(newItem);

    select(m_noteID);
    setTitle(tr("(unnamed)"));
    setData("");
    setRef(currentNoteRef);

}
void SimpleNotes::addNote(QString id)
{
    if(m_noteID != id) {
        //todo: if there is already this note(but something like that should never happen)
        QStandardItem *parentItem = m_itemModel->invisibleRootItem();
        QStandardItem *newItem = new QStandardItem;
        newItem->setText(m_notes->getTitle(id));
        newItem->setData(id);
        parentItem->appendRow(newItem);
    }
}

void SimpleNotes::newNoteWithLink(VerseSelection selection)
{
    //DEBUG_FUNC_NAME

    aktNote();
    fastSave();

    QString link;
    UrlConverter urlConverter(UrlConverter::None, UrlConverter::PersistentUrl, "");
    urlConverter.m_biblesIniPath = m_moduleManager->m_bible.biblesIniPath;
    urlConverter.m_bibleID =  QString::number(m_moduleManager->m_bible.bibleID());
    urlConverter.m_bookID = m_moduleManager->m_bible.bookID();
    urlConverter.m_chapterID = m_moduleManager->m_bible.chapterID() - m_moduleManager->m_bible.chapterAdd();
    urlConverter.m_verseID = selection.startVerse - 1;
    urlConverter.m_bookName = m_moduleManager->m_bible.bookFullName.at(m_moduleManager->m_bible.bookID());
    link = urlConverter.convert();

    QString newID = m_notes->generateNewID();
    m_notes->setData(newID, "");
    m_notes->setTitle(newID, tr("(unnamed)"));
    m_notes->setType(newID, "text");

    QMap<QString, QString> ref;
    ref["link"] = link;
    currentNoteRef = ref;

    m_notes->setRef(newID, currentNoteRef);

    m_notes->insertID(newID);
    m_noteID = newID;

    QStandardItem *parentItem = m_itemModel->invisibleRootItem();
    QStandardItem *newItem = new QStandardItem;
    newItem->setText(m_notes->getTitle(m_noteID));
    newItem->setData(m_noteID);
    parentItem->appendRow(newItem);

    select(m_noteID);
    setTitle(tr("(unnamed)"));
    setData("");
    setRef(currentNoteRef);

    emit reloadChapter();
}
void SimpleNotes::notesContextMenu(void)
{
    QMenu *contextMenu = new QMenu(m_treeView);
    contextMenu->setObjectName("contextMenu");

    QAction *actionCopy = new QAction(this);
    actionCopy->setObjectName("actionCopy");
    actionCopy->setText(tr("Copy"));
    QIcon iconCopy;
    iconCopy.addPixmap(QPixmap(":/icons/16x16/edit-copy.png"), QIcon::Normal, QIcon::Off);
    actionCopy->setIcon(iconCopy);
    connect(actionCopy, SIGNAL(triggered()), this, SLOT(copyNote()));

    QAction *actionNew = new QAction(this);
    actionNew->setObjectName(QString::fromUtf8("actionNew"));
    connect(actionNew, SIGNAL(triggered()), this, SLOT(newNote()));
    QIcon iconNew;
    iconNew.addPixmap(QPixmap(":/icons/16x16/document-new.png"), QIcon::Normal, QIcon::Off);
    actionNew->setIcon(iconNew);
    actionNew->setText(tr("New"));

    QAction *actionRemove = new QAction(this);
    actionRemove->setObjectName("actionRemove");
    actionRemove->setText(tr("Delete"));
    QIcon iconRemove;
    iconRemove.addPixmap(QPixmap(":/icons/16x16/edit-delete.png"), QIcon::Normal, QIcon::Off);
    actionRemove->setIcon(iconRemove);
    connect(actionRemove, SIGNAL(triggered()), this, SLOT(removeNote()));

    contextMenu->addAction(actionCopy);
    contextMenu->addAction(actionNew);
    contextMenu->addAction(actionRemove);
    contextMenu->exec(QCursor::pos());
}
void SimpleNotes::removeNote(void)
{
    DEBUG_FUNC_NAME
    QModelIndexList list = m_selectionModel->selectedRows(0);
    for(int i = 0; i < list.size();i++) {
        QString id = list.at(i).data(Qt::UserRole+1).toString();
        if(id == m_noteID) {
            setTitle("");
            setData("");
            setRef(QMap<QString,QString>());
        }
        m_notes->removeNote(id);
        m_treeView->model()->removeRow(list.at(i).row());
    }
}
void SimpleNotes::removeNote(QString id)
{
    if(id == m_noteID) {
        setTitle("");
        setData("");
        setRef(QMap<QString,QString>());
    }
    QModelIndexList list = m_treeView->model()->match(m_treeView->model()->index(0, 0), Qt::UserRole + 1, id);
    if (list.size() != 1) {
        myDebug() << "invalid noteID = " << m_noteID;
        return;
    }
    QModelIndex index = list.at(0);
    m_treeView->model()->removeRow(index.row());
}
