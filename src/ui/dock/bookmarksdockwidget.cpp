#include "bookmarksdockwidget.h"
#include "ui_bookmarksdockwidget.h"
#include "src/core/xbelreader.h"
#include "src/core/xbelwriter.h"
#include "src/core/dbghelper.h"
#include "src/core/urlconverter.h"
#include "src/ui/dialog/biblepassagedialog.h"
#include <QtGui/QClipboard>
#include <QtGui/QMessageBox>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QMenu>
#include <QtCore/QMimeData>
BookmarksDockWidget::BookmarksDockWidget(QWidget *parent) :
        DockWidget(parent),
        ui(new Ui::BookmarksDockWidget)
{
    ui->setupUi(this);
}

BookmarksDockWidget::~BookmarksDockWidget()
{
    delete ui;
}
int BookmarksDockWidget::init()
{
    bookmarksFileName = m_settings->homePath + "bookmarks.xml";
    ui->treeWidget_bookmarks->clear();
    connect(ui->treeWidget_bookmarks, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(bookmarksContextMenu()));
    connect(ui->treeWidget_bookmarks, SIGNAL(itemActivated(QTreeWidgetItem *, int)), this, SLOT(bookmarksGo(QTreeWidgetItem *)));

    QFile file(bookmarksFileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
        return 1;
    XbelReader reader(ui->treeWidget_bookmarks);
    if (!reader.read(&file))
        return 1;
    return 0;
}
void BookmarksDockWidget::newBookmark(VerseSelection selection)
{
    QTreeWidgetItem *bookmark = new QTreeWidgetItem();
    bookmark->setFlags(bookmark->flags() | Qt::ItemIsEditable);
    QIcon bookmarkIcon;
    QStyle *style = ui->treeWidget_bookmarks->style();
    bookmarkIcon.addPixmap(style->standardPixmap(QStyle::SP_FileLinkIcon));
    bookmark->setIcon(0, bookmarkIcon);
    bookmark->setText(0,
                      m_moduleManager->m_bible.bookFullName.at(m_moduleManager->m_bible.bookID()) +
                      " " +
                      QString::number(m_moduleManager->m_bible.chapterID() - m_moduleManager->m_bible.chapterAdd() + 1, 10) +
                      "," +
                      QString::number(selection.startVerse, 10));
    UrlConverter urlConverter(UrlConverter::None, UrlConverter::PersistentUrl, "");
    urlConverter.m_biblesIniPath = m_moduleManager->m_bible.biblesIniPath;
    urlConverter.m_bibleID = QString::number(m_moduleManager->m_bible.bibleID());
    urlConverter.m_bookID = m_moduleManager->m_bible.bookID();
    urlConverter.m_chapterID = m_moduleManager->m_bible.chapterID() - m_moduleManager->m_bible.chapterAdd();
    urlConverter.m_verseID = selection.startVerse - 1;
    bookmark->setText(1, urlConverter.convert());

    bookmark->setData(0, Qt::UserRole, "bookmark");
    if (ui->treeWidget_bookmarks->currentItem() && ui->treeWidget_bookmarks->currentItem()->data(0, Qt::UserRole).toString() == "folder") {
        ui->treeWidget_bookmarks->currentItem()->addChild(bookmark);
    } else {
        ui->treeWidget_bookmarks->insertTopLevelItem(0, bookmark);
    }
    saveBookmarks();
}
void BookmarksDockWidget::saveBookmarks(void)
{
    QString fileName = bookmarksFileName;
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
        return;
    XbelWriter writer(ui->treeWidget_bookmarks);
    if (writer.writeFile(&file)) {
        //statusBar()->showMessage(tr("Bookmarks saved"), 5000);
    }
}
void BookmarksDockWidget::newBookmarksFolder(void)
{
    QTreeWidgetItem *folder = new QTreeWidgetItem();
    folder->setFlags(folder->flags() | Qt::ItemIsEditable);
    QStyle *style = ui->treeWidget_bookmarks->style();
    QIcon folderIcon;
    folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirClosedIcon), QIcon::Normal, QIcon::Off);
    folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirOpenIcon), QIcon::Normal, QIcon::On);
    folder->setIcon(0, folderIcon);
    folder->setText(0, tr("new folder"));
    folder->setText(1, "");
    folder->setData(0, Qt::UserRole, "folder");
    if (ui->treeWidget_bookmarks->currentItem() && ui->treeWidget_bookmarks->currentItem()->data(0, Qt::UserRole).toString() == "folder") {
        ui->treeWidget_bookmarks->currentItem()->addChild(folder);
    } else {
        ui->treeWidget_bookmarks->insertTopLevelItem(0, folder);
    }
    saveBookmarks();
}

