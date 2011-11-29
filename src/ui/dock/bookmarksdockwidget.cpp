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
#include "bookmarksdockwidget.h"
#include "ui_bookmarksdockwidget.h"
#include "src/core/xbelreader.h"
#include "src/core/xbelwriter.h"
#include "src/core/dbghelper.h"
#include "src/core/link/urlconverter.h"
#include "src/ui/dialog/biblepassagedialog.h"
#include <QtGui/QClipboard>
#include <QtGui/QMessageBox>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QMenu>
#include <QtCore/QMimeData>
#include <QtCore/QPointer>
#include "src/core/obvcore.h"
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
    m_bookmarksFileName = m_settings->homePath + "bookmarks.xml";
    ui->treeWidget_bookmarks->clear();
    connect(ui->treeWidget_bookmarks, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(bookmarksContextMenu()));
    connect(ui->treeWidget_bookmarks, SIGNAL(itemActivated(QTreeWidgetItem *, int)), this, SLOT(bookmarksGo(QTreeWidgetItem *)));

    QFile file(m_bookmarksFileName);
    if(!file.open(QFile::ReadOnly | QFile::Text))
        return 1;
    XbelReader reader(ui->treeWidget_bookmarks);
    if(!reader.read(&file))
        return 1;
    return 0;
}
void BookmarksDockWidget::newBookmark(VerseSelection selection, Versification * v11n)
{
    QTreeWidgetItem *bookmark = new QTreeWidgetItem();
    bookmark->setFlags(bookmark->flags() | Qt::ItemIsEditable);
    QIcon bookmarkIcon;
    QStyle *style = ui->treeWidget_bookmarks->style();
    bookmarkIcon.addPixmap(style->standardPixmap(QStyle::SP_FileLinkIcon));
    bookmark->setIcon(0, bookmarkIcon);
    bookmark->setText(0,
                      v11n->bookName(selection.bookID) +
                      " " +
                      QString::number(selection.startChapterID + 1) +
                      "," +
                      QString::number(selection.startVerse + 1));

    VerseUrlRange range;
    range.setModule(selection.moduleID);
    range.setBook(selection.bookID);
    range.setChapter(selection.startChapterID);
    range.setStartVerse(selection.startVerse);
    range.setEndVerse(selection.endVerse);
    VerseUrl url(range);

    UrlConverter urlConverter(UrlConverter::InterfaceUrl, UrlConverter::PersistentUrl, url);
    urlConverter.setSettings(m_settings);
    urlConverter.setModuleMap(m_moduleManager->m_moduleMap);
    urlConverter.setV11n(v11n);

    const VerseUrl newUrl = urlConverter.convert();
    //myDebug() << "new url = " << newUrl.toString();
    bookmark->setText(1, newUrl.toString());

    bookmark->setData(0, Qt::UserRole, "bookmark");
    if(ui->treeWidget_bookmarks->currentItem() && ui->treeWidget_bookmarks->currentItem()->data(0, Qt::UserRole).toString() == "folder") {
        ui->treeWidget_bookmarks->currentItem()->addChild(bookmark);
    } else {
        ui->treeWidget_bookmarks->insertTopLevelItem(0, bookmark);
    }
    saveBookmarks();
}
void BookmarksDockWidget::saveBookmarks(void)
{
    QFile file(m_bookmarksFileName);
    if(!file.open(QFile::WriteOnly | QFile::Text))
        return;
    XbelWriter writer(ui->treeWidget_bookmarks);
    if(writer.writeFile(&file)) {
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
    if(ui->treeWidget_bookmarks->currentItem() && ui->treeWidget_bookmarks->currentItem()->data(0, Qt::UserRole).toString() == "folder") {
        ui->treeWidget_bookmarks->currentItem()->addChild(folder);
    } else {
        ui->treeWidget_bookmarks->insertTopLevelItem(0, folder);
    }
    saveBookmarks();
}

void BookmarksDockWidget::bookmarksContextMenu()
{
    QMenu *contextMenu = new QMenu(this);
    contextMenu->setObjectName("contextMenu");

    QAction *actionGoTo = new QAction(QIcon::fromTheme("go-jump", QIcon(":/icons/32x32/go-jump.png")), tr("Go To"), contextMenu);
    actionGoTo->setObjectName("actionGoTo");
    connect(actionGoTo, SIGNAL(triggered()), this, SLOT(bookmarksGo()));

    QAction *actionNewFolder = new QAction(QIcon::fromTheme("folder-new", QIcon(":/icons/32x32/folder-new.png")), tr("New Folder"), contextMenu) ;
    actionNewFolder->setObjectName("actionNewFolder");
    connect(actionNewFolder, SIGNAL(triggered()), this, SLOT(newBookmarksFolder()));

    QAction *actionEdit = new QAction(QIcon::fromTheme("document-edit", QIcon(":/icons/32x32/document-edit.png")), tr("Edit"), contextMenu);
    actionEdit->setObjectName("actionEdit");
    connect(actionEdit, SIGNAL(triggered()), this, SLOT(editBookmark()));

    QAction *actionRemove = new QAction(QIcon::fromTheme("list-remove", QIcon(":/icons/32x32/list-remove.png")), tr("Remove"), contextMenu);
    actionRemove->setObjectName("actionRemove");
    connect(actionRemove, SIGNAL(triggered()), this, SLOT(removeBookmark()));

    contextMenu->addAction(actionGoTo);
    contextMenu->addAction(actionNewFolder);
    contextMenu->addAction(actionEdit);
    contextMenu->addAction(actionRemove);

    contextMenu->exec(QCursor::pos());
    delete contextMenu;
}
void BookmarksDockWidget::removeBookmark()
{
    if(ui->treeWidget_bookmarks->currentItem())
        delete ui->treeWidget_bookmarks->currentItem();
    else
        myWarning() << "nothing selected";
}
void BookmarksDockWidget::editBookmark()
{
    if(!ui->treeWidget_bookmarks->currentItem()) {
        myWarning() << "nothing selected";
        return;
    }

    const QString pos = ui->treeWidget_bookmarks->currentItem()->text(1);
    VerseUrl url;
    url.fromString(pos);

    UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::InterfaceUrl, url);
    urlConverter.setSettings(m_settings);
    urlConverter.setModuleMap(m_moduleManager->m_moduleMap);
    urlConverter.setV11n(NULL);
    VerseUrl newUrl = urlConverter.convert();

    QPointer<BiblePassageDialog> passageDialog = new  BiblePassageDialog(this);
    connect(passageDialog, SIGNAL(updated(VerseUrl)), this, SLOT(updateBookmarkLink(VerseUrl)));
    setAll(passageDialog);
    passageDialog->init();
    passageDialog->frame()->setVerseUrl(newUrl);
    passageDialog->exec();
    delete passageDialog;
}
void BookmarksDockWidget::bookmarksGo()
{
    const QString pos = ui->treeWidget_bookmarks->currentItem()->text(1);
    if(pos.isEmpty())
        QMessageBox::critical(0, tr("Error"), tr("This Bookmark is invalid."));
    else
        internalOpenPos(pos);
}
void BookmarksDockWidget::updateBookmarkLink(VerseUrl url)
{
    ui->treeWidget_bookmarks->currentItem()->setText(1, url.toString());
}
void BookmarksDockWidget::bookmarksGo(QTreeWidgetItem * item)
{
    if(m_settings->onClickBookmarkGo == true) {
        const QString pos = item->text(1);
                   /* QMessageBox::critical(0, tr("Error"), tr("This Bookmark is invalid."));*/
        if(!pos.isEmpty()) {
            internalOpenPos(pos);
        }
    }
}
int BookmarksDockWidget::internalOpenPos(const QString &pos)
{
    //DEBUG_FUNC_NAME;
    //myDebug() << pos;
    VerseUrl url;
    url.fromString(pos);

    UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::InterfaceUrl, url);
    urlConverter.setSettings(m_settings);
    urlConverter.setModuleMap(m_moduleManager->m_moduleMap);
    VerseUrl newUrl = urlConverter.convert();
    newUrl.setOpenToTransformation(true);

    m_actions->get(newUrl);
    return 0;
}

void BookmarksDockWidget::changeEvent(QEvent *e)
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