void BookmarksDockWidget::bookmarksContextMenu(void)
{
    QMenu *contextMenu = new QMenu(this);
    contextMenu->setObjectName("contextMenu");

    QAction *actionGoTo = new QAction(this);
    actionGoTo->setObjectName("actionGoTo");
    QIcon iconGoTo;
    iconGoTo.addPixmap(QPixmap(":/icons/16x16/go-jump.png"), QIcon::Normal, QIcon::Off);
    actionGoTo->setIcon(iconGoTo);
    actionGoTo->setText(tr("Go to"));

    connect(actionGoTo, SIGNAL(triggered()), this, SLOT(bookmarksGo()));

    QAction *actionNewFolder = new QAction(this);
    actionNewFolder->setObjectName("actionNewFolder");
    connect(actionNewFolder, SIGNAL(triggered()), this, SLOT(newBookmarksFolder()));
    QIcon iconNewFolder;
    iconNewFolder.addPixmap(QPixmap(":/icons/16x16/folder-new.png"), QIcon::Normal, QIcon::Off);
    actionNewFolder->setIcon(iconNewFolder);
    actionNewFolder->setText(tr("New Folder"));

    QAction *actionEdit = new QAction(this);
    actionEdit->setObjectName("actionEdit");
    QIcon iconEdit;
    iconEdit.addPixmap(QPixmap(":/icons/16x16/document-edit.png"), QIcon::Normal, QIcon::Off);
    actionEdit->setIcon(iconEdit);
    actionEdit->setText(tr("Edit"));
    connect(actionEdit, SIGNAL(triggered()), this, SLOT(editBookmark()));

    QAction *actionRemove = new QAction(this);
    actionRemove->setObjectName("actionRemove");
    QIcon iconRemove;
    iconRemove.addPixmap(QPixmap(":/icons/16x16/list-remove.png"), QIcon::Normal, QIcon::Off);
    actionRemove->setIcon(iconRemove);
    actionRemove->setText(tr("Remove"));
    connect(actionRemove, SIGNAL(triggered()), this, SLOT(removeBookmark()));

    contextMenu->addAction(actionGoTo);
    contextMenu->addAction(actionNewFolder);
    contextMenu->addAction(actionEdit);
    contextMenu->addAction(actionRemove);

    contextMenu->exec(QCursor::pos());
}
void BookmarksDockWidget::removeBookmark()
{
    if (ui->treeWidget_bookmarks->currentItem())
        delete ui->treeWidget_bookmarks->currentItem();
    else
        myDebug() << "nothing selected";
}
void BookmarksDockWidget::editBookmark()
{
    if (!ui->treeWidget_bookmarks->currentItem()) {
        myDebug() << "nothing selected";
        return;
    }

    QString pos = ui->treeWidget_bookmarks->currentItem()->text(1);
    /*if (internalOpenPos(pos) != 0) {
        QMessageBox::critical(0, tr("Error"), tr("This Bookmark is invalid."));
        myDebug() << "invalid bookmark";
    }*/

    UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::None, pos);
    urlConverter.m_biblesIniPath = m_moduleManager->m_bible.biblesIniPath;//not nice, i know
    urlConverter.pharse();

    BiblePassageDialog *passageDialog = new  BiblePassageDialog(this);
    connect(passageDialog, SIGNAL(updated(QString)), this, SLOT(updateBookmark(QString)));
    passageDialog->setSettings(m_settings);
    passageDialog->setCurrent(urlConverter.m_bibleID.toInt(), urlConverter.m_path, urlConverter.m_bookID, urlConverter.m_chapterID, urlConverter.m_verseID);
    passageDialog->show();
    passageDialog->exec();
}
void BookmarksDockWidget::bookmarksGo()
{
    QString pos = ui->treeWidget_bookmarks->currentItem()->text(1);
    if (internalOpenPos(pos) != 0)
        QMessageBox::critical(0, tr("Error"), tr("This Bookmark is invalid."));
}
void BookmarksDockWidget::updateBookmark(QString pos)
{
    ui->treeWidget_bookmarks->currentItem()->setText(1, pos);
}
void BookmarksDockWidget::bookmarksGo(QTreeWidgetItem * item)
{
    if (m_settings->onClickBookmarkGo == true) {
        QString pos = item->text(1);
        if (internalOpenPos(pos) != 0)
            QMessageBox::critical(0, tr("Error"), tr("This Bookmark is invalid."));
    }
}
int BookmarksDockWidget::internalOpenPos(const QString &pos)
{
    DEBUG_FUNC_NAME
    UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::InterfaceUrl, pos);
    urlConverter.m_biblesIniPath = m_moduleManager->m_bible.biblesIniPath;//not nice, i know
    urlConverter.pharse();
    myDebug() << "url = " << urlConverter.convert();
    emit get(urlConverter.convert());
    return 0;
}


void BookmarksDockWidget::changeEvent(QEvent *e)
{
    QDockWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
